#include "BPMainActorBase/AGrating.h"
#include "Components/BoxComponent.h"
#include "Components/TimelineComponent.h"
#include "CoreObj/Manager/MapObjManagerSubsystem.h"

AAGrating::AAGrating()
{
	PrimaryActorTick.bCanEverTick = false;

	MainSceneRoot = CreateDefaultSubobject<USceneComponent>("Grating_SceneRoot");
	if (MainSceneRoot)
		SetRootComponent(MainSceneRoot);

	CollisionComp = CreateDefaultSubobject<UBoxComponent>("Grating_Col");
	if (CollisionComp)
	{
		CollisionComp->SetupAttachment(MainSceneRoot);
		CollisionComp->SetBoxExtent(FVector(200.0f, 300.0f, 360.0f));
		CollisionComp->SetCollisionProfileName(FName("OverlapAll"));
		CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		CollisionComp->ShapeColor = FColor::Yellow; // Debug
		CollisionComp->SetHiddenInGame(false); // Debug
	}

	StaticMeshSceneRoot = CreateDefaultSubobject<USceneComponent>("SM_SceneRoot");
	if (StaticMeshSceneRoot)
	{
		StaticMeshSceneRoot->SetupAttachment(CollisionComp);
		StaticMeshSceneRoot->SetRelativeLocationAndRotation(
			FVector(-1546.9f, 0.0f, -490.7f), FRotator(0.0f, -90.0f, 0.0f)
		);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SMFinder_CageBelow(TEXT("/Game/VRContent/Modeling/Building_Jail/CageBelow.CageBelow"));
	if (SMFinder_CageBelow.Succeeded())
		ActorBaseMesh->SetStaticMesh(SMFinder_CageBelow.Object);
	if (ActorBaseMesh)
	{
		ActorBaseMesh->SetupAttachment(StaticMeshSceneRoot);
		ActorBaseMesh->SetCollisionProfileName(FName("BlockAllDynamic"));
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialFinder_Hatch(TEXT("/Game/VRContent/Material/SRS_STAGE_hatch.SRS_STAGE_hatch"));
	if (MaterialFinder_Hatch.Succeeded())
		ActorBaseMesh->SetMaterial(0, MaterialFinder_Hatch.Object);

	mUpTimelineComp = CreateDefaultSubobject<UTimelineComponent>("UpTL_Comp");
	mUpTimelineComp->SetLooping(false);
	mUpTimelineComp->SetTimelineLength(0.51f);

	mDownTimelineComp = CreateDefaultSubobject<UTimelineComponent>("DownTL_Comp");
	mDownTimelineComp->SetLooping(false);
	mDownTimelineComp->SetTimelineLength(0.51f);

	IndividualNum = 0;

	static ConstructorHelpers::FObjectFinder<UCurveFloat> CurveFinder_GratingUp(TEXT("/Game/VRContent/Blueprints/TimelineCurve/GratingUp_Curve.GratingUp_Curve"));
	static ConstructorHelpers::FObjectFinder<UCurveFloat> CurveFinder_GratingDown(TEXT("/Game/VRContent/Blueprints/TimelineCurve/GratingDown_Curve.GratingDown_Curve"));
	if (CurveFinder_GratingUp.Succeeded() && CurveFinder_GratingDown.Succeeded())
	{
		UpTheGratingFloatCurve = CurveFinder_GratingUp.Object;
		DownTheGratingFloatCurve = CurveFinder_GratingDown.Object;
	}

	bIsGratingNotInPlace = 0;

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

void AAGrating::BeginPlay()
{
	Super::BeginPlay();
	this->EquipmentRegistrable(this);

	FlyDirection = FVector((MainSceneRoot->GetForwardVector() * -1.0f).X, (MainSceneRoot->GetForwardVector() * -1.0f).Y, 2000.0f);
	FlyDirection = FVector(FlyDirection.X * 3000.0f, FlyDirection.Y * 3000.0f, FlyDirection.Z);

	if (mUpTimelineComp)
	{
		FOnTimelineFloat GratingOpenProgressFunc;
		FOnTimelineEvent GratingOpenFinishedEvent;
		GratingOpenProgressFunc.BindUFunction(this, FName("GratingOpenPlayEvent"));
		GratingOpenFinishedEvent.BindUFunction(this, FName("GratingOpenFinishedEvent"));
		mUpTimelineComp->AddInterpFloat(UpTheGratingFloatCurve, GratingOpenProgressFunc);
		mUpTimelineComp->SetTimelineFinishedFunc(GratingOpenFinishedEvent);

		FOnTimelineFloat GratingCloseProgressFunc;
		FOnTimelineEvent GratingCloseFinishedEvent;
		GratingCloseProgressFunc.BindUFunction(this, FName("GratingClosePlayEvent"));
		GratingCloseFinishedEvent.BindUFunction(this, FName("GratingCloseFinishedEvent"));
		mDownTimelineComp->AddInterpFloat(DownTheGratingFloatCurve, GratingCloseProgressFunc);
		mDownTimelineComp->SetTimelineFinishedFunc(GratingCloseFinishedEvent);
	}
}

void AAGrating::GratingOpen()
{
	GratingOpenStart();
}

void AAGrating::GratingOpenStart()
{
	CollisionComp->SetCollisionProfileName(FName("NoCollision"));
	ActorBaseMesh->SetCollisionProfileName(FName("NoCollision"));

	if (!bIsGratingOpen)
		mUpTimelineComp->PlayFromStart();
}

void AAGrating::GratingOpenPlayEvent(float Value)
{
	ActorBaseMesh->SetRelativeLocation(FVector(
		ActorBaseMesh->GetRelativeLocation().X,
		ActorBaseMesh->GetRelativeLocation().Y,
		Value * 100.0f
	));
}

void AAGrating::GratingOpenFinishedEvent()
{
	bIsGratingOpen = 1;

	GetWorld()->GetTimerManager().SetTimer(
		DelayTimerHandle,
		this,
		&AAGrating::GratingCloseStart,
		7.0f,
		false
	);
}

void AAGrating::GratingCloseStart()
{
	if (bIsGratingOpen)
		mDownTimelineComp->PlayFromStart();
}

void AAGrating::GratingClosePlayEvent(float Value)
{
	ActorBaseMesh->SetRelativeLocation(FVector(
		ActorBaseMesh->GetRelativeLocation().X,
		ActorBaseMesh->GetRelativeLocation().Y,
		Value * 100.0f
	));
}

void AAGrating::GratingCloseFinishedEvent()
{
	CollisionComp->SetCollisionProfileName(FName("OverlapAll"));
	ActorBaseMesh->SetCollisionProfileName(FName("BlockAllDynamic"));
	bIsGratingOpen = 0;
}

void AAGrating::GratingFly(int32 InIndex)
{
	if (bIsGratingNotInPlace)
	{
		return;
	}
	else
	{
		if (IndividualNum == InIndex)
		{
			if (!GetWorld()->GetTimerManager().IsTimerActive(ReGenerationTimerHandle))
			{
				GetWorld()->GetTimerManager().SetTimer(
					ReGenerationTimerHandle,
					this,
					&AAGrating::ReGenerationGrating,
					7.0f,
					false
				);
			}

			ActorBaseMesh->SetCollisionProfileName(TEXT("OverlapAll"));
			CollisionComp->SetCollisionProfileName(TEXT("OverlapAll"));
			CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			CollisionComp->SetSimulatePhysics(true);
			CollisionComp->AddImpulse(FlyDirection, NAME_None, true);

			if (!GetWorld()->GetTimerManager().IsTimerActive(FlyingStopTimer))
			{
				GetWorld()->GetTimerManager().SetTimer(
					FlyingStopTimer,
					this,
					&AAGrating::GratingFlyStop,
					0.2f,
					false
				);
			}
		}
	}
}

void AAGrating::GratingFlyStop()
{
	CollisionComp->SetSimulatePhysics(false);
	CollisionComp->SetSimulatePhysics(true);

	if (!GetWorld()->GetTimerManager().IsTimerActive(GravityDisableTimer))
	{
		GetWorld()->GetTimerManager().SetTimer(
			GravityDisableTimer,
			this,
			&AAGrating::GratingGravityDisable,
			3.0f,
			false
		);
	}
}

void AAGrating::GratingGravityDisable()
{
	CollisionComp->SetSimulatePhysics(false);
	ActorBaseMesh->SetSimulatePhysics(false);
	bIsGratingNotInPlace = 1;
}

void AAGrating::ReGenerationGrating()
{
	GetWorld()->GetTimerManager().ClearTimer(FlyingStopTimer);
	GetWorld()->GetTimerManager().ClearTimer(GravityDisableTimer);
	if (CollisionComp->IsSimulatingPhysics())
	{
		CollisionComp->SetSimulatePhysics(false);
	}

	CollisionComp->SetCollisionProfileName(TEXT("OverlapAll"));
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ActorBaseMesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	ActorBaseMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	UMapObjManagerSubsystem* TempMapObjManangerRef = GetGameInstance()->GetSubsystem<UMapObjManagerSubsystem>();

	this->SetActorLocationAndRotation(
		TempMapObjManangerRef->GetFinalAllGratingSpawnPositions()[IndividualNum],
		TempMapObjManangerRef->GetFinalAllGratingSpawnRotations()[IndividualNum]
	);

	if (CollisionComp->AttachToComponent(MainSceneRoot, FAttachmentTransformRules::KeepRelativeTransform))
	{
		CollisionComp->SetRelativeLocationAndRotation(
			FVector(0.0f, 0.0f, 0.0f),
			FRotator(0.0f, 0.0f, 0.0f)
		);
		bIsGratingNotInPlace = 0;
	}
}

void AAGrating::EquipmentRegistrable(AActor* InActor)
{
	Super::EquipmentRegistrable(InActor);
}