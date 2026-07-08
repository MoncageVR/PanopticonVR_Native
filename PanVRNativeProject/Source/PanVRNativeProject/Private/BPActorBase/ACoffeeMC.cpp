// Fill out your copyright notice in the Description page of Project Settings.


#include "BPActorBase/ACoffeeMC.h"

AACoffeeMC::AACoffeeMC()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_CMCBody(TEXT("/Game/VRContent/Modeling/CoffeeMachine/CoffeeMCBody.CoffeeMCBody"));
	if (ModelingFinder_CMCBody.Succeeded())
	{
		ActorBaseMesh->SetStaticMesh(ModelingFinder_CMCBody.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_CMCHandle(TEXT("/Game/VRContent/Modeling/CoffeeMachine/CoffeeMCHandle.CoffeeMCHandle"));
	CoffeeMCHandle = CreateDefaultSubobject<UStaticMeshComponent>("SM_Handle");
	if (CoffeeMCHandle)
	{
		CoffeeMCHandle->SetupAttachment(ActorBaseMesh);
		if (ModelingFinder_CMCHandle.Succeeded())
		{
			CoffeeMCHandle->SetStaticMesh(ModelingFinder_CMCHandle.Object);
		}
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

void AACoffeeMC::BeginPlay()
{
	Super::BeginPlay();
}

void AACoffeeMC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
