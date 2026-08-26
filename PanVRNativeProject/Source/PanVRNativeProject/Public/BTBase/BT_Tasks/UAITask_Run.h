

#pragma once

#include "CoreMinimal.h"
#include "BTBase/UAITask_Base.h"
#include "UAITask_Run.generated.h"

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API UUAITask_Run : public UUAITask_Base
{
	GENERATED_BODY()
	
public:
	UUAITask_Run();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY()
	TObjectPtr<USoundBase> SFX_Walking;
private:
	const bool HasReachedTargetPos(const FVector InChaVec, const FVector InTargetVec);

};
