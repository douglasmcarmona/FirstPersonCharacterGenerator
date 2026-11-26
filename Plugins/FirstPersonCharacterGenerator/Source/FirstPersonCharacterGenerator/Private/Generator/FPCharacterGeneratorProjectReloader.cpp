// Copyright (c) 2026 Douglas Mendes Carmona All Rights Reserved.


#include "Generator/FPCharacterGeneratorProjectReloader.h"

#include "FirstPersonCharacterGenerator.h"
#include "GameProjectUtils.h"
#include "ILiveCodingModule.h"
#include "SourceCodeNavigation.h"
#include "Editor/EditorPerProjectUserSettings.h"
#include "Misc/HotReloadInterface.h"

#define LOCTEXT_NAMESPACE "FFirstPersonCharacterGeneratorModule"

bool UFPCharacterGeneratorProjectReloader::ReloadProject(FText& ErrorMessage)
{	
	const bool bProjectHadCodeFiles = GameProjectUtils::ProjectHasCodeFiles();
	const FModuleContextInfo ModuleInfo = FFirstPersonCharacterGeneratorModule::Get().GetModuleContextInfo();
	
#if WITH_LIVE_CODING
	ILiveCodingModule* LiveCoding = FModuleManager::GetModulePtr<ILiveCodingModule>(LIVE_CODING_MODULE_NAME);
	if (LiveCoding != nullptr && LiveCoding->IsEnabledForSession())
	{
		if (!bProjectHadCodeFiles)
		{
			FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("LiveCodingNoSources", "Project now includes sources, please close the editor and build from your IDE."));
			return true;
		}

		if (LiveCoding->AutomaticallyCompileNewClasses())
		{
			LiveCoding->Compile(ELiveCodingCompileFlags::None, nullptr);
			// OutReloadStatus = EReloadStatus::Reloaded;
		}
		UE_LOG(LogTemp, Display, TEXT("Live coding succeeded"))
		return true; //EAddCodeToProjectResult::Succeeded;
	}
#endif

	if (!bProjectHadCodeFiles)
	{
		// This is the first time we add code to this project so compile its game DLL
		const FString GameModuleName = FApp::GetProjectName();
		check(ModuleInfo.ModuleName == GameModuleName);

		// Because this project previously didn't have any code, the UBT target name will just be UnrealEditor. Now that we've
		// added some code, the target name will be changed to match the editor target for the new source. 
		FString NewUBTTargetName = GameModuleName + TEXT("Editor");
		FPlatformMisc::SetUBTTargetName(*NewUBTTargetName);

		IHotReloadInterface& HotReloadSupport = FModuleManager::LoadModuleChecked<IHotReloadInterface>("HotReload");
		if (!HotReloadSupport.RecompileModule(*GameModuleName, *GWarn, ERecompileModuleFlags::ReloadAfterRecompile | ERecompileModuleFlags::ForceCodeProject))
		{
			ErrorMessage = LOCTEXT("FailedToCompileNewGameModule", "Failed to compile newly created game module.");
			return false;
		}

		// Notify that we've created a brand new module
		FSourceCodeNavigation::AccessOnNewModuleAdded().Broadcast(*GameModuleName);
		// OutReloadStatus = EReloadStatus::Reloaded;
	}
	else if (GetDefault<UEditorPerProjectUserSettings>()->bAutomaticallyHotReloadNewClasses)
	{
		FModuleStatus ModuleStatus;
		const FName ModuleFName = *ModuleInfo.ModuleName;
		if (ensure(FModuleManager::Get().QueryModule(ModuleFName, ModuleStatus)))
		{
			// Compile the module that the class was added to so that the newly added class with appear in the Content Browser
			TArray<UPackage*> PackagesToRebind;
			if (ModuleStatus.bIsLoaded)
			{
				const bool bIsHotReloadable = FModuleManager::Get().DoesLoadedModuleHaveUObjects(ModuleFName);
				if (bIsHotReloadable)
				{
					// Is there a UPackage with the same name as this module?
					const FString PotentialPackageName = FString(TEXT("/Script/")) + ModuleInfo.ModuleName;
					UPackage* Package = FindPackage(nullptr, *PotentialPackageName);
					if (Package)
					{
						PackagesToRebind.Add(Package);
					}
				}
			}

			IHotReloadInterface& HotReloadSupport = FModuleManager::LoadModuleChecked<IHotReloadInterface>("HotReload");
			if (PackagesToRebind.Num() > 0)
			{
				// Perform a hot reload
				ECompilationResult::Type CompilationResult = HotReloadSupport.RebindPackages( PackagesToRebind, EHotReloadFlags::WaitForCompletion, *GWarn );
				if( CompilationResult != ECompilationResult::Succeeded && CompilationResult != ECompilationResult::UpToDate )
				{
					ErrorMessage = FText::Format(LOCTEXT("FailedToHotReloadFmt", "Failed to automatically hot reload the {0} module."), FText::FromString(ModuleInfo.ModuleName));					
					return false;					
				}
			}
			else
			{
				// Perform a regular unload, then reload
				if (!HotReloadSupport.RecompileModule(ModuleFName, *GWarn, ERecompileModuleFlags::ReloadAfterRecompile))
				{					
					ErrorMessage = FText::Format(LOCTEXT("FailedToCompileModuleFmt", "Failed to automatically compile the '{0}' module."), FText::FromString(ModuleInfo.ModuleName));										
					return false;
				}
			}
		}		
	}
	return true;
}

bool UFPCharacterGeneratorProjectReloader::GenerateProjectFiles()
{
#if PLATFORM_WINDOWS

	const FString ProjectFile = FPaths::GetProjectFilePath();

	// Unreal Engine installation directory.
	const FString EngineDir = FPaths::EngineDir();
	const FString Generator = EngineDir / TEXT("Build/BatchFiles/GenerateProjectFiles.bat");

	if (!FPaths::FileExists(Generator) || !FPaths::FileExists(ProjectFile))
	{
		return false;
	}

	const FString Parameters = FString::Printf(
		TEXT("/C \"\"%s\" \"%s\" -CurrentPlatform\""),
		*Generator,
		*ProjectFile
	);

	FProcHandle Proc = FPlatformProcess::CreateProc(
		TEXT("cmd.exe"),
		*Parameters,
		true,   // bLaunchDetached
		false,  // bLaunchHidden
		false,  // bLaunchReallyHidden
		nullptr,
		0,
		nullptr,
		nullptr
	);

	if (!Proc.IsValid())
	{
		return false;
	}

	// Synchronous wait: project files must finish generating
	// before proceeding.
	FPlatformProcess::WaitForProc(Proc);

	int32 ReturnCode = -1;
	FPlatformProcess::GetProcReturnCode(Proc, &ReturnCode);

	FPlatformProcess::CloseProc(Proc);

	return ReturnCode == 0;

#else
	return false;
#endif
}
