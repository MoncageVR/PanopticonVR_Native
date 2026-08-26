#include "BTBase/BT_Tasks/UAITask_Golf.h"
#include "PanVRNativeProject/PanVRNativeProject.h"
#include "CoreObj/Manager/GameInstanceSubSystem/PrisonerManagerSubsystem.h"
#include "CoreObj/Manager/WorldSubSystem/VREquipmentWorldSubsystem.h"
#include "CoreCommon/Interface/IEquipmentInitInterface.h"
#include "EquipmentActor/AElevatorButton.h"
#include "EquipmentActor/Spawned_Actors/AGolfClub.h"
#include "EquipmentActor/Spawned_Actors/AGolfBall.h"

UUAITask_Golf::UUAITask_Golf()
{
	NodeName = TEXT("BTTask_Golf");
	bCreateNodeInstance = true;

	GolfSwingCount = 0;
	GolfBallMake_Time = 1.2f;
	GolfNextActionWait_Time = 4.63f;
	
	static ConstructorHelpers::FObjectFinder<USoundBase> SFXFinder_GolfShooting(TEXT("/Game/VRContent/Sound/Wavs/PrisonerRelated/Golf/sfx_golf_shoot.sfx_golf_shoot"));
	if (SFXFinder_GolfShooting.Succeeded())
		SFX_GolfShooting = SFXFinder_GolfShooting.Object;
}

EBTNodeResult::Type UUAITask_Golf::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (Super::ExecuteTask(OwnerComp, NodeMemory) == EBTNodeResult::Failed) return EBTNodeResult::Failed;
	//UE_LOG(LogTemp, Log, TEXT("UAI_Task Golf Execute!!"));

	GolfSwingCount = 0;

	if (!IsValid(MyGolfClubObj))
	{
		SetStateEntryTransform();
		SpawnAndAttachGolfClub();

		if (MyAnimInst)
		{
			// UpperState : Confusion(5) | LowerState : Golf(18)
			PrisonerControllerObj->GetPrisonerAnimInstance()->SetPrisonerUpperStates(5, 18);
		}
		GolfBallFly();
	}

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UUAITask_Golf::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (UWorld* mWorld = GetWorld())
	{
		mWorld->GetTimerManager().PauseTimer(GolfBallMakeTimer);
		mWorld->GetTimerManager().ClearTimer(GolfBallMakeTimer);

		mWorld->GetTimerManager().PauseTimer(GolfStateNextForWaitTimer);
		mWorld->GetTimerManager().ClearTimer(GolfStateNextForWaitTimer);

		MyGolfClubObj->Destroy();
		// UpperState : Idle(0) | LowerState : Default(0)
		PrisonerControllerObj->GetPrisonerAnimInstance()->SetPrisonerUpperStates(0, 0);
	}

	return Super::AbortTask(OwnerComp, NodeMemory);
}

void UUAITask_Golf::GolfBallFly()
{
	if (GolfSwingCount <= 4)
	{
		HVRSoundPlayer::PlaySoundEffect(this, SFX_GolfShooting, PrisonerCharacterObj->GetRootComponent()->GetComponentLocation());
		GetWorld()->GetTimerManager().SetTimer(
			GolfBallMakeTimer,
			this,
			&UUAITask_Golf::ActuallyGolfBallFly,
			GolfBallMake_Time,
			false
		);
	}
	else
	{
		GolfStateEnd();
	}
}

void UUAITask_Golf::ActuallyGolfBallFly()
{
	DetermineTheinCurveEnd(PrisonerControllerObj->GetBBComp()->GetValueAsInt(TEXT("UniqueNum")));

	GolfBallStartAndSpawnVec = (PrisonerCharacterObj->GetRootComponent()->GetComponentLocation()) + (PrisonerCharacterObj->GetRootComponent()->GetForwardVector() * FVector(200.0f, 200.0f, 0.0f));

	// Initialize For GolfBall Spawn Transform And Spawn Parameters
	FTransform TempGolfBallTransform = FTransform(FRotator::ZeroRotator, GolfBallStartAndSpawnVec, FVector::OneVector);
	FActorSpawnParameters TempGolfBallSpawnParams;
	TempGolfBallSpawnParams.Owner = PrisonerCharacterObj;
	TempGolfBallSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	// GolfBall Spawn Part
	MyGolfBallObj = GetWorld()->SpawnActor<AAGolfBall>(AAGolfBall::StaticClass(), TempGolfBallTransform, TempGolfBallSpawnParams);

	// Updating And Launching the Golf Ball Trajectory
	MyGolfBallObj->UpdateBallTrajectorySpline(GolfBallStartAndSpawnVec, GolfBallTargetVec);

	MyGolfBallObj->SetGolfEffectEmergenceDegree(GolfBallTargetMinDegree, GolfBallTargetMaxDegree, GolfBallTargetVec.Z);

	GolfSwingCount++;

	GetWorld()->GetTimerManager().SetTimer(
		GolfStateNextForWaitTimer,
		this,
		&UUAITask_Golf::GolfBallFly,
		GolfNextActionWait_Time,
		false
	);
}

