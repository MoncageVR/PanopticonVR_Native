#include "BTBase/BT_Tasks/UAITask_Escape.h"
#include "CoreObj/Manager/GameInstanceSubSystem/MapObjManagerSubsystem.h"
#include "PanVRNativeProject/PanVRNativeProject.h"

UUAITask_Escape::UUAITask_Escape()
{
	NodeName = TEXT("BTTask_Escape");
	bCreateNodeInstance = true;

	static ConstructorHelpers::FObjectFinder<USoundBase> SFXFinder_Button(TEXT("/Game/VRContent/Sound/Wavs/PrisonerRelated/Escape/sfx_escape_button.sfx_escape_button"));
	if (SFXFinder_Button.Succeeded())
		SFX_ButtonClick = SFXFinder_Button.Object;
}

EBTNodeResult::Type UUAITask_Escape::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (Super::ExecuteTask(OwnerComp, NodeMemory) == EBTNodeResult::Failed)
		return EBTNodeResult::Failed;

	// UpperState : Dangerous(4) | LowerState : Escape(12)
	PrisonerControllerObj->GetPrisonerAnimInstance()->SetPrisonerUpperStates(4, 12);
	PrisonerCharacterObj->HandlePlayAPSound(SFX_ButtonClick);

	if (UWorld* MyWolrd = GetWorld())
	{
		if (!(MyWolrd->GetTimerManager().IsTimerActive(ExitDoorControlTimer)))
		{
			// Debug Value : 1.9f , DefaultValue : 17.9f;
			GetWorld()->GetTimerManager().SetTimer(
				ExitDoorControlTimer,
				this,
				&UUAITask_Escape::CallControlExitDoorFunction,
				1.9f,
				false
			);
			return EBTNodeResult::Succeeded;
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("ExitDoor Control Timer Is Activate!!"));
			return EBTNodeResult::InProgress;
		}
	}
	else
	{
		return EBTNodeResult::Failed;
	}
}

void UUAITask_Escape::CallControlExitDoorFunction()
{
	PrisonerCharacterObj->HandlePauseAPSound();
	MapObjManagerSubSystemInst->ControlExitDoorFunction();
}
