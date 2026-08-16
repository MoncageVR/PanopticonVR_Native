#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "CPumpHorse.generated.h"

UCLASS()
class PANVRNATIVEPROJECT_API ACPumpHorse : public ACharacter
{
	GENERATED_BODY()

public:
	ACPumpHorse();
	virtual void BeginPlay() override;

	void HandleMoveEndAndDestroySelf();

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBoxComponent> CL_PumpHorseForSubdue;

	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<class APrisonerController>> OverlapPrisonerConArrs;

protected:
	UFUNCTION()
	void MoveEndCompleted(FAIRequestID InRequestID, EPathFollowingResult::Type InResult);

	UFUNCTION()
	void OnSubdueOverlapBegin(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	// Spawn LP Check Collision Functions
	UFUNCTION()
	void OnSubdueOverlapEnd(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

	UFUNCTION()
	void OnOverlappingPrisonerStateChanged(class APrisonerController* Prisoner, uint8 NewLowerState);

private:
	FVector SpawnPos;
	uint8 bIsCantMove;

	//FTimerHandle CheckEscapePrisonerTimer;

	UPROPERTY()
	TObjectPtr<AAIController> AICon;

private:
	void MoveOneStepByPump();

	void ActuallySubduePrisoner(class APrisonerController* Prisoner);
};
