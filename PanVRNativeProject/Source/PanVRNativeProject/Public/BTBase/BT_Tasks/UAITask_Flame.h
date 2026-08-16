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

	/*
	0814_
	FireAlarm 의 스프링쿨러 로직 구현중에, Return 상태 부여는 잘 구현함.
	하지만, Return 중에 Flame 상태에서 남아있던 전이(Transition) Collision 과 Timer가 남아있어, Return 하는 과정에 Flame 상태를 전이시킴.
	그래서 Claude의 해결법은 Service기능을 이용해서, Flame 상태가 바뀌면 알림을 보내, Transition 관리 처리를 구현중에 멈춤
	*/

private:

	UPROPERTY()
	uint8 MoveCount;

	UPROPERTY()
	FVector FireEffectSpawnVec;

	UPROPERTY()
	TArray<FVector> FlameTargetVecArrs;

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
};
