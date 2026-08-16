#pragma once

#include "CoreMinimal.h"
#include "ActorBase/VRGrabActorBase.h"
#include "CoreCommon/Interface/IGrabInterface.h"
#include "AStamp.generated.h"

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API AAStamp : public AVRGrabActorBase, public IIGrabInterface
{
	GENERATED_BODY()
	
public:
	AAStamp();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTimes) override;

	virtual void OnGrabbed(UMotionControllerComponent& InMCRef, const FVector& HandGrabPos, class AVRHand* InGrabbingHand) override;
	virtual void OnDropped() override;

protected:
#pragma region Components
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> STMainRoot;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USplineComponent> STMoveRail;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBoxComponent> CLPaperTarget;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> STHandleRoot;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> SMStampHandle;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> SMStampHandleHead;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBoxComponent> CLStamp;
#pragma endregion

	UFUNCTION()
	void MoveStamp();

	UFUNCTION()
	void CLPaperTargetOverlapBegin(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void CLPaperTargetOverlapEnd(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

	UFUNCTION()
	void CLStampOverlapBegin(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

private:
	FTimerHandle StampMoveTimer;

	UPROPERTY()
	TObjectPtr<class UMotionControllerComponent> TempMCRef = nullptr;

	UPROPERTY()
	TObjectPtr<class AAPaper> AttachingPaper = nullptr;

	UPROPERTY()
	TObjectPtr<USoundBase> SFXStampMove;

private:
	// InFlag 1 = OnGrabbed , InFlag 0 = OnDropped
	void AdjustGCPosNRot(uint32 InFlag);

};
