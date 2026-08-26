#pragma once

#include "CoreMinimal.h"
#include "BTBase/UAITask_Base.h"
#include "UAITask_Teleport.generated.h"

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API UUAITask_Teleport : public UUAITask_Base
{
	GENERATED_BODY()
	
public:
	UUAITask_Teleport();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UFUNCTION()
	void OnTeleportMontageEnded(UAnimMontage* Montage, bool bInterrupted);

private:
	UPROPERTY()
	TObjectPtr<UAnimMontage> TeleportingMontage;

	UPROPERTY()
	TObjectPtr<USoundBase> SFX_Teleport;
};
