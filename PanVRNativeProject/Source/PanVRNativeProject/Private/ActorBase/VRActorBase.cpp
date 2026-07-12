// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorBase/VRActorBase.h"
#include "Components/BoxComponent.h"
#include "Core/Debug/FDebugLib.h"
#include "CoreObj/VREquipmentWorldSubsystem.h"
#include "CoreObj/VRGameInstanceSubsystem.h"
#include "Core/Interface/IEquipmentInitInterface.h"

AVRActorBase::AVRActorBase()
{
	PrimaryActorTick.bCanEverTick = false;

	ActorBaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	if (ActorBaseMesh)
	{
		SetRootComponent(ActorBaseMesh);
		ActorBaseMesh->SetCollisionProfileName(FName("NoCollision"));
	}
}

void AVRActorBase::BeginPlay()
{
	Super::BeginPlay();
}

void AVRActorBase::EquipmentRegistrable(AActor* InActor)
{
	if (!GetWorld())
	{
		return;
	}

	if (InActor->Implements<UIEquipmentInitInterface>())
	{
		IIEquipmentInitInterface* TempActor = Cast<IIEquipmentInitInterface>(InActor);
		if (TempActor)
		{
			EquipmentWorldSubSystem = GetWorld()->GetSubsystem<UVREquipmentWorldSubsystem>();

			if (EquipmentWorldSubSystem)
			{
				EquipmentWorldSubSystem->HandleAddEquipmentArrs(TempActor);
				//UE_LOG(LogTemp, Log, TEXT("EquipmentWorldSubsystme Cast Success!"));
			}
			else
			{
				//UE_LOG(LogTemp, Log, TEXT("EquipmentWorldSubsystme not Valid!"));
				return;
			}
		}
	}
}

void AVRActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void HVRSoundPlayer::PlaySoundEffect(UObject* PlayEquipment, USoundBase* Sound, FVector TempPlayPos)
{
	UVRGameInstanceSubsystem* GameInstSub = PlayEquipment->GetWorld()->GetGameInstance()->GetSubsystem<UVRGameInstanceSubsystem>();

	if (GameInstSub && Sound)
	{
		FVector TempSoundPos = TempPlayPos + FVector(0.f, 0.f, 20.0f);

		UGameplayStatics::PlaySoundAtLocation(
			PlayEquipment,
			Sound,
			TempSoundPos,
			GameInstSub->GetWorldVolumeMultiplier(),
			GameInstSub->GetWorldVolumePitch()
		);
	}

	return;
}