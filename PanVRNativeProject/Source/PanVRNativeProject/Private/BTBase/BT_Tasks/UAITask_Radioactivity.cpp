// Fill out your copyright notice in the Description page of Project Settings.


#include "BTBase/BT_Tasks/UAITask_Radioactivity.h"
#include "PanVRNativeProject/PanVRNativeProject.h"
#include "CoreObj/VREquipmentWorldSubsystem.h"
#include "BPActorBase/AFuelTank.h"
#include "BPActorBase/AFuelRod.h"

UUAITask_Radioactivity::UUAITask_Radioactivity()
{
	NodeName = TEXT("BTTask_Radioactivity");
	bCreateNodeInstance = true;
	RadioactivityCheckTime = 40.0f;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AMFinder_Explode(TEXT("/Game/VRContent/Prisoner/PrisonerAnimation/TargetAnim/Montages/Retarget_Explode_Anim_Montage.Retarget_Explode_Anim_Montage"));
	if (AMFinder_Explode.Succeeded())
	{
		ExplodeMontage = AMFinder_Explode.Object;
	}
}

EBTNodeResult::Type UUAITask_Radioactivity::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (Super::ExecuteTask(OwnerComp, NodeMemory) == EBTNodeResult::Failed) return EBTNodeResult::Failed;
	UE_LOG(LogTemp, Log, TEXT("UAI_Task Radioactivity Execute!!"));

	UVREquipmentWorldSubsystem* TempVREquipmentSubSystem = GetWorld()->GetSubsystem<UVREquipmentWorldSubsystem>();
	if (!ensure(TempVREquipmentSubSystem)) return EBTNodeResult::Failed;
	for (IIEquipmentInitInterface* Var : TempVREquipmentSubSystem->GetEquipmentArr())
	{
		FuelTankObj = Cast<AAFuelTank>(Var);
		if (FuelTankObj)
			break;
		else
			continue;
	}

	// 4 = UpperState : Dangerous , 15 = LowerState : Radioactivity
	PrisonerControllerObj->GetPrisonerAnimInstance()->SetPrisonerUpperStates(4, 15);

	MyAnimInst->OnMontageEnded.RemoveDynamic(this, &UUAITask_Radioactivity::OnExplodeMontageEnded);
	MyAnimInst->OnMontageEnded.AddDynamic(this, &UUAITask_Radioactivity::OnExplodeMontageEnded);

	GetWorld()->GetTimerManager().SetTimer(CheckExplosionForGameOverTimer, this, &UUAITask_Radioactivity::ExplosureAndGameover, RadioactivityCheckTime, false);

	return EBTNodeResult::InProgress;
}

void UUAITask_Radioactivity::ExplosureAndGameover()
{
	if (!ensure(FuelTankObj)) return;
	if (IsValid(FuelTankObj))
	{
		UE_LOG(LogTemp, Log, TEXT("AttachFlag : %d"), FuelTankObj->GetCurrEquipFuelRod()->GetFRIsAttaching());
		UE_LOG(LogTemp, Log, TEXT("LowGaugeFlag : %d"), FuelTankObj->GetCurrEquipFuelRod()->GetWasLowGaugeFlag());

		if (FuelTankObj->GetCurrEquipFuelRod()->GetFRIsAttaching() && !FuelTankObj->GetCurrEquipFuelRod()->GetWasLowGaugeFlag())
		{
			UE_LOG(LogTemp, Log, TEXT("Radioactivity Subdue!"));
			PrisonerControllerObj->GetBBComp()->SetValueAsEnum(TEXT("CurrUpperState"), 0);
			PrisonerControllerObj->GetBBComp()->SetValueAsEnum(TEXT("CurrLowerState"), 0);

			// 0 = UpperState : Idle , 1 = LowerState : Default
			PrisonerControllerObj->GetPrisonerAnimInstance()->SetPrisonerUpperStates(0, 1);

		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Radioactivity Failed, Explosure Play~~"));
			MyAnimInst->Montage_Play(ExplodeMontage);
		}
	}
}

void UUAITask_Radioactivity::OnExplodeMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	UE_LOG(LogTemp, Log, TEXT("Explode Montage Play End!"));
	// GameOver Check Logic Execute Parts!

	MyVRGameMode->SetIsGameOverFlag(true);
	MyVRGameMode->GameOverCheckEvent();
	// Finish Latented(Tasks)
}