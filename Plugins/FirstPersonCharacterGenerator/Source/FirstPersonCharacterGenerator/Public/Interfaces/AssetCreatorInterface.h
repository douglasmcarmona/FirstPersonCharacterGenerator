// Copyright (c) 2026 Douglas Mendes Carmona All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AssetCreatorInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UAssetCreatorInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FIRSTPERSONCHARACTERGENERATOR_API IAssetCreatorInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void CreateAssets(const FString& EntityName) = 0;
};
