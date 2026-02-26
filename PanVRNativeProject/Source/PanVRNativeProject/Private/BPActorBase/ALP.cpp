// Fill out your copyright notice in the Description page of Project Settings.


#include "BPActorBase/ALP.h"
#include "Components/BoxComponent.h"
#include "Core/Debug/FDebugLib.h"

AALP::AALP()
{
	PrimaryActorTick.bCanEverTick = false;

	SongNum = 0;
	bIsHanding = 0;
	bIsAttaching = 0;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_LP(TEXT("/Game/VRContent/Modeling/TurnTable/TurnTableLP.TurnTableLP"));
	if (ModelingFinder_LP.Succeeded())
	{
		ActorBaseMesh->SetStaticMesh(ModelingFinder_LP.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialFinder_LP(TEXT("/Game/VRContent/Material/SRS_Stage_LP.SRS_Stage_LP"));
	if (MaterialFinder_LP.Succeeded())
	{
		ActorBaseMesh->SetMaterial(0, MaterialFinder_LP.Object);
	}

	GC->EEGrabType = EGrabType::Free;

	CLLP = CreateDefaultSubobject<UBoxComponent>("BoxCollisionComp");
	if (CLLP)
	{
		CLLP->SetupAttachment(ActorBaseMesh);
		CLLP->SetBoxExtent(FVector(4.8f, 4.8f, 2.0f));
		CLLP->SetHiddenInGame(false); // Debug
		CLLP->ShapeColor = FColor::Red; // Debug
	}

	CLLP->ComponentTags.Add(FName("LP"));

	//CLLP->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

}

void AALP::BeginPlay()
{
	Super::BeginPlay();
}

void AALP::Tick(float DeletaTime)
{
	Super::Tick(DeletaTime);
}

void AALP::OnGrabbed(UMotionControllerComponent& InMCRef, const FVector& HandGrabPos, class AVRHand* InGrabbingHand)
{
	bIsHanding = 1;

	GetWorldTimerManager().PauseTimer(SelfDestroyTimer);
}

void AALP::OnDropped()
{
	bIsHanding = 0;

	GC->SetPrimitiveCompPhysics(true); // Turn on the Physics

	if (!bIsHanding && !bIsAttaching)
	{
		GetWorldTimerManager().SetTimer(
			SelfDestroyTimer,
			this,
			&AALP::DestroyLPSelf,
			3.0f,
			false);
	}
}

void AALP::DestroyLPSelf()
{
	this->Destroy();
	return;
}

const int32 AALP::GetSongNum() const { return SongNum; }
const uint32 AALP::GetIsHanding() const { return bIsHanding; }
const uint32 AALP::GetIsAttaching() const { return bIsAttaching; }

void AALP::SetSongNum(int32 TempSongNum) { this->SongNum = TempSongNum; }
void AALP::SetIsAttaching(uint32 TempbIsAttaching) { this->bIsAttaching = TempbIsAttaching; }

void AALP::SetMeshCollisionEnabled(uint32 CollisionEnabled)
{
	if (CollisionEnabled)
	{
		ActorBaseMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		CLLP->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		CLLP->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	}
	else
	{
		ActorBaseMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		CLLP->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		CLLP->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Ignore);

	}

	return;
}

