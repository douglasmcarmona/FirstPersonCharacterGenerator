// Copyright (c) 2026 Douglas Mendes Carmona All Rights Reserved.


#include "Generator/FPCharacterGeneratorAssetCreator.h"

#include "AssetToolsModule.h"
#include "EditorClassUtils.h"
#include "InputAction.h"
#include "InputEditorModule.h"
#include "InputMappingContext.h"
#include "Factories/BlueprintFactory.h"
#include "Types/FPCharacterGeneratorTypes.h"

#define GET_ASSET_TOOLS() \
	FAssetToolsModule& AssetToolsModule = FModuleManager::Get().LoadModuleChecked<FAssetToolsModule>(TEXT("AssetTools")); \
	IAssetTools& AssetTools = AssetToolsModule.Get();

void UFPCharacterGeneratorAssetCreator::CreateAssets(const FFPCharacterGeneratorMetadata& Metadata)
{
	UInputAction* MoveAction = GetInputAction(TEXT("IA_Move"), EInputActionValueType::Axis2D);
	UInputAction* LookAroundAction = GetInputAction(TEXT("IA_Look"), EInputActionValueType::Axis2D);
	UInputMappingContext* InputMappingContext = GetInputMappingContext(Metadata.EntityName);
	
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
	GetBlueprint(Metadata, EClass::Character);
	GetBlueprint(Metadata, EClass::PlayerController);		
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

UBlueprint* UFPCharacterGeneratorAssetCreator::GetBlueprint(const FFPCharacterGeneratorMetadata& Metadata, EClass Class)
{
	FString ClassString = UEnum::GetValueAsString(Class).Replace(TEXT("EClass::"),TEXT(""));
	const FString HeaderPath = Metadata.GeneratedFiles.FindByPredicate([ClassString](const FGeneratedFile& File)
	{
		return File.ClassFileTypeId == ClassString + TEXT("Header");
	})->FullFileName;
	
	UClass* ParentClass = GetClassFromHeaderPath(HeaderPath);
	if (!ParentClass) return nullptr;
	
	GET_ASSET_TOOLS()
	
	UBlueprintFactory* Factory = NewObject<UBlueprintFactory>();
	Factory->ParentClass = ParentClass;
	UObject* NewCharacterBP = AssetTools.CreateAsset(
		FString::Printf(TEXT("BP_%s%s"), *Metadata.EntityName, *ClassString),
		FString::Printf(TEXT("/Game/%s"), *ClassString),
		UBlueprint::StaticClass(),
		Factory
	);
	return CastChecked<UBlueprint>(NewCharacterBP);
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
