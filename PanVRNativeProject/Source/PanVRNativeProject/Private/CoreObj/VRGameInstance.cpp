// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreObj/VRGameInstance.h"
#include "CoreObj/VRGameInstanceSubsystem.h"
#include "CoreObj/Manager/PrisonerManagerSubsystem.h"

void UVRGameInstance::Init()
{
	Super::Init();

	UVRGameInstanceSubsystem* tempVRGameInstanceSubSystem = GetSubsystem<UVRGameInstanceSubsystem>();

	P_PrisonerManager = GetSubsystem<UPrisonerManagerSubsystem>();
}


