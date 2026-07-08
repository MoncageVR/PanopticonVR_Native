// Fill out your copyright notice in the Description page of Project Settings.


#include "BPActorBase/ASodaBullet.h"
#include "Components/BoxComponent.h"
#include "Core/Component/UGrabComp.h"

AASodaBullet::AASodaBullet()
{
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Finder_SodaBulletBodyModeling(TEXT("/Game/VRContent/Modeling/SodaGun/Mesh_SodaCan.Mesh_SodaCan"));
	if (Finder_SodaBulletBodyModeling.Succeeded())
	{
		ActorBaseMesh->SetStaticMesh(Finder_SodaBulletBodyModeling.Object);
	}
	ActorBaseMesh->SetRelativeScale3D(FVector(0.8f, 0.8f, 0.8f));

	ActorBaseMesh->SetSimulatePhysics(true);
	ActorBaseMesh->SetCollisionProfileName("PhysicsActor");

	GC->EEGrabType = EGrabType::Free;
	GC->SetRelativeLocation(FVector(0.0f, 0.0f, 6.5f));

	CLBox = CreateDefaultSubobject<UBoxComponent>("CLBoxBullet");
	CLBox->SetupAttachment(ActorBaseMesh);
	CLBox->SetBoxExtent(FVector(4.2f, 4.2f, 6.56f));
	CLBox->SetRelativeLocation(FVector(0.0f, 0.0f, 6.5f));
	CLBox->ComponentTags.Add(FName("SodaBullet"));
	CLBox->ComponentTags.Add(FName("Trash"));


	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialFinder_SodaBullet(TEXT("/Game/VRContent/Material/SRS_Stage_Sodagun_Bullet.SRS_Stage_Sodagun_Bullet"));
	if (MaterialFinder_SodaBullet.Succeeded())
	{
		ActorBaseMesh->SetMaterial(0, MaterialFinder_SodaBullet.Object);
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

void AASodaBullet::SetActorPhysics(bool PhysicsFlag)
{
	if (PhysicsFlag)
	{
		ActorBaseMesh->SetSimulatePhysics(PhysicsFlag);
	}
	else
	{
		ActorBaseMesh->SetSimulatePhysics(PhysicsFlag);
	}
}

void AASodaBullet::BeginPlay()
{
	Super::BeginPlay();



}

void AASodaBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AASodaBullet::OnGrabbed(UMotionControllerComponent& InMCRef, const FVector& HandGrabPos, class AVRHand* InGrabbingHand)
{
	GC->SetPrimitiveCompPhysics(false);
	GetWorldTimerManager().PauseTimer(DestroySelfTimer);
	return;
}

void AASodaBullet::OnDropped()
{
	if (bIsSBAttaching != 1)
	{
		GC->SetPrimitiveCompPhysics(true);
		GetWorldTimerManager().SetTimer(
			DestroySelfTimer,
			this,
			&AASodaBullet::DestroySelf,
			3.0f,
			false
		);
	}
	return;
}

void AASodaBullet::DestroySelf()
{
	this->Destroy();
	return;
}

void AASodaBullet::CurveMove(FVector InDir)
{
	bIsSBAttaching = 0;
	FVector FinalDir = InDir * 100.0f;

	ActorBaseMesh->AddImpulse(FinalDir);

	ActorBaseMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	GC->SetPrimitiveCompPhysics(true);
	GetWorldTimerManager().SetTimer(
		DestroySelfTimer,
		this,
		&AASodaBullet::DestroySelf,
		1.0f,
		false
	);
}

void AASodaBullet::SetSBIsAttaching(uint32 TempFlag) { bIsSBAttaching = TempFlag; }
uint32 AASodaBullet::GetSBIsAttaching() const { return bIsSBAttaching; }