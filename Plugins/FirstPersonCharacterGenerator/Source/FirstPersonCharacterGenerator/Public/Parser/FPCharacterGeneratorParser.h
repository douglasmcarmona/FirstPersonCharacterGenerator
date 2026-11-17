// Copyright (c) 2026 Douglas Mendes Carmona All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/ParserInterface.h"
#include "UObject/Object.h"
#include "FPCharacterGeneratorParser.generated.h"

struct FModuleContextInfo;
struct FFPCharacterGeneratorMetadata; 

UCLASS()
class FIRSTPERSONCHARACTERGENERATOR_API UFPCharacterGeneratorParser : public UObject, public IParserInterface
{
	GENERATED_BODY()
	
public:
	virtual bool Parse(const FFPCharacterGeneratorMetadata& Metadata, TMap<FString, FString>& OutGeneratedFiles, FText& ErrorMessage) override;
	
private:	
	FString MakeCopyrightLine() const;
	FString MakeInputActionBoundFunctionDeclarations() const;
	FString MakeInputActionProperties() const;
	FString MakeInputActionBoundFunctionDefinitions(const FString& PrefixedClassName) const;
	FString MakeSetupPlayerInputComponent(const FString& PrefixedClassName) const;
	FString ParseCharacterHeader(const FFPCharacterGeneratorMetadata& Metadata, const FString& CharacterHeaderTemplate, const FModuleContextInfo& ModuleContextInfo) const;
	FString ParseCharacterSource(const FFPCharacterGeneratorMetadata& Metadata, const FString& CharacterSourceTemplate) const;
	FString ParsePlayerControllerHeader(const FFPCharacterGeneratorMetadata& Metadata, const FString& PlayerControllerHeaderTemplate, const FModuleContextInfo& ModuleContextInfo) const;
	FString ParsePlayerControllerSource(const FFPCharacterGeneratorMetadata& Metadata, const FString& PlayerControllerSourceTemplate) const;	
};
