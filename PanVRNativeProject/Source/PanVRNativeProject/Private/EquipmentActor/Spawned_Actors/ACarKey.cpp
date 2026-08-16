#include "EquipmentActor/Spawned_Actors/ACarKey.h"

AACarKey::AACarKey()
{
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_CarKey(TEXT("/Game/VRContent/Modeling/03_CarTypePanel/SM_CarKey.SM_CarKey"));
	if (ModelingFinder_CarKey.Succeeded())
	{
		ActorBaseMesh->SetStaticMesh(ModelingFinder_CarKey.Object);
	}

	ActorBaseMesh->ComponentTags.Add(FName("CarKey"));
	ActorBaseMesh->ComponentTags.Add(FName("Trash"));
	ActorBaseMesh->SetCollisionProfileName(FName("PhysicsActor"));

	GC->SetPrimitiveCompPhysics(true);


	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MatFinder_Main(TEXT("/Game/VRContent/Material/SRS_STAGE_Main.SRS_STAGE_Main"));
	if (MatFinder_Main.Succeeded())
		ActorBaseMesh->SetMaterial(0, MatFinder_Main.Object);

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

void AACarKey::BeginPlay()
{
	Super::BeginPlay();
}

void AACarKey::OnGrabbed(UMotionControllerComponent& InMCRef, const FVector& HandGrabPos, class AVRHand* InGrabbingHand)
{
	GC->SetPrimitiveCompPhysics(false);
	GetWorldTimerManager().PauseTimer(DestroySelfTimer);
	return;
}

void AACarKey::OnDropped()
{
	GC->SetPrimitiveCompPhysics(true);
	GetWorldTimerManager().SetTimer(
		DestroySelfTimer,
		this,
		&AACarKey::DestroySelf,
		3.0f,
		false
	);
	return;
}

void AACarKey::SetDontGrabPhysics()
{
	ActorBaseMesh->SetCollisionProfileName(FName("NoCollision"));
}


void AACarKey::DestroySelf()
{
	this->Destroy();
	return;
}