#include "CoreCommon/PrisonerRelated/PrisonerController.h"
#include "CoreCommon/PrisonerRelated/PrisonerCharacter.h"
#include "CoreCommon/PrisonerCharacterBase/Animation/PrisonerAnimInstance.h"
#include "CoreCommon/Enum/EPrisonerStates.h"
#include "CoreCommon/Struct/FPrisonerInfoRow.h"
#include "CoreObj/Manager/GameInstanceSubSystem/PrisonerManagerSubsystem.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/DataTable.h"
#include "NavFilters/RecastFilter_UseDefaultArea.h"
#include "Components/SphereComponent.h"

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

	this->DefaultNavigationFilterClass = URecastFilter_UseDefaultArea::StaticClass();

	OnTaskFinished.AddDynamic(this, &APrisonerController::HandleNextTask);
}

void APrisonerController::HandleNextTask()
{
	if (!Debug_Upper_State.IsValidIndex(Debug_CurrStateIndex) || !Debug_Lower_State.IsValidIndex(Debug_CurrStateIndex))
	{
		if (BlackboardComp)
		{
			this->GetBBComp()->SetValueAsEnum(TEXT("CurrUpperState"), 0);
			this->GetBBComp()->SetValueAsEnum(TEXT("CurrLowerState"), 0);
			this->GetBBComp()->SetValueAsBool(TEXT("bIsPatrolBlocked"), true);
		}
		UE_LOG(LogTemp, Log, TEXT("%d Prisoner is Next Task Not Valid"), GetBBComp()->GetValueAsInt(FName("UniqueNum")));
		return;
	}
	else
	{
		BlackboardComp->SetValueAsEnum(TEXT("CurrUpperState"), Debug_Upper_State[Debug_CurrStateIndex]);
		BlackboardComp->SetValueAsEnum(TEXT("CurrLowerState"), Debug_Lower_State[Debug_CurrStateIndex]);
		//UE_LOG(LogTemp, Log, TEXT("%d : Current Index : %d, Current Length : %d"),GetBBComp()->GetValueAsInt(FName("UniqueNum")), Debug_CurrStateIndex, Debug_Length);
		Debug_CurrStateIndex++;
		//UE_LOG(LogTemp, Log, TEXT("%d : Current Index : %d, Current Length : %d"), GetBBComp()->GetValueAsInt(FName("UniqueNum")), Debug_CurrStateIndex, Debug_Length);
		if (FOnPrisonerLowerStateChangedSignature.IsBound())
		{
			FOnPrisonerLowerStateChangedSignature.Broadcast(this, this->GetCurrLowerState());
		}
	}

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

void APrisonerController::HandlePlayPrisonerLogic(int32 InRanNum)
{
	// UE_LOG(LogTemp, Log, TEXT("Create Paranormal Phenomenon Random Num : %d"), InRanNum);

	UPrisonerManagerSubsystem* TempPrisonerManagerRef = GetWorld()->GetGameInstance()->GetSubsystem<UPrisonerManagerSubsystem>();

	TempPrisonerManagerRef->GetAllPrisonerControllerArr()[InRanNum]->HandleNextTask();
	return;
}

void APrisonerController::InitializeStatesFromLogicDT()
{
	//UE_LOG(LogTemp, Log, TEXT("Logic DT Init And State Init Start!"));

	Debug_Upper_State.Empty();
	Debug_Lower_State.Empty();
	Debug_Length = 0;
	Debug_CurrStateIndex = 0;

	/*if (mLogicDT)
	{
		UE_LOG(LogTemp, Log, TEXT("1. Logic DT Is Valid"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("1. Logic DT Is Not Valid"));
	}*/

	TArray<FName> TempRowNames = mLogicDT->GetRowNames();

	int32 RowRandomChoice =11;//FMath::RandRange(0, TempRowNames.Num() - 1);

	//UE_LOG(LogTemp, Log, TEXT("Row Rand Choice Number : %d"), RowRandomChoice);

	FPrisonerInfoRow* MyRow = mLogicDT->FindRow<FPrisonerInfoRow>(TempRowNames[RowRandomChoice], TEXT(""));
	UEnum* UpperEnum = StaticEnum<EPrisonerUpperStateType>();
	UEnum* LowerEnum = StaticEnum<EPrisonerLowerStateType>();

	//UE_LOG(LogTemp, Log, TEXT("4. Row Print!!!!"));
	for (int32 y = 0; y < MyRow->ActionUpperState.Num(); y++)
	{
		EPrisonerUpperStateType TempUpperStates = MyRow->ActionUpperState[y];
		EPrisonerLowerStateType TempLowerStates = MyRow->ActionLowerState[y];

		// Enum String Print Debugging
		FString UpperStr = UpperEnum->GetNameStringByValue((int64)TempUpperStates);
		FString LowerStr = LowerEnum->GetNameStringByValue((int64)TempLowerStates);

		UE_LOG(LogTemp, Log, TEXT("Upper : %s(%d) | Lower : %s(%d)"), *UpperStr, (uint8)TempUpperStates, *LowerStr, (uint8)TempLowerStates);

		Debug_Upper_State.Add((uint8)TempUpperStates);
		Debug_Lower_State.Add((uint8)TempLowerStates);
		Debug_Length = MyRow->ActionUpperState.Num();

		//UE_LOG(LogTemp, Log, TEXT("Index : %s = Upper : %s | Lower : %s"), *TempRowNames[RowRandomChoice].ToString(), *UpperStr, *LowerStr);

	}

}

void APrisonerController::HandleFlameTransitionColNTimer(uint8 InHandleFlag)
{
	if (InHandleFlag)
	{
		this->AttachSphereCollision();

		if (UWorld* MyWorld = GetWorld())
		{
			MyWorld->GetTimerManager().SetTimer(
				FLameTransitionTimer,
				this,
				&APrisonerController::ActuallyFlameTransition,
				10.0f,
				true
			);
		}
	}
	else
	{
		if (IsValid(CL_FlameTransition))
		{
			CL_FlameTransition->OnComponentBeginOverlap.RemoveDynamic(this, &APrisonerController::FlameCLOverlapBegin);
			CL_FlameTransition->DestroyComponent();
			CL_FlameTransition = nullptr;
		}
		if (UWorld* MyWorld = GetWorld())
		{
			MyWorld->GetTimerManager().PauseTimer(FLameTransitionTimer);
			MyWorld->GetTimerManager().ClearTimer(FLameTransitionTimer);
		}
	}
}

void APrisonerController::HandleRunBT()
{
	if (BehaviorTreeAsset)
	{
		RunBehaviorTree(BehaviorTreeAsset);
	}
}

void APrisonerController::FlameCLOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp && OtherComp->ComponentHasTag(FName("PrisonerCharacter")))
	{
		APrisonerCharacter* OverlapOtherCha = Cast<APrisonerCharacter>(OtherActor);
		checkf(OverlapOtherCha, TEXT("Overlap Prisoner Not Valid!"));
		AAIController* OtherAICon = Cast<AAIController>(OverlapOtherCha->GetController());
		checkf(OtherAICon, TEXT("Prisoner AI Controller Not Valid"));
		APrisonerController* OtherPrisonerCon = Cast<APrisonerController>(OtherAICon);
		checkf(OtherPrisonerCon, TEXT("Prsioenr Controller Not Valid"));

		if (OtherPrisonerCon->GetBBComp()->GetValueAsEnum(TEXT("CurrUpperState")) == 0 && OtherPrisonerCon->GetBBComp()->GetValueAsEnum(TEXT("CurrLowerState")) == 0)
		{
			//UE_LOG(LogTemp, Log, TEXT("Default State Prisoenr Overlap Success!"));
			TArray<uint8> GivenUpperState = { 4 };
			TArray<uint8> GivenLowerState = { 15 };

			OtherPrisonerCon->State_based_ExecutionTasks_GiventoSomeone(GivenUpperState, GivenLowerState);

			CL_FlameTransition->SetGenerateOverlapEvents(false);
			CL_FlameTransition->OnComponentBeginOverlap.RemoveDynamic(this, &APrisonerController::FlameCLOverlapBegin);
		}
	}
}

void APrisonerController::AttachSphereCollision()
{
	if (!IsValid(CL_FlameTransition))
	{
		//UE_LOG(LogTemp, Log, TEXT("FlameTransition Collision Attach Success!"));
		APrisonerCharacter* TempCha = Cast<APrisonerCharacter>(this->GetCharacter());
		CL_FlameTransition = Cast<USphereComponent>(TempCha->AddComponentByClass(USphereComponent::StaticClass(), false, FTransform::Identity, false));

		CL_FlameTransition->SetSphereRadius(900.0f, true);
		CL_FlameTransition->SetHiddenInGame(false);// Debug
		CL_FlameTransition->SetCollisionProfileName(FName("OverlapAll"));
		CL_FlameTransition->SetGenerateOverlapEvents(true);
	}
}

void APrisonerController::ActuallyFlameTransition()
{
	//UE_LOG(LogTemp, Log, TEXT("Called every 10 seconds, Flame state transition!"));
	CL_FlameTransition->OnComponentBeginOverlap.AddDynamic(this, &APrisonerController::FlameCLOverlapBegin);
	CL_FlameTransition->SetGenerateOverlapEvents(true);
	return;
}