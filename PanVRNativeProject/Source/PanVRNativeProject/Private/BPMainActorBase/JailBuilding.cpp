// Fill out your copyright notice in the Description page of Project Settings.


#include "BPMainActorBase/JailBuilding.h"
#include "Components/BoxComponent.h"

AJailBuilding::AJailBuilding()
{
	PrimaryActorTick.bCanEverTick = true;

	JailMainRoot = CreateDefaultSubobject<USceneComponent>("MainSceneComp");
	if (JailMainRoot)
	{
		SetRootComponent(JailMainRoot);
		JailMainRoot->Mobility = EComponentMobility::Static;
	}

	FirstFloorRoot = CreateDefaultSubobject<USceneComponent>("1stFloorSceneComp");
	if (FirstFloorRoot)
	{
		FirstFloorRoot->SetupAttachment(JailMainRoot);
		FirstFloorRoot->Mobility = EComponentMobility::Movable;
	}

	SecondFloorRoot = CreateDefaultSubobject<USceneComponent>("2ndFloorSceneComp");
	if (SecondFloorRoot)
	{
		SecondFloorRoot->SetupAttachment(JailMainRoot);
		SecondFloorRoot->Mobility = EComponentMobility::Movable;
	}

	ThirdFloorRoot = CreateDefaultSubobject<USceneComponent>("3rdFloorSceneComp");
	if (ThirdFloorRoot)
	{
		ThirdFloorRoot->SetupAttachment(JailMainRoot);
		ThirdFloorRoot->Mobility = EComponentMobility::Movable;
	}

	MoveableBuildingsRoot = CreateDefaultSubobject<USceneComponent>("MoveableSceneComp");
	if (MoveableBuildingsRoot)
	{
		MoveableBuildingsRoot->SetupAttachment(JailMainRoot);
		MoveableBuildingsRoot->Mobility = EComponentMobility::Movable;
	}

	StaticBuildingsRoot = CreateDefaultSubobject<USceneComponent>("StaticSceneComp");
	if (StaticBuildingsRoot)
	{
		StaticBuildingsRoot->SetupAttachment(JailMainRoot);
		StaticBuildingsRoot->Mobility = EComponentMobility::Static;
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_JailBody(TEXT("/Game/VRContent/Modeling/Building_Jail/Jail_Frame.Jail_Frame"));
	JailMainBody = CreateDefaultSubobject<UStaticMeshComponent>("JailBody_SMComp");
	if (JailMainBody)
	{
		JailMainBody->Mobility = EComponentMobility::Static;
		JailMainBody->SetupAttachment(StaticBuildingsRoot);
		JailMainBody->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
		if (ModelingFinder_JailBody.Succeeded())
		{
			JailMainBody->SetStaticMesh(ModelingFinder_JailBody.Object);
		}
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_JailExitDoor(TEXT("/Game/VRContent/Modeling/Building_Jail/ExitDoor.ExitDoor"));
	JailExitDoor = CreateDefaultSubobject<UStaticMeshComponent>("JailExitDoor_SMComp");
	if (JailExitDoor)
	{
		JailExitDoor->Mobility = EComponentMobility::Movable;
		JailExitDoor->SetupAttachment(MoveableBuildingsRoot);
		JailExitDoor->SetRelativeLocation(FVector(0.0f, -245.0f, -0.5f));
		JailExitDoor->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
		if (ModelingFinder_JailExitDoor.Succeeded())
		{
			JailExitDoor->SetStaticMesh(ModelingFinder_JailExitDoor.Object);
		}
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_JailHatch(TEXT("/Game/VRContent/Modeling/Building_Jail/Roof_Hatch.Roof_Hatch"));
	JailHatch = CreateDefaultSubobject<UStaticMeshComponent>("JailHatch_SMComp");
	if (JailHatch)
	{
		JailHatch->Mobility = EComponentMobility::Movable;
		JailHatch->SetupAttachment(MoveableBuildingsRoot);
		JailHatch->SetRelativeLocation(FVector(0.0f, 0.0f, 3850.0f));
		JailHatch->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
		if (ModelingFinder_JailHatch.Succeeded())
		{
			JailHatch->SetStaticMesh(ModelingFinder_JailHatch.Object);
		}
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_JailRoof(TEXT("/Game/VRContent/Modeling/Building_Jail/Roof.Roof"));
	JailRoof = CreateDefaultSubobject<UStaticMeshComponent>("JailRoof_SMComp");
	if (JailRoof)
	{
		JailRoof->SetMobility(EComponentMobility::Static);
		JailRoof->SetupAttachment(StaticBuildingsRoot);
		JailRoof->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
		if (ModelingFinder_JailRoof.Succeeded())
		{
			JailRoof->SetStaticMesh(ModelingFinder_JailRoof.Object);
		}
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_JailWeaponDoor(TEXT("/Game/VRContent/Modeling/Building_Jail/WeaponDoor.WeaponDoor"));
	Jail1FWeaponDoor = CreateDefaultSubobject<UStaticMeshComponent>("Jail1FWeaponDoor_SMComp");
	Jail2FWeaponDoor = CreateDefaultSubobject<UStaticMeshComponent>("Jail2FWeaponDoor_SMComp");
	Jail3FWeaponDoor = CreateDefaultSubobject<UStaticMeshComponent>("Jail3FWeaponDoor_SMComp");
	if (Jail1FWeaponDoor && Jail2FWeaponDoor && Jail3FWeaponDoor)
	{
		FRotator TempCommonRot = FRotator(0.0f, -90.0f, 0.0f);

		Jail1FWeaponDoor->SetMobility(EComponentMobility::Movable);
		Jail1FWeaponDoor->SetupAttachment(FirstFloorRoot);
		Jail1FWeaponDoor->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		Jail1FWeaponDoor->SetRelativeRotation(TempCommonRot);

		Jail2FWeaponDoor->SetMobility(EComponentMobility::Movable);
		Jail2FWeaponDoor->SetupAttachment(SecondFloorRoot);
		Jail2FWeaponDoor->SetRelativeLocation(FVector(0.0f, 0.0f, 929.25f));
		Jail2FWeaponDoor->SetRelativeRotation(TempCommonRot);

		Jail3FWeaponDoor->SetMobility(EComponentMobility::Movable);
		Jail3FWeaponDoor->SetupAttachment(ThirdFloorRoot);
		Jail3FWeaponDoor->SetRelativeLocation(FVector(0.0f, 0.0f, 1860.0f));
		Jail3FWeaponDoor->SetRelativeRotation(TempCommonRot);

		if (ModelingFinder_JailWeaponDoor.Succeeded())
		{
			Jail1FWeaponDoor->SetStaticMesh(ModelingFinder_JailWeaponDoor.Object);
			Jail2FWeaponDoor->SetStaticMesh(ModelingFinder_JailWeaponDoor.Object);
			Jail3FWeaponDoor->SetStaticMesh(ModelingFinder_JailWeaponDoor.Object);
		}
	}

	CLSubdueHatch = CreateDefaultSubobject<UBoxComponent>("CL_TopEscapeSubdue");
	if (CLSubdueHatch)
	{
		CLSubdueHatch->SetupAttachment(JailHatch);
		CLSubdueHatch->SetRelativeLocation(FVector(0.0f, 0.0f, -125.0f));
		CLSubdueHatch->SetBoxExtent(FVector(200.0f, 200.0f, 200.0f));
		CLSubdueHatch->SetHiddenInGame(false); // Debug
	}

	Jail1FWeaponDoor->SetCanEverAffectNavigation(false);
	Jail2FWeaponDoor->SetCanEverAffectNavigation(false);
	Jail3FWeaponDoor->SetCanEverAffectNavigation(false);
	JailExitDoor->SetCanEverAffectNavigation(false);

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialFinder_Hatch(TEXT("/Game/VRContent/Material/SRS_STAGE_hatch.SRS_STAGE_hatch"));
	if (MaterialFinder_Hatch.Succeeded())
	{
		JailExitDoor->SetMaterial(0, MaterialFinder_Hatch.Object);
		JailHatch->SetMaterial(0, MaterialFinder_Hatch.Object);
		JailRoof->SetMaterial(0, MaterialFinder_Hatch.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialFinder_Wall(TEXT("/Game/VRContent/Material/SRS_STAGE_WALL.SRS_STAGE_WALL"));
	if (MaterialFinder_Wall.Succeeded())
	{
		JailMainBody->SetMaterial(0, MaterialFinder_Wall.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialFinder_Main(TEXT("/Game/VRContent/Material/SRS_STAGE_Main.SRS_STAGE_Main"));
	if (MaterialFinder_Main.Succeeded())
	{
		Jail1FWeaponDoor->SetMaterial(0, MaterialFinder_Main.Object);
		Jail2FWeaponDoor->SetMaterial(0, MaterialFinder_Main.Object);
		Jail3FWeaponDoor->SetMaterial(0, MaterialFinder_Main.Object);
	}

}

void AJailBuilding::BeginPlay()
{
	Super::BeginPlay();
	
}

void AJailBuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

