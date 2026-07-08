// Fill out your copyright notice in the Description page of Project Settings.


#include "BPActorBase/AGlove.h"
#include "CoreObj/VREquipmentWorldSubsystem.h"
#include "Components/CapsuleComponent.h"
#include "Components/TimelineComponent.h"

AAGlove::AAGlove()
{
	PrimaryActorTick.bCanEverTick = false;

	const int32 GloveCount = 10;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_GloveBody(TEXT("/Game/VRContent/Modeling/Glove/GloveBody.GloveBody"));

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialFinder_Main(TEXT("/Game/VRContent/Material/SRS_STAGE_Main.SRS_STAGE_Main"));

	static ConstructorHelpers::FObjectFinder<USoundBase> SoundFinder_Glove(TEXT("/Game/VRContent/Sound/Wavs/Glove/sfx_glove.sfx_glove"));
	if (SoundFinder_Glove.Succeeded())
	{
		//GloveOperationSFX = SoundFinder_Glove.Object;
	}

	if (ModelingFinder_GloveBody.Succeeded() && MaterialFinder_Main.Succeeded())
	{
		for (int32 i = 0; i < GloveCount; i++)
		{
			FString MeshName = FString::Printf(TEXT("SM_Glove_%d"), i + 1);
			UStaticMeshComponent* TempMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(*MeshName);
			TempMeshComp->SetupAttachment(ActorBaseMesh);

			TempMeshComp->SetStaticMesh(ModelingFinder_GloveBody.Object);
			TempMeshComp->SetMaterial(0, MaterialFinder_Main.Object);

			GloveStaticMeshes.Add(TempMeshComp);

			// ----------

			FString CollisionName = FString::Printf(TEXT("CL_Glove_%d"), i + 1);
			UCapsuleComponent* TempCollisionComp = CreateDefaultSubobject<UCapsuleComponent>(*CollisionName);

			TempCollisionComp->SetupAttachment(GloveStaticMeshes[i]);
			TempCollisionComp->SetRelativeLocation(FVector(0.f, -110.f, -20.f));
			TempCollisionComp->SetRelativeRotation(FRotator(0.f, 0.f, -90.f));
			TempCollisionComp->SetCapsuleRadius(100.0f);
			TempCollisionComp->SetCapsuleHalfHeight(130.0f);

			TempCollisionComp->SetHiddenInGame(false); // Debug

			GloveCollisions.Add(TempCollisionComp);

			if (i == 0)
			{
				continue;
			}
			else
			{
				WorldYaws.Add(WorldYaws[i - 1] - 36);
				LocalYaws.Add(LocalYaws[i - 1] - 36);
			}
		}

		int TempWorldYawIndex = 9;
		for (int32 i = 0; i <= 9; i++)
		{
			float TempRadian = FMath::DegreesToRadians(WorldYaws[TempWorldYawIndex]);

			GloveStaticMeshes[i]->SetRelativeLocation(FVector(
				1571.0f * FMath::Cos(float(TempRadian)),
				1571.0f * FMath::Sin(float(TempRadian)),
				0.f
			));
			TempWorldYawIndex -= 1;
		}

		int TempLocalYawIndex = 9;
		for (int i = 0; i <= 9; i++)
		{
			GloveStaticMeshes[i]->SetRelativeRotation(FRotator(
				0.f,
				LocalYaws[TempLocalYawIndex],
				0.f
			));
			TempLocalYawIndex -= 1;
		}
	}

	ForwardMoveTheGloveTimeline = CreateDefaultSubobject<UTimelineComponent>("ForwardMoveTLComp");
	BackwardMoveTheGloveTimeline = CreateDefaultSubobject<UTimelineComponent>("BackwardMoveTLComp");

	static ConstructorHelpers::FObjectFinder<UCurveFloat> CurveFinder_Glove(TEXT("/Game/VRContent/Blueprints/TimelineCurve/Glove_MoveCurve.Glove_MoveCurve"));
	if (CurveFinder_Glove.Succeeded())
	{
		MoveTheGloveFloatCurve = CurveFinder_Glove.Object;
	}

	for (UStaticMeshComponent* Mesh : GloveStaticMeshes)
	{
		if (!Mesh) continue;
		GloveStartVecs.Add(Mesh->GetRelativeLocation());
	}

	for (int i = 9; i >= 0; i--)
	{
		float TempRadians = FMath::DegreesToRadians(WorldYaws[i]);

		GloveTargetVecs.Add(
			FVector(
				1250.f * FMath::Cos(TempRadians),
				1250.f * FMath::Sin(TempRadians),
				0.f
			)
		);
	}

	TArray<UPrimitiveComponent*> AllComps;
	GetComponents<UPrimitiveComponent>(AllComps);
	for (UPrimitiveComponent* AllComp : AllComps)
	{
		if (!AllComp) continue;

		if (AllComp->CanEverAffectNavigation())
			AllComp->SetCanEverAffectNavigation(false);
		else
			continue;
	}
}

