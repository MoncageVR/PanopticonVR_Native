// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Character/PrisonerController.h"
#include "Core/Character/PrisonerCharacter.h"
#include "Core/Animation/PrisonerAnimInstance.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"      
#include "BehaviorTree/BlackboardComponent.h"

APrisonerController::APrisonerController()
{
	this->bAttachToPawn = true;

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/VRContent/Prisoner/BehaviorTree/BT_PrisonerAI.BT_PrisonerAI"));
	if (BTObject.Succeeded())
	{
		BehaviorTreeAsset = BTObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/VRContent/Prisoner/BehaviorTree/BB_PrisonerAI.BB_PrisonerAI"));
	if (BBObject.Succeeded())
	{
		BlackboardAsset = BBObject.Object;
	}

	if (GetBlackboardComponent())
	{
		BlackboardComp = GetBlackboardComponent();
	}

	// Debug

	//Debug_Upper_State.Add(3); // Interact
	//Debug_Upper_State.Add(3); // Interact
	//Debug_Upper_State.Add(2); // Move
	//Debug_Upper_State.Add(2); // Move
	//Debug_Upper_State.Add(4); // Dangerous

	//Debug_Lower_State.Add(10); // Anger
	//Debug_Lower_State.Add(8); // DoorPicking
	//Debug_Lower_State.Add(4); // RandomMove
	//Debug_Lower_State.Add(5); // Run
	//Debug_Lower_State.Add(13); // Escape

	//Debug_Length = Debug_Upper_State.Num();
	//Debug_CurrStateIndex = 0;
	// Debug

	OnTaskFinished.AddDynamic(this, &APrisonerController::HandleNextTask);
}

void APrisonerController::HandleNextTask()
{
	if (Debug_Length == Debug_CurrStateIndex)
	{
		return;
	}
	BlackboardComp->SetValueAsEnum(TEXT("CurrUpperState"), Debug_Upper_State[Debug_CurrStateIndex]);
	BlackboardComp->SetValueAsEnum(TEXT("CurrLowerState"), Debug_Lower_State[Debug_CurrStateIndex]);
	Debug_CurrStateIndex++;
}

void APrisonerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	Debug_Upper_State.Empty();
	Debug_Lower_State.Empty();

	Debug_Upper_State.Add(2); // Move
	Debug_Lower_State.Add(5); // Run
	Debug_Length = Debug_Upper_State.Num();
	Debug_CurrStateIndex = 0;

	APrisonerCharacter* TempPrisonerCha = Cast<APrisonerCharacter>(this->GetCharacter());
	UAnimInstance* TempAnimInst = nullptr;
	if (TempPrisonerCha)
	{
		TempAnimInst = TempPrisonerCha->GetMesh()->GetAnimInstance();
		if (TempAnimInst)
		{
			mPrisonerAnimInstancePtr = Cast< UPrisonerAnimInstance>(TempAnimInst);
		}
	}

	if (!ensure(TempPrisonerCha) && !ensure(TempAnimInst) && !ensure(mPrisonerAnimInstancePtr)) return;

	if (BlackboardAsset)
	{
		UseBlackboard(BlackboardAsset, BlackboardComp);
	}

	if (BehaviorTreeAsset)
	{
		RunBehaviorTree(BehaviorTreeAsset);
	}
}

void APrisonerController::Tick(float DeltaTimes)
{
	Super::Tick(DeltaTimes);
}
