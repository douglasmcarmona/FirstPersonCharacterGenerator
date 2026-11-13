// Copyright (c) 2026 Douglas Mendes Carmona All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class UFPCharacterGeneratorWidget;

class FFirstPersonCharacterGeneratorModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
private:
	TSharedRef<SDockTab> SpawnGeneratorTab(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> GetErrorTab(const FSpawnTabArgs& Args) const;
	
	static const FName FPCharacterGeneratorTabName;
	TSharedPtr<FSlateStyleSet> PluginStyleSet;
	TStrongObjectPtr<UFPCharacterGeneratorWidget> GeneratorWidget;	
};
