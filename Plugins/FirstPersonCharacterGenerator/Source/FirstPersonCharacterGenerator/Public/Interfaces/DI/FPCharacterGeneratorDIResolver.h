// Copyright (c) 2026 Douglas Mendes Carmona All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "FPCharacterGeneratorDIResolver.generated.h"

UCLASS()
class FIRSTPERSONCHARACTERGENERATOR_API UFPCharacterGeneratorDIResolver : public UObject
{
	GENERATED_BODY()
	
public:
	UFPCharacterGeneratorDIResolver();
	UObject* GetParser();
	
private:
	UPROPERTY()
	TObjectPtr<UObject> Parser;
};
