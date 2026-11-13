// Copyright (c) 2026 Douglas Mendes Carmona All Rights Reserved.

#include "FirstPersonCharacterGenerator.h"

#include "ToolMenus.h"
#include "WorkspaceMenuStructure.h"
#include "WorkspaceMenuStructureModule.h"
#include "Blueprint/UserWidget.h"
#include "Framework/Commands/UIAction.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleRegistry.h"
#include "UI/FPCharacterGeneratorWidget.h"

#define LOCTEXT_NAMESPACE "FFirstPersonCharacterGeneratorModule"

const FName FFirstPersonCharacterGeneratorModule::FPCharacterGeneratorTabName(TEXT("First Person Character Generator"));

void FFirstPersonCharacterGeneratorModule::StartupModule()
{
	PluginStyleSet = MakeShared<FSlateStyleSet>(FName("FirstPersonCharacterGeneratorStyle"));
	const FString PluginDir = IPluginManager::Get().FindPlugin(TEXT("FirstPersonCharacterGenerator"))->GetBaseDir();
	PluginStyleSet->SetContentRoot(	PluginDir / TEXT("Resources"));
	PluginStyleSet->Set(
		"FirstPersonCharacterGenerator.Icon",
		new FSlateImageBrush(
			PluginStyleSet->RootToContentDir(
				TEXT("Icon128"),
				TEXT(".png")
			),
			FVector2D(16.0f, 16.0f)
		)
	);

	FSlateStyleRegistry::RegisterSlateStyle(*PluginStyleSet);
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(
		FPCharacterGeneratorTabName,
		FOnSpawnTab::CreateRaw(this, &FFirstPersonCharacterGeneratorModule::SpawnGeneratorTab))
	.SetDisplayName(LOCTEXT("FirstPersonCharacterGenerator", "First Person Character Generator"))
	.SetTooltipText(LOCTEXT("FirstPersonCharacterGeneratorTooltip", 
		"Generates a basic first-person character with movement and camera control capabilities (at least)"))
	.SetMenuType(ETabSpawnerMenuType::Enabled)
	.SetIcon(FSlateIcon(PluginStyleSet->GetStyleSetName(), "FirstPersonCharacterGenerator.Icon"))
	.SetGroup(WorkspaceMenu::GetMenuStructure().GetToolsCategory());
}

TSharedRef<SDockTab> FFirstPersonCharacterGeneratorModule::SpawnGeneratorTab(const FSpawnTabArgs& Args)
{
	UClass* GeneratorWidgetClass = LoadClass<UFPCharacterGeneratorWidget>(
		nullptr, TEXT("/FirstPersonCharacterGenerator/FPCGW_Main.FPCGW_Main_C"));	
	if (!GeneratorWidgetClass) return GetErrorTab(Args);
	
	UWorld* World = GEditor->GetEditorWorldContext().World();
	if (!World) return GetErrorTab(Args);
	
	GeneratorWidget = TStrongObjectPtr(
		CreateWidget<UFPCharacterGeneratorWidget>(World, GeneratorWidgetClass));
	if (!GeneratorWidget) return GetErrorTab(Args);
	
	return SNew(SDockTab)
		.TabRole(NomadTab)
		[
			GeneratorWidget->TakeWidget()
		];
}

TSharedRef<SDockTab> FFirstPersonCharacterGeneratorModule::GetErrorTab(const FSpawnTabArgs& Args) const
{
	return SNew(SDockTab).TabRole(NomadTab)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("ErrorText", "Unable to load main widget"))
		];
}

void FFirstPersonCharacterGeneratorModule::ShutdownModule()
{
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(FPCharacterGeneratorTabName);
	FSlateStyleRegistry::UnRegisterSlateStyle(*PluginStyleSet);
	PluginStyleSet.Reset();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FFirstPersonCharacterGeneratorModule, FirstPersonCharacterGenerator)