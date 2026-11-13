// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FPCharacterGeneratorWidget.h"

#include "DesktopPlatformModule.h"
#include "GameProjectUtils.h"
#include "SourceCodeNavigation.h"

/**
 * - Plugin tab starts with module root
 * - In that case, the Path text box should be empty
 * - The value typed in the Path text box must be appended to the Public and Private resulting paths
 * - If the resulting paths don't exist, create them.
 */

static TSet<FString> InvalidEntityNames = FSourceCodeNavigation::GetSourceFileDatabase().GetDisallowedHeaderNames();

UFPCharacterGeneratorWidget::UFPCharacterGeneratorWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	TArray<FModuleContextInfo> CurrentModules = GameProjectUtils::GetCurrentProjectModules();	
	
	const FString ProjectName = FApp::GetProjectName();
	FString DefaultPath;
	for (const FModuleContextInfo& ModuleContext : CurrentModules)
	{
		if (ModuleContext.ModuleName == ProjectName)
		{
			ModuleContextInfo = MakeShareable(new FModuleContextInfo(ModuleContext));
			DefaultPath = ModuleContext.ModuleSourcePath;			
			break;
		}
	}	
	GameProjectUtils::CalculateSourcePaths(
		DefaultPath,
		*ModuleContextInfo, 
		DefaultHeaderPath, 
		DefaultSourcePath);
	
	DefaultHeaderPath /= FString("Public/");
	DefaultSourcePath /= FString("Private/");
}

void UFPCharacterGeneratorWidget::ValidateData(const FString& EntityName, const FString& Path, FString& HeaderFile,
	FString& SourceFile, bool& bIsDataValid, FText& ErrorMessage) const
{
	HeaderFile = DefaultHeaderPath + Path /= EntityName + FString(".h");
	SourceFile = DefaultSourcePath + Path /= EntityName + FString(".cpp");
	
	bIsDataValid = FPaths::ValidatePath(DefaultHeaderPath + Path, &ErrorMessage);
	if (!bIsDataValid) return;	
	
	bIsDataValid = GameProjectUtils::IsValidClassNameForCreation(EntityName, *ModuleContextInfo, InvalidEntityNames, ErrorMessage);	
}
