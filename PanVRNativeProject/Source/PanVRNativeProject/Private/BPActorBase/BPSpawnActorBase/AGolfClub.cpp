// Fill out your copyright notice in the Description page of Project Settings.


#include "BPActorBase/BPSpawnActorBase/AGolfClub.h"

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

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SMFinder_ClubBody(TEXT("/Game/VRContent/Prisoner/GolfRelate/Modeling/GolfClub.GolfClub"));
	if (SMFinder_ClubBody.Succeeded())
	{
		ActorBaseMesh->SetStaticMesh(SMFinder_ClubBody.Object);
	}
}

void AAGolfClub::BeginPlay()
{
	Super::BeginPlay();
}

void AAGolfClub::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
