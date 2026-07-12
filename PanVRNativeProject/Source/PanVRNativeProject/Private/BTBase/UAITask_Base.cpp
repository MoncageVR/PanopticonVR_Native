// Fill out your copyright notice in the Description page of Project Settings.


#include "BTBase/UAITask_Base.h"
#include "Core/Character/PrisonerCharacter.h"
#include "Core/Character/PrisonerController.h"
#include "CoreObj/Manager/MapObjManagerSubsystem.h"
#include "CoreObj/Manager/PrisonerManagerSubsystem.h"
#include "Animation/AnimInstance.h"

UUAITask_Base::UUAITask_Base()
{
	NodeName = TEXT("BTTask_BaseNode");
    bCreateNodeInstance = true;
}

EBTNodeResult::Type UUAITask_Base::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    PrisonerControllerObj = Cast<APrisonerController>(OwnerComp.GetAIOwner());
    if (!ensure(PrisonerControllerObj)) return EBTNodeResult::Failed;
    PrisonerCharacterObj = Cast<APrisonerCharacter>(PrisonerControllerObj ? PrisonerControllerObj->GetPawn() : nullptr);
    if (!ensure(PrisonerCharacterObj)) return EBTNodeResult::Failed;
    MyAnimInst = PrisonerCharacterObj->GetMesh()->GetAnimInstance();
    if (!ensure(MyAnimInst)) return EBTNodeResult::Failed;
    MapObjManagerSubSystemInst = GetWorld()->GetGameInstance()->GetSubsystem<UMapObjManagerSubsystem>();
    if(!ensure(MapObjManagerSubSystemInst)) return EBTNodeResult::Failed;
    PrisonerManagerSubSystemInst = GetWorld()->GetGameInstance()->GetSubsystem<UPrisonerManagerSubsystem>();
    if (!ensure(PrisonerManagerSubSystemInst)) return EBTNodeResult::Failed;

    if (PrisonerControllerObj && PrisonerCharacterObj && MyAnimInst && MapObjManagerSubSystemInst && PrisonerManagerSubSystemInst)
    {
        return EBTNodeResult::InProgress;
    }
    return EBTNodeResult::Failed;
}
