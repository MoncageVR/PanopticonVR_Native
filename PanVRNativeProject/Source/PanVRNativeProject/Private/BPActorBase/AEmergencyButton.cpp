// Fill out your copyright notice in the Description page of Project Settings.


#include "BPActorBase/AEmergencyButton.h"
#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"

AAEmergencyButton::AAEmergencyButton()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_ButtonBody(TEXT("/Game/VRContent/Modeling/EmergencyButton/ButtonBody.ButtonBody"));
	if (ActorBaseMesh && ModelingFinder_ButtonBody.Succeeded())
	{
		ActorBaseMesh->SetRelativeScale3D(FVector(0.4f, 0.4f, 0.4f));
		ActorBaseMesh->SetStaticMesh(ModelingFinder_ButtonBody.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_ButtonHead(TEXT("/Game/VRContent/Modeling/EmergencyButton/ButtonHead.ButtonHead"));
	EMButton = CreateDefaultSubobject<UStaticMeshComponent>("SM_Button");
	if (EMButton && ModelingFinder_ButtonHead.Succeeded())
	{
		EMButton->SetupAttachment(ActorBaseMesh);
		EMButton->SetStaticMesh(ModelingFinder_ButtonHead.Object);
	}

	CLButton = CreateDefaultSubobject<UBoxComponent>("Col_Button");
	if (CLButton)
	{
		CLButton->SetupAttachment(EMButton);
		CLButton->SetRelativeLocation(FVector(0.f, 0.f, 17.75f));
		CLButton->SetBoxExtent(FVector(5.5f, 5.5f, 1.7f));
		CLButton->SetHiddenInGame(false); // Debug
	}

	SCButtonMoveRail = CreateDefaultSubobject<USplineComponent>("Button_SplineComp");
	if (SCButtonMoveRail)
	{
		SCButtonMoveRail->SetupAttachment(ActorBaseMesh);

		SCButtonMoveRail->SetLocationAtSplinePoint(0, FVector(0.f, 0.f, 0.f), ESplineCoordinateSpace::Local);
		SCButtonMoveRail->SetLocationAtSplinePoint(1, FVector(0.f, 0.f, -4.f), ESplineCoordinateSpace::Local);

		SCButtonMoveRail->SetTangentAtSplinePoint(0, FVector(0.f, 0.f, 0.f), ESplineCoordinateSpace::Local);
		SCButtonMoveRail->SetTangentAtSplinePoint(1, FVector(0.f, 0.f, 0.f), ESplineCoordinateSpace::Local);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialFinder_Main(TEXT("/Game/VRContent/Material/SRS_STAGE_Main.SRS_STAGE_Main"));
	if (MaterialFinder_Main.Succeeded())
	{
		ActorBaseMesh->SetMaterial(0, MaterialFinder_Main.Object);
		EMButton->SetMaterial(0, MaterialFinder_Main.Object);
	}

	CLButton->OnComponentBeginOverlap.AddDynamic(this, &AAEmergencyButton::EMButtonColOverlapBegin);

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

void AAEmergencyButton::EMButtonColOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->ComponentHasTag(FName("HandIndex")) && !bIsHitting)
	{
		EMButton->SetRelativeLocation(SCButtonMoveRail->GetLocationAtSplinePoint(1, ESplineCoordinateSpace::Local));
		bIsHitting = 1;

		CLButton->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		GetWorldTimerManager().SetTimer(
			ReturnTimer,
			this,
			&AAEmergencyButton::OperateAfterReturn,
			1.0f,
			false
		);
	}
}

void AAEmergencyButton::OperateAfterReturn()
{
	EMButton->SetRelativeLocation(SCButtonMoveRail->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::Local));

	CLButton->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	bIsHitting = 0;
}
