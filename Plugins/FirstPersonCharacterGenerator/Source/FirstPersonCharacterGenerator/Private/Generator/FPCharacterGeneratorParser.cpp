// Copyright (c) 2026 Douglas Mendes Carmona All Rights Reserved.

#include "Generator/FPCharacterGeneratorParser.h"
#include "FirstPersonCharacterGenerator.h"
#include "GeneralProjectSettings.h"
#include "Types/FPCharacterGeneratorTypes.h"

UFPCharacterGeneratorParser::UFPCharacterGeneratorParser()
{
	Handlers.Add(TEXT("CharacterHeader"), &UFPCharacterGeneratorParser::ParseCharacterHeader);
	Handlers.Add(TEXT("CharacterSource"), &UFPCharacterGeneratorParser::ParseCharacterSource);
	Handlers.Add(TEXT("PlayerControllerHeader"), &UFPCharacterGeneratorParser::ParsePlayerControllerHeader);
	Handlers.Add(TEXT("PlayerControllerSource"), &UFPCharacterGeneratorParser::ParsePlayerControllerSource);
	
	ModuleNameString = FFirstPersonCharacterGeneratorModule::Get().GetModuleContextInfo().ModuleName.ToUpper();
}

void UFPCharacterGeneratorParser::Parse(FFPCharacterGeneratorMetadata& Metadata)
{
	for (FGeneratedFile& GeneratedFile : Metadata.GeneratedFiles)
	{
		ParseClassFile(Metadata.EntityName, GeneratedFile);
	}
}

void UFPCharacterGeneratorParser::ParseClassFile(const FString& EntityName, FGeneratedFile& GeneratedFile)
{
	if (FHandler* Handler = Handlers.Find(GeneratedFile.ClassFileTypeId))
	{
		(this->*(*Handler))(EntityName, GeneratedFile);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to find function to parse %s"), *GeneratedFile.ClassFileTypeId);
	}	
}

void UFPCharacterGeneratorParser::ParseCharacterHeader(const FString& EntityName, FGeneratedFile& GeneratedFile) const
{
	FString Template;
	FFileHelper::LoadFileToString(Template, *FFPCharacterGeneratorGlobals::CharacterHeaderTemplate);
	FString Result = Template.Replace(TEXT("%COPYRIGHT_LINE%"), *MakeCopyrightLine());
	Result = Result.Replace(TEXT("%UNPREFIXED_CLASS_NAME%"), *(EntityName + "Character"));
	Result = Result.Replace(TEXT("%CLASS_MODULE_API_MACRO%"),*(ModuleNameString + "_API"));
	Result = Result.Replace(TEXT("%PREFIXED_CLASS_NAME%"), *("A" + EntityName + "Character"));
	GeneratedFile.FileContent = Result;
}

void UFPCharacterGeneratorParser::ParseCharacterSource(const FString& EntityName, FGeneratedFile& GeneratedFile) const
{
	FString Template;
	FFileHelper::LoadFileToString(Template, *FFPCharacterGeneratorGlobals::CharacterSourceTemplate);
	FString Result = Template.Replace(TEXT("%COPYRIGHT_LINE%"), *MakeCopyrightLine());
	Result = Result.Replace(TEXT("%MY_HEADER_INCLUDE_DIRECTIVE%"), *("#include \"Character/"+ EntityName + "Character.h\""));
	Result = Result.Replace(TEXT("%PREFIXED_CLASS_NAME%"), *("A" + EntityName + "Character"));	
	GeneratedFile.FileContent = Result;
}

void UFPCharacterGeneratorParser::ParsePlayerControllerHeader(const FString& EntityName, FGeneratedFile& GeneratedFile) const
{
	FString Template;
	FFileHelper::LoadFileToString(Template, *FFPCharacterGeneratorGlobals::PlayerControllerHeaderTemplate);
	FString Result = Template.Replace(TEXT("%COPYRIGHT_LINE%"), *MakeCopyrightLine());
	Result = Result.Replace(TEXT("%CLASS_MODULE_API_MACRO%"),*(ModuleNameString + "_API"));
	Result = Result.Replace(TEXT("%UNPREFIXED_CLASS_NAME%"), *(EntityName + "PlayerController"));
	Result = Result.Replace(TEXT("%PREFIXED_CLASS_NAME%"), *("A" + EntityName + "PlayerController"));
	Result = Result.Replace(TEXT("%INPUT_ACTION_PROPERTIES%"), *MakeInputActionProperties());
	Result = Result.Replace(TEXT("%INPUT_ACTION_BOUND_FUNCTION_DECLARATIONS%"), *MakeInputActionBoundFunctionDeclarations());	
	GeneratedFile.FileContent = Result;
}

