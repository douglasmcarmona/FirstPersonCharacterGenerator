// Copyright (c) 2026 Douglas Mendes Carmona All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/ProjectReloaderInterface.h"
#include "UObject/Object.h"
#include "FPCharacterGeneratorProjectReloader.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTPERSONCHARACTERGENERATOR_API UFPCharacterGeneratorProjectReloader : public UObject, public IProjectReloaderInterface
{
	GENERATED_BODY()

public:
	virtual bool ReloadProject(FText& ErrorMessage) override;
	
};
