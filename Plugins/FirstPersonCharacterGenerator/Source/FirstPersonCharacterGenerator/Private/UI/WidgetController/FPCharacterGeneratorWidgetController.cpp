// Copyright (c) 2026 Douglas Mendes Carmona All Rights Reserved.

#include "UI/WidgetController/FPCharacterGeneratorWidgetController.h"

#include "FirstPersonCharacterGenerator.h"
#include "SourceCodeNavigation.h"
#include "Interfaces/AssetCreatorInterface.h"
#include "Interfaces/FileSystemInterface.h"
#include "Interfaces/ParserInterface.h"
#include "Interfaces/ProjectReloaderInterface.h"
#include "Interfaces/DI/FPCharacterGeneratorDIResolver.h"

#define LOCTEXT_NAMESPACE "FFirstPersonCharacterGeneratorModule"

void UFPCharacterGeneratorWidgetController::CreateEntity(FFPCharacterGeneratorMetadata& Metadata)
{
	UFPCharacterGeneratorDIResolver* DIResolver = Cast<UFPCharacterGeneratorDIResolver>(FFirstPersonCharacterGeneratorModule::Get().GetDIResolver());
	IParserInterface* Parser = Cast<IParserInterface>(DIResolver->GetParser());
	IFileSystemInterface* FileSystem = Cast<IFileSystemInterface>(DIResolver->GetFileSystem());
	IProjectReloaderInterface* ProjectReloader = Cast<IProjectReloaderInterface>(DIResolver->GetProjectReloader());
	IAssetCreatorInterface* AssetCreator = Cast<IAssetCreatorInterface>(DIResolver->GetAssetCreator());
	IFileManager& FileManager = IFileManager::Get();
	
	FText ErrorMessage;
	Parser->Parse(Metadata);
	TArray<FString> AbsoluteFilePaths;
	
	FScopedSlowTask SlowTask(5.f, LOCTEXT("CreatingFiles", "Creating new entity files..."));
	SlowTask.MakeDialog();
	SlowTask.EnterProgressFrame(1.f, LOCTEXT("SaveFiles","Saving header and source files..."));
	for (const FGeneratedFile& File : Metadata.GeneratedFiles)
	{
		FileSystem->SaveFile(File.FullFileName, File.FileContent);
		AbsoluteFilePaths.Add(FileManager.ConvertToAbsolutePathForExternalAppForRead(*File.FullFileName));	
	}
	SlowTask.EnterProgressFrame(1.f, LOCTEXT("GenerateProjectFiles", "Updating project files..."));
	ProjectReloader->GenerateProjectFiles();
	
	SlowTask.EnterProgressFrame(1.f, LOCTEXT("Compiling", "Compiling new C++ classes..."));
	if (!ProjectReloader->ReloadProject(ErrorMessage))
	{
		FMessageDialog::Open(EAppMsgType::Ok, ErrorMessage);
	}
	
	if (Metadata.Features.bOpenIDE)
	{
		SlowTask.EnterProgressFrame(1.f, LOCTEXT("OpeningIDE", "Opening IDE..."));	
		FSourceCodeNavigation::OpenSourceFilesAsync(AbsoluteFilePaths);		
	}
	else
	{
		SlowTask.EnterProgressFrame();
	}
	SlowTask.EnterProgressFrame(1.f, LOCTEXT("CreateAssets", "Creating assets"));
	AssetCreator->CreateAssets(Metadata.EntityName);
}
