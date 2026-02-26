// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreObj/VRGameInstance.h"
#include "CoreObj/VRGameInstanceSubsystem.h"

void UVRGameInstance::Init()
{
	Super::Init();

	//UE_LOG(LogTemp, Log, TEXT("VRGameInstance Initialize Success!"));


	UVRGameInstanceSubsystem* tempVRGameInstanceSubSystem = GetSubsystem<UVRGameInstanceSubsystem>();

	if (tempVRGameInstanceSubSystem)
	{
		//UE_LOG(LogTemp, Log, TEXT("VRGameInstanceSubSystem Referencing Success!"));
	}
}
