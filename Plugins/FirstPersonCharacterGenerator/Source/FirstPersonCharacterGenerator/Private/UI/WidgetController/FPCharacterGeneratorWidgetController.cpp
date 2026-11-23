// Copyright (c) 2026 Douglas Mendes Carmona All Rights Reserved.

#include "UI/WidgetController/FPCharacterGeneratorWidgetController.h"

#include "FirstPersonCharacterGenerator.h"
#include "Interfaces/FileSystemInterface.h"
#include "Interfaces/ParserInterface.h"
#include "Interfaces/DI/FPCharacterGeneratorDIResolver.h"

void UFPCharacterGeneratorWidgetController::CreateEntity(FFPCharacterGeneratorMetadata& Metadata)
{
	UFPCharacterGeneratorDIResolver* DIResolver = Cast<UFPCharacterGeneratorDIResolver>(FFirstPersonCharacterGeneratorModule::Get().GetDIResolver());
	IParserInterface* Parser = Cast<IParserInterface>(DIResolver->GetParser());
	IFileSystemInterface* FileSystem = Cast<IFileSystemInterface>(DIResolver->GetFileSystem());	
	FText ErrorMessage;
	Parser->Parse(Metadata);	
	
	for (const FGeneratedFile& File : Metadata.GeneratedFiles)
	{
		FileSystem->SaveFile(File.FullFileName, File.FileContent);
	}
}
