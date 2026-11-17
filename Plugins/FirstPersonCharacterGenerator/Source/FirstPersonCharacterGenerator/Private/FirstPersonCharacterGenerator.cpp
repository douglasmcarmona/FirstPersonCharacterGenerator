// Copyright (c) 2026 Douglas Mendes Carmona All Rights Reserved.

#include "FirstPersonCharacterGenerator.h"

#include "GameProjectUtils.h"
#include "ToolMenus.h"
#include "WorkspaceMenuStructure.h"
#include "WorkspaceMenuStructureModule.h"
#include "Blueprint/UserWidget.h"
#include "Framework/Commands/UIAction.h"
#include "Interfaces/IPluginManager.h"
#include "Interfaces/DI/FPCharacterGeneratorDIResolver.h"
#include "Styling/SlateStyleRegistry.h"
#include "UI/FPCharacterGeneratorWidget.h"
#include "UI/WidgetController/FPCharacterGeneratorWidgetController.h"

#define LOCTEXT_NAMESPACE "FFirstPersonCharacterGeneratorModule"

const FName FFirstPersonCharacterGeneratorModule::FPCharacterGeneratorTabName(TEXT("First Person Character Generator"));

FFirstPersonCharacterGeneratorModule& FFirstPersonCharacterGeneratorModule::Get()
{
	return FModuleManager::LoadModuleChecked<FFirstPersonCharacterGeneratorModule>("FirstPersonCharacterGenerator");
}

UObject* FFirstPersonCharacterGeneratorModule::GetDIResolver()
{
	if (!DIResolver)
	{
		DIResolver = TStrongObjectPtr(NewObject<UFPCharacterGeneratorDIResolver>());
	}
	return DIResolver.Get();
}

FModuleContextInfo FFirstPersonCharacterGeneratorModule::GetModuleContextInfo()
{
	if (!ModuleContextInfo)
	{
		TArray<FModuleContextInfo> CurrentModules = GameProjectUtils::GetCurrentProjectModules();	
	
		const FString ProjectName = FApp::GetProjectName();		
		for (const FModuleContextInfo& ModuleContext : CurrentModules)
		{
			if (ModuleContext.ModuleName == ProjectName)
			{
				ModuleContextInfo = MakeShareable(new FModuleContextInfo(ModuleContext));						
				break;
			}
		}
	}
	return *ModuleContextInfo;
}

void FFirstPersonCharacterGeneratorModule::StartupModule()
{
	PluginStyleSet = MakeShared<FSlateStyleSet>(FName("FirstPersonCharacterGeneratorStyle"));
	const FString PluginDirectory = IPluginManager::Get().FindPlugin(TEXT("FirstPersonCharacterGenerator"))->GetBaseDir();
	PluginStyleSet->SetContentRoot(	PluginDirectory / TEXT("Resources"));
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
	
	DIResolver = TStrongObjectPtr<UFPCharacterGeneratorDIResolver>(NewObject<UFPCharacterGeneratorDIResolver>());
	
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
	
	GeneratorWidget->SetWidgetController(NewObject<UFPCharacterGeneratorWidgetController>());
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