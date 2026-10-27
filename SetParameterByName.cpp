// Fill out your copyright notice in the Description page of Project Settings.


#include "SetParameterByName.h"

// --- SET VARIABLE BY NAME ---
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
                //TargetProperty->CopyCompleteValue(TargetValuePtr, ValueAddress);

                bool bTypesMatch = false;

                // Check if both properties are arrays
                FArrayProperty* TargetArrayProp = CastField<FArrayProperty>(TargetProperty);
                FArrayProperty* ValueArrayProp = CastField<FArrayProperty>(ValueProperty);

                if (TargetArrayProp && ValueArrayProp)
                {
                    // Ensure the inner types of the arrays match (e.g., both are arrays of Int)
                    if (TargetArrayProp->Inner && ValueArrayProp->Inner && TargetArrayProp->Inner->SameType(ValueArrayProp->Inner))
                    {
                        bTypesMatch = true;
                    }
                }
                else
                {
                    // Standard property type matching (primitives, structs, objects)
                    if (TargetProperty->SameType(ValueProperty))
                    {
                        bTypesMatch = true;
                    }
                }

                if (bTypesMatch)
                {
                    // Safely copy data (handles resizing arrays, copying struct fields, and primitives)
                    TargetProperty->CopyCompleteValue(TargetValuePtr, ValueAddress);
                }
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("SetVariableByName: Type mismatch when trying to set variable '%s' on '%s'."),
                        *VariableName.ToString(), *ActualTarget->GetName());
                }
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("SetVariableByName: Variable '%s' was not found on target '%s'."),
                    *VariableName.ToString(), *ActualTarget->GetName());
            }
        }
    }
    P_NATIVE_END;
}

// --- GET VARIABLE BY NAME ---
bool USetParameterByName::GetVariableByName(UObject* Target, FName VariableName, int32& OutValue)
{
    ensure(false);
    return false;
}

DEFINE_FUNCTION(USetParameterByName::execGetVariableByName)
{
    P_GET_OBJECT(UObject, Target);
    P_GET_PROPERTY(FNameProperty, VariableName);

    Stack.MostRecentProperty = nullptr;
    Stack.MostRecentPropertyAddress = nullptr;
    Stack.StepCompiledIn(nullptr, nullptr);

    void* OutValueAddress = Stack.MostRecentPropertyAddress;
    FProperty* OutValueProperty = Stack.MostRecentProperty;

    P_FINISH;

    bool bSuccess = false;

    P_NATIVE_BEGIN;
    {
        UObject* ActualTarget = Target ? Target : Stack.Object;

        if (ActualTarget && OutValueProperty && OutValueAddress)
        {
            FProperty* TargetProperty = FindFProperty<FProperty>(ActualTarget->GetClass(), VariableName);

            if (TargetProperty)
            {
                void* TargetValuePtr = TargetProperty->ContainerPtrToValuePtr<void>(ActualTarget);
                bool bTypesMatch = false;

                FArrayProperty* TargetArrayProp = CastField<FArrayProperty>(TargetProperty);
                FArrayProperty* OutArrayProp = CastField<FArrayProperty>(OutValueProperty);

                if (TargetArrayProp && OutArrayProp)
                {
                    if (TargetArrayProp->Inner && OutArrayProp->Inner && TargetArrayProp->Inner->SameType(OutArrayProp->Inner))
                    {
                        bTypesMatch = true;
                    }
                }
                else
                {
                    if (TargetProperty->SameType(OutValueProperty))
                    {
                        bTypesMatch = true;
                    }
                }

                if (bTypesMatch)
                {
                    TargetProperty->CopyCompleteValue(OutValueAddress, TargetValuePtr);
                    bSuccess = true;
                }
                else
                    {
                    UE_LOG(LogTemp, Warning, TEXT("GetVariableByName: Type mismatch for variable '%s' on '%s'."),
                        *VariableName.ToString(), *ActualTarget->GetName());
                }
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("GetVariableByName: Variable '%s' was not found on '%s'."),
                    *VariableName.ToString(), *ActualTarget->GetName());
            }
        }
    }
    P_NATIVE_END;

    *(bool*)RESULT_PARAM = bSuccess;
}