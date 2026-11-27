// Copyright (c) 2026 Douglas Mendes Carmona All Rights Reserved.


#include "Interfaces/DI/FPCharacterGeneratorDIResolver.h"

#include "Generator/FPCharacterGeneratorAssetCreator.h"
#include "Generator/FPCharacterGeneratorFileSystem.h"
#include "Generator/FPCharacterGeneratorParser.h"
#include "Generator/FPCharacterGeneratorProjectReloader.h"

UFPCharacterGeneratorDIResolver::UFPCharacterGeneratorDIResolver()
{
	Parser = NewObject<UFPCharacterGeneratorParser>();
	FileSystem = NewObject<UFPCharacterGeneratorFileSystem>();
	ProjectReloader = NewObject<UFPCharacterGeneratorProjectReloader>();
	AssetCreator = NewObject<UFPCharacterGeneratorAssetCreator>();
}

UObject* UFPCharacterGeneratorDIResolver::GetParser()
{	
	return Parser;
}

UObject* UFPCharacterGeneratorDIResolver::GetFileSystem()
{
	return FileSystem;
}

UObject* UFPCharacterGeneratorDIResolver::GetProjectReloader()
{
	return ProjectReloader;
}

UObject* UFPCharacterGeneratorDIResolver::GetAssetCreator()
{
	return AssetCreator;
}