void UUAITask_Golf::SetStateEntryTransform()
{
	int32 TempIndex = PrisonerControllerObj->GetBBComp()->GetValueAsInt(TEXT("UniqueNum"));

	FVector TempPos = FVector(PrisonerManagerSubSystemInst->GetFinalAllSpawnPositions()[TempIndex].X, PrisonerManagerSubSystemInst->GetFinalAllSpawnPositions()[TempIndex].Y, PrisonerCharacterObj->GetRootComponent()->GetComponentLocation().Z);
	FRotator TempRot = PrisonerManagerSubSystemInst->GetFinalAllSpawnRoations()[TempIndex];

	PrisonerCharacterObj->GetRootComponent()->SetWorldLocationAndRotation(TempPos,
		TempRot, false, nullptr, ETeleportType::TeleportPhysics);

	return;
}

void UUAITask_Golf::SpawnAndAttachGolfClub()
{
	FTransform TempClubSpawnTransform = PrisonerCharacterObj->GetMesh()->GetSocketTransform(TEXT("GolfClub_Socket"));
	FActorSpawnParameters TempSpawnParams;
	TempSpawnParams.Owner = PrisonerCharacterObj;
	TempSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	MyGolfClubObj = GetWorld()->SpawnActor<AAGolfClub>(AAGolfClub::StaticClass(), TempClubSpawnTransform, TempSpawnParams);

	MyGolfClubObj->AttachToComponent(PrisonerCharacterObj->GetMesh(), FAttachmentTransformRules::KeepWorldTransform, TEXT("GolfClub_Socket"));
}

void UUAITask_Golf::DetermineTheinCurveEnd(int32 InmUniqueNum)
{

	if (PrisonerManagerSubSystemInst->GetAlphaZonePrisonerUniqueNumArrs().Contains(InmUniqueNum))
	{
		GolfBallTargetVec = FVector(155.0f, 0.0f, DeterminetheTowerHeight());
		GolfBallTargetMinDegree = -60.0f;
		GolfBallTargetMaxDegree = 60.0f;
	}
	else if (PrisonerManagerSubSystemInst->GetBetaZonePrisonerUniqueNumArrs().Contains(InmUniqueNum))
	{
		UE_LOG(LogTemp, Log, TEXT("%d PrisonerCharacter In Beta Zone"), InmUniqueNum);
		GolfBallTargetVec = FVector(-81.0f, 131.0f, DeterminetheTowerHeight());
		GolfBallTargetMinDegree = 65.0f;
		GolfBallTargetMaxDegree = 180.0f;
	}
	else if (PrisonerManagerSubSystemInst->GetGammaZonePrisonerUniqueNumArrs().Contains(InmUniqueNum))
	{
		UE_LOG(LogTemp, Log, TEXT("%d PrisonerCharacter In Gamma Zone"), InmUniqueNum);
		GolfBallTargetVec = FVector(-81.0f, -131.0f, DeterminetheTowerHeight());
		GolfBallTargetMinDegree = 185.0f;
		GolfBallTargetMaxDegree = 310.0f;
	}
}

float UUAITask_Golf::DeterminetheTowerHeight()
{
	UVREquipmentWorldSubsystem* TempVREquipmentWorldSubSystemRef = GetWorld()->GetSubsystem<UVREquipmentWorldSubsystem>();
	AAElevatorButton* TempElevatorObj = nullptr;

	check(TempVREquipmentWorldSubSystemRef);

	for (TScriptInterface<IIEquipmentInitInterface> Equip : TempVREquipmentWorldSubSystemRef->GetEquipmentArr())
	{
		IIEquipmentInitInterface* IEquipPtr = Equip.GetInterface();
		TempElevatorObj = Cast<AAElevatorButton>(IEquipPtr);
		if (TempElevatorObj)
			break;
		else
			continue;
	}

	if (TempElevatorObj->GetCurrFloorNum() == 1)
	{
		return 300.0f;
	}
	else if (TempElevatorObj->GetCurrFloorNum() == 2)
	{
		return 1200.0f;
	}
	else if (TempElevatorObj->GetCurrFloorNum() == 3)
	{
		return 2150.0f;
	}
	else
	{
		return 0.0f;
	}
}

void UUAITask_Golf::GolfStateEnd()
{
	MyGolfClubObj->Destroy();
	// UpperState : Idle(0) | LowerState : Default(0)
	PrisonerControllerObj->GetPrisonerAnimInstance()->SetPrisonerUpperStates(0, 0);
	PrisonerControllerObj->HandleNextTask();
	return;
}
