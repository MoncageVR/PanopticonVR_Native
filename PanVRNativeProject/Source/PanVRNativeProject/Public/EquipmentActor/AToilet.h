#pragma once

#include "CoreMinimal.h"
#include "ActorBase/VRGrabActorBase.h"
#include "CoreCommon/Interface/IGrabInterface.h"
#include "AToilet.generated.h"

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API AAToilet : public AVRGrabActorBase , public IIGrabInterface
{
	GENERATED_BODY()

public:
	AAToilet();

	virtual void BeginPlay() override;

	virtual void OnGrabbed(UMotionControllerComponent& InMCRef, const FVector& HandGrabPos, class AVRHand* InGrabbingHand) override;
	virtual void OnDropped() override;
protected:
#pragma region Components
	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> TRubberDuck;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> TLever;

	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> TOvenDoor;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UCapsuleComponent> TCLLever;

	UPROPERTY()
	TObjectPtr<class UCapsuleComponent> TCLOvenDoor;

	UPROPERTY()
	TObjectPtr<class UUGrabComp> OvenDoorGC;
#pragma endregion
	
#pragma region CollisionEvents
	UFUNCTION()
	void LeverGrabColOverlapBegin(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void LeverGrabColOverlapEnd(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

	UFUNCTION()
	void OvenDoorGrabColOverlapBegin(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void OvenDoorGrabColOverlapEnd(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);
#pragma endregion
	
private:
	UPROPERTY()
	TObjectPtr<class UMotionControllerComponent> TempMCRef = nullptr;
	UPROPERTY()
	TObjectPtr<class AVRHand> TempGrabbingHandRed = nullptr;

	uint32 bIsOvenDoorHanding = 0;
	FTimerHandle OvenDoorMovementTimer;

	uint32 bIsLeverHanding = 0;
	FTimerHandle LeverMovementTimer;
	uint32 bIsLeverCoolDown = 0;
	FTimerHandle RubberDuckMovementTimer;

	FTimerHandle RubberDuckMovementStopTimer;

	UPROPERTY()
	TObjectPtr<USoundBase> SFXGrabHeavy;
	UPROPERTY()
	TObjectPtr<USoundBase> SFXOperateFlush;

	UPROPERTY()
	TObjectPtr<class AAElevatorButton> TempElevatorObj;

private:
	void AdjustOvenDoorGC(uint32 TempHandDir);
	void UpdateOvenDoorRotation();

	void AdjustLeverGC(uint32 TempHandDir);
	void UpdateLeverRotation();

	void WaterFlushing();
	void UpdateRubberDuck();
	void StopRubberDuck();

};
