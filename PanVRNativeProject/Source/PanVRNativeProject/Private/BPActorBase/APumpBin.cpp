// Fill out your copyright notice in the Description page of Project Settings.


#include "BPActorBase/APumpBin.h"
#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"
#include "CoreObj/VREquipmentWorldSubsystem.h"

AAPumpBin::AAPumpBin()
{
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_BinBody(TEXT("/Game/VRContent/Modeling/Pump/SMPumpBin.SMPumpBin"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_DeloreanLever(TEXT("/Game/VRContent/Modeling/Pump/SMDeLoreanLever.SMDeLoreanLever"));
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialFinder_Main(TEXT("/Game/VRContent/Material/SRS_STAGE_Main.SRS_STAGE_Main"));

	if (ActorBaseMesh && ModelingFinder_BinBody.Succeeded())
	{
		ActorBaseMesh->SetStaticMesh(ModelingFinder_BinBody.Object);
	}

	SMDeLoreanLever = CreateDefaultSubobject<UStaticMeshComponent>("LeverSMComp");
	if (SMDeLoreanLever && ModelingFinder_DeloreanLever.Succeeded())
	{
		SMDeLoreanLever->SetupAttachment(ActorBaseMesh);
		SMDeLoreanLever->SetStaticMesh(ModelingFinder_DeloreanLever.Object);
		SMDeLoreanLever->SetRelativeLocation(FVector(0.f, 16.f, 0.f));
	}

	if (GC)
	{
		GC->SetupAttachment(SMDeLoreanLever);
		GC->SetRelativeLocation(FVector(0.f, 3.f, 3.f));
		GC->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));
		GC->EEGrabType = EGrabType::HandToObj;
	}

	SCMoveStandard = CreateDefaultSubobject<USceneComponent>("MoveStandardSceneComp");
	if (SCMoveStandard)
	{
		SCMoveStandard->SetupAttachment(SMDeLoreanLever);
		SCMoveStandard->SetRelativeLocation(FVector(0.f, 0.f, 3.f));
	}

	CLRemover = CreateDefaultSubobject<UBoxComponent>("BoxCollisionComp");
	if (CLRemover)
	{
		CLRemover->SetupAttachment(ActorBaseMesh);
		CLRemover->SetRelativeLocation(FVector(-24.0f, 0.f, 3.f));
		CLRemover->SetRelativeRotation(FRotator(17.5f, 0.f, 0.f));
		CLRemover->SetBoxExtent(FVector(3.f, 9.f, 5.f));
		CLRemover->SetHiddenInGame(false); // Debug
	}

	TRTrashNum = CreateDefaultSubobject<UTextRenderComponent>("TRComp");
	if (TRTrashNum)
	{
		TRTrashNum->SetupAttachment(ActorBaseMesh);
		TRTrashNum->SetRelativeLocation(FVector(-0.78f, -24.5f, 4.9f));
		TRTrashNum->SetRelativeRotation(FRotator(21.f, 68.f, 0.f));

		TRTrashNum->SetTextRenderColor(FColor::Red);
		TRTrashNum->SetWorldSize(10.f);
		TRTrashNum->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
		TRTrashNum->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);
	}

	if (MaterialFinder_Main.Succeeded())
	{
		ActorBaseMesh->SetMaterial(0, MaterialFinder_Main.Object);
		SMDeLoreanLever->SetMaterial(0, MaterialFinder_Main.Object);
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

void AAPumpBin::BeginPlay()
{
	Super::BeginPlay();
	this->EquipmentRegistrable(this);
}

void AAPumpBin::Tick(float DeltaTimes)
{
	Super::Tick(DeltaTimes);
}

void AAPumpBin::EquipmentRegistrable(AActor* InActor)
{
	Super::EquipmentRegistrable(InActor);
}

void AAPumpBin::OnGrabbed(UMotionControllerComponent& InMCRef, const FVector& HandGrabPos, AVRHand* InGrabbingHand)
{
}

void AAPumpBin::OnDropped()
{
}

