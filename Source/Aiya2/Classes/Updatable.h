// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Updatable.generated.h"
/**
 * 
 */
UINTERFACE(BlueprintType)
class AIYA2_API UUpdatable : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class AIYA2_API IUpdatable
{
	GENERATED_IINTERFACE_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Updatable")
	bool DoOnUpdate(const UObject* ObjectRef);
};
