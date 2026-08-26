#include "BTBase/BT_Tasks/UAITask_Floating.h"
#include "PanVRNativeProject/PanVRNativeProject.h"
#include "Kismet/GameplayStatics.h"

UUAITask_Floating::UUAITask_Floating()
{
	NodeName = TEXT("BTTask_Floating");
	bCreateNodeInstance = true;
	FloatingExecutionTime = 30.0f;

	static ConstructorHelpers::FObjectFinder<USoundBase> SFXFinder_Floating(TEXT("/Game/VRContent/Sound/Wavs/PrisonerRelated/Floating/sfx_float.sfx_float"));
	if (SFXFinder_Floating.Succeeded())
		SFX_Floating = SFXFinder_Floating.Object;
}

EBTNodeResult::Type UUAITask_Floating::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (Super::ExecuteTask(OwnerComp, NodeMemory) == EBTNodeResult::Failed) return EBTNodeResult::Failed;
	//UE_LOG(LogTemp, Log, TEXT("UAI_Task Floating Execute!!"));

	FloatingTargetVec = PrisonerControllerObj->GetBBComp()->GetValueAsVector(TEXT("TopEscapeTargetVec"));
	PrisonerCharacterObj->GetCapsuleComponent()->SetEnableGravity(true);
	PrisonerCharacterObj->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);

	PrisonerCharacterObj->HandlePlayAPSound(SFX_Floating);

	this->ActuallyMoveToTargetVec();
	MyVRGameMode->HandleListOfFloatNTelePrisoners(1, PrisonerControllerObj->GetBBComp()->GetValueAsInt(FName("UniqueNum")));

	return EBTNodeResult::InProgress;

}

EBTNodeResult::Type UUAITask_Floating::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UE_LOG(LogTemp, Log, TEXT("Floating State Abort By Someone"));
	if (UWorld* World = GetWorld())
	{
		World->GetLatentActionManager().RemoveActionsForObject(this);
	}

	return EBTNodeResult::Failed;
}

void UUAITask_Floating::OnFloatingFinishedFunc()
{
	UE_LOG(LogTemp, Log, TEXT("Floating Arrived!"));
	MyVRGameMode->HandleListOfFloatNTelePrisoners(0, PrisonerControllerObj->GetBBComp()->GetValueAsInt(FName("UniqueNum")));
	PrisonerControllerObj->OnTaskFinished.Broadcast();
}

void UUAITask_Floating::ActuallyMoveToTargetVec()
{
	// UpperState : Move(2) | LowerState : Floating(5)
	PrisonerControllerObj->GetPrisonerAnimInstance()->SetPrisonerUpperStates(2, 5);

	bool TempNearlyFlag = false;
	TempNearlyFlag = FloatingTargetVec.Equals(PrisonerCharacterObj->GetCapsuleComponent()->GetComponentLocation(), 10.0f);

	FloatingStateLatentInfo.CallbackTarget = this;
	FloatingStateLatentInfo.Linkage = 0;
	FloatingStateLatentInfo.ExecutionFunction = FName("OnFloatingFinishedFunc");
	FloatingStateLatentInfo.UUID = 1003;

	if (!TempNearlyFlag)
	{
		UKismetSystemLibrary::MoveComponentTo(
			PrisonerCharacterObj->GetCapsuleComponent(),
			FloatingTargetVec,
			FRotator(0.0f, 180.f, 0.0f),
			false,
			false,
			FloatingExecutionTime,
			false,
			EMoveComponentAction::Move,
			FloatingStateLatentInfo
		);
	}
}
