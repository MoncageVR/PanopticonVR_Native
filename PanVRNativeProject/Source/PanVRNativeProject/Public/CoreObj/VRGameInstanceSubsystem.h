// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "VRGameInstanceSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API UVRGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	int testNum = 1;

	
#pragma region Setter
	void SetWorldVolumeMultiplier(float TempVPlaySize);
	void SetWorldVolumePitch(float TempVPlaySpeed);
#pragma endregion

#pragma region Getter
	float GetWorldVolumeMultiplier() const;
	float GetWorldVolumePitch() const;
#pragma endregion


protected:
	

private:
	// In Game , Variables that Controls the All Sound
	float World_VolumeMultiplier = 1.f; // Volume Play Size
	float World_VolumePitch = 1.f; // Volume Play Speed

};
