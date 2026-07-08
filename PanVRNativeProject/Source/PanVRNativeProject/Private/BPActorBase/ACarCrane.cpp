// Fill out your copyright notice in the Description page of Project Settings.


#include "BPActorBase/ACarCrane.h"
#include "Components/BoxComponent.h"

AACarCrane::AACarCrane()
{
	PrimaryActorTick.bCanEverTick = false;

	CarCraneMainRoot = CreateDefaultSubobject<USceneComponent>("SceneComp");
	if (CarCraneMainRoot)
	{
		SetRootComponent(CarCraneMainRoot);
		ActorBaseMesh->SetupAttachment(CarCraneMainRoot);
	}

	// Setting UFOBody Static Mesh Component
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_UFOBody(TEXT("/Game/VRContent/Modeling/CarCrane/SM_UFO_Body.SM_UFO_Body"));
	UFOMainBody = CreateDefaultSubobject<UStaticMeshComponent>("SM_UFOMainBody");
	if (UFOMainBody)
	{
		UFOMainBody->SetupAttachment(ActorBaseMesh);
		UFOMainBody->SetRelativeLocation(FVector(750.0f, 0.0f, 0.0f));
		if (ModelingFinder_UFOBody.Succeeded())
		{
			UFOMainBody->SetStaticMesh(ModelingFinder_UFOBody.Object);
		}
	}

	// Setting _UFOPortal Static Mesh Component
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_UFOPortal(TEXT("/Game/VRContent/Modeling/CarCrane/SM_UFO_Portal.SM_UFO_Portal"));
	UFOPortal = CreateDefaultSubobject<UStaticMeshComponent>("SM_UFOPortal");
	if (UFOPortal)
	{
		UFOPortal->SetupAttachment(UFOMainBody);
		if (ModelingFinder_UFOPortal.Succeeded())
		{
			UFOPortal->SetStaticMesh(ModelingFinder_UFOPortal.Object);
		}
	}

	// Setting UFOCraneHead Static Mesh Component
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_UFOCraneHead(TEXT("/Game/VRContent/Modeling/CarCrane/SM_UFO_CarCrane.SM_UFO_CarCrane"));
	UFOCraneHead = CreateDefaultSubobject<UStaticMeshComponent>("SM_UFOCraneHead");
	if (UFOCraneHead)
	{
		UFOCraneHead->SetupAttachment(UFOPortal);
		UFOCraneHead->SetRelativeLocation(FVector(0.0f, 0.0f, -110.0f));
		UFOCraneHead->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
		UFOCraneHead->SetRelativeScale3D(FVector(40.0f, 40.0f, 40.0f));
		if (ModelingFinder_UFOCraneHead.Succeeded())
		{
			UFOCraneHead->SetStaticMesh(ModelingFinder_UFOCraneHead.Object);
		}
	}

	// Setting UFO Subdue Box Collision
	CLUFOSubdue = CreateDefaultSubobject<UBoxComponent>("CL_UFOSubdue");
	if (CLUFOSubdue)
	{
		CLUFOSubdue->SetupAttachment(UFOCraneHead);
		CLUFOSubdue->SetRelativeLocation(FVector(0.0f, 0.0f, 1.625f));
		CLUFOSubdue->SetRelativeScale3D(FVector(0.025f, 0.025f, 0.025f));
		CLUFOSubdue->SetBoxExtent(FVector(135.0f, 237.5f, 65.0f));

		CLUFOSubdue->SetVisibility(true); // Debug
		CLUFOSubdue->SetHiddenInGame(false); // Debug
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

void AACarCrane::BeginPlay()
{
	Super::BeginPlay();
}

void AACarCrane::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
