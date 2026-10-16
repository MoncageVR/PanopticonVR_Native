#include "EquipmentActor/Support_Actors/ABeamProjectors.h"

AABeamProjectors::AABeamProjectors()
{
	PrimaryActorTick.bCanEverTick = false;

	MainRoot = CreateDefaultSubobject<USceneComponent>(FName(TEXT("MainSceneComp")));
	if (MainRoot)
	{
		this->SetRootComponent(MainRoot);
	}

	if (ActorBaseMesh)
	{
		ActorBaseMesh->SetupAttachment(MainRoot);
	}

	BP_Parts2 = CreateDefaultSubobject<UStaticMeshComponent>(FName(TEXT("SM_BeamProjector_Part2")));
	BP_Parts3 = CreateDefaultSubobject<UStaticMeshComponent>(FName(TEXT("SM_BeamProjector_Part3")));
	if (BP_Parts2 && BP_Parts3)
	{
		BP_Parts2->SetupAttachment(MainRoot);
		BP_Parts3->SetupAttachment(MainRoot);
	}

	BP_PlaneParts1 = CreateDefaultSubobject<UStaticMeshComponent>(FName(TEXT("SM_BeamProjector_Plane_Part1")));
	BP_PlaneParts2 = CreateDefaultSubobject<UStaticMeshComponent>(FName(TEXT("SM_BeamProjector_Plane_Part2")));
	BP_PlaneParts3 = CreateDefaultSubobject<UStaticMeshComponent>(FName(TEXT("SM_BeamProjector_Plane_Part3")));

	if (BP_PlaneParts1 && BP_PlaneParts2 && BP_PlaneParts3)
	{
		BP_PlaneParts1->SetupAttachment(ActorBaseMesh);
		BP_PlaneParts2->SetupAttachment(BP_Parts2);
		BP_PlaneParts3->SetupAttachment(BP_Parts3);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SMFinder_BeamProjector(TEXT("/Game/VRContent/Modeling/30_BeamProjector/BeamProjector.BeamProjector"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SMFinder_BP_Plane(TEXT("/Engine/BasicShapes/Plane.Plane"));
	if (SMFinder_BeamProjector.Succeeded() && SMFinder_BP_Plane.Succeeded())
	{
		ActorBaseMesh->SetStaticMesh(SMFinder_BeamProjector.Object);
		BP_Parts2->SetStaticMesh(SMFinder_BeamProjector.Object);
		BP_Parts3->SetStaticMesh(SMFinder_BeamProjector.Object);

		BP_PlaneParts1->SetStaticMesh(SMFinder_BP_Plane.Object);
		BP_PlaneParts2->SetStaticMesh(SMFinder_BP_Plane.Object);
		BP_PlaneParts3->SetStaticMesh(SMFinder_BP_Plane.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MatFinder_MI_LSDefault(TEXT("/Game/VRContent/Material/M_LS1_1.M_LS1_1"));
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MatFinder_MI_Main(TEXT("/Game/VRContent/Material/SRS_STAGE_Main.SRS_STAGE_Main"));
	static ConstructorHelpers::FObjectFinder<UMaterial> MatFinder_M_Hologram(TEXT("/Game/VRContent/Material/M_Hologram.M_Hologram"));

	if (MatFinder_MI_LSDefault.Succeeded() && MatFinder_MI_Main.Succeeded() && MatFinder_M_Hologram.Succeeded())
	{
		BP_PlaneParts1->SetMaterial(0, MatFinder_MI_LSDefault.Object);
		BP_PlaneParts2->SetMaterial(0, MatFinder_MI_LSDefault.Object);
		BP_PlaneParts3->SetMaterial(0, MatFinder_MI_LSDefault.Object);

		ActorBaseMesh->SetMaterial(0, MatFinder_MI_Main.Object);
		BP_Parts2->SetMaterial(0, MatFinder_MI_Main.Object);
		BP_Parts3->SetMaterial(0, MatFinder_MI_Main.Object);

		ActorBaseMesh->SetMaterial(1, MatFinder_M_Hologram.Object);
		BP_Parts2->SetMaterial(1, MatFinder_M_Hologram.Object);
		BP_Parts3->SetMaterial(1, MatFinder_M_Hologram.Object);
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

void AABeamProjectors::BeginPlay()
{
	Super::BeginPlay();

}
