// Copyright (c) 2026 Douglas Mendes Carmona All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ProjectReloaderInterface.generated.h"

UINTERFACE()
class UProjectReloaderInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FIRSTPERSONCHARACTERGENERATOR_API IProjectReloaderInterface
{
	GENERATED_BODY()
	
public:
	virtual bool ReloadProject(FText& ErrorMessage) = 0;
	virtual bool GenerateProjectFiles() = 0;
};
