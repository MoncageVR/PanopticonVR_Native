#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PrisonerAnimInstance.generated.h"

/**
 *
 */
UCLASS()
class PANVRNATIVEPROJECT_API UPrisonerAnimInstance : public UAnimInstance
{
    GENERATED_BODY()

public:
    UPrisonerAnimInstance();

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
    TArray<bool> PrisonerUpperStateArrs;

    UFUNCTION(BlueprintImplementableEvent, Category = "State")
    void SetPrisonerUpperStates(int32 InIndex);
};