// Fill out your copyright notice in the Description page of Project Settings.


#include "VRPawn/Hand/VRHand.h"
#include "MotionControllerComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Core/Component/UGrabComp.h"

AVRHand::AVRHand()
{
	PrimaryActorTick.bCanEverTick = true;

	// MotionController Component Create And Setting RootComponent
	MC = CreateDefaultSubobject<UMotionControllerComponent>("MotionController");
	SetRootComponent(MC);
	MC->SetCollisionProfileName("OverlapAll");

	// SkeletalMesh Component Create And Attached RootComponent(MC)
	HandMesh = CreateDefaultSubobject<USkeletalMeshComponent>("HandMesh");
	HandMesh->SetupAttachment(MC);
	HandMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 7.0f));

	// Sphere Collision Component Create And Setting Default Data
	CLGrabSphere = CreateDefaultSubobject<USphereComponent>("CLGrabSphere");
	CLGrabSphere->SetupAttachment(HandMesh);
	CLGrabSphere->SetSphereRadius(6.0f);
	CLGrabSphere->SetRelativeLocation(FVector(0.0f, 7.0f, -2.0f));
	//CLGrabSphere->SetVisibility(true); // Debug;
	//CLGrabSphere->SetHiddenInGame(false); // Debug;

	CLBox = CreateDefaultSubobject<UBoxComponent>("CLOverlapBox");
	CLBox->SetupAttachment(HandMesh);
	CLBox->SetRelativeLocation(FVector(0.0f, 7.0f, -1.5f));
	CLBox->SetRelativeRotation(FRotator(0.0f, 0.0f, 90.0f));
	CLBox->SetBoxExtent(FVector(5.0f, 3.0f, 2.5f));
	CLBox->SetVisibility(true); // Debug;
	CLBox->SetHiddenInGame(false); // Debug;
	CLBox->SetGenerateOverlapEvents(true);

	CLCapsuleIndex = CreateDefaultSubobject<UCapsuleComponent>("HandIndex_Collision");
	CLCapsuleIndex->SetupAttachment(HandMesh);
	CLCapsuleIndex->SetRelativeLocation(FVector(1.5f, 0.0f, 0.0f));
	CLCapsuleIndex->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
	CLCapsuleIndex->SetCapsuleRadius(1.1f);
	CLCapsuleIndex->SetCapsuleHalfHeight(2.2f);
	CLCapsuleIndex->ComponentTags.Add(FName("HandIndex"));
	CLCapsuleIndex->SetHiddenInGame(false); // Debug;
}

void AVRHand::OnConstruction(const FTransform& InTransform)
{
	Super::OnConstruction(InTransform);

	switch (HandType)
	{
	case EControllerHand::Left:
		MC->MotionSource = "Left";
		MC->ComponentTags.Add(FName("LeftMC"));
		CLBox->ComponentTags.Add(FName("HandCLBox"));
		break;
	case EControllerHand::Right:
		MC->MotionSource = "Right";
		MC->ComponentTags.Add(FName("RightMC"));
		CLBox->ComponentTags.Add(FName("HandCLBox"));
		break;
	default:
		break;
	}
}

void AVRHand::BeginPlay()
{
	Super::BeginPlay();

	switch (HandType)
	{
	case EControllerHand::Left:
		AtFirstLeftHandTransform = HandMesh->GetRelativeTransform();
		CLCapsuleIndex->AttachToComponent(HandMesh, FAttachmentTransformRules::KeepRelativeTransform, TEXT("L_Index003"));
		break;
	case EControllerHand::Right:
		AtFirstRightHandTransform = HandMesh->GetRelativeTransform();
		CLCapsuleIndex->AttachToComponent(HandMesh, FAttachmentTransformRules::KeepRelativeTransform, TEXT("R_Index003"));
		break;
	default:
		break;
	}
}

void AVRHand::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AVRHand::GrabObject()
{
	TArray<AActor*> OverlappingActors;
	CLGrabSphere->GetOverlappingActors(OverlappingActors);

	const FVector CurrentHandPos = HandMesh->GetComponentLocation(); // Current Hand Mesh Location Variable
	float NearestRelativeDis = TNumericLimits<float>::Max();

	if (!OverlappingActors.IsEmpty()) // Check OverlappingActors Not Empty
	{
		for (AActor* TempOverlpaiingActors : OverlappingActors)
		{
			if (!TempOverlpaiingActors) continue; // Check TempOverlappingActors Not Empty

			TArray<UUGrabComp*> TempGCs; // TempOverlappingActors in All GC Array
			TempOverlpaiingActors->GetComponents<UUGrabComp>(TempGCs);

			if (TempGCs.Num() == 0) continue; // Check TempGCs Not Empty

			for (UUGrabComp* TempGC : TempGCs)
			{
				if (!TempGC) continue; // Check TempGC Not Empty

				const float CalcDistsq = FVector::DistSquared(TempGC->GetComponentLocation(), CurrentHandPos);
				if (CalcDistsq < NearestRelativeDis)
				{
					NearestRelativeDis = CalcDistsq;
					GrabbleActorGC = TempGC;
				}
			}
		}

		// Finally HandMesh Nearest GC
		if (GrabbleActorGC)
		{
			//UE_LOG(LogTemp, Log, TEXT("First Overlap GC Name : %s"), *GrabbleActorGC->GetName()); // Debug
			GrabbleActorGC->GCTryGrab(MC, HandMesh, this);
		}
	}

	return;
}

void AVRHand::ReleaseObject()
{
	if (GrabbleActorGC)
	{
		GrabbleActorGC->GCTryRelease();

		if (MC != Cast<UMotionControllerComponent>(HandMesh->GetAttachParent()))
		{
			AfterReleaseResetHandMesh();
		}
	}

	GrabbleActorGC = nullptr;

	return;
}

void AVRHand::AfterReleaseResetHandMesh()
{
	if (HandType == EControllerHand::Left)
	{
		HandMesh->AttachToComponent(MC, FAttachmentTransformRules::KeepRelativeTransform);
		HandMesh->SetRelativeTransform(AtFirstLeftHandTransform);
		CLGrabSphere->AttachToComponent(HandMesh, FAttachmentTransformRules::KeepRelativeTransform);
	}
	else
	{
		HandMesh->AttachToComponent(MC, FAttachmentTransformRules::KeepRelativeTransform);
		HandMesh->SetRelativeTransform(AtFirstRightHandTransform);
		CLGrabSphere->AttachToComponent(HandMesh, FAttachmentTransformRules::KeepRelativeTransform);
	}
}

UCapsuleComponent* AVRHand::GetCLCapsuleIndex() const
{
	return CLCapsuleIndex;
}
