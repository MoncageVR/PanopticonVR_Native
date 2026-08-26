#pragma once

#include "CoreMinimal.h"
#include "BTBase/UAITask_Base.h"
#include "UAITask_RandomMove.generated.h"

/**
 *
 */
UCLASS()
class PANVRNATIVEPROJECT_API UUAITask_RandomMove : public UUAITask_Base
{
	GENERATED_BODY()

public:
	UUAITask_RandomMove();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY()
	TObjectPtr<USoundBase> SFX_RanMoveWalk;
private:
	const FVector MakeRandomVec();
	const bool HasReachedRandomTargetPos(const FVector InChaVec, const FVector InTargetVec);
};
