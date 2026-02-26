// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorBase/VRGrabActorBase.h"
#include "Core/Component/UGrabComp.h"
//#include "CoreObj/VRGameInstanceSubsystem.h"

AVRGrabActorBase::AVRGrabActorBase()
{
	PrimaryActorTick.bCanEverTick = false;

	GC = CreateDefaultSubobject<UUGrabComp>("GrabComponent");
	GC->SetupAttachment(ActorBaseMesh);
}

void AVRGrabActorBase::BeginPlay()
{
	Super::BeginPlay();

}

void AVRGrabActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/*
---
*/

//void HVRSoundPlayer::PlaySoundEffect(UObject* PlayEquipment, USoundBase* Sound, FVector TempPlayPos)
//{
//	UVRGameInstanceSubsystem* GameInstSub = PlayEquipment->GetWorld()->GetGameInstance()->GetSubsystem<UVRGameInstanceSubsystem>();
//
//	if (GameInstSub && Sound)
//	{
//		FVector TempSoundPos = TempPlayPos + FVector(0.f, 0.f, 20.0f);
//
//		UGameplayStatics::PlaySoundAtLocation(
//			PlayEquipment,
//			Sound,
//			TempSoundPos,
//			GameInstSub->GetWorldVolumeMultiplier(),
//			GameInstSub->GetWorldVolumePitch()
//		);
//	}
//	
//	return;
//}
