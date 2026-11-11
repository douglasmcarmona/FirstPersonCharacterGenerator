// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FFirstPersonCharacterGeneratorModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
private:
	TSharedRef<SDockTab> SpawnGeneratorTab(const FSpawnTabArgs& Args);
	static const FName FPCharacterGeneratorTabName;
	TSharedPtr<FSlateStyleSet> PluginStyleSet; 
};
