

#pragma once

#include "CoreMinimal.h"
#include "BTBase/UAITask_Base.h"
#include "UAITask_Distract.generated.h"

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API UUAITask_Distract : public UUAITask_Base
{
	GENERATED_BODY()

public:
	UUAITask_Distract();

protected:
	UPROPERTY()
	TArray<TObjectPtr<UAnimMontage>> DistractMontageArrs;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UFUNCTION()
	void OnDistractMontageEnded(UAnimMontage* Montage, bool bInterrupted);

private:
	
};
