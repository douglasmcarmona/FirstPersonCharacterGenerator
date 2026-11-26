// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/FPCharacterGeneratorTypes.h"
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
	UFUNCTION(BlueprintCallable, Category="First person character generator")
	void InitDefaultPaths();
	void SetWidgetController(UObject* InWidgetController);
	
protected:
	UFUNCTION(BlueprintPure, Category="First person character generator")
	FString GetFullFilePath(const FString& EntityName, EClass Class, EClassFileType ClassFileType);
	
	UFUNCTION(BlueprintCallable, Category="First person character generator")
	bool ValidateEntityName(const FString& EntityName, FText& ErrorMessage);
	
	/**
	 * Initial header path. Defaults to MainModule/Public
	 */
	UPROPERTY(BlueprintReadOnly, Category="First person character generator")
	FString DefaultHeaderPath;
	
	/**
	 * Initial source path. Defaults to MainModule/Private
	 */
	UPROPERTY(BlueprintReadOnly, Category="First person character generator")
	FString DefaultSourcePath;

	/**
	 * Initial entity name
	 */
	UPROPERTY(BlueprintReadOnly, Category="First person character generator")
	FString DefaultEntityName = FString("MyFP");
	
	UPROPERTY(BlueprintReadOnly, Category="First person character generator")
	TObjectPtr<UObject> WidgetController;
};
