#pragma once

#include "CoreMinimal.h"
#include "BTBase/UAITask_Base.h"
#include "UAITask_AssistNeighbor.generated.h"

/**
 *
 */
UCLASS()
class PANVRNATIVEPROJECT_API UUAITask_AssistNeighbor : public UUAITask_Base
{
	GENERATED_BODY()

public:
	UUAITask_AssistNeighbor();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:

};