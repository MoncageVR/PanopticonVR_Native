


#include "BTBase/BT_Tasks/UAITask_Flame.h"
#include "PanVRNativeProject/PanVRNativeProject.h"
#include "CoreObj/Manager/GameInstanceSubSystem/MapObjManagerSubsystem.h"
#include "NavigationSystem.h"
#include "NavigationSystem/Public/NavigationPath.h"
#include "Components/SphereComponent.h"

UUAITask_Flame::UUAITask_Flame()
{
	NodeName = TEXT("BTTask_Flame");
	bCreateNodeInstance = true;

	BaseCoordinateHeightArrs.Empty();
	BaseCoordinateHeightArrs.Reserve(4);
	BaseCoordinateHeightArrs.Add(65.1f);
	BaseCoordinateHeightArrs.Add(194.4f);
	BaseCoordinateHeightArrs.Add(1123.8f);
	BaseCoordinateHeightArrs.Add(2053.1f);
	
	BaseCoordinate1stRadius.Empty();
	BaseCoordinate1stRadius.Reserve(2);
	BaseCoordinate1stRadius.Add(300.0f);
	BaseCoordinate1stRadius.Add(1050.0f);
}

EBTNodeResult::Type UUAITask_Flame::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (Super::ExecuteTask(OwnerComp, NodeMemory) == EBTNodeResult::Failed) return EBTNodeResult::Failed;
	//UE_LOG(LogTemp, Log, TEXT("UAI_Task Flame Execute!!"));

	if (HasReachedFlameTargetVec(PrisonerCharacterObj->GetRootComponent()->GetComponentLocation(), PrisonerControllerObj->GetBBComp()->GetValueAsVector("FlameTargetVec")))
	{
		if (MoveCount)
		{
			ActuallyMoveFlameRun();
			return EBTNodeResult::Succeeded;
		}
	}
	else
	{
		InitBeforeFlameRun();
		// UpperState : Dangerous(4) | LowerState : Flame(15)
		PrisonerControllerObj->GetPrisonerAnimInstance()->SetPrisonerUpperStates(4, 15);

		if (AttachSphereCollision())
		{
			MapObjManagerSubSystemInst->HandleFlyingTheGrating(PrisonerControllerObj->GetBBComp()->GetValueAsInt(TEXT("UniqueNum")));

			ActuallyMoveFlameRun();

			GetWorld()->GetTimerManager().SetTimer(
				FlameTransitionTimer,
				this,
				&UUAITask_Flame::ActuallyFlameTransition,
				10.0f,
				true
			);
		}

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Succeeded;
}

bool UUAITask_Flame::AttachSphereCollision()
{

	if (!IsValid(FlameTransitionCL))
	{
		FlameTransitionCL = Cast<USphereComponent>(PrisonerCharacterObj->AddComponentByClass(USphereComponent::StaticClass(), false, FTransform::Identity, false));
	}

	FlameTransitionCL->SetSphereRadius(900.0f, true);
	FlameTransitionCL->SetHiddenInGame(false); // Debug
	FlameTransitionCL->SetGenerateOverlapEvents(true);

	return FlameTransitionCL->GetGenerateOverlapEvents();
}

void UUAITask_Flame::FlameCLOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->ComponentHasTag(TEXT("PrisonerCharacter")))
	{
		APrisonerCharacter* OtherPrisonerCha = Cast<APrisonerCharacter>(OtherActor);
		checkf(OtherPrisonerCha, TEXT("PrisonerCha Not Valid"));
		AAIController* OtherAiController = Cast<AAIController>(OtherPrisonerCha->GetController());
		checkf(OtherAiController, TEXT("PrisonerCha AI Controller Not Valid"));
		APrisonerController* OtherPrisonerCon = Cast<APrisonerController>(OtherAiController);
		checkf(OtherPrisonerCon, TEXT("PrisonerCha Controller Not Valid"));

		if (OtherPrisonerCon->GetBBComp()->GetValueAsEnum(TEXT("CurrUpperState")) == 0 && OtherPrisonerCon->GetBBComp()->GetValueAsEnum(TEXT("CurrLowerState")) == 0)
		{
			TArray<uint8> GivenUpperState = { 4 };
			TArray<uint8> GivenLowerState = { 15 };

			OtherPrisonerCon->State_based_ExecutionTasks_GiventoSomeone(GivenUpperState, GivenLowerState);

			FlameTransitionCL->SetGenerateOverlapEvents(false);
			FlameTransitionCL->OnComponentBeginOverlap.RemoveDynamic(this, &UUAITask_Flame::FlameCLOverlapBegin);
		}
	}
}

