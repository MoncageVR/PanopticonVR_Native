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
#include "Components/AudioComponent.h"

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
	if (MyPrisonerCha->GetAudioPlayer()->IsPlaying())
	{
		MyPrisonerCha->GetAudioPlayer()->Stop();
	}

	if (!Upper_States.IsValidIndex(CurrStateIndex) || !Lower_States.IsValidIndex(CurrStateIndex))
	{
		CurrStateIndex = 0;
		this->GetBBComp()->SetValueAsEnum(TEXT("CurrUpperState"), 0);
		this->GetBBComp()->SetValueAsEnum(TEXT("CurrLowerState"), 0);
		this->GetBBComp()->SetValueAsBool(TEXT("bIsPatrolBlocked"), true);
		int32 TempUniqueNum = this->GetBBComp()->GetValueAsInt(FName("UniqueNum"));
		//UPrisonerMgrSubSytemPtr->GetPrisonerPossibleNumArrs()[TempUniqueNum] = TempUniqueNum;

		UPrisonerMgrSubSytemPtr->SetPrisonerPossibleNumArrByIndex(TempUniqueNum);

		UE_LOG(LogTemp, Log, TEXT("%d Prisoner is Next Task Not Valid"), GetBBComp()->GetValueAsInt(FName("UniqueNum")));
		return;
	}
	else
	{
		BlackboardComp->SetValueAsEnum(TEXT("CurrUpperState"), Upper_States[CurrStateIndex]);
		BlackboardComp->SetValueAsEnum(TEXT("CurrLowerState"), Lower_States[CurrStateIndex]);
		CurrStateIndex++;
		if (FOnPrisonerLowerStateChangedSignature.IsBound())
		{
			FOnPrisonerLowerStateChangedSignature.Broadcast(this, this->GetCurrLowerState());
		}
	}

}

void APrisonerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	Upper_States.Empty();
	Lower_States.Empty();

	Upper_States.Add(0);
	Lower_States.Add(0);

	States_Length = Upper_States.Num();
	CurrStateIndex = 0;

	MyPrisonerCha = Cast<APrisonerCharacter>(this->GetCharacter());
	UAnimInstance* TempAnimInst = nullptr;
	if (MyPrisonerCha)
	{
		TempAnimInst = MyPrisonerCha->GetMesh()->GetAnimInstance();
		if (TempAnimInst)
		{
			mPrisonerAnimInstancePtr = Cast<UPrisonerAnimInstance>(TempAnimInst);
		}
	}

	if (!ensure(MyPrisonerCha) && !ensure(TempAnimInst) && !ensure(mPrisonerAnimInstancePtr)) return;

	if (BlackboardAsset)
	{
		UseBlackboard(BlackboardAsset, BlackboardComp);
	}

	UPrisonerMgrSubSytemPtr = Cast<UPrisonerManagerSubsystem>(GetWorld()->GetGameInstance()->GetSubsystem<UPrisonerManagerSubsystem>());
	check(UPrisonerMgrSubSytemPtr);
}

void APrisonerController::Tick(float DeltaTimes)
{
	Super::Tick(DeltaTimes);
}

void APrisonerController::State_based_ExecutionTasks_GiventoSomeone(TArray<uint8> InUpperStates, TArray<uint8> InLowerStates)
{
	Upper_States.Empty();
	Lower_States.Empty();
	States_Length = InUpperStates.Num();

	this->BlackboardComp->SetValueAsEnum(TEXT("CurrUpperState"), InUpperStates[0]);
	this->BlackboardComp->SetValueAsEnum(TEXT("CurrLowerState"), InLowerStates[0]);

	for (int32 i = 1; i < States_Length; i++)
	{
		Upper_States.Add(InUpperStates[i]);
		Lower_States.Add(InLowerStates[i]);
	}

}

void APrisonerController::HandlePlayPrisonerLogic(int32 InRanNum)
{
	UPrisonerMgrSubSytemPtr->GetAllPrisonerControllerArr()[InRanNum]->HandleNextTask();
	return;
}

void APrisonerController::InitializeStatesFromLogicDT()
{
	//UE_LOG(LogTemp, Log, TEXT("Logic DT Init And State Init Start!"));

	Upper_States.Empty();
	Lower_States.Empty();
	States_Length = 0;
	CurrStateIndex = 0;

	/*if (mLogicDT)
	{
		UE_LOG(LogTemp, Log, TEXT("1. Logic DT Is Valid"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("1. Logic DT Is Not Valid"));
	}*/

	TArray<FName> TempRowNames = mLogicDT->GetRowNames();
	if (!TempRowNames.IsEmpty())
	{
		// Number that Allows you to Select a Row Within DataTable(Starting form 0)
		int32 RowRandomChoice = FMath::RandRange(0, TempRowNames.Num() - 1);

		FPrisonerInfoRow* MyRow = mLogicDT->FindRow<FPrisonerInfoRow>(TempRowNames[RowRandomChoice], TEXT(""));
		UEnum* UpperEnum = StaticEnum<EPrisonerUpperStateType>();
		UEnum* LowerEnum = StaticEnum<EPrisonerLowerStateType>();

		//UE_LOG(LogTemp, Log, TEXT("4. Row Print!!!!"));
		for (int32 y = 0; y < MyRow->ActionUpperState.Num(); y++)
		{
			EPrisonerUpperStateType TempUpperStates = MyRow->ActionUpperState[y];
			EPrisonerLowerStateType TempLowerStates = MyRow->ActionLowerState[y];

			FString UpperStr = UpperEnum->GetNameStringByValue((int64)TempUpperStates);
			FString LowerStr = LowerEnum->GetNameStringByValue((int64)TempLowerStates);

			Upper_States.Add((uint8)TempUpperStates);
			Lower_States.Add((uint8)TempLowerStates);
			States_Length = MyRow->ActionUpperState.Num();
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("TempRowNames Array is Empty!"));
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
		CL_FlameTransition->SetHiddenInGame(true);
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