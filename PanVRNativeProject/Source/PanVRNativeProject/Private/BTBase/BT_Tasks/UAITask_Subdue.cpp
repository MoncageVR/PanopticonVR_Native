// Fill out your copyright notice in the Description page of Project Settings.


#include "BTBase/BT_Tasks/UAITask_Subdue.h"
#include "PanVRNativeProject/PanVRNativeProject.h"
#include "CoreObj/Manager/MapObjManagerSubsystem.h"
#include "CoreObj/Manager/PrisonerManagerSubsystem.h"

UUAITask_Subdue::UUAITask_Subdue()
{
	NodeName = TEXT("BTTask_Subdue");
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UUAITask_Subdue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (Super::ExecuteTask(OwnerComp, NodeMemory) == EBTNodeResult::Failed) return EBTNodeResult::Failed;
	UE_LOG(LogTemp, Log, TEXT("UAI_Task Subdue Execute!!"));

	if (PrisonerCharacterObj->GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Flying)
	{
		PrisonerCharacterObj->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}

	PrisonerCharacterObj->LaunchCharacter(FVector(0.f, -1000.f, 1000.f), false, false);
	PrisonerCharacterObj->GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"), true);

	GetWorld()->GetTimerManager().SetTimer(SimulDelayTimer, this, &UUAITask_Subdue::thisMeshSimulatingFunc, 0.01f, false);
	GetWorld()->GetTimerManager().SetTimer(HideMeshDelayTimer, this, &UUAITask_Subdue::BeginReSpawnRagdool, 5.0f, false);

	return EBTNodeResult::InProgress;
}

void UUAITask_Subdue::thisMeshSimulatingFunc() { PrisonerCharacterObj->GetMesh()->SetSimulatePhysics(true); }

void UUAITask_Subdue::BeginReSpawnRagdool()
{
	int32 TempUniqueNum = PrisonerControllerObj->GetBBComp()->GetValueAsInt(TEXT("UniqueNum"));
	PrisonerCharacterObj->GetMesh()->SetHiddenInGame(false, false);
	PrisonerCharacterObj->GetCapsuleComponent()->SetWorldLocation(PrisonerManagerSubSystemInst->GetFinalAllSpawnPositions()[TempUniqueNum]);
	PrisonerCharacterObj->GetCapsuleComponent()->SetWorldRotation(PrisonerManagerSubSystemInst->GetFinalAllSpawnRoations()[TempUniqueNum]);
	PrisonerCharacterObj->GetMesh()->SetWorldLocation(PrisonerManagerSubSystemInst->GetFinalAllReSpawnPositions()[TempUniqueNum], false, nullptr, ETeleportType::TeleportPhysics);

	PrisonerCharacterObj->GetMesh()->SetSimulatePhysics(true);

	GetWorld()->GetTimerManager().SetTimer(ReSpawnDelayTimer, this, &UUAITask_Subdue::RegenerationFromRagdoll, 5.0f, false);
}

void UUAITask_Subdue::RegenerationFromRagdoll()
{
	PrisonerCharacterObj->GetMesh()->SetSimulatePhysics(false);
	PrisonerCharacterObj->GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));

	bool AttachResult = false;
	AttachResult = PrisonerCharacterObj->GetMesh()->AttachToComponent(PrisonerCharacterObj->GetCapsuleComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	if (AttachResult)
		PrisonerCharacterObj->GetMesh()->SetRelativeLocationAndRotation(PrisonerCharacterObj->GetMeshDefaultPos(), PrisonerCharacterObj->GetMeshDefaultRot(), false, nullptr, ETeleportType::TeleportPhysics);

	PrisonerControllerObj->GetBBComp()->SetValueAsEnum(TEXT("CurrUpperState"), 0);
	PrisonerControllerObj->GetBBComp()->SetValueAsEnum(TEXT("CurrLowerState"), 0);
}
