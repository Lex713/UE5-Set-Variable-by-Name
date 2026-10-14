// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SetParameterByName.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTNYOOM_API USetParameterByName : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
    /**
     * Finds a variable on the target object by name and sets its value.
     * If Target is empty/null, it defaults to Self.
     */
    UFUNCTION(BlueprintCallable, CustomThunk, Category = "Utilities", meta = (DefaultToSelf = "Target", CustomStructureParam = "Value"))
    static void SetVariableByName(UObject* Target, FName VariableName, const int32& Value);

    // Required for CustomThunk wildcard evaluation
    DECLARE_FUNCTION(execSetVariableByName);
};