// Fill out your copyright notice in the Description page of Project Settings.


#include "BPActorBase/BPSpawnActorBase/AGolfBall.h"
#include "BPActorBase/BPSpawnActorBase/AGolfEffect.h"
#include "Components/SphereComponent.h"
#include "Components/SplineComponent.h"
#include "Components/TimelineComponent.h"
#include "Kismet/KismetMathLibrary.h"

AAGolfBall::AAGolfBall()
{
	SC_BallMainRoot = CreateDefaultSubobject<USceneComponent>(TEXT("GolfBallMainRoot"));
	if (SC_BallMainRoot)
	{
		this->SetRootComponent(SC_BallMainRoot);
	}

	if (ActorBaseMesh)
	{
		ActorBaseMesh->SetupAttachment(SC_BallMainRoot);
		ActorBaseMesh->SetCollisionProfileName(TEXT("OverlapAll"));
		ActorBaseMesh->ComponentTags.Add(TEXT("GolfBall"));
	}

	CL_Ball = CreateDefaultSubobject<USphereComponent>(TEXT("BallCollision"));
	if (CL_Ball)
	{
		CL_Ball->SetupAttachment(SC_BallMainRoot);
		CL_Ball->SetRelativeLocation(FVector(0.f, 0.f, 5.f));
		CL_Ball->SetCollisionProfileName(TEXT("OverlapAll"));
		CL_Ball->SetSphereRadius(6.0f);
	}

	SP_BallTrajectoryRoute = CreateDefaultSubobject<USplineComponent>(TEXT("BallTrajectorySplineComp"));
	if (SP_BallTrajectoryRoute)
	{
		SP_BallTrajectoryRoute->SetupAttachment(SC_BallMainRoot);
		SP_BallTrajectoryRoute->SetTangentAtSplinePoint(0, FVector(0.f, 0.f, 0.f), ESplineCoordinateSpace::Local, true);

		SP_BallTrajectoryRoute->SetLocationAtSplinePoint(1, FVector(1.f, 0.f, 0.f), ESplineCoordinateSpace::Local, true);
		SP_BallTrajectoryRoute->SetTangentAtSplinePoint(1, FVector(0.f, 0.f, 0.f), ESplineCoordinateSpace::Local, true);
	}

	TL_BallTrajectory = CreateDefaultSubobject<UTimelineComponent>(TEXT("BallTimelineComp"));
	if (TL_BallTrajectory)
	{
		TL_BallTrajectory->SetLooping(false);
		TL_BallTrajectory->SetTimelineLength(3.01f);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SMFinder_BallBody(TEXT("/Game/VRContent/Prisoner/GolfRelate/Modeling/GolfBall.GolfBall"));
	if (SMFinder_BallBody.Succeeded())
	{
		ActorBaseMesh->SetStaticMesh(SMFinder_BallBody.Object);
	}

	static ConstructorHelpers::FObjectFinder<UCurveFloat> CurveFinder_BallTrajectory(TEXT("/Game/VRContent/Blueprints/TimelineCurve/GolfBallTrajectory_Curve.GolfBallTrajectory_Curve"));
	if (CurveFinder_BallTrajectory.Succeeded())
	{
		CF_BallTrajectory = CurveFinder_BallTrajectory.Object;
	}

	SpawnRadius = 150.0f;
}

void AAGolfBall::BeginPlay()
{
	Super::BeginPlay();

	if (TL_BallTrajectory)
	{
		FOnTimelineFloat BallFlyProgressFunc;
		FOnTimelineEvent BallFlyFinishedEvent;
		BallFlyProgressFunc.BindUFunction(this, FName("BallFlyPlayEvent"));
		BallFlyFinishedEvent.BindUFunction(this, FName("BallFlyFinishedEvent"));
		TL_BallTrajectory->AddInterpFloat(CF_BallTrajectory, BallFlyProgressFunc);
		TL_BallTrajectory->SetTimelineFinishedFunc(BallFlyFinishedEvent);
	}

	CL_Ball->OnComponentBeginOverlap.AddDynamic(this, &AAGolfBall::BallCLOverlapBegin);
}

void AAGolfBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAGolfBall::BallCLOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->ComponentHasTag(TEXT("GolfBallBlock")) || OtherActor->ActorHasTag(TEXT("GolfBallBlock")))
	{
		//UE_LOG(LogTemp, Log, TEXT("Golf Ball Blocking!"));

		
	}
}

