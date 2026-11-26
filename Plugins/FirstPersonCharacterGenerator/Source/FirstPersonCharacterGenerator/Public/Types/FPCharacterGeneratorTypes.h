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
		if (DefaultHeaderPath == DefaultSourcePath)
		{
			DefaultHeaderPath /= FString("Public");
			DefaultSourcePath /= FString("Private");
		}
	}
	
};

USTRUCT(Blueprintable, BlueprintType, Category="First person character generator")
struct FFPCharacterGeneratorFeatures
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, Category="First person character generator|Features")
	bool bHasMovement = true;
	
	UPROPERTY(BlueprintReadWrite, Category="First person character generator|Features")
	bool bHasCameraControl = true;
	
	UPROPERTY(BlueprintReadWrite, Category="First person character generator|Features")
	bool bOpenIDE = false;
};

USTRUCT(Blueprintable, BlueprintType, Category="First person character generator")
struct FGeneratedFile
{
	GENERATED_BODY();
	
	UPROPERTY(BlueprintReadWrite, Category="Generated file")
	FString ClassFileTypeId;
	
	UPROPERTY(BlueprintReadWrite, Category="Generated file")
	FString FullFileName;
	
	FString FileContent;
};

USTRUCT(Blueprintable, BlueprintType, Category="First person character generator")
struct FFPCharacterGeneratorMetadata
{
	GENERATED_BODY();
	
	UPROPERTY(BlueprintReadWrite, Category="Metadata")
	FString EntityName;
	
	UPROPERTY(BlueprintReadWrite, Category="Metadata")
	TArray<FGeneratedFile> GeneratedFiles;
	
	UPROPERTY(BlueprintReadWrite, Category="Metadata")
	FFPCharacterGeneratorFeatures Features;
};

UENUM(BlueprintType, Category="First person character generator")
enum class EClass : uint8
{
	Character UMETA(DisplayName = "Character"),
	PlayerController UMETA(DisplayName = "PlayerController"),
};

UENUM(BlueprintType, Category="First person character generator")
enum class EClassFileType : uint8
{
	Header UMETA(DisplayName = "Header"),
	Source UMETA(DisplayName = "Source")
};
