#pragma once

#include "CoreMinimal.h"
#include "FirstPersonCharacterGenerator.h"
#include "GameProjectUtils.h"
#include "Interfaces/IPluginManager.h"
#include "FPCharacterGeneratorTypes.generated.h"
 
namespace FFPCharacterGeneratorGlobals
{
	static FString PluginDir = IPluginManager::Get().FindPlugin(TEXT("FirstPersonCharacterGenerator"))->GetBaseDir();
	static FString CharacterHeaderTemplate = PluginDir + FString("/Resources/Templates/Header/CharacterClass.h.template");
	static FString CharacterSourceTemplate = PluginDir + FString("/Resources/Templates/Source/CharacterClass.cpp.template");
	static FString PlayerControllerSourceTemplate = PluginDir + FString("/Resources/Templates/Source/PlayerControllerClass.cpp.template");
	static FString PlayerControllerHeaderTemplate = PluginDir + FString("/Resources/Templates/Header/PlayerControllerClass.h.template");
	static FString CharacterString = FString("Character");
	static FString PlayerControllerString = FString("PlayerController");
	static FString HeaderExtension = FString(".h");
	static FString SourceExtension = FString(".cpp");	
	inline FString DefaultHeaderPath;
	inline FString DefaultSourcePath;	
	inline void InitDefaultPaths()
	{
		const FModuleContextInfo& ModuleContextInfo = FFirstPersonCharacterGeneratorModule::Get().GetModuleContextInfo();
		GameProjectUtils::CalculateSourcePaths(
			ModuleContextInfo.ModuleSourcePath,
			ModuleContextInfo,
			DefaultHeaderPath, 
			DefaultSourcePath);
	}
	
};

USTRUCT(Blueprintable, BlueprintType)
struct FFPCharacterGeneratorFeatures
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	bool bHasMovement = true;
	
	UPROPERTY(BlueprintReadWrite)
	bool bHasCameraControl = true;
};

USTRUCT(Blueprintable, BlueprintType)
struct FFPCharacterGeneratorMetadata
{
	GENERATED_BODY();
	
	UPROPERTY(BlueprintReadWrite)
	FString EntityName;
	
	UPROPERTY(BlueprintReadWrite)
	FString HeaderPath;
	
	UPROPERTY(BlueprintReadWrite)
	FString SourcePath;
	
	UPROPERTY(BlueprintReadWrite)
	FFPCharacterGeneratorFeatures Features;
};
