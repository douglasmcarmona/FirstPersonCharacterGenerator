// Copyright (c) 2026 Douglas Mendes Carmona All Rights Reserved.

#include "UI/FPCharacterGeneratorWidget.h"
#include "FirstPersonCharacterGenerator.h"
#include "GameProjectUtils.h"
#include "SourceCodeNavigation.h"
#include "Types/FPCharacterGeneratorTypes.h"

void UFPCharacterGeneratorWidget::InitDefaultPaths()
{
	FFPCharacterGeneratorGlobals::InitDefaultPaths();
}

void UFPCharacterGeneratorWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
}

FString UFPCharacterGeneratorWidget::GetFullFilePath(const FString& EntityName, EClass Class,
	EClassFileType ClassFileType)
{
	FString DefaultPath;
	FString FileExtension;
	FString ClassString = UEnum::GetValueAsString(Class).Replace(TEXT("EClass::"),TEXT(""));
	switch (ClassFileType)
	{
	case EClassFileType::Header:
		DefaultPath = FFPCharacterGeneratorGlobals::DefaultHeaderPath;
		FileExtension = FFPCharacterGeneratorGlobals::HeaderExtension;
		break;
		
	case EClassFileType::Source:
		DefaultPath = FFPCharacterGeneratorGlobals::DefaultSourcePath;
		FileExtension = FFPCharacterGeneratorGlobals::SourceExtension;
		break;
		
	default: break;		
	}
	
	return DefaultPath /= ClassString /= EntityName + ClassString + FileExtension;
}

bool UFPCharacterGeneratorWidget::ValidateEntityName(const FString& EntityName, FText& ErrorMessage)
{
	const TSet<FString> InvalidEntityNames = FSourceCodeNavigation::GetSourceFileDatabase().GetDisallowedHeaderNames();
	const FModuleContextInfo& ModuleContextInfo = FFirstPersonCharacterGeneratorModule::Get().GetModuleContextInfo();
	return GameProjectUtils::IsValidClassNameForCreation(EntityName, ModuleContextInfo, InvalidEntityNames, ErrorMessage);
	
}
