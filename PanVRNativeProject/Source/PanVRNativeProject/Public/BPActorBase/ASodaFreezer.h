// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorBase/VRGrabActorBase.h"
#include "Core/Interface/IGrabInterface.h"
#include "ASodaFreezer.generated.h"

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API AASodaFreezer : public AVRGrabActorBase, public IIGrabInterface
{
	GENERATED_BODY()
	
public:
	AASodaFreezer(); // Constructor

	// IGrabInterface Function override
	virtual void OnGrabbed(UMotionControllerComponent& InMCRef, const FVector& HandGrabPos, class AVRHand* InGrabbingHand) override;

	virtual void OnDropped() override;

protected:
	virtual void BeginPlay() override;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USceneComponent> FreezerMainRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Collision")
	TObjectPtr<class UBoxComponent> CL_MainFreezer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|SM")
	TObjectPtr<UStaticMeshComponent> UpwardFreezerDoorMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|GC")
	TObjectPtr<class UUGrabComp> UpwardDoorGC;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|SM")
	TObjectPtr<UStaticMeshComponent> DownwardFreezerDoorMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|GC")
	TObjectPtr<class UUGrabComp> DownwardDoorGC;

public:
	virtual void Tick(float DeltaTime) override;

private:
	// Freezer In Spawn SodaBullet Pointing Variable
	TObjectPtr<class AASodaBullet> NewSodaBullet = nullptr;

	USoundBase* SFXDoor;
	USoundBase* SFXCanPickUp;

};
