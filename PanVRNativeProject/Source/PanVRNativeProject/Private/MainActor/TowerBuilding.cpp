#include "MainActor/TowerBuilding.h"
#include "CoreObj/Manager/WorldSubSystem/VREquipmentWorldSubsystem.h"
#include "CoreCommon/PrisonerRelated/PrisonerCharacter.h"
#include "CoreCommon/PrisonerRelated/PrisonerController.h"
#include "Components/BoxComponent.h"
#include "Components/AudioComponent.h"
#include "Components/SplineComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

ATowerBuilding::ATowerBuilding()
{
	PrimaryActorTick.bCanEverTick = false;

	// Setting Main Root SceneComponent
	MainRoot = CreateDefaultSubobject<USceneComponent>("SceneComp");
	if (MainRoot) { this->SetRootComponent(MainRoot); }

	// Setting Modeling - ActorBaseMesh StaticMesh Component
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_TowerBody(TEXT("/Game/VRContent/Modeling/24_Tower(Building_Tower)/SM_TowerMainBody.SM_TowerMainBody"));
	ActorBaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("SM_TowerBody");
	if (ActorBaseMesh)
	{
		ActorBaseMesh->SetupAttachment(MainRoot);
		ActorBaseMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 1250.0f));
		ActorBaseMesh->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
		if (ModelingFinder_TowerBody.Succeeded()) { ActorBaseMesh->SetStaticMesh(ModelingFinder_TowerBody.Object); }
	}

	// Setting Modeling - MainDesk StaticMesh Component
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_TowerMainDesk(TEXT("/Game/VRContent/Modeling/25_TowerDesk/MainDesk.MainDesk"));
	MainDesk = CreateDefaultSubobject<UStaticMeshComponent>("SM_MainDesk");
	if (MainDesk)
	{
		MainDesk->SetupAttachment(MainRoot);
		MainDesk->SetRelativeLocation(FVector(0.0f, 0.0f, 1250.0f));
		MainDesk->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
		if (ModelingFinder_TowerMainDesk.Succeeded()) { MainDesk->SetStaticMesh(ModelingFinder_TowerMainDesk.Object); }
	}

	// Setting Modeling - SubDesk StaticMesh Component
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_TowerSubDesk(TEXT("/Game/VRContent/Modeling/25_TowerDesk/SubDesk.SubDesk"));
	SubDesk = CreateDefaultSubobject<UStaticMeshComponent>("SM_SubDesk");
	if (SubDesk)
	{
		SubDesk->SetupAttachment(MainRoot);
		SubDesk->SetRelativeLocation(FVector(-3.8f, -5.4f, 1250.0f));
		SubDesk->SetRelativeRotation(FRotator(0.0f, 145.0f, 0.0f));
		if (ModelingFinder_TowerSubDesk.Succeeded()) { SubDesk->SetStaticMesh(ModelingFinder_TowerSubDesk.Object); }
	}

	// Setting Modeling And Individual Vars - Barricade StaticMesh Component
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_TowerBarricade(TEXT("/Game/VRContent/Modeling/24_Tower(Building_Tower)/SM_Tower_Barricade.SM_Tower_Barricade"));
	Barricade = CreateDefaultSubobject<UStaticMeshComponent>("SM_Barricade");
	if (Barricade)
	{
		Barricade->SetupAttachment(MainRoot);
		Barricade->SetRelativeLocation(FVector(0.0f, 0.0f, 1540.0f));
		if (ModelingFinder_TowerBarricade.Succeeded()) { Barricade->SetStaticMesh(ModelingFinder_TowerBarricade.Object); }
		Barricade->SetHiddenInGame(true);
		Barricade->SetVisibility(false);
		Barricade->ComponentTags.Add(TEXT("GolfBallBlock"));
		Barricade->SetCollisionProfileName(TEXT("OverlapAll"));
	}

	// Setting Collision - Feature : Subdue For TowerRaid Status Prisoner
	CLSubdueForToilet = CreateDefaultSubobject<UBoxComponent>("CL_SubdueTowerRaid");
	if (CLSubdueForToilet)
	{
		CLSubdueForToilet->SetupAttachment(MainRoot);
		CLSubdueForToilet->SetBoxExtent(FVector(350.0f, 250.0f, 1700.0f));
		CLSubdueForToilet->SetHiddenInGame(false); // Debug
		CLSubdueForToilet->SetGenerateOverlapEvents(false);
	}

	// Setting Material - Tower - ActorBaseMesh Component
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MatFinder_Tower(TEXT("/Game/VRContent/Material/SRS_STAGE_TOWER.SRS_STAGE_TOWER"));
	if (MatFinder_Tower.Succeeded()) { ActorBaseMesh->SetMaterial(0, MatFinder_Tower.Object); }

	// Setting Material - Main - MainDesk , SubDesk Component
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MatFinder_Main(TEXT("/Game/VRContent/Material/SRS_STAGE_Main.SRS_STAGE_Main"));
	if (MatFinder_Main.Succeeded())
	{
		MainDesk->SetMaterial(0, MatFinder_Main.Object);
		SubDesk->SetMaterial(0, MatFinder_Main.Object);
	}

	// Initialize Tower Heights For Using ElevatorButton
	if (TargetTowerHeights.IsEmpty())
	{
		TargetTowerHeights.Add(-1100.0f); // 1st Floor Height
		TargetTowerHeights.Add(-190.0f);  // 2nd Floor Height
		TargetTowerHeights.Add(790.0f);   // 3rd Floor Height
	}

	// Setting Sound - Tower Move Sound Cues
	static ConstructorHelpers::FObjectFinder<USoundBase> SoundFinder_TowerMoveCue(TEXT("/Game/VRContent/Sound/Ques/ElevatorButton/sfx_elevator_move_Cue.sfx_elevator_move_Cue"));
	TBAudioPlayer = CreateDefaultSubobject<UAudioComponent>("SC_TBAudioComp");
	if (TBAudioPlayer && SoundFinder_TowerMoveCue.Succeeded())
	{
		TowerMoveSFXCue = SoundFinder_TowerMoveCue.Object;
		TBAudioPlayer->SetupAttachment(MainRoot);
		TBAudioPlayer->SetAutoActivate(false);
		TBAudioPlayer->bAllowSpatialization = false;
	}

	// Setting Spline - Using TowerRaid Prisoner Move Route
	mTowerRaidMoveRoute = CreateDefaultSubobject<USplineComponent>("mSplineComp");
	if (mTowerRaidMoveRoute)
	{
		mTowerRaidMoveRoute->SetupAttachment(MainRoot);
		// Setting Spline Component Default
		mTowerRaidMoveRoute->SetRelativeLocation(FVector(0.0f, 0.0f, -1900.0f));
		mTowerRaidMoveRoute->AddSplineLocalPoint(FVector(0.0f, 0.0f, 0.0f));
		mTowerRaidMoveRoute->AddSplineLocalPoint(FVector(0.0f, 0.0f, 0.0f));

		Init_TowerSplinePointValue();
		Init_TowerSplineDefaultPointValue();
	}
}

