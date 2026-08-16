#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlueprintBase.h"
#include "UAIService_FlameEnd.generated.h"

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API UUAIService_FlameEnd : public UBTService_BlueprintBase
{
	GENERATED_BODY()

public:
	UUAIService_FlameEnd();

protected:
	virtual void OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	
};
