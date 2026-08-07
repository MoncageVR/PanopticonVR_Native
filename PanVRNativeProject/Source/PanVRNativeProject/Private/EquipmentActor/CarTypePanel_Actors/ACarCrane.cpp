#include "EquipmentActor/CarTypePanel_Actors/ACarCrane.h"
#include "CoreCommon/PrisonerRelated/PrisonerCharacter.h"
#include "CoreCommon/PrisonerRelated/PrisonerController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/BoxComponent.h"
#include "Components/TimelineComponent.h"

AACarCrane::AACarCrane()
{
	PrimaryActorTick.bCanEverTick = true;

	CarCraneMainRoot = CreateDefaultSubobject<USceneComponent>("SceneComp");
	if (CarCraneMainRoot)
		SetRootComponent(CarCraneMainRoot);

	// Setting UFOBody Static Mesh Component
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_UFOBody(TEXT("/Game/VRContent/Modeling/27_UFO(CarCrane)/SM_UFO_Body.SM_UFO_Body"));
	if (ActorBaseMesh)
	{
		ActorBaseMesh->SetupAttachment(CarCraneMainRoot);
		ActorBaseMesh->SetRelativeLocation(FVector(750.0f, 0.0f, 0.0f));
		if (ModelingFinder_UFOBody.Succeeded())
		{
			ActorBaseMesh->SetStaticMesh(ModelingFinder_UFOBody.Object);
		}
	}

	// Setting _UFOPortal Static Mesh Component
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_UFOPortal(TEXT("/Game/VRContent/Modeling/27_UFO(CarCrane)/SM_UFO_Portal.SM_UFO_Portal"));
	UFOPortal = CreateDefaultSubobject<UStaticMeshComponent>("SM_UFOPortal");
	if (UFOPortal)
	{
		UFOPortal->SetupAttachment(ActorBaseMesh);
		if (ModelingFinder_UFOPortal.Succeeded())
		{
			UFOPortal->SetStaticMesh(ModelingFinder_UFOPortal.Object);
		}
	}

	// Setting UFOCraneHead Static Mesh Component
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_UFOCraneHead(TEXT("/Game/VRContent/Modeling/27_UFO(CarCrane)/SM_UFO_CarCrane.SM_UFO_CarCrane"));
	UFOCraneHead = CreateDefaultSubobject<UStaticMeshComponent>("SM_UFOCraneHead");
	if (UFOCraneHead)
	{
		UFOCraneHead->SetupAttachment(UFOPortal);
		UFOCraneHead->SetRelativeLocation(FVector(0.0f, 55.4f, -50.0f));
		UFOCraneHead->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
		UFOCraneHead->SetRelativeScale3D(FVector(40.0f, 40.0f, 40.0f));
		if (ModelingFinder_UFOCraneHead.Succeeded())
		{
			UFOCraneHead->SetStaticMesh(ModelingFinder_UFOCraneHead.Object);
		}
	}

	// Setting UFO Subdue Box Collision
	CLUFOSubdue = CreateDefaultSubobject<UBoxComponent>("CL_UFOSubdue");
	if (CLUFOSubdue)
	{
		CLUFOSubdue->SetupAttachment(UFOCraneHead);
		CLUFOSubdue->SetRelativeLocation(FVector(0.0f, 0.0f, 1.625f));
		CLUFOSubdue->SetRelativeScale3D(FVector(0.025f, 0.025f, 0.025f));
		CLUFOSubdue->SetBoxExtent(FVector(135.0f, 237.5f, 65.0f));
		CLUFOSubdue->SetGenerateOverlapEvents(false);
		CLUFOSubdue->SetHiddenInGame(false); // Debug
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MatFinder_Main(TEXT("/Game/VRContent/Material/SRS_STAGE_Main.SRS_STAGE_Main"));
	if (MatFinder_Main.Succeeded())
	{
		ActorBaseMesh->SetMaterial(0, MatFinder_Main.Object);
		UFOCraneHead->SetMaterial(0, MatFinder_Main.Object);
		UFOPortal->SetMaterial(0, MatFinder_Main.Object);
	}

	TL_CraneDownward = CreateDefaultSubobject<UTimelineComponent>("CraneTimelineComp");
	if (TL_CraneDownward)
	{
		TL_CraneDownward->SetLooping(false);
		TL_CraneDownward->SetTimelineLength(1.51f);
	}

	static ConstructorHelpers::FObjectFinder<UCurveFloat> CurveFinder_CraneHead(TEXT("/Game/VRContent/Blueprints/TimelineCurve/CraneHead_DownMove_Curve.CraneHead_DownMove_Curve"));
	if (CurveFinder_CraneHead.Succeeded())
		CraneHeadDownwardMove_Curve = CurveFinder_CraneHead.Object;

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

void AACarCrane::BeginPlay()
{
	Super::BeginPlay();
	this->EquipmentRegistrable(this);

	CraneHeadDefaultVec = UFOCraneHead->GetRelativeLocation();

	CLUFOSubdue->OnComponentBeginOverlap.AddDynamic(this, &AACarCrane::CarCraneOverlapBegin);

	FOnTimelineFloat Progress;
	FOnTimelineEvent FinishedEvent;
	Progress.BindUFunction(this, FName("CraneHeadOperatingTimeline"));
	FinishedEvent.BindUFunction(this, FName("CraneHeadOperatingTimelineEnd"));

	TL_CraneDownward->AddInterpFloat(CraneHeadDownwardMove_Curve, Progress);
	TL_CraneDownward->SetTimelineFinishedFunc(FinishedEvent);
}

void AACarCrane::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AACarCrane::EquipmentRegistrable(AActor* InActor)
{
	Super::EquipmentRegistrable(InActor);
}

void AACarCrane::HandleAddCraneYaw(float InDeltaYaw)
{
	if (CarCraneMainRoot)
	{
		CarCraneMainRoot->AddLocalRotation(FRotator(0.f, InDeltaYaw, 0.f));
	}
}

void AACarCrane::HandleOperatingCraneHead()
{
	ActuallyOperateCraneHead();
}

void AACarCrane::CarCraneOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->ComponentHasTag(FName(TEXT("PrisonerCharacter"))))
	{
		UE_LOG(LogTemp, Log, TEXT("Overlap PrisonerCharacter"));
		APrisonerCharacter* OverlapPrisonerCha = Cast<APrisonerCharacter>(OtherActor);
		checkf(OverlapPrisonerCha, TEXT("In CarCrane, Overlap Prisoner Not Valid!"));
		APrisonerController* OverlapPrisonerCon = Cast<APrisonerController>(OverlapPrisonerCha->GetController());
		checkf(OverlapPrisonerCon, TEXT("In CarCrane, Overlap Prisoner Controller Not Valid"));

		TArray<uint8> GivenUpperStates = { 1 };
		TArray<uint8> GivenLowerStates = { 1 };

		OverlapPrisonerCon->State_based_ExecutionTasks_GiventoSomeone(GivenUpperStates, GivenLowerStates);
	}
}

void AACarCrane::CraneHeadOperatingTimeline(float value)
{
	UFOCraneHead->SetRelativeLocation(FVector(0.f, 0.f, (value * -1000.0f)));
}

void AACarCrane::CraneHeadOperatingTimelineEnd()
{
	CLUFOSubdue->SetGenerateOverlapEvents(false);
	UFOCraneHead->SetRelativeLocation(CraneHeadDefaultVec);
}

void AACarCrane::ActuallyOperateCraneHead()
{
	CLUFOSubdue->SetGenerateOverlapEvents(true);
	TL_CraneDownward->PlayFromStart();
}
