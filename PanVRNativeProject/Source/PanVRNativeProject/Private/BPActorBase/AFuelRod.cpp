// Fill out your copyright notice in the Description page of Project Settings.


#include "BPActorBase/AFuelRod.h"
#include "Components/BoxComponent.h"
#include "Core/Debug/FDebugLib.h"
#include "Components/TimelineComponent.h"

AAFuelRod::AAFuelRod()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_FuelRodBody(TEXT("/Game/VRContent/Modeling/FuelRelated/FuelRod_Stick.FuelRod_Stick"));
	if (ActorBaseMesh && ModelingFinder_FuelRodBody.Succeeded())
	{
		ActorBaseMesh->SetStaticMesh(ModelingFinder_FuelRodBody.Object);
		ActorBaseMesh->SetWorldScale3D(FVector(0.8f, 0.8f, 0.8f));
		ActorBaseMesh->SetSimulatePhysics(true);
		ActorBaseMesh->SetCollisionProfileName("PhysicsActor");
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialFinder_FuelRodGauge(TEXT("/Game/VRContent/Material/STAGE_StickGauge_Inst.STAGE_StickGauge_Inst"));
	if (MaterialFinder_FuelRodGauge.Succeeded())
	{
		ActorBaseMesh->SetMaterial(0, MaterialFinder_FuelRodGauge.Object);
	}

	FRCLStick = CreateDefaultSubobject<UBoxComponent>("StickCollision");
	if (FRCLStick)
	{
		FRCLStick->SetupAttachment(ActorBaseMesh);
		FRCLStick->SetRelativeLocation(FVector(0.f, 0.f, 12.f));
		FRCLStick->ComponentTags.Add(FName("FuelRod"));
		FRCLStick->SetBoxExtent(FVector(2.f, 2.f, 13.f));
		FRCLStick->SetHiddenInGame(false); // Debug
	}

	if (GC)
	{
		GC->SetRelativeLocation(FVector(0.f, 0.f, 12.f));
		GC->EEGrabType = EGrabType::Free;
	}

	GaugeTimelineComp = CreateDefaultSubobject<UTimelineComponent>("TimelineComp");
	if (GaugeTimelineComp)
	{
		GaugeTimelineComp->SetLooping(false);
		GaugeTimelineComp->SetTimelineLength(50.01f);
	}

	static ConstructorHelpers::FObjectFinder<UCurveFloat> CurveFinder_Gauge(TEXT("/Game/VRContent/Blueprints/TimelineCurve/FuelRod_Gauge_MoveCurve.FuelRod_Gauge_MoveCurve"));
	if (CurveFinder_Gauge.Succeeded())
	{
		GaugeMoveCurve = CurveFinder_Gauge.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> SoundFinder_FuelRodLowGauge(TEXT("/Game/VRContent/Sound/Wavs/FuelRelated/sfx_radiation_low.sfx_radiation_low"));
	if (SoundFinder_FuelRodLowGauge.Succeeded())
	{
		SFXFuelRodLowGauge = SoundFinder_FuelRodLowGauge.Object;
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

void AAFuelRod::BeginPlay()
{
	Super::BeginPlay();

	FOnTimelineFloat Progress;
	Progress.BindUFunction(this, FName("GaugeOperatingTimeline"));

	GaugeTimelineComp->AddInterpFloat(GaugeMoveCurve, Progress);
}

void AAFuelRod::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAFuelRod::HandleGaugeOperation(bool bIsOperFlag)
{
	if(bIsOperFlag)
		GaugeTimelineComp->Play();
	else
		GaugeTimelineComp->Stop();
}

//void AAFuelRod::SetFRIsAttaching(uint32 TempAttaching) { bIsAttaching = TempAttaching; }
//uint32 AAFuelRod::GetFRIsAttaching() const { return bIsAttaching; }

//void AAFuelRod::GaugeOperationStart()
//{
//	GaugeTimelineComp->Play();
//}
//
//void AAFuelRod::GaugeOperationStop()
//{
//	GaugeTimelineComp->Stop();
//}

void AAFuelRod::GaugeOperatingTimeline(float Value)
{
	float TempGauge = Value / 100;
	ActorBaseMesh->SetScalarParameterValueOnMaterials(FName("Gauge"), TempGauge);
	
	bool bIsLowGauge = (Value < 25.f);

	if (bIsLowGauge)
	{
		UE_LOG(LogTemp, Log, TEXT("FuelRod is LowGauge!"));
	}

	if (bIsLowGauge)
	{
		ActorBaseMesh->SetVectorParameterValueOnMaterials(FName("GaugeColor"), FVector(1.f, 0.f, 0.f));
		if (!bWasLowGauge)
		{
			mSoundPlayer->PlaySoundEffect(this, SFXFuelRodLowGauge, FRCLStick->GetComponentLocation());
		}
	}
	else
	{
		ActorBaseMesh->SetVectorParameterValueOnMaterials(FName("GaugeColor"), FVector(0.f, 1.f, 0.f));
	}

	bWasLowGauge = bIsLowGauge;
}

void AAFuelRod::OnGrabbed(UMotionControllerComponent& InMCRef, const FVector& HandGrabPos, class AVRHand* InGrabbingHand)
{
	GC->SetPrimitiveCompPhysics(false);
	GetWorldTimerManager().PauseTimer(DestroySelfTimer);
}

void AAFuelRod::OnDropped()
{
	if (bIsAttaching != 1)
	{
		//UE_LOG(LogTemp, Log, TEXT("FuelRod this OnDropped Call!"))
		GC->SetPrimitiveCompPhysics(true);
		GetWorldTimerManager().SetTimer(
			DestroySelfTimer,
			this,
			&AAFuelRod::DestroySelf,
			3.0f,
			false
		);
	}
}

void AAFuelRod::DestroySelf()
{
	this->Destroy();
	return;
}