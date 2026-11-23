// Copyright (c) 2026 Douglas Mendes Carmona All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/ParserInterface.h"
#include "Types/FPCharacterGeneratorTypes.h"
#include "UObject/Object.h"
#include "FPCharacterGeneratorParser.generated.h"

struct FModuleContextInfo;
struct FFPCharacterGeneratorMetadata; 

UCLASS()
class FIRSTPERSONCHARACTERGENERATOR_API UFPCharacterGeneratorParser : public UObject, public IParserInterface
{
	GENERATED_BODY()
	
public:
	virtual void Parse(FFPCharacterGeneratorMetadata& Metadata) override;
	
private:
	UFPCharacterGeneratorParser();
	FString MakeCopyrightLine() const;
	FString MakeInputActionBoundFunctionDeclarations() const;
	FString MakeInputActionProperties() const;
	FString MakeInputActionBoundFunctionDefinitions(const FString& PrefixedClassName) const;
	FString MakeSetupPlayerInputComponent(const FString& PrefixedClassName) const;	
	
	void ParseCharacterHeader(const FString& EntityName, FGeneratedFile& GeneratedFile) const;
	void ParseCharacterSource(const FString& EntityName, FGeneratedFile& GeneratedFile) const;
	void ParsePlayerControllerHeader(const FString& EntityName, FGeneratedFile& GeneratedFile) const;
	void ParsePlayerControllerSource(const FString& EntityName, FGeneratedFile& GeneratedFile) const;
	
	using FHandler = void(UFPCharacterGeneratorParser::*)(const FString&, FGeneratedFile&) const;
	TMap<FString, FHandler> Handlers;
	void ParseClassFile(const FString& EntityName, FGeneratedFile& GeneratedFile);
	
	FString ModuleNameString;
};
