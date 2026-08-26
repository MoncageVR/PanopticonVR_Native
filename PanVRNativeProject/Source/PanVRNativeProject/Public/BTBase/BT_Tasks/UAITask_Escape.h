#pragma once

#include "CoreMinimal.h"
#include "BTBase/UAITask_Base.h"
#include "UAITask_Escape.generated.h"

/**
 *
 */
UCLASS()
class PANVRNATIVEPROJECT_API UUAITask_Escape : public UUAITask_Base
{
	GENERATED_BODY()

public:
	UUAITask_Escape();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	FTimerHandle ExitDoorControlTimer;

	UPROPERTY()
	TObjectPtr<USoundBase> SFX_ButtonClick;

private:
	void CallControlExitDoorFunction();
};