void AAGlove::BeginPlay()
{
	Super::BeginPlay();

	this->EquipmentRegistrable(this);
	if (EquipmentWorldSubSystem)
	{
		EquipmentWorldSubSystem->FEBMoveOrderSignature.AddDynamic(this, &AAGlove::HandleGloveReceiveByEB);

		// Jail ¡æ Glove : Bind
		EquipmentWorldSubSystem->FPunchStartSignature.BindUObject(this, &AAGlove::HandleGloveReceiveByJail);
	}

	if (MoveTheGloveFloatCurve)
	{
		FOnTimelineFloat ForwardProgressFunc;
		ForwardProgressFunc.BindUFunction(this, FName("ForwardMoveTheGlovePlayEvent"));
		ForwardMoveTheGloveTimeline->AddInterpFloat(MoveTheGloveFloatCurve, ForwardProgressFunc);

		FOnTimelineEvent ForwardFinishedEvent;
		ForwardFinishedEvent.BindUFunction(this, FName("ForwardMoveTheGloveFinishedEvent"));
		ForwardMoveTheGloveTimeline->SetTimelineFinishedFunc(ForwardFinishedEvent);

		FOnTimelineFloat BackwardProgressFunc;
		BackwardProgressFunc.BindUFunction(this, FName("BackwardMoveTheGlovePlayEvent"));
		BackwardMoveTheGloveTimeline->AddInterpFloat(MoveTheGloveFloatCurve, BackwardProgressFunc);

		FOnTimelineEvent BackwardFinishedEvent;
		BackwardFinishedEvent.BindUFunction(this, FName("BackwardMoveTheGloveFinishedEvent"));
		BackwardMoveTheGloveTimeline->SetTimelineFinishedFunc(BackwardFinishedEvent);
	}
}

void AAGlove::EquipmentRegistrable(AActor* InActor)
{
	Super::EquipmentRegistrable(InActor);
}

// In Effect, by TimelineComponent And Timeline Value, Glove Moves Forward Function
void AAGlove::ForwardMoveTheGlovePlayEvent(float Value)
{
	for (int i = 0; i < GloveStaticMeshes.Num(); i++)
	{
		GloveStaticMeshes[i]->SetRelativeLocation(FMath::Lerp(
			GloveStaticMeshes[i]->GetRelativeLocation(),
			GloveTargetVecs[i],
			Value
		));
	}
}

// After moving Forward, the Function that moves Backward is Called
void AAGlove::ForwardMoveTheGloveFinishedEvent()
{
	MoveTheGloveBackward();
}

// In Effect, by TimelineComponent And Timeline Value, Glove Moves Backward Function
void AAGlove::BackwardMoveTheGlovePlayEvent(float Value)
{
	for (int i = 0; i < GloveStaticMeshes.Num(); i++)
	{
		GloveStaticMeshes[i]->SetRelativeLocation(FMath::Lerp(
			GloveStaticMeshes[i]->GetRelativeLocation(),
			GloveStartVecs[i],
			Value
		));
	}
}

// After moving backward, the function that closes the door is called.
void AAGlove::BackwardMoveTheGloveFinishedEvent()
{
	if (IsValid(EquipmentWorldSubSystem))
	{
		EquipmentWorldSubSystem->NotifyCloseDoorBroadCast(); 
		// Glove ¡æ Jail : BroadCast Function
	}
}

void AAGlove::HandleGloveReceiveByEB(FName InTag, int32 InFloor)
{
	if (InTag == FName("EB"))
	{
		switch (InFloor)
		{
		case 1:
			this->SetActorLocation(
				FVector(0.f, 0.f, 370.0f), false, nullptr, ETeleportType::TeleportPhysics
			);
			break;
		case 2:
			this->SetActorLocation(
				FVector(0.f, 0.f, 1299.25f), false, nullptr, ETeleportType::TeleportPhysics
			);
			break;
		case 3:
			this->SetActorLocation(
				FVector(0.f, 0.f, 2230.0f), false, nullptr, ETeleportType::TeleportPhysics
			);
			break;
		default:
			break;
		}
	}
}

void AAGlove::HandleGloveReceiveByJail()
{
	MoveTheGloveForward();
}

// Forward Move Call Function In Use Timeline PlayFromStart
void AAGlove::MoveTheGloveForward() 
{ 
	ForwardMoveTheGloveTimeline->PlayFromStart(); 
}

// Backward Move Call Function In Use Timeline PlayFromStart
void AAGlove::MoveTheGloveBackward() { BackwardMoveTheGloveTimeline->PlayFromStart(); }
