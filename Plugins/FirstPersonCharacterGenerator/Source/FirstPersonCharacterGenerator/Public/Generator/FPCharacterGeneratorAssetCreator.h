// Copyright (c) 2026 Douglas Mendes Carmona All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Interfaces/AssetCreatorInterface.h"
#include "UObject/Object.h"
#include "FPCharacterGeneratorAssetCreator.generated.h"

class UInputAction;
class UInputMappingContext;

UCLASS()
class FIRSTPERSONCHARACTERGENERATOR_API UFPCharacterGeneratorAssetCreator : public UObject, public IAssetCreatorInterface
{
	GENERATED_BODY()
	
public:
	virtual void CreateAssets(const FString& EntityName) override;
	
private:
	UClass* GetClassFromHeaderPath(const FString& HeaderPath);
	UInputAction* GetInputAction(const FString& AssetName, EInputActionValueType ValueType);	
	UInputMappingContext* GetInputMappingContext(const FString& EntityName);
};
