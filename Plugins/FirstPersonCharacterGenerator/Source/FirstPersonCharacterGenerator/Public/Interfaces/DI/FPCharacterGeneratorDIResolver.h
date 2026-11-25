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
	UObject* GetFileSystem();
	UObject* GetProjectReloader();
	
private:
	UPROPERTY()
	TObjectPtr<UObject> Parser;
	
	UPROPERTY()
	TObjectPtr<UObject> FileSystem;
	
	UPROPERTY()
	TObjectPtr<UObject> ProjectReloader;
};
