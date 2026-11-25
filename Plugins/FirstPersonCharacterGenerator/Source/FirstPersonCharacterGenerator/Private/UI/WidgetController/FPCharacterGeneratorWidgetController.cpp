// Copyright (c) 2026 Douglas Mendes Carmona All Rights Reserved.

#include "UI/WidgetController/FPCharacterGeneratorWidgetController.h"

#include "FirstPersonCharacterGenerator.h"
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
	
	FText ErrorMessage;
	Parser->Parse(Metadata);
	
	FScopedSlowTask SlowTask(2.f, LOCTEXT("CreatingFiles", "Creating new entity files..."));
	SlowTask.MakeDialog();
	SlowTask.EnterProgressFrame(1.f, LOCTEXT("SaveFiles","Saving header and source files..."));
	for (const FGeneratedFile& File : Metadata.GeneratedFiles)
	{
		FileSystem->SaveFile(File.FullFileName, File.FileContent);
	}	
	SlowTask.EnterProgressFrame(1.f, LOCTEXT("Compiling","Compiling new C++ classes..."));	
	if (!ProjectReloader->ReloadProject(ErrorMessage))
	{
		FMessageDialog::Open(EAppMsgType::Ok, ErrorMessage);
	}
}
