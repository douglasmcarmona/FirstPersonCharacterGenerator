// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FPCharacterGeneratorWidget.h"

#include "DesktopPlatformModule.h"
#include "GameProjectUtils.h"
/**
	 * Given the path provided, work out where generated .h and .cpp files would be placed
	 *
	 * @param	InPath				The path to use a base
	 * @param	ModuleInfo			Information about the module being validated
	 * @param	OutHeaderPath		The path where the .h file should be placed
	 * @param	OutSourcePath		The path where the .cpp file should be placed
	 * @param	OutFailReason		Optional parameter to fill with failure information
	 *
	 * @return	false if the paths are invalid
	 
	static UE_API bool CalculateSourcePaths(
	const FString& InPath, 
	const FModuleContextInfo& ModuleInfo,
	FString& OutHeaderPath,
	FString& OutSourcePath,
	FText* const OutFailReason = nullptr);*/

/**
* Given the path provided, work out where it's located within the Source folder
*
* @param	InPath				The path to use a base
* @param	ModuleInfo			Information about the module being validated
* @param	OutClassLocation	The location within the Source folder
* @param	OutFailReason		Optional parameter to fill with failure information
*
* @return	false if the paths are invalid
	 
static UE_API bool GetClassLocation(
const FString& InPath,
const FModuleContextInfo& ModuleInfo,
EClassLocation& OutClassLocation,
FText* const OutFailReason = nullptr);	
*/

//FSourceCodeNavigation::GetSourceFileDatabase().GetDisallowedHeaderNames()

/** Returns true if the specified class name is properly formed and does not conflict with another class, including source/header files
	static UE_API bool IsValidClassNameForCreation(
	const FString& NewClassName, 
	const FModuleContextInfo& ModuleInfo, 
	const TSet<FString>& DisallowedHeaderNames,
	FText& OutFailReason);*/

/**
 * - Plugin tab starts with module root
 * - In that case, the Path text box should be empty
 * - The value typed in the Path text box must be appended to the Public and Private resulting paths
 * - If the resulting paths don't exist, create them.
 */

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

void UFPCharacterGeneratorWidget::CalculateFileNames(const FString& EntityName, const FString& Path, FString& HeaderFile, FString& SourceFile) const
{
	HeaderFile = DefaultHeaderPath + Path /= EntityName + FString(".h");
	SourceFile = DefaultSourcePath + Path /= EntityName + FString(".cpp");
}
