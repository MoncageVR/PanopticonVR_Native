#include "BTBase/BT_Tasks/UAITask_TopEscape.h"
#include "PanVRNativeProject/PanVRNativeProject.h"
#include "CoreObj/Manager/WorldSubSystem/VREquipmentWorldSubsystem.h"
#include "MainActor/JailBuilding.h"

UUAITask_TopEscape::UUAITask_TopEscape()
{
	NodeName = TEXT("BTTask_TopEscape");
	bCreateNodeInstance = true;
	bNotifyTick = true;
	bIsRotatePossibleFlag = false;

	static ConstructorHelpers::FObjectFinder<USoundBase> SFXFinder_TopEscape(TEXT("/Game/VRContent/Sound/Wavs/PrisonerRelated/TopEscape/sfx_topescape.sfx_topescape"));
	if (SFXFinder_TopEscape.Succeeded())
		SFX_TopEscape = SFXFinder_TopEscape.Object;
}

EBTNodeResult::Type UUAITask_TopEscape::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (Super::ExecuteTask(OwnerComp, NodeMemory) == EBTNodeResult::Failed) return EBTNodeResult::Failed;
	UE_LOG(LogTemp, Log, TEXT("UAI_Task TopEscape Execute!!"));

	//bIsRotatePossibleFlag = false;
	UVREquipmentWorldSubsystem* TempVREquipmentSubSystem = GetWorld()->GetSubsystem<UVREquipmentWorldSubsystem>();
	if (!ensure(TempVREquipmentSubSystem)) return EBTNodeResult::Failed;

	for (TScriptInterface<IIEquipmentInitInterface> Equip : TempVREquipmentSubSystem->GetEquipmentArr())
	{
		IIEquipmentInitInterface* IEquipPtr = Equip.GetInterface();
		mJailBuildingObj = Cast<AJailBuilding>(IEquipPtr);
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

		// UpperState : Dangerous(4) | LowerState : TopEscape(11)
		PrisonerControllerObj->GetPrisonerAnimInstance()->SetPrisonerUpperStates(4, 11);
		PrisonerCharacterObj->HandlePlayAPSound(SFX_TopEscape);
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
