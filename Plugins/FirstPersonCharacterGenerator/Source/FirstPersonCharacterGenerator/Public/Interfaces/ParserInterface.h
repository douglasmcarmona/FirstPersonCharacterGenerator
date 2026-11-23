// Copyright (c) 2026 Douglas Mendes Carmona All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "../Public/Types/FPCharacterGeneratorTypes.h"
#include "ParserInterface.generated.h"

struct FFPCharacterGeneratorMetadata;

// This class does not need to be modified.
UINTERFACE()
class UParserInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FIRSTPERSONCHARACTERGENERATOR_API IParserInterface
{
	GENERATED_BODY()
	
public:
	virtual void Parse(FFPCharacterGeneratorMetadata& Metadata) = 0;
};
