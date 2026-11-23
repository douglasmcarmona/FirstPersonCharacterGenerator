// Copyright (c) 2026 Douglas Mendes Carmona All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FileSystemInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UFileSystemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FIRSTPERSONCHARACTERGENERATOR_API IFileSystemInterface
{
	GENERATED_BODY()
	
public:
	virtual bool SaveFile(const FString& FullFileName, const FString& FileContent) = 0;
};
