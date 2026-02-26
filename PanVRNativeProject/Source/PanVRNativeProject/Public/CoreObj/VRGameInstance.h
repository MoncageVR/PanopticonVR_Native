// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "VRGameInstance.generated.h"

class UVRGameInstanceSubsystem;

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API UVRGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
};
