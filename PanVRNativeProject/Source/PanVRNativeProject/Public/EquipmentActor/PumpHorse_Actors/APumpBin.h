#pragma once

#include "CoreMinimal.h"
#include "ActorBase/VRGrabActorBase.h"
#include "CoreCommon/Interface/IGrabInterface.h"
#include "APumpBin.generated.h"

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API AAPumpBin : public AVRGrabActorBase, public IIGrabInterface
{
	GENERATED_BODY()
	
public:
	AAPumpBin();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTimes) override;

	// Actor On Grabbed
	virtual void OnGrabbed(UMotionControllerComponent& InMCRef, const FVector& HandGrabPos, class AVRHand* InGrabbingHand) override;
	virtual void OnDropped() override;

	virtual void EquipmentRegistrable(AActor* InActor) override;

protected:
	// Begin Overlap For Collision Component Target Trash
	UFUNCTION()
	void OverlapTrashBoxBegin(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void UpdateDeLoreanLeverMove();

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> SMDeLoreanLever;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBoxComponent> CLRemover;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> SCMoveStandard;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UTextRenderComponent> TRTrashNum;

private:
	int32 RemoveTrashNum;

	FTimerHandle DeLoreanLeverMoveTimer;
	FTimerHandle PumpOperationTimer;

	UPROPERTY()
	TObjectPtr<UMotionControllerComponent> TempMC;

	UPROPERTY()
	TObjectPtr<class ACPumpHorse> SpawnPumpHorse;

private:
	UFUNCTION()
	void PumpOperation();

	UFUNCTION()
	void PumpOperationStop();
};
