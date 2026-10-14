// Fill out your copyright notice in the Description page of Project Settings.


#include "SetParameterByName.h"

// Fallback stub if called directly in C++
void USetParameterByName::SetVariableByName(UObject* Target, FName VariableName, const int32& Value)
{
    ensure(false);
}

DEFINE_FUNCTION(USetParameterByName::execSetVariableByName)
{
    // 1. Pop 'Target' parameter from the execution stack
    P_GET_OBJECT(UObject, Target);

    // 2. Pop 'VariableName' parameter from the execution stack
    P_GET_PROPERTY(FNameProperty, VariableName);

    // 3. Prepare and step into the wildcard 'Value' parameter
    Stack.MostRecentProperty = nullptr;
    Stack.MostRecentPropertyAddress = nullptr;
    Stack.StepCompiledIn(nullptr, nullptr);

    void* ValueAddress = Stack.MostRecentPropertyAddress;
    FProperty* ValueProperty = Stack.MostRecentProperty;

    P_FINISH;

    P_NATIVE_BEGIN;
    {
        // If Target is null/empty, fallback to 'Self' (Stack.Object)
        UObject* ActualTarget = Target ? Target : Stack.Object;

        if (ActualTarget && ValueProperty && ValueAddress)
        {
            // Find the property on the target object's class by name using reflection
            FProperty* TargetProperty = FindFProperty<FProperty>(ActualTarget->GetClass(), VariableName);

            if (TargetProperty)
            {
                // Get memory pointer for the target variable inside the object instance
                void* TargetValuePtr = TargetProperty->ContainerPtrToValuePtr<void>(ActualTarget);

                // Safely copy the value over (handles basic types, structs, strings, etc.)
                TargetProperty->CopyCompleteValue(TargetValuePtr, ValueAddress);
            }
        }
    }
    P_NATIVE_END;
}