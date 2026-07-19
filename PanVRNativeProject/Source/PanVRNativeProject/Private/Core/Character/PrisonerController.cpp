// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Character/PrisonerController.h"
#include "Core/Character/PrisonerCharacter.h"
#include "Core/Animation/PrisonerAnimInstance.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/DataTable.h"

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

	OnTaskFinished.AddDynamic(this, &APrisonerController::HandleNextTask);
}

void APrisonerController::HandleNextTask()
{
	if (Debug_Length <= Debug_CurrStateIndex)
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

	Debug_Upper_State.Add(0);   // Idle
	//Debug_Upper_State.Add(1);   // Stop
	//Debug_Upper_State.Add(2);  // Move
	//Debug_Upper_State.Add(2);   // Move
	//Debug_Upper_State.Add(2);   // Move 
	//Debug_Upper_State.Add(4);   // Dangerous
	//Debug_Upper_State.Add(4); // Dangerous
	//Debug_Upper_State.Add(3); // Interact
	//Debug_Upper_State.Add(2); // Move
	//Debug_Upper_State.Add(4); // Dangerous

	Debug_Lower_State.Add(0);   // Default
	//Debug_Lower_State.Add(1);   // Subdue
	//Debug_Lower_State.Add(6); // Floating
	//Debug_Lower_State.Add(3);   // GoHome
	//Debug_Lower_State.Add(7);    // SpiderMan
	//Debug_Lower_State.Add(12);   // TopEscape
	//Debug_Lower_State.Add(14); // TowerRaid
	//Debug_Lower_State.Add(10); // AssistNeighbor
	//Debug_Lower_State.Add(5);  // Run
	//Debug_Lower_State.Add(13); // Escape

	Debug_Length = Debug_Upper_State.Num();
	Debug_CurrStateIndex = 0;

	APrisonerCharacter* TempPrisonerCha = Cast<APrisonerCharacter>(this->GetCharacter());
	UAnimInstance* TempAnimInst = nullptr;
	if (TempPrisonerCha)
	{
		TempAnimInst = TempPrisonerCha->GetMesh()->GetAnimInstance();
		if (TempAnimInst)
		{
			mPrisonerAnimInstancePtr = Cast<UPrisonerAnimInstance>(TempAnimInst);
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

	//mLogicDT->GetRowStruct();
}

void APrisonerController::Tick(float DeltaTimes)
{
	Super::Tick(DeltaTimes);
}

void APrisonerController::State_based_ExecutionTasks_GiventoSomeone(TArray<uint8> InUpperStates, TArray<uint8> InLowerStates)
{
	Debug_Upper_State.Empty();
	Debug_Lower_State.Empty();
	Debug_Length = InUpperStates.Num();

	//Debug_Upper_State = InUpperStates;
	//Debug_Lower_State = InLowerStates;
	//Debug_Length = InUpperStates.Num();

	this->BlackboardComp->SetValueAsEnum(TEXT("CurrUpperState"), InUpperStates[0]);
	this->BlackboardComp->SetValueAsEnum(TEXT("CurrLowerState"), InLowerStates[0]);

	for (int32 i = 1; i < Debug_Length; i++)
	{
		Debug_Upper_State.Add(InUpperStates[i]);
		Debug_Lower_State.Add(InLowerStates[i]);
	}

}

void APrisonerController::HandlePrisonerLogic(int32 InRanNum)
{
	UE_LOG(LogTemp, Log, TEXT("Random Num : %d"), InRanNum);
}
