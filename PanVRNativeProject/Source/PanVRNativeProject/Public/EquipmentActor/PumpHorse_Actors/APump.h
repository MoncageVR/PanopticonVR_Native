#pragma once

#include "CoreMinimal.h"
#include "ActorBase/VRGrabActorBase.h"
#include "CoreCommon/Interface/IGrabInterface.h"
#include "APump.generated.h"

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API AAPump : public AVRGrabActorBase, public IIGrabInterface
{
	GENERATED_BODY()

public:
	AAPump();

	virtual void BeginPlay() override;

	// Actor On Grabbed
	virtual void OnGrabbed(UMotionControllerComponent& InMCRef, const FVector& HandGrabPos, class AVRHand* InGrabbingHand) override;
	virtual void OnDropped() override;

	virtual void EquipmentRegistrable(AActor* InActor) override;

protected:
	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> SMPumpHandle;

	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> SMPumpHandleBellow;

	UPROPERTY()
	TObjectPtr<USceneComponent> SCMoveStandard;

private:
	FVector DefaultHandlePos;
	uint8 bIsDeLoreanOperationFlag;
	uint8 bIsPumpOperationFlag;
	uint32 PumpOperationNum;

	class UMotionControllerComponent* MCRef;

	FTimerHandle PumpOperateTimer;
	FTimerHandle PumpHorseOperateTimer;

private:
	void UpdatePumpUpNDown();
	void CheckOperatingPump();
	void SetHanldeDefaultPosNRot();

	void HandlePumpOperateByBin(float InOperTime);
};
