// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "YamlNode.h"
#include "YamlFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class YUEML_API UYamlFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static TSharedPtr<FYamlNode> LoadYaml(const FString& FilePath);
	
};
