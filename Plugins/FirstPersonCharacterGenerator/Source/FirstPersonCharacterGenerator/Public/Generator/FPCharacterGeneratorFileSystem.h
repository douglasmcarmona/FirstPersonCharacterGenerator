// Copyright (c) 2026 Douglas Mendes Carmona All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/FileSystemInterface.h"
#include "UObject/Object.h"
#include "FPCharacterGeneratorFileSystem.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTPERSONCHARACTERGENERATOR_API UFPCharacterGeneratorFileSystem : public UObject, public IFileSystemInterface
{
	GENERATED_BODY()
	
public:
	virtual bool SaveFile(const FString& FullFileName, const FString& FileContent) override;
};
