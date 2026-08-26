#pragma once

#include "CoreMinimal.h"
#include "BTBase/UAITask_Base.h"
#include "UAITask_GoHome.generated.h"

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API UUAITask_GoHome : public UUAITask_Base
{
	GENERATED_BODY()

public:
	UUAITask_GoHome();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY()
	TObjectPtr<USoundBase> SFX_GoHomeWalk;

private:
	bool HasReachedGoHomeTargetVec(const FVector InChaVec, const FVector InGoHomeTargetVec);

};