void AAGolfBall::BallFlyPlayEvent(float InFlyTimeValue)
{
	this->GetRootComponent()->SetRelativeLocation(SP_BallTrajectoryRoute->GetLocationAtTime(InFlyTimeValue, ESplineCoordinateSpace::Local, false), true, nullptr, ETeleportType::None);

	// Debug
	DrawDebugPoint(GetWorld(), this->RootComponent->GetComponentLocation(), 5.0f, FColor::Red, false, 3.0f);
}

void AAGolfBall::BallFlyFinishedEvent()
{
	// UE_LOG(LogTemp, Log, TEXT("GolfBall Fly Finish And GolfEffect Spawn Logic Execute"));

	this->ActorBaseMesh->SetVisibility(false);

	AAGolfEffect* TempGolfEffect = nullptr;

	float TempGenerationPosX = SpawnRadius * (FMath::Cos(FMath::RadiansToDegrees(SpawnTheta)));
	float TempGenerationPosY = SpawnRadius * (FMath::Sin(FMath::RadiansToDegrees(SpawnTheta)));
	float TempGenerationPosZ = FMath::RandRange(SpawnGenerationPosZ - 100.f, SpawnGenerationPosZ + 100.0f);

	FVector TempFinalStartVec = FVector(TempGenerationPosX, TempGenerationPosY, TempGenerationPosZ);
	FVector TempFinalTargetVec = FVector(0.0f, 0.0f, TempGenerationPosZ);

	FRotator TempFinalRot = UKismetMathLibrary::FindLookAtRotation(TempFinalStartVec, TempFinalTargetVec);

	FTransform FinalGolfEffectSpawnTransform = FTransform(TempFinalRot, FVector(TempGenerationPosX, TempGenerationPosY, TempGenerationPosZ), FVector::OneVector);

	FActorSpawnParameters TempGolfEffectSpawnParams;
	TempGolfEffectSpawnParams.Owner = this;
	TempGolfEffectSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	TempGolfEffect = GetWorld()->SpawnActor<AAGolfEffect>(AAGolfEffect::StaticClass(), FinalGolfEffectSpawnTransform, TempGolfEffectSpawnParams);

	if (TempGolfEffect->MakeGolfEggEffect())
	{
		this->Destroy();
	}
}

void AAGolfBall::UpdateBallTrajectorySpline(FVector InCurveStartVec, FVector InCurveEndVec)
{
	TArray<FVector> GolfBallSplinePointArrs;
	GolfBallSplinePointArrs.Empty();
	GolfBallSplinePointArrs.Reserve(3);

	GolfBallSplinePointArrs.Add(InCurveStartVec);
	GolfBallSplinePointArrs.Add((((InCurveStartVec + InCurveEndVec) / 2.0f) + FVector(0.f, -250.f, 100.f)));
	GolfBallSplinePointArrs.Add(InCurveEndVec);

	SP_BallTrajectoryRoute->SetSplinePoints(GolfBallSplinePointArrs, ESplineCoordinateSpace::Local);

	TL_BallTrajectory->PlayFromStart();
}

void AAGolfBall::SetGolfEffectEmergenceDegree(float InMinDegree, float InMaxDegree, float InHeight)
{
	SpawnTheta = FMath::RandRange(InMinDegree, InMaxDegree);
	SpawnRadius = 150.0f;
	SpawnGenerationPosZ = InHeight;
}
