// Copyright (c) 2026 Douglas Mendes Carmona All Rights Reserved.

#include "UI/FPCharacterGeneratorWidget.h"
#include "FirstPersonCharacterGenerator.h"
#include "GameProjectUtils.h"
#include "SourceCodeNavigation.h"

UFPCharacterGeneratorWidget::UFPCharacterGeneratorWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	const FModuleContextInfo& ModuleContextInfo = FFirstPersonCharacterGeneratorModule::Get().GetModuleContextInfo();
	GameProjectUtils::CalculateSourcePaths(
		ModuleContextInfo.ModuleSourcePath,
		ModuleContextInfo,
		DefaultHeaderPath, 
		DefaultSourcePath);
}

void UFPCharacterGeneratorWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
}

void UFPCharacterGeneratorWidget::ValidateData(const FString& EntityName, const FString& Path, FString& HeaderFile,
                                               FString& SourceFile, bool& bIsDataValid, FText& ErrorMessage) const
{
	HeaderFile = DefaultHeaderPath + Path /= EntityName + FString(".h");
	SourceFile = DefaultSourcePath + Path /= EntityName + FString(".cpp");
	const TSet<FString> InvalidEntityNames = FSourceCodeNavigation::GetSourceFileDatabase().GetDisallowedHeaderNames();
	const FModuleContextInfo& ModuleContextInfo = FFirstPersonCharacterGeneratorModule::Get().GetModuleContextInfo();
	
	bIsDataValid = FPaths::ValidatePath(DefaultHeaderPath + Path, &ErrorMessage);
	if (!bIsDataValid) return;
	
	bIsDataValid = GameProjectUtils::IsValidClassNameForCreation(EntityName, ModuleContextInfo, InvalidEntityNames, ErrorMessage);	
}
