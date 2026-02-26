// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreObj/VRGameInstanceSubsystem.h"
#include "ActorBase/VRGrabActorBase.h"
#include "Core/Component/UGrabComp.h"
#include "BPActorBase/AGTWLever.h"

void UVRGameInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	
}

void UVRGameInstanceSubsystem::SetWorldVolumeMultiplier(float TempVPlaySize) { World_VolumeMultiplier = TempVPlaySize; }
void UVRGameInstanceSubsystem::SetWorldVolumePitch(float TempVPlaySpeed) { World_VolumePitch = TempVPlaySpeed; }
float UVRGameInstanceSubsystem::GetWorldVolumeMultiplier() const { return World_VolumeMultiplier; }
float UVRGameInstanceSubsystem::GetWorldVolumePitch() const { return World_VolumePitch; }
