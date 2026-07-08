// Fill out your copyright notice in the Description page of Project Settings.


#include "BPMainActorBase/TowerBuilding.h"
#include "Components/BoxComponent.h"
#include "Components/AudioComponent.h"
#include "CoreObj/VREquipmentWorldSubsystem.h"

ATowerBuilding::ATowerBuilding()
{
	PrimaryActorTick.bCanEverTick = false;

	MainRoot = CreateDefaultSubobject<USceneComponent>("SceneComp");
	if (MainRoot)
	{
		this->SetRootComponent(MainRoot);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_TowerBody(TEXT("/Game/VRContent/Modeling/Building_Tower/TowerMainBody.TowerMainBody"));
	TowerMainBody = CreateDefaultSubobject<UStaticMeshComponent>("SM_TowerBody");
	if (TowerMainBody)
	{
		TowerMainBody->SetupAttachment(MainRoot);
		TowerMainBody->SetRelativeLocation(FVector(0.0f, 0.0f, 1250.0f));
		TowerMainBody->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
		if (ModelingFinder_TowerBody.Succeeded())
		{
			TowerMainBody->SetStaticMesh(ModelingFinder_TowerBody.Object);
		}
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_TowerMainDesk(TEXT("/Game/VRContent/Modeling/Building_Tower/MainDesk.MainDesk"));
	MainDesk = CreateDefaultSubobject<UStaticMeshComponent>("SM_MainDesk");
	if (MainDesk)
	{
		MainDesk->SetupAttachment(MainRoot);
		MainDesk->SetRelativeLocation(FVector(0.0f, 0.0f, 1250.0f));
		MainDesk->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
		if (ModelingFinder_TowerMainDesk.Succeeded())
		{
			MainDesk->SetStaticMesh(ModelingFinder_TowerMainDesk.Object);
		}
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_TowerSubDesk(TEXT("/Game/VRContent/Modeling/Building_Tower/SubDesk.SubDesk"));
	SubDesk = CreateDefaultSubobject<UStaticMeshComponent>("SM_SubDesk");
	if (SubDesk)
	{
		SubDesk->SetupAttachment(MainRoot);
		SubDesk->SetRelativeLocation(FVector(-3.8f, -5.4f, 1250.0f));
		SubDesk->SetRelativeRotation(FRotator(0.0f, 145.0f, 0.0f));
		if (ModelingFinder_TowerSubDesk.Succeeded())
		{
			SubDesk->SetStaticMesh(ModelingFinder_TowerSubDesk.Object);
		}
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_TowerBarricade(TEXT("/Game/VRContent/Modeling/Building_Tower/Tower_Barricade.Tower_Barricade"));
	Barricade = CreateDefaultSubobject<UStaticMeshComponent>("SM_Barricade");
	if (Barricade)
	{
		Barricade->SetupAttachment(MainRoot);
		Barricade->SetRelativeLocation(FVector(0.0f, 0.0f, 1540.0f));
		if (ModelingFinder_TowerBarricade.Succeeded())
		{
			Barricade->SetStaticMesh(ModelingFinder_TowerBarricade.Object);
		}
		Barricade->SetHiddenInGame(true);
		Barricade->SetVisibility(false);
	}

	CLSubdueForToilet = CreateDefaultSubobject<UBoxComponent>("CL_SubdueTowerRaid");
	if (CLSubdueForToilet)
	{
		CLSubdueForToilet->SetupAttachment(MainRoot);
		CLSubdueForToilet->SetBoxExtent(FVector(350.0f, 250.0f, 1700.0f));
		CLSubdueForToilet->SetHiddenInGame(false); // Debug
	}

	CLSubdueForKeyPad = CreateDefaultSubobject<UBoxComponent>("CL_SubdueFiveState");
	if (CLSubdueForKeyPad)
	{
		CLSubdueForKeyPad->SetupAttachment(CLSubdueForToilet);
		CLSubdueForKeyPad->SetRelativeLocation(FVector(0.0f, 0.0f, 1300.0f));
		CLSubdueForKeyPad->SetBoxExtent(FVector(2300.0f, 2300.f, 50.0f));
		CLSubdueForKeyPad->SetHiddenInGame(false); // Debug
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialFinder_Tower(TEXT("/Game/VRContent/Material/SRS_STAGE_TOWER.SRS_STAGE_TOWER"));
	if (MaterialFinder_Tower.Succeeded())
	{
		TowerMainBody->SetMaterial(0, MaterialFinder_Tower.Object);
		MainDesk->SetMaterial(0, MaterialFinder_Tower.Object);
		SubDesk->SetMaterial(0, MaterialFinder_Tower.Object);
	}

	if (TargetTowerHeights.IsEmpty())
	{
		TargetTowerHeights.Add(-1100.0f);
		TargetTowerHeights.Add(-190.0f);
		TargetTowerHeights.Add(790.0f);
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> SoundFinder_TowerMoveCue(TEXT("/Game/VRContent/Sound/Ques/ElevatorButton/sfx_elevator_move_Cue.sfx_elevator_move_Cue"));
	TBAudioPlayer = CreateDefaultSubobject<UAudioComponent>("SC_TBAudioComp");
	if (TBAudioPlayer && SoundFinder_TowerMoveCue.Succeeded())
	{
		TowerMoveSFXCue = SoundFinder_TowerMoveCue.Object;
		TBAudioPlayer->SetupAttachment(MainRoot);
		TBAudioPlayer->SetAutoActivate(false);
		TBAudioPlayer->bAllowSpatialization = false;
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
	}

	if (TBAudioPlayer && TowerMoveSFXCue)
	{
		TBAudioPlayer->Stop();
		TBAudioPlayer->SetSound(TowerMoveSFXCue);
	}
}

void ATowerBuilding::EquipmentRegistrable(AActor* InActor)
{
	Super::EquipmentRegistrable(InActor);
}

void ATowerBuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int32 ATowerBuilding::GetTowerCurrFloorNum() const
{
	return ActuallyCurrFloorNum;
}
void ATowerBuilding::SetTowerCurrFloorNum(int32 InCurrFloor)
{
	ActuallyCurrFloorNum = InCurrFloor;
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
			{
				ActuallyMoveTower(TargetTowerHeights[ActuallyCurrFloorNum - 1]);
			}
			else if (ActuallyCurrFloorNum == 2 && InFloor == 2)
			{
				ActuallyMoveTower(TargetTowerHeights[ActuallyCurrFloorNum - 1]);
			}
			else if (ActuallyCurrFloorNum == 3 && InFloor == 3)
			{
				ActuallyMoveTower(TargetTowerHeights[ActuallyCurrFloorNum - 1]);
			}
		}
	}
}

void ATowerBuilding::ActuallyTowerMoveCompleted()
{
	TBAudioPlayer->Stop();
	return;
}

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

