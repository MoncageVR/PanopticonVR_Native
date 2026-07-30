// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BTBase/UAITask_Base.h"
#include "UAITask_Flame.generated.h"

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API UUAITask_Flame : public UUAITask_Base
{
	GENERATED_BODY()
	
public:
	UUAITask_Flame();

protected:
	UPROPERTY()
	TArray<float> BaseCoordinateHeightArrs; // TempZPos

	UPROPERTY()
	TArray<float> BaseCoordinate1stRadius; // Temp1stRadius

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	UFUNCTION()
	bool AttachSphereCollision();

	UFUNCTION()
	void FlameCLOverlapBegin(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

private:

	UPROPERTY()
	uint8 MoveCount;

	UPROPERTY()
	FVector FireEffectSpawnVec;

	UPROPERTY()
	TArray<FVector> FlameTargetVecArrs;

	UPROPERTY()
	TObjectPtr<class USphereComponent> FlameTransitionCL;

	UPROPERTY()
	FTimerHandle FlameTransitionTimer;

private:
	UFUNCTION()
	void ActuallyMoveFlameRun();

	// MoveSpeed , MoveMode , FireEffectSPawnPos , MoveCount Etc Initialize
	UFUNCTION()
	void InitBeforeFlameRun();

	UFUNCTION()
	void MakeFlameRandomVec();

	UFUNCTION()
	const FVector CalculateFlameRandomvec(float InDegree, float InRadius, float InZPos);

	UFUNCTION()
	bool HasReachedFlameTargetVec(FVector InChaVec, FVector InTargetVec);

	UFUNCTION()
	void ActuallyFlameTransition();
};
