// Fill out your copyright notice in the Description page of Project Settings.


#include "BTBase/UAITask_Base.h"
#include "Core/Character/PrisonerCharacter.h"
#include "Core/Character/PrisonerController.h"

UUAITask_Base::UUAITask_Base()
{
	NodeName = TEXT("MyBTNode_Test_Task");
}

EBTNodeResult::Type UUAITask_Base::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    PrisonerControllerObj = Cast<APrisonerController>(OwnerComp.GetAIOwner());
    PrisonerCharacterObj = Cast<APrisonerCharacter>(PrisonerControllerObj ? PrisonerControllerObj->GetPawn() : nullptr);

    if (PrisonerControllerObj && PrisonerCharacterObj)
    {
        return EBTNodeResult::InProgress;
    }
    return EBTNodeResult::Failed;
}
