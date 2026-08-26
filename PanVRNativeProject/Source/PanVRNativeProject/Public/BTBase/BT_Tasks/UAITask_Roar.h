#pragma once

#include "CoreMinimal.h"
#include "BTBase/UAITask_Base.h"
#include "UAITask_Roar.generated.h"

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API UUAITask_Roar : public UUAITask_Base
{
	GENERATED_BODY()

public:
	UUAITask_Roar();

protected:
	TObjectPtr<UAnimMontage> RoarMontage;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UFUNCTION()
	void OnRoarMontageEnded(UAnimMontage* Montage, bool bInterrupted);

private:
	UPROPERTY()
	TObjectPtr<USoundBase> SFX_Roar;
	
};
