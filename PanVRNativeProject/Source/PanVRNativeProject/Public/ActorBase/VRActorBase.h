// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/Interface/IEquipmentInitInterface.h"
#include "Kismet/GameplayStatics.h"
#include "VRActorBase.generated.h"

class UVREquipmentWorldSubsystem;

UCLASS()
class PANVRNATIVEPROJECT_API AVRActorBase : public AActor , public IIEquipmentInitInterface
{
	GENERATED_BODY()
	
public:	
	AVRActorBase();

	class HVRSoundPlayer* mSoundPlayer;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Mesh")
	TObjectPtr<class UStaticMeshComponent> ActorBaseMesh;

	virtual void EquipmentRegistrable(AActor* InActor) override;

	UPROPERTY()
	TObjectPtr<UVREquipmentWorldSubsystem> EquipmentWorldSubSystem;

public:
	virtual void Tick(float DeltaTime) override;

private:
};


class HVRSoundPlayer
{
public:
	HVRSoundPlayer() = default;

	void PlaySoundEffect(UObject* PlayEquipment, USoundBase* Sound, FVector TempPlayPos);
};