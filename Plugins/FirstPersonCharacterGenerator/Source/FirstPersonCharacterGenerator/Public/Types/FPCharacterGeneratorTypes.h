#pragma once

#include "CoreMinimal.h"
#include "FPCharacterGeneratorTypes.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct FFPCharacterGeneratorFeatures
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	bool bHasMovement = true;
	
	UPROPERTY(BlueprintReadWrite)
	bool bHasCameraControl = true;
};

USTRUCT(Blueprintable, BlueprintType)
struct FFPCharacterGeneratorMetadata
{
	GENERATED_BODY();
	
	UPROPERTY(BlueprintReadWrite)
	FString EntityName;
	
	UPROPERTY(BlueprintReadWrite)
	FString HeaderPath;
	
	UPROPERTY(BlueprintReadWrite)
	FString SourcePath;
	
	UPROPERTY(BlueprintReadWrite)
	FFPCharacterGeneratorFeatures Features;
};
