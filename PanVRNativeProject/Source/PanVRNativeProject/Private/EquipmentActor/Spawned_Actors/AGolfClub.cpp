


#include "EquipmentActor/Spawned_Actors/AGolfClub.h"

AAGolfClub::AAGolfClub()
{
	ClubMainRoot = CreateDefaultSubobject<USceneComponent>(TEXT("GolfClubMainRoot"));
	if (ClubMainRoot)
	{
		this->SetRootComponent(ClubMainRoot);
	}

	if (ActorBaseMesh)
	{
		ActorBaseMesh->SetupAttachment(ClubMainRoot);
		ActorBaseMesh->SetCanEverAffectNavigation(false);
		ActorBaseMesh->SetCollisionProfileName(TEXT("NoCollision"));
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SMFinder_ClubBody(TEXT("/Game/VRContent/Modeling/11_GolfRelated/GolfClub.GolfClub"));
	if (SMFinder_ClubBody.Succeeded())
	{
		ActorBaseMesh->SetStaticMesh(SMFinder_ClubBody.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MatFinder_Main(TEXT("/Game/VRContent/Material/SRS_STAGE_Main.SRS_STAGE_Main"));
	if (MatFinder_Main.Succeeded())
		ActorBaseMesh->SetMaterial(0, MatFinder_Main.Object);
}

void AAGolfClub::BeginPlay()
{
	Super::BeginPlay();
}

void AAGolfClub::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
