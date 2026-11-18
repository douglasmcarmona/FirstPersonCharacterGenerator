// Copyright (c) 2026 Douglas Mendes Carmona All Rights Reserved.

#include "UI/FPCharacterGeneratorWidget.h"
#include "FirstPersonCharacterGenerator.h"
#include "GameProjectUtils.h"
#include "SourceCodeNavigation.h"
#include "Types/FPCharacterGeneratorTypes.h"

#define MAKE_FILE_PATH(Class, HeaderOrSource) \
Class##HeaderOrSource##File = FFPCharacterGeneratorGlobals::Default##HeaderOrSource##Path + \
FString(#Class) /= EntityName + FFPCharacterGeneratorGlobals::Class##String + \
FFPCharacterGeneratorGlobals::HeaderOrSource##Extension;

UFPCharacterGeneratorWidget::UFPCharacterGeneratorWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	FFPCharacterGeneratorGlobals::InitDefaultPaths();
}

void UFPCharacterGeneratorWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
}

void UFPCharacterGeneratorWidget::ValidateData(const FString& EntityName, const FString& Path,
	FString& CharacterHeaderFile, FString& CharacterSourceFile, FString& PlayerControllerHeaderFile,
	FString& PlayerControllerSourceFile, bool& bIsDataValid, FText& ErrorMessage) const
{
	MAKE_FILE_PATH(Character, Header)
	MAKE_FILE_PATH(Character, Source);
	MAKE_FILE_PATH(PlayerController, Header)
	MAKE_FILE_PATH(PlayerController, Source)
	
	const TSet<FString> InvalidEntityNames = FSourceCodeNavigation::GetSourceFileDatabase().GetDisallowedHeaderNames();
	const FModuleContextInfo& ModuleContextInfo = FFirstPersonCharacterGeneratorModule::Get().GetModuleContextInfo();
	
	// bIsDataValid = FPaths::ValidatePath(DefaultHeaderPath + Path, &ErrorMessage);
	// if (!bIsDataValid) return;
	
	bIsDataValid = GameProjectUtils::IsValidClassNameForCreation(EntityName, ModuleContextInfo, InvalidEntityNames, ErrorMessage);	
}
