// Copyright (c) 2026 Douglas Mendes Carmona All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Types/FPCharacterGeneratorTypes.h"
#include "FPCharacterGeneratorWidgetController.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class FIRSTPERSONCHARACTERGENERATOR_API UFPCharacterGeneratorWidgetController : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void CreateEntity(const FFPCharacterGeneratorMetadata& Metadata);
};