void UFPCharacterGeneratorParser::ParsePlayerControllerSource(const FString& EntityName, FGeneratedFile& GeneratedFile) const
{
	FString Template;
	FFileHelper::LoadFileToString(Template, *FFPCharacterGeneratorGlobals::PlayerControllerSourceTemplate);
	const FString EnhancedInputIncludes = TEXT(
		"#include \"EnhancedInputComponent.h\"\n"
		"#include \"EnhancedInputSubsystems.h\"\n"
		"#include \"InputActionValue.h\"\n");
	
	FString Result = Template.Replace(TEXT("%COPYRIGHT_LINE%"), *MakeCopyrightLine());
	Result = Result.Replace(TEXT("%MY_HEADER_INCLUDE_DIRECTIVE%"), *("#include \"PlayerController/" + EntityName + "PlayerController.h\"" ));
	Result = Result.Replace(TEXT("%ENHANCED_INPUT_INCLUDE_DIRECTIVES%"), *EnhancedInputIncludes);		
	Result = Result.Replace(TEXT("%PREFIXED_CLASS_NAME%"), *("A" + EntityName + "PlayerController"));
	Result = Result.Replace(TEXT("%SETUP_PLAYER_INPUT_COMPONENT%"), *MakeSetupPlayerInputComponent("A" + EntityName + "PlayerController"));
	Result = Result.Replace(TEXT("%INPUT_ACTION_BOUND_FUNCTION_DEFINITIONS%"), *MakeInputActionBoundFunctionDefinitions("A" + EntityName + "PlayerController"));	
	GeneratedFile.FileContent = Result;
}

FString UFPCharacterGeneratorParser::MakeCopyrightLine() const
{
	const FString CopyrightNotice = GetDefault<UGeneralProjectSettings>()->CopyrightNotice;
	if (!CopyrightNotice.IsEmpty())
	{
		return FString(TEXT("// ")) + CopyrightNotice;
	}	
	return FString();
}

FString UFPCharacterGeneratorParser::MakeInputActionBoundFunctionDeclarations() const
{
	const FString MoveFunction = FString("void Move(const FInputActionValue& InputValue);");
	const FString LookAroundFunction = FString("\tvoid LookAround(const FInputActionValue& InputValue);");
	return MoveFunction + "\n" + LookAroundFunction;
}

FString UFPCharacterGeneratorParser::MakeInputActionProperties() const
{
	const FString MoveAction = FString("UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)\n"
									"\tTObjectPtr<UInputAction> MoveAction;");
	const FString LookAroundAction = FString("\tUPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)\n"
										  "\tTObjectPtr<UInputAction> LookAroundAction;");
	return MoveAction + "\n\n" + LookAroundAction;
}

FString UFPCharacterGeneratorParser::MakeInputActionBoundFunctionDefinitions(const FString& PrefixedClassName) const
{
	FString MoveFunctionCode = TEXT(
	"void %PREFIXED_CLASS_NAME%::Move(const FInputActionValue& InputValue)\n"
	"{\n"
	"    const FVector2D MovementVector = InputValue.Get<FVector2D>();\n"
	"    APawn* ControlledPawn = GetPawn();\n"
	"\n"
	"    if (!ControlledPawn)\n"
	"    {\n"
	"        return;\n"
	"    }\n"
	"\n"	
	"    const FRotator YawRotation(0.0f, GetControlRotation().Yaw, 0.0f);\n"
	"\n"
	"    const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);\n"
	"    const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);\n"
	"\n"
	"    ControlledPawn->AddMovementInput(ForwardDirection, MovementVector.Y);\n"
	"    ControlledPawn->AddMovementInput(RightDirection, MovementVector.X);\n"
	"}\n\n");
	
	FString LookFunctionCode = TEXT(
	"void %PREFIXED_CLASS_NAME%::LookAround(const FInputActionValue& InputValue)\n"
	"{\n"
	"    const FVector2D LookAxisVector = InputValue.Get<FVector2D>();\n"
	"\n"
	"    AddYawInput(LookAxisVector.X * CameraMovementSensitivity);\n"
	"    AddPitchInput(LookAxisVector.Y * CameraMovementSensitivity);\n"
	"}\n");
	
	return (MoveFunctionCode + LookFunctionCode).Replace(TEXT("%PREFIXED_CLASS_NAME%"), *PrefixedClassName);
}

FString UFPCharacterGeneratorParser::MakeSetupPlayerInputComponent(const FString& PrefixedClassName) const
{
	FString Result = TEXT(
	"void %PREFIXED_CLASS_NAME%::SetupInputComponent()\n"
	"{\n"
	"    Super::SetupInputComponent();\n"
	"\n"
	"    UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);\n"
	"\n"
	"    if (EnhancedInputComponent && MoveAction && LookAroundAction)\n"
	"    {\n"
	"        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &%PREFIXED_CLASS_NAME%::Move);\n"
	"        EnhancedInputComponent->BindAction(LookAroundAction, ETriggerEvent::Triggered, this, &%PREFIXED_CLASS_NAME%::LookAround);\n"
	"    }\n"	
	"}");
	return Result.Replace(TEXT("%PREFIXED_CLASS_NAME%"), *PrefixedClassName);
}
