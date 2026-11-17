// Copyright (c) 2026 Douglas Mendes Carmona All Rights Reserved.


#include "Interfaces/DI/FPCharacterGeneratorDIResolver.h"

#include "Parser/FPCharacterGeneratorParser.h"

UFPCharacterGeneratorDIResolver::UFPCharacterGeneratorDIResolver()
{
	Parser = NewObject<UFPCharacterGeneratorParser>();
}

UObject* UFPCharacterGeneratorDIResolver::GetParser()
{	
	return Parser;
}
