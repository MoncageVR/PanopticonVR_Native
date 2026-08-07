#pragma once

#include "CoreMinimal.h"
#include "ActorBase/VRGrabActorBase.h"
#include "CoreCommon/Component/UGrabComp.h"
#include "CoreCommon/Interface/IGrabInterface.h"
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
	TObjectPtr<UStaticMeshComponent> SM_RoomWall;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> SM_RoomStuff;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> SM_StartLeverHandle;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> SM_SL_Roller01;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> SM_SL_Roller02;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> SM_SL_Roller03;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UCapsuleComponent> CL_Handle;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBoxComponent> CL_TapeTarget;


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
