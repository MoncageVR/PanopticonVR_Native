// Fill out your copyright notice in the Description page of Project Settings.


#include "BPActorBase/AFuelRodStorage.h"
#include "Components/BoxComponent.h"
#include "BPActorBase/AFuelRod.h"

AAFuelRodStorage::AAFuelRodStorage()
{
	PrimaryActorTick.bCanEverTick = true;

	if (GC)
	{
		GC->EEGrabType = EGrabType::SpawnObj;
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_Body(TEXT("/Game/VRContent/Modeling/FuelRelated/FuelStorage.FuelStorage"));
	if (ActorBaseMesh && ModelingFinder_Body.Succeeded())
	{
		ActorBaseMesh->SetStaticMesh(ModelingFinder_Body.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialFinder_Main(TEXT("/Game/VRContent/Material/SRS_STAGE_Main.SRS_STAGE_Main"));
	if (MaterialFinder_Main.Succeeded())
	{
		ActorBaseMesh->SetMaterial(0, MaterialFinder_Main.Object);
	}

	FRCLStorage = CreateDefaultSubobject<UBoxComponent>("CL_SpawnFR");
	if (FRCLStorage)
	{
		FRCLStorage->SetupAttachment(ActorBaseMesh);
		FRCLStorage->SetRelativeLocation(FVector(0.f, 0.f, 8.3f));
		FRCLStorage->SetBoxExtent(FVector(16.f, 21.f, 8.f));
		FRCLStorage->SetHiddenInGame(false); // Debug
	}
}

void AAFuelRodStorage::BeginPlay()
{
	Super::BeginPlay();
}

void AAFuelRodStorage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAFuelRodStorage::OnGrabbed(UMotionControllerComponent& InMCRef, const FVector& HandGrabPos, class AVRHand* InGrabbingHand)
{
	UE_LOG(LogTemp, Log, TEXT("FuelRod Spawn Part!"));

	UWorld* TempMyWorld = GetWorld();
	FTransform TempSpawnTransform(FTransform::Identity); // Default Spawn Trnasform

	FRotator TempAttachRot = FRotator(-100.f, 0.f, -5.f);

	if (GC->MCRef == &InMCRef)
	{
		if (GC->EEGrabType == EGrabType::SpawnObj)
		{
			SpawnNewFR = TempMyWorld->SpawnActor<AAFuelRod>(AAFuelRod::StaticClass(), TempSpawnTransform);

			if (SpawnNewFR != nullptr)
			{
				SpawnNewFR->GC->SetPrimitiveCompPhysics(false);
				SpawnNewFR->AttachToComponent(&InMCRef, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
				SpawnNewFR->OnGrabbed(InMCRef, HandGrabPos, InGrabbingHand);

				if (GC->MCRef->ComponentHasTag(FName("RightMC")))
				{
					SpawnNewFR->GetRootComponent()->SetRelativeLocation(SpawnNewFR->GetRootComponent()->GetRelativeLocation() - FVector(0.f, 2.5f, 0.f));
					SpawnNewFR->GetRootComponent()->SetRelativeRotation(TempAttachRot);
				}
				else
				{
					SpawnNewFR->GetRootComponent()->SetRelativeLocation(SpawnNewFR->GetRootComponent()->GetRelativeLocation() + FVector(0.f, 2.5f, 0.f));
					SpawnNewFR->GetRootComponent()->SetRelativeRotation(TempAttachRot);
				}
			}
		}
	}
}

void AAFuelRodStorage::OnDropped()
{
	if (GC->MCRef)
	{
		GC->MCRef = nullptr;
	}

	// Spawn FuelRod Exist, FuelRod OnDropped Fucntion Call And Other Function Call
	if (SpawnNewFR != nullptr)
	{
		if (!SpawnNewFR->GetFRIsAttaching())
		{
			SpawnNewFR->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform); // Detach SodaBullet
			SpawnNewFR->GC->SetPrimitiveCompPhysics(true); // SodaBullet Physics Turn On
			SpawnNewFR->OnDropped();
			SpawnNewFR = nullptr;
		}
		else
		{
			SpawnNewFR = nullptr;
			return;
		}
	}
}