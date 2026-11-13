// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FPCharacterGeneratorWidget.generated.h"

struct FModuleContextInfo;
/**
 * Our plugin's main window. Entry point to the designer's settings used to create all classes and assets later on
 */
UCLASS()
class FIRSTPERSONCHARACTERGENERATOR_API UFPCharacterGeneratorWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFPCharacterGeneratorWidget(const FObjectInitializer& ObjectInitializer);
	
protected:
	/**
	 * Updates the UI after assembling and validating incoming data
	 * @param EntityName Used as the file name
	 * @param Path Subfolder path structure which the files must follow
	 * @param HeaderFile Output parameter with the resulting header file name
	 * @param SourceFile Output parameter with the resulting source file name
	 * @param bIsDataValid True if file name and path validation succeeded. False otherwise
	 * @param ErrorMessage Reason why the validation didn't succeed
	 */
	UFUNCTION(BlueprintCallable)
	void ValidateData(const FString& EntityName, 
		const FString& Path, 
		FString& HeaderFile, 
		FString& SourceFile,
		bool& bIsDataValid,
		FText& ErrorMessage) const;

	/**
	 * Initial header path. Defaults to MainModule/Public
	 */
	UPROPERTY(BlueprintReadOnly)
	FString DefaultHeaderPath;
	
	/**
	 * Initial source path. Defaults to MainModule/Private
	 */
	UPROPERTY(BlueprintReadOnly)
	FString DefaultSourcePath;

	/**
	 * Initial entity name
	 */
	UPROPERTY(BlueprintReadOnly)
	FString DefaultEntityName = FString("MyFPCharacter");
	
	/**
	 * Gives the plugin context to calculate the destination of source and header files
	 */
	TSharedPtr<FModuleContextInfo> ModuleContextInfo;
};
