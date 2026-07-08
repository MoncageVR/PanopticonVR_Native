// Fill out your copyright notice in the Description page of Project Settings.


#include "BPActorBase/APump.h"
#include "Kismet/KismetMathLibrary.h"
#include "Core/Component/UGrabComp.h"
#include "CoreObj/VREquipmentWorldSubsystem.h"

AAPump::AAPump()
{
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_PumpBody(TEXT("/Game/VRContent/Modeling/Pump/SMPumpBody.SMPumpBody"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_PumpHandle(TEXT("/Game/VRContent/Modeling/Pump/SMHandle.SMHandle"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_PumpHandleBellows(TEXT("/Game/VRContent/Modeling/Pump/SMHandleBellows.SMHandleBellows"));

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialFinder_Main(TEXT("/Game/VRContent/Material/SRS_STAGE_Main.SRS_STAGE_Main"));

	if (ActorBaseMesh && ModelingFinder_PumpBody.Succeeded())
	{
		ActorBaseMesh->SetStaticMesh(ModelingFinder_PumpBody.Object);
	}

	SMPumpHandle = CreateDefaultSubobject<UStaticMeshComponent>("HandleSMComp");
	if (SMPumpHandle && ModelingFinder_PumpHandle.Succeeded())
	{
		SMPumpHandle->SetupAttachment(ActorBaseMesh);
		SMPumpHandle->SetStaticMesh(ModelingFinder_PumpHandle.Object);
		SMPumpHandle->SetRelativeLocation(FVector(0.f, 0.f, 23.3f));
	}

	if (GC)
	{
		GC->SetupAttachment(SMPumpHandle);
		GC->SetRelativeLocation(FVector(0.f, 6.f, 32.75f));
		GC->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));
		GC->EEGrabType = EGrabType::HandToObj;
	}

	SCMoveStandard = CreateDefaultSubobject<USceneComponent>("MoveSceneComp");
	if (SCMoveStandard)
	{
		SCMoveStandard->SetupAttachment(SMPumpHandle);
		SCMoveStandard->SetRelativeLocation(FVector(0.f, 0.f, 30.75f));
	}

	SMPumpHandleBellow = CreateDefaultSubobject<UStaticMeshComponent>("HandleBellowSMComp");
	if (SMPumpHandleBellow)
	{
		SMPumpHandleBellow->SetupAttachment(ActorBaseMesh);
		SMPumpHandleBellow->SetStaticMesh(ModelingFinder_PumpHandleBellows.Object);
		SMPumpHandleBellow->SetRelativeLocation(FVector(0.f, 0.f, 13.3f));
	}

	if (MaterialFinder_Main.Succeeded())
	{
		ActorBaseMesh->SetMaterial(0, MaterialFinder_Main.Object);
		SMPumpHandle->SetMaterial(0, MaterialFinder_Main.Object);
		SMPumpHandleBellow->SetMaterial(0, MaterialFinder_Main.Object);
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

void AAPump::BeginPlay()
{
	Super::BeginPlay();
	this->EquipmentRegistrable(this);

	DefaultHandlePos = SMPumpHandle->GetRelativeLocation();
	bIsDeLoreanOperationFlag = 1; // Debug
}

void AAPump::Tick(float DeltaTimes)
{
	Super::Tick(DeltaTimes);
}

void AAPump::EquipmentRegistrable(AActor* InActor)
{
	Super::EquipmentRegistrable(InActor);
}

void AAPump::OnGrabbed(UMotionControllerComponent& InMCRef, const FVector& HandGrabPos, AVRHand* InGrabbingHand)
{
	if (bIsDeLoreanOperationFlag)
	{
		MCRef = &InMCRef;

		GetWorldTimerManager().SetTimer(
			PumpOperateTimer,
			this,
			&AAPump::UpdatePumpUpNDown,
			0.01f,
			true
		);
	}
	else
	{
		MCRef = nullptr;
		if (GetWorldTimerManager().IsTimerActive(PumpOperateTimer))
		{
			GetWorldTimerManager().PauseTimer(PumpOperateTimer);
			GetWorldTimerManager().ClearTimer(PumpOperateTimer);
		}
	}
}

void AAPump::UpdatePumpUpNDown()
{
	float TempPosZ = FMath::Clamp(
		(SMPumpHandle->GetRelativeLocation().Z + (MCRef->GetComponentTransform().GetRelativeTransform(SCMoveStandard->GetComponentTransform()).GetLocation().Z)),
		14.4f,
		23.3f
	);
	SMPumpHandle->SetRelativeLocation(
		FVector(
			SMPumpHandle->GetRelativeLocation().X,
			SMPumpHandle->GetRelativeLocation().Y,
			TempPosZ
		)
	);


	float TempScaleZ = FMath::GetMappedRangeValueClamped(
		FVector2D(14.4f, 23.3f),
		FVector2D(0.1f, 1.0f),
		(SMPumpHandle->GetRelativeLocation().Z + (MCRef->GetComponentTransform().GetRelativeTransform(SCMoveStandard->GetComponentTransform()).GetLocation().Z))
	);
	SMPumpHandleBellow->SetWorldScale3D(
		FVector(
			SMPumpHandleBellow->GetRelativeScale3D().X,
			SMPumpHandleBellow->GetRelativeScale3D().Y,
			TempScaleZ
		)
	);
}

void AAPump::OnDropped()
{
	MCRef = nullptr;
	if (GetWorldTimerManager().IsTimerActive(PumpOperateTimer))
	{
		GetWorldTimerManager().PauseTimer(PumpOperateTimer);
		GetWorldTimerManager().ClearTimer(PumpOperateTimer);
	}
}
