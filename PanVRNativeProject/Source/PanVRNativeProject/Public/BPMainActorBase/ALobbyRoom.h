// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorBase/VRGrabActorBase.h"
#include "Core/Component/UGrabComp.h"
#include "Core/Interface/IGrabInterface.h"
#include "ALobbyRoom.generated.h"

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API AALobbyRoom : public AVRGrabActorBase, public IIGrabInterface
{
	GENERATED_BODY()

public:
	AALobbyRoom();
	virtual void BeginPlay() override;

	// Actor On Grabbed
	virtual void OnGrabbed(UMotionControllerComponent& InMCRef, const FVector& HandGrabPos, class AVRHand* InGrabbingHand) override;
	virtual void OnDropped() override;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> SC_MainRoot;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> SM_SpaceEV;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> SM_Cup;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> SC_GlassRoot;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> SM_Glass001;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> SM_Glass002;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> SM_Glass003;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> SM_Glass004;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> SM_Glass005;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> SM_Glass006;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> SM_RoomWall;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> SM_RoomStuff;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> SM_StartLeverHandle;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UCapsuleComponent> CL_Handle;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBoxComponent> CL_TapeTarget;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> SM_LavaLamp;

protected:
	UFUNCTION()
	void SLHandleOverlapBegin(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void SLHandleFOverlapEnd(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

private:
	
	
private:
	FRotator AtFirstHandleRot;
	bool bIsHanding;
	FTimerHandle StartLeverMoveTimer;

	UPROPERTY()
	UMotionControllerComponent* TempMCRef;

private:
	void AdjustVecNRot(UMotionControllerComponent* InMC);

	void UpdateStartLever();

	void LeverOnGameStartEvent();
};
