// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FPCharacterGeneratorWidget.generated.h"

struct FModuleContextInfo;

/**
 * Called each time path and entity name content change to validate them
 * @param bIsValid True if the resulting file names are valid. False otherwise
 * @param ErrorMessage The reason the content has been considered invalid
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFileNameValidationSignature, bool const, bIsValid, const FText&, ErrorMessage);
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
	 * Updates the calculated names for the resulting header(.h) and source(.cpp) files
	 * @param EntityName Used as the file name
	 * @param Path Subfolder path structure where the files must follow
	 * @param HeaderFile Output parameter with the resulting header file name
	 * @param SourceFile Output parameter with the resulting source file name
	 */
	UFUNCTION(BlueprintCallable)
	void CalculateFileNames(const FString& EntityName, const FString& Path, FString& HeaderFile, FString& SourceFile) const;

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
	
	UPROPERTY(BlueprintAssignable)
	FOnFileNameValidationSignature OnFileNameValidation;

	/**
	 * Gives the plugin context to calculate the destination of source and header files
	 */
	TSharedPtr<FModuleContextInfo> ModuleContextInfo;
};
