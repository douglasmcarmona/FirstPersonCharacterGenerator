// Copyright (c) 2026 Douglas Mendes Carmona All Rights Reserved.


#include "Interfaces/DI/FPCharacterGeneratorDIResolver.h"

#include "Generator/FPCharacterGeneratorFileSystem.h"
#include "Generator/FPCharacterGeneratorParser.h"

UFPCharacterGeneratorDIResolver::UFPCharacterGeneratorDIResolver()
{
	Parser = NewObject<UFPCharacterGeneratorParser>();
	FileSystem = NewObject<UFPCharacterGeneratorFileSystem>();
}

UObject* UFPCharacterGeneratorDIResolver::GetParser()
{	
	return Parser;
}

UObject* UFPCharacterGeneratorDIResolver::GetFileSystem()
{
	return FileSystem;
}
