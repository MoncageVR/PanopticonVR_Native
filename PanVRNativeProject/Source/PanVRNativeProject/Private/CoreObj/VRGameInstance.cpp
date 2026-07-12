// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreObj/VRGameInstance.h"
#include "CoreObj/VRGameInstanceSubsystem.h"
#include "CoreObj/Manager/PrisonerManagerSubsystem.h"
#include "CoreObj/Manager/MapObjManagerSubsystem.h"
#include "CoreObj/VREquipmentWorldSubsystem.h"

void UVRGameInstance::Init()
{
	Super::Init();

	UVRGameInstanceSubsystem* tempVRGameInstanceSubSystem = GetSubsystem<UVRGameInstanceSubsystem>();

	P_PrisonerManager = GetSubsystem<UPrisonerManagerSubsystem>();
	P_MapObjManager = GetSubsystem<UMapObjManagerSubsystem>();
	P_VREquipmentManager = GetWorld()->GetSubsystem<UVREquipmentWorldSubsystem>();//GetSubsystem<UVREquipmentWorldSubsystem>();
}


