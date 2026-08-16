#include "MainActor/ACCTVMonitor.h"

AACCTVMonitor::AACCTVMonitor()
{
	PrimaryActorTick.bCanEverTick = false;

	SC_MainRoot = CreateDefaultSubobject<USceneComponent>("MainRoot_SceneComp");
	if (SC_MainRoot)
	{
		this->SetRootComponent(SC_MainRoot);
	}

	CCTVArm = CreateDefaultSubobject<UStaticMeshComponent>("SM_CCTVArm");
	if (CCTVArm)
	{
		CCTVArm->SetupAttachment(SC_MainRoot);
		CCTVArm->SetCollisionProfileName(FName("NoCollision"));
	}

	if (ActorBaseMesh)
	{
		ActorBaseMesh->SetupAttachment(CCTVArm);
		ActorBaseMesh->SetRelativeRotation(FRotator(0.f, 0.f, 30.0f));
	}

	CCTVGlass = CreateDefaultSubobject<UStaticMeshComponent>("SM_CCTVGlass");
	if (CCTVGlass)
	{
		CCTVGlass->SetupAttachment(ActorBaseMesh);
		CCTVGlass->SetCollisionProfileName(FName("NoCollision"));
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_Arm(TEXT("/Game/VRContent/Modeling/04_CCTVMonitor/SM_CCTVArm.SM_CCTVArm"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_Body(TEXT("/Game/VRContent/Modeling/04_CCTVMonitor/SM_CCTVBody.SM_CCTVBody"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_Glass(TEXT("/Game/VRContent/Modeling/04_CCTVMonitor/SM_CCTVGlass.SM_CCTVGlass"));
	if (ModelingFinder_Arm.Succeeded() && ModelingFinder_Body.Succeeded() && ModelingFinder_Glass.Succeeded())
	{
		CCTVArm->SetStaticMesh(ModelingFinder_Arm.Object);
		ActorBaseMesh->SetStaticMesh(ModelingFinder_Body.Object);
		CCTVGlass->SetStaticMesh(ModelingFinder_Glass.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MatFinder_Main(TEXT("/Game/VRContent/Material/SRS_STAGE_Main.SRS_STAGE_Main"));
	if (MatFinder_Main.Succeeded())
	{
		CCTVArm->SetMaterial(0, MatFinder_Main.Object);
		ActorBaseMesh->SetMaterial(0, MatFinder_Main.Object);
	}
}

void AACCTVMonitor::BeginPlay()
{
	Super::BeginPlay();
}
