// Fill out your copyright notice in the Description page of Project Settings.


#include "BTBase/BT_Tasks/UAITask_TopEscape.h"
#include "PanVRNativeProject/PanVRNativeProject.h"
#include "CoreObj/VREquipmentWorldSubsystem.h"
#include "BPMainActorBase/JailBuilding.h"

UUAITask_TopEscape::UUAITask_TopEscape()
{
	NodeName = TEXT("BTTask_TopEscape");
	bCreateNodeInstance = true;
	bNotifyTick = true;
	bIsRotatePossibleFlag = false;
}

EBTNodeResult::Type UUAITask_TopEscape::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (Super::ExecuteTask(OwnerComp, NodeMemory) == EBTNodeResult::Failed) return EBTNodeResult::Failed;
	UE_LOG(LogTemp, Log, TEXT("UAI_Task TopEscape Execute!!"));

	//bIsRotatePossibleFlag = false;
	UVREquipmentWorldSubsystem* TempVREquipmentSubSystem = GetWorld()->GetSubsystem<UVREquipmentWorldSubsystem>();
	if (!ensure(TempVREquipmentSubSystem)) return EBTNodeResult::Failed;
	for (IIEquipmentInitInterface* Var : TempVREquipmentSubSystem->GetEquipmentArr())
	{
		mJailBuildingObj = Cast<AJailBuilding>(Var);
		if (mJailBuildingObj)
			break;
		else
			continue;
	}

	bool TempAttachResult = PrisonerCharacterObj->GetRootComponent()->AttachToComponent(mJailBuildingObj->GetHatchSMComp(), FAttachmentTransformRules::KeepWorldTransform);
	if (TempAttachResult)
	{
		PrisonerCharacterObj->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);

		PrisonerCharacterObj->GetRootComponent()->SetWorldLocationAndRotation(
			FVector(60.f, 40.f, 3630.f),
			FRotator(0.f, -144.f, 0.f),
			false,
			nullptr,
			ETeleportType::TeleportPhysics
		);

		// 4 = UpperState : Dangerous , 12 = LowerState : TopEscape
		PrisonerControllerObj->GetPrisonerAnimInstance()->SetPrisonerUpperStates(4, 12);

		bIsRotatePossibleFlag = true;

		GetWorld()->GetTimerManager().SetTimer(
			HatchOpenSuccessTimer,
			this,
			&UUAITask_TopEscape::CallHandleHatchDoor,
			17.9f,
			false
		);
	}
	else
	{
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::InProgress;

}

void UUAITask_TopEscape::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (bIsRotatePossibleFlag)
	{
		mJailBuildingObj->GetHatchSMComp()->SetRelativeRotation(
			FRotator(
				mJailBuildingObj->GetHatchSMComp()->GetRelativeRotation().Pitch,
				mJailBuildingObj->GetHatchSMComp()->GetRelativeRotation().Yaw + 1.0f,
				mJailBuildingObj->GetHatchSMComp()->GetRelativeRotation().Roll
			)
		);
	}
}

void UUAITask_TopEscape::CallHandleHatchDoor()
{
	bIsRotatePossibleFlag = false;
	mJailBuildingObj->HandleHatchDoor();
	GetWorld()->GetTimerManager().ClearTimer(HatchOpenSuccessTimer);
	GetWorld()->GetTimerManager().PauseTimer(HatchOpenSuccessTimer);
}
