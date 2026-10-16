#include "EquipmentActor/Support_Actors/AVRBossMonitor.h"

AAVRBossMonitor::AAVRBossMonitor()
{
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_BossMonitorBody(TEXT("/Game/VRContent/Modeling/02_BossMonitor/BossMonitorBody.BossMonitorBody"));
	if (ModelingFinder_BossMonitorBody.Succeeded())
		ActorBaseMesh->SetStaticMesh(ModelingFinder_BossMonitorBody.Object);

	SM_Glass = CreateDefaultSubobject<UStaticMeshComponent>(FName(TEXT("GlassSMComp")));
	if (SM_Glass)
	{
		SM_Glass->SetupAttachment(ActorBaseMesh);

		static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_BossMonitorGlass(TEXT("/Game/VRContent/Modeling/02_BossMonitor/BossMonitorGlass.BossMonitorGlass"));
		if (ModelingFinder_BossMonitorGlass.Succeeded())
		{
			SM_Glass->SetStaticMesh(ModelingFinder_BossMonitorGlass.Object);
		}
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> MatFinder_Hologram(TEXT("/Game/VRContent/Material/M_Hologram.M_Hologram"));
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MatFinder_MonitorBody(TEXT("/Game/VRContent/Material/SRS_STAGE_ABbutton.SRS_STAGE_ABbutton"));
	if (MatFinder_Hologram.Succeeded() && MatFinder_MonitorBody.Succeeded())
	{
		ActorBaseMesh->SetMaterial(0, MatFinder_MonitorBody.Object);
		SM_Glass->SetMaterial(0, MatFinder_Hologram.Object);
	}

	ActorBaseMesh->SetCanEverAffectNavigation(false);
	SM_Glass->SetCanEverAffectNavigation(false);
}

void AAVRBossMonitor::BeginPlay()
{
	Super::BeginPlay();
}
