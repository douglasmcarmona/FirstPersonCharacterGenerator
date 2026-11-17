// Copyright (c) 2026 Douglas Mendes Carmona All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

struct FModuleContextInfo;
class UFPCharacterGeneratorDIResolver;
class UFPCharacterGeneratorWidget;

class FFirstPersonCharacterGeneratorModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	static FFirstPersonCharacterGeneratorModule& Get();
	
	UObject* GetDIResolver();
	FModuleContextInfo GetModuleContextInfo();
	
private:
	TSharedRef<SDockTab> SpawnGeneratorTab(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> GetErrorTab(const FSpawnTabArgs& Args) const;
	
	static const FName FPCharacterGeneratorTabName;
	TSharedPtr<FSlateStyleSet> PluginStyleSet;
	TSharedPtr<FModuleContextInfo> ModuleContextInfo;
	TStrongObjectPtr<UFPCharacterGeneratorWidget> GeneratorWidget;
	TStrongObjectPtr<UFPCharacterGeneratorDIResolver> DIResolver;
};