void UUAITask_Flame::ActuallyMoveFlameRun()
{
	if (MoveCount < 3)
	{
		MakeFlameRandomVec();
		MoveCount++;
		return;
	}
	else // Flame Run 3 Time Finish Logic Parts
	{
		// UpperState : Idle(0) | LowerState : Default(0)
		PrisonerControllerObj->GetPrisonerAnimInstance()->SetPrisonerUpperStates(0, 0);
		FlameTransitionCL->OnComponentBeginOverlap.RemoveDynamic(this, &UUAITask_Flame::FlameCLOverlapBegin);
		GetWorld()->GetTimerManager().PauseTimer(FlameTransitionTimer);
		GetWorld()->GetTimerManager().ClearTimer(FlameTransitionTimer);
		PrisonerControllerObj->HandleNextTask();
	}
}

void UUAITask_Flame::InitBeforeFlameRun()
{
	PrisonerCharacterObj->GetCharacterMovement()->MaxWalkSpeed = PrisonerControllerObj->GetBBComp()->GetValueAsFloat(TEXT("RunningSpeed"));
	PrisonerCharacterObj->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	FireEffectSpawnVec = PrisonerCharacterObj->GetActorLocation();
	MoveCount = 0;
}

void UUAITask_Flame::MakeFlameRandomVec()
{
	float TempReturnZ = 0.0f;
	float TempDegree = 0.0f;
	float TempRadius = 0.0f;
	FVector TempReturnVec = FVector::ZeroVector;

	// Initialize Common Values For Calculate 
	TempDegree = FMath::RandRange(0.0f, 360.0f);
	TempReturnZ = BaseCoordinateHeightArrs[FMath::RandRange(0, BaseCoordinateHeightArrs.Num() - 1)];

	// When Making a Flame Target Vector at the 1st Floor Coordinates
	if (TempReturnZ == BaseCoordinateHeightArrs[0])
	{
		TempRadius = FMath::FRandRange(BaseCoordinate1stRadius[0], BaseCoordinate1stRadius[1]);
		TempReturnVec = CalculateFlameRandomvec(TempDegree, TempRadius, TempReturnZ);
	}
	else // When Making a Flame Target Vector at the Coordinates of the Remaining Floors
	{
		TempRadius = 1425.0f;
		TempReturnVec = CalculateFlameRandomvec(TempDegree, TempRadius, TempReturnZ);
	}

	PrisonerControllerObj->GetBBComp()->SetValueAsVector(TEXT("FlameTargetVec"), TempReturnVec);
}

const FVector UUAITask_Flame::CalculateFlameRandomvec(float InDegree, float InRadius, float InZPos)
{
	float TempRadianAngle = FMath::DegreesToRadians(InDegree);

	FVector TempReturnVector = FVector(
		FMath::Cos(TempRadianAngle) * InRadius,
		FMath::Sin(TempRadianAngle) * InRadius,
		InZPos
	);

	return TempReturnVector;
}

bool UUAITask_Flame::HasReachedFlameTargetVec(FVector InChaVec, FVector InTargetVec)
{
	bool XReturnValue = false;
	bool YReturnValue = false;
	if (FMath::IsNearlyEqual(InChaVec.X, InTargetVec.X, 5.0f))
		XReturnValue = true;

	if (FMath::IsNearlyEqual(InChaVec.Y, InTargetVec.Y, 5.0f))
		YReturnValue = true;

	return (XReturnValue && YReturnValue);
}

void UUAITask_Flame::ActuallyFlameTransition()
{
	//UE_LOG(LogTemp, Log, TEXT("Called every 10 seconds, Flame state transition!"));
	FlameTransitionCL->OnComponentBeginOverlap.AddDynamic(this, &UUAITask_Flame::FlameCLOverlapBegin);
	FlameTransitionCL->SetGenerateOverlapEvents(true);
	return;
}