void ATowerBuilding::Init_TowerSplinePointValue()
{
	FVector TempLastPointValue = FVector(350.0f, 0.0f, 3450.0f);
	// Initialize Spline Point Value Based on the 1st floor
	First_SplinePointValueArrs.Add(FVector(325.f, 0.0f, 3060.0f));
	First_SplinePointValueArrs.Add(FVector(325.f, 0.0f, 3200.0f));
	First_SplinePointValueArrs.Add(FVector(325.f, 0.0f, 3300.0f));
	First_SplinePointValueArrs.Add(FVector(TempLastPointValue));
	// Initialize Spline Point Value Based on the 2nd floor
	Second_SplinePointValueArrs.Add(FVector(400.0f, 0.0f, 2200.0f));
	Second_SplinePointValueArrs.Add(FVector(400.0f, 0.0f, 2800.0f));
	Second_SplinePointValueArrs.Add(FVector(450.0f, 0.0f, 3025.0f));
	Second_SplinePointValueArrs.Add(FVector(TempLastPointValue));
	// Initialize Spline Point Value Based on the 3rd floor
	Third_SplinePointValueArrs.Add(FVector(350.0f, 0.0f, 1350.0f));
	Third_SplinePointValueArrs.Add(FVector(350.0f, 0.0f, 2800.0f));
	Third_SplinePointValueArrs.Add(FVector(450.0f, 0.0f, 3025.0f));
	Third_SplinePointValueArrs.Add(FVector(TempLastPointValue));
}

void ATowerBuilding::Init_TowerSplineDefaultPointValue()
{
	// Initialize Spline Point Value based on the 3rd floor
	for (int32 i = 0; i < 4; i++)
	{
		mTowerRaidMoveRoute->SetLocationAtSplinePoint(i, Third_SplinePointValueArrs[i], ESplineCoordinateSpace::Local, true);
		mTowerRaidMoveRoute->SetRotationAtSplinePoint(i, FRotator(0.0f, 0.0f, 0.0f), ESplineCoordinateSpace::Local, true);
		mTowerRaidMoveRoute->SetTangentsAtSplinePoint(i, FVector(0.0f, 0.f, 0.f), FVector(0.f, 0.f, 0.f), ESplineCoordinateSpace::Local, true);
	}
}

void ATowerBuilding::HandleSplinePointValue(int32 InFloorNum)
{
	this->SetSplinePointValueByCurrFloorNum(InFloorNum);
}

