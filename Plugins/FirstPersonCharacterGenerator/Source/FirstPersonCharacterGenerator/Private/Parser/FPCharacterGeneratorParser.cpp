// Copyright (c) 2026 Douglas Mendes Carmona All Rights Reserved.

#include "Parser/FPCharacterGeneratorParser.h"

#include "FirstPersonCharacterGenerator.h"
#include "GeneralProjectSettings.h"
#include "Types/FPCharacterGeneratorTypes.h"

#define LOAD_TEMPLATE(TemplateType) \
FString TemplateType##Template; \
if (!FFileHelper::LoadFileToString(TemplateType##Template, *FFPCharacterGeneratorGlobals::TemplateType##Template)) \
{ \
ErrorMessage = FText::FromString("Unable to read " #TemplateType " template file"); \
return false; \
}

bool UFPCharacterGeneratorParser::Parse(const FFPCharacterGeneratorMetadata& Metadata,
                                        TMap<FString, FString>& OutGeneratedFiles, FText& ErrorMessage)
{
	const FModuleContextInfo& ModuleContextInfo = FFirstPersonCharacterGeneratorModule::Get().GetModuleContextInfo();
	// Read templates
	LOAD_TEMPLATE(CharacterHeader)
	LOAD_TEMPLATE(CharacterSource)
	LOAD_TEMPLATE(PlayerControllerHeader)
	LOAD_TEMPLATE(PlayerControllerSource)
	
	// parse
	const FString& CharacterHeader = ParseCharacterHeader(Metadata, CharacterHeaderTemplate, ModuleContextInfo);
	const FString& CharacterSource = ParseCharacterSource(Metadata, CharacterSourceTemplate);
	const FString& PlayerControllerHeader = ParsePlayerControllerHeader(Metadata, PlayerControllerHeaderTemplate, ModuleContextInfo);
	const FString& PlayerControllerSource = ParsePlayerControllerSource(Metadata, PlayerControllerSourceTemplate);
	OutGeneratedFiles.Add(FString("CharacterHeader"), CharacterHeader);
	OutGeneratedFiles.Add(FString("CharacterSource"), CharacterSource);
	OutGeneratedFiles.Add(FString("PlayerControllerHeader"), PlayerControllerHeader);
	OutGeneratedFiles.Add(FString("PlayerControllerSource"), PlayerControllerSource);
	
	// create header and source files
	// const FString CharacterFileSuffix = Metadata.EntityName + "Character";
	// const FString PlayerControllerSuffix = Metadata.EntityName + "PlayerController";
	
	return true;
}

FString UFPCharacterGeneratorParser::ParseCharacterHeader(const FFPCharacterGeneratorMetadata& Metadata,
                                                           const FString& CharacterHeaderTemplate, const FModuleContextInfo& ModuleContextInfo) const
{
	FString Result = CharacterHeaderTemplate.Replace(TEXT("%COPYRIGHT_LINE%"), *MakeCopyrightLine());
	Result = Result.Replace(TEXT("%UNPREFIXED_CLASS_NAME%"), *(Metadata.EntityName + "Character"));
	Result = Result.Replace(TEXT("%CLASS_MODULE_API_MACRO%"),*(ModuleContextInfo.ModuleName.ToUpper() + "_API"));
	Result = Result.Replace(TEXT("%PREFIXED_CLASS_NAME%"), *("A" + Metadata.EntityName + "Character"));
	return Result;
}

FString UFPCharacterGeneratorParser::ParseCharacterSource(const FFPCharacterGeneratorMetadata& Metadata, const FString& CharacterSourceTemplate) const
{
	FString Result = CharacterSourceTemplate.Replace(TEXT("%COPYRIGHT_LINE%"), *MakeCopyrightLine());
	Result = Result.Replace(TEXT("%MY_HEADER_INCLUDE_DIRECTIVE%"), *("#include " + Metadata.EntityName + "Character.h"));
	Result = Result.Replace(TEXT("%PREFIXED_CLASS_NAME%"), *("A" + Metadata.EntityName + "Character"));	
	return Result;
}

FString UFPCharacterGeneratorParser::ParsePlayerControllerHeader(const FFPCharacterGeneratorMetadata& Metadata,
	const FString& PlayerControllerHeaderTemplate, const FModuleContextInfo& ModuleContextInfo) const
{
	FString Result = PlayerControllerHeaderTemplate.Replace(TEXT("%COPYRIGHT_LINE%"), *MakeCopyrightLine());
	Result = Result.Replace(TEXT("%CLASS_MODULE_API_MACRO%"),*(ModuleContextInfo.ModuleName.ToUpper() + "_API"));
	Result = Result.Replace(TEXT("%UNPREFIXED_CLASS_NAME%"), *(Metadata.EntityName + "PlayerController"));
	Result = Result.Replace(TEXT("%PREFIXED_CLASS_NAME%"), *("A" + Metadata.EntityName + "PlayerController"));
	Result = Result.Replace(TEXT("%INPUT_ACTION_PROPERTIES%"), *MakeInputActionProperties());
	Result = Result.Replace(TEXT("%INPUT_ACTION_BOUND_FUNCTION_DECLARATIONS%"), *MakeInputActionBoundFunctionDeclarations());	
	return Result;
}

FString UFPCharacterGeneratorParser::ParsePlayerControllerSource(const FFPCharacterGeneratorMetadata& Metadata,
	const FString& PlayerControllerSourceTemplate) const
{
	const FString EnhancedInputIncludes = TEXT(
		"#include \"EnhancedInputComponent.h\"\n"
		"#include \"EnhancedInputSubsystems.h\"\n"
		"#include \"InputActionValue.h\"\n");
	const FString CharacterIncludes = TEXT(
		"#include \"GameFramework/Character.h\"\n"
		"#include \"GameFramework/CharacterMovementComponent.h\"\n");
	
	FString Result = PlayerControllerSourceTemplate.Replace(TEXT("%COPYRIGHT_LINE%"), *MakeCopyrightLine());	
	Result = Result.Replace(TEXT("%ENHANCED_INPUT_INCLUDE_DIRECTIVES%"), *EnhancedInputIncludes);
	Result = Result.Replace(TEXT("%CHARACTER_INCLUDE_DIRECTIVES%"), *CharacterIncludes);
	Result = Result.Replace(TEXT("%MY_HEADER_INCLUDE_DIRECTIVE%"), *("#include " + Metadata.EntityName + "PlayerController.h" ));
	Result = Result.Replace(TEXT("%PREFIXED_CLASS_NAME%"), *("A" + Metadata.EntityName + "PlayerController"));
	Result = Result.Replace(TEXT("%SETUP_PLAYER_INPUT_COMPONENT%"), *MakeSetupPlayerInputComponent("A" + Metadata.EntityName + "PlayerController"));
	Result = Result.Replace(TEXT("%INPUT_ACTION_BOUND_FUNCTION_DEFINITIONS%"), *MakeInputActionBoundFunctionDefinitions("A" + Metadata.EntityName + "PlayerController"));	
	return Result;
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
	"void %PREFIXED_CLASS_NAME%::Move(const FInputActionValue& Value)\n"
	"{\n"
	"    const FVector2D MovementVector = Value.Get<FVector2D>();\n"
	"    APawn* ControlledPawn = GetPawn();\n"
	"\n"
	"    if (!ControlledPawn)\n"
	"    {\n"
	"        return;\n"
	"    }\n"
	"\n"
	"    const FRotator ControlRotation = GetControlRotation();\n"
	"    const FRotator YawRotation(0.0f, ControlRotation.Yaw, 0.0f);\n"
	"\n"
	"    const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);\n"
	"    const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);\n"
	"\n"
	"    ControlledPawn->AddMovementInput(ForwardDirection, MovementVector.Y);\n"
	"    ControlledPawn->AddMovementInput(RightDirection, MovementVector.X);\n"
	"}\n\n");
	
	FString LookFunctionCode = TEXT(
	"void %PREFIXED_CLASS_NAME%::Look(const FInputActionValue& Value)\n"
	"{\n"
	"    const FVector2D LookAxisVector = Value.Get<FVector2D>();\n"
	"\n"
	"    AddYawInput(LookAxisVector.X);\n"
	"    AddPitchInput(LookAxisVector.Y);\n"
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
	"    UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);\n"
	"\n"
	"    if (EnhancedInputComponent && MoveAction && LookAroundAction)\n"
	"    {\n"
	"        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &%PREFIXED_CLASS_NAME%::Move);\n"
	"        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &%PREFIXED_CLASS_NAME%::LookAround);\n"
	"    }\n"	
	"}");
	return Result.Replace(TEXT("%PREFIXED_CLASS_NAME%"), *PrefixedClassName);
}