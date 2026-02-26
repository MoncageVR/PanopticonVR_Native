// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorBase/VRActorBase.h"
#include "Core/Component/UGrabComp.h"
#include "Kismet/GameplayStatics.h"
#include "VRGrabActorBase.generated.h"

class HVRSoundPlayer;

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API AVRGrabActorBase : public AVRActorBase
{
	GENERATED_BODY()

public:
	AVRGrabActorBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UUGrabComp> GC;

	HVRSoundPlayer* mSoundPlayer;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:

};

//class HVRSoundPlayer
//{
//public:
//	HVRSoundPlayer() = default;
//
//	void PlaySoundEffect(UObject* PlayEquipment, USoundBase* Sound, FVector TempPlayPos);
//};