// Copyright (c) 2026 Douglas Mendes Carmona All Rights Reserved.


#include "Generator/FPCharacterGeneratorAssetCreator.h"

#include "AssetToolsModule.h"
#include "EditorClassUtils.h"
#include "InputAction.h"
#include "InputEditorModule.h"
#include "InputMappingContext.h"
#include "UObject/SavePackage.h"

#define GET_ASSET_TOOLS() \
	FAssetToolsModule& AssetToolsModule = FModuleManager::Get().LoadModuleChecked<FAssetToolsModule>(TEXT("AssetTools")); \
	IAssetTools& AssetTools = AssetToolsModule.Get();

void UFPCharacterGeneratorAssetCreator::CreateAssets(const FString& EntityName)
{
	UInputAction* MoveAction = GetInputAction(TEXT("IA_Move"), EInputActionValueType::Axis2D);
	UInputAction* LookAroundAction = GetInputAction(TEXT("IA_Look"), EInputActionValueType::Axis2D);
	UInputMappingContext* InputMappingContext = GetInputMappingContext(EntityName);
	
	InputMappingContext->Modify();
	FEnhancedActionKeyMapping& W_Mapping = InputMappingContext->MapKey(MoveAction, EKeys::W);
	W_Mapping.Modifiers.Add(NewObject<UInputModifierSwizzleAxis>());
	FEnhancedActionKeyMapping& S_Mapping = InputMappingContext->MapKey(MoveAction, EKeys::S);
	S_Mapping.Modifiers.Add(NewObject<UInputModifierSwizzleAxis>());
	S_Mapping.Modifiers.Add(NewObject<UInputModifierNegate>());
	FEnhancedActionKeyMapping& A_Mapping = InputMappingContext->MapKey(MoveAction, EKeys::A);
	A_Mapping.Modifiers.Add(NewObject<UInputModifierNegate>());
	InputMappingContext->MapKey(MoveAction, EKeys::D);
	FEnhancedActionKeyMapping& MouseXY_Mapping = InputMappingContext->MapKey(LookAroundAction, EKeys::Mouse2D);
	UInputModifierNegate* NegateMouseY = NewObject<UInputModifierNegate>();
	NegateMouseY->bX = false;
	NegateMouseY->bY = true;
	NegateMouseY->bZ = false;
	MouseXY_Mapping.Modifiers.Add(NegateMouseY);
	
	//
	// UPackage* Package = MoveAction->GetPackage();
	// Package->MarkPackageDirty();
	// const FString PackageFilename = FPackageName::LongPackageNameToFilename(Package->GetName(), FPackageName::GetAssetPackageExtension());
	// FSavePackageArgs SaveArgs;	
	// SaveArgs.TopLevelFlags = RF_Public | RF_Standalone;
	// checkf(UPackage::SavePackage(Package, MoveAction, *PackageFilename, SaveArgs), TEXT("Failed to save MoveAction"));
	// checkf(UPackage::SavePackage(Package, InputMappingContext, *PackageFilename, SaveArgs), TEXT("Failed to save InputMappingContext"));	
}

UClass* UFPCharacterGeneratorAssetCreator::GetClassFromHeaderPath(const FString& HeaderPath)
{
	if (!FPaths::FileExists(HeaderPath))
	{
		return nullptr;
	}
	const FString ClassName = FPaths::GetBaseFilename(HeaderPath);
	return FEditorClassUtils::GetClassFromString(ClassName);
}

UInputAction* UFPCharacterGeneratorAssetCreator::GetInputAction(const FString& AssetName, EInputActionValueType ValueType)
{
	GET_ASSET_TOOLS()
	
	UInputAction_Factory* Factory = NewObject<UInputAction_Factory>();
	const FString AssetPath = TEXT("/Game/Input");
	UObject* NewInputAction = AssetTools.CreateAsset(
		AssetName,
		AssetPath,
		UInputAction::StaticClass(),
		Factory
		);
	
	UInputAction* MoveAction = CastChecked<UInputAction>(NewInputAction);
	MoveAction->ValueType = ValueType;
	MoveAction->bTriggerWhenPaused = false;
	return MoveAction;
}

UInputMappingContext* UFPCharacterGeneratorAssetCreator::GetInputMappingContext(const FString& EntityName)
{
	GET_ASSET_TOOLS()
	
	UInputMappingContext_Factory* Factory = NewObject<UInputMappingContext_Factory>();
	const FString AssetPath = TEXT("/Game/Input");
	UObject* NewInputMappingContext = AssetTools.CreateAsset(
		FString::Printf(TEXT("IMC_%sContext"), *EntityName),
		AssetPath,
		UInputMappingContext::StaticClass(),
		Factory);
	
	return CastChecked<UInputMappingContext>(NewInputMappingContext);
}