void ATowerBuilding::HandleTowerReceiveByEB(FName InTag, int32 InFloor)
{
	if (InTag == FName("EB"))
	{
		if (ActuallyCurrFloorNum == InFloor)
		{
			return;
		}
		else
		{
			this->SetTowerCurrFloorNum(InFloor);
			if (ActuallyCurrFloorNum == 1 && InFloor == 1)
				ActuallyMoveTower(TargetTowerHeights[ActuallyCurrFloorNum - 1]);
			else if (ActuallyCurrFloorNum == 2 && InFloor == 2)
				ActuallyMoveTower(TargetTowerHeights[ActuallyCurrFloorNum - 1]);
			else if (ActuallyCurrFloorNum == 3 && InFloor == 3)
				ActuallyMoveTower(TargetTowerHeights[ActuallyCurrFloorNum - 1]);
		}
	}
}

void ATowerBuilding::SetSplinePointValueByCurrFloorNum(int32 InTempFloorNum)
{
	if (InTempFloorNum == 1)
	{
		for (int32 i = 0; i < 4; i++)
		{
			mTowerRaidMoveRoute->SetLocationAtSplinePoint(i, First_SplinePointValueArrs[i], ESplineCoordinateSpace::Local, true);
		}
	}
	else if (InTempFloorNum == 2)
	{
		for (int32 i = 0; i < 4; i++)
		{
			mTowerRaidMoveRoute->SetLocationAtSplinePoint(i, Second_SplinePointValueArrs[i], ESplineCoordinateSpace::Local, true);
		}
	}
	else if (InTempFloorNum == 3)
	{
		for (int32 i = 0; i < 4; i++)
		{
			mTowerRaidMoveRoute->SetLocationAtSplinePoint(i, Third_SplinePointValueArrs[i], ESplineCoordinateSpace::Local, true);
		}
	}
	else
	{
		return;
	}
}

void ATowerBuilding::HandleRaidSubdueReceiveByToilet(uint8 bIsSubdueFlag)
{
	if (bIsSubdueFlag) { CLSubdueForToilet->SetGenerateOverlapEvents(true); }
	else { CLSubdueForToilet->SetGenerateOverlapEvents(false); }
}

void ATowerBuilding::BeginPlay()
{
	Super::BeginPlay();
	
	TowerLatentInfo.CallbackTarget = this;
	TowerLatentInfo.Linkage = 0;
	TowerLatentInfo.ExecutionFunction = FName("ActuallyTowerMoveCompleted");
	TowerLatentInfo.UUID = 1002;

	this->EquipmentRegistrable(this);
	if (EquipmentWorldSubSystem)
	{
		EquipmentWorldSubSystem->FEBMoveOrderSignature.AddDynamic(this, &ATowerBuilding::HandleTowerReceiveByEB);
		EquipmentWorldSubSystem->FToiletToTowerSignature.BindUObject(this, &ATowerBuilding::HandleRaidSubdueReceiveByToilet);
	}

	CLSubdueForToilet->OnComponentBeginOverlap.AddDynamic(this, &ATowerBuilding::TowerSubdueOverlapBegin);

	if (TBAudioPlayer && TowerMoveSFXCue)
	{
		TBAudioPlayer->Stop();
		TBAudioPlayer->SetSound(TowerMoveSFXCue);
	}
}

void ATowerBuilding::EquipmentRegistrable(AActor* InActor) { Super::EquipmentRegistrable(InActor); }

void ATowerBuilding::TowerSubdueOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->ComponentHasTag(FName(TEXT("PrisonerCharacter"))))
	{
		APrisonerCharacter* OverlapPrisonerCha = Cast<APrisonerCharacter>(OtherActor);
		checkf(OverlapPrisonerCha, TEXT("In CarCrane, Overlap Prisoner Not Valid!"));
		APrisonerController* OverlapPrisonerCon = Cast<APrisonerController>(OverlapPrisonerCha->GetController());
		checkf(OverlapPrisonerCon, TEXT("In CarCrane, Overlap Prisoner Controller Not Valid"));

		if (OverlapPrisonerCon->GetBBComp()->GetValueAsEnum(TEXT("CurrUpperState")) == 4 && OverlapPrisonerCon->GetBBComp()->GetValueAsEnum(TEXT("CurrLowerState")) == 13)
		{
			UE_LOG(LogTemp, Log, TEXT("TowerRaid State Prisoner Overlap Success!"));

			TArray<uint8> GivenUpperStates = { 1 };
			TArray<uint8> GivenLowerStates = { 1 };

			OverlapPrisonerCon->State_based_ExecutionTasks_GiventoSomeone(GivenUpperStates, GivenLowerStates);
		}
	}
}

// Actually Move the Tower By the ElevatorButton
void ATowerBuilding::ActuallyMoveTower(float TargetTowerHeight)
{
	TBAudioPlayer->Play();
	UKismetSystemLibrary::MoveComponentTo(
		MainRoot,
		FVector(0.f, 0.f, TargetTowerHeight),
		FRotator(0.f, 0.f, 0.f),
		true,
		true,
		5.0f,
		false,
		EMoveComponentAction::Move,
		TowerLatentInfo
	);
}

// Actually Tower Move After Stop
void ATowerBuilding::ActuallyTowerMoveCompleted() { TBAudioPlayer->Stop(); }// Move Sound Stop 