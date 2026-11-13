// Copyright (c) 2026 Douglas Mendes Carmona All Rights Reserved.


#include "UI/WidgetController/FPCharacterGeneratorWidgetController.h"

void UFPCharacterGeneratorWidgetController::CreateEntity(const FFPCharacterGeneratorMetadata& Metadata)
{
	FString Log = FString::Printf(
		TEXT("Entity: %s\n"
	   "Header Path: %s\n"
		"Source Path: %s\n"
		"Has movement? %i\n"
		"Has camera control? %i\n"),
		*Metadata.EntityName, *Metadata.HeaderPath, *Metadata.SourcePath, Metadata.Features.bHasMovement, Metadata.Features.bHasCameraControl );
	UE_LOG(LogTemp, Display, TEXT("%s"), *Log);
}
