// Fill out your copyright notice in the Description page of Project Settings.


#include "BPActorBase/ACarKey.h"

AACarKey::AACarKey()
{
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_CarKey(TEXT("/Game/VRContent/Modeling/CarPanel/SM_CarKey.SM_CarKey"));
	if (ModelingFinder_CarKey.Succeeded())
	{
		ActorBaseMesh->SetStaticMesh(ModelingFinder_CarKey.Object);
	}

	ActorBaseMesh->ComponentTags.Add(FName("CarKey"));
	ActorBaseMesh->SetCollisionProfileName(FName("PhysicsActor"));

	GC->SetPrimitiveCompPhysics(true);

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