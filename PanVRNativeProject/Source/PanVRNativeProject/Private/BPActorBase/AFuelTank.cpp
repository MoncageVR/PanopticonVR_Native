// Fill out your copyright notice in the Description page of Project Settings.


#include "BPActorBase/AFuelTank.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "MotionControllerComponent.h"
#include "Components/SplineComponent.h"
#include "BPActorBase/AFuelRod.h"

AAFuelTank::AAFuelTank()
{
	PrimaryActorTick.bCanEverTick = false;

	FTMainRoot = CreateDefaultSubobject<USceneComponent>("MainSceneComp");
	if (FTMainRoot)
	{
		SetRootComponent(FTMainRoot);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_CellCase(TEXT("/Game/VRContent/Modeling/FuelRelated/CellCaseBelow.CellCaseBelow"));
	if (ActorBaseMesh && ModelingFinder_CellCase.Succeeded())
	{
		ActorBaseMesh->SetupAttachment(FTMainRoot);
		ActorBaseMesh->SetStaticMesh(ModelingFinder_CellCase.Object);
		ActorBaseMesh->SetRelativeScale3D(FVector(0.8f, 0.8f, 0.8f));
	}

	FTMeshRoot = CreateDefaultSubobject<USceneComponent>("MeshSceneComp");
	if (FTMeshRoot)
	{
		FTMeshRoot->SetupAttachment(FTMainRoot);
		FTMeshRoot->SetRelativeScale3D(FVector(0.8f, 0.8f, 0.8f));
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_MainBody(TEXT("/Game/VRContent/Modeling/FuelRelated/CellCase.CellCase"));
	FTMainBody = CreateDefaultSubobject<UStaticMeshComponent>("FuelTankMainBody");
	if (FTMainBody && ModelingFinder_MainBody.Succeeded())
	{
		FTMainBody->SetupAttachment(FTMeshRoot);
		FTMainBody->SetStaticMesh(ModelingFinder_MainBody.Object);
		FTMainBody->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));
	}

	FTAttachRoot = CreateDefaultSubobject<USceneComponent>("AttachSceneComp");
	if (FTAttachRoot)
	{
		FTAttachRoot->SetupAttachment(FTMainBody);
		FTAttachRoot->SetRelativeLocation(FVector(0.f, -5.5f, 0.5f));
	}

	if (GC)
	{
		GC->SetupAttachment(FTMainBody);
		GC->SetRelativeLocation(FVector(0.f, -7.f, 0.f));
		GC->EEGrabType = EGrabType::HandToObj;
	}

	FTCLRod = CreateDefaultSubobject<UBoxComponent>("FuelTank_Col");
	if (FTCLRod)
	{
		FTCLRod->SetupAttachment(FTMainBody);
		FTCLRod->SetRelativeLocation(FVector(0.f, 0.f, -43.5f));
		FTCLRod->SetBoxExtent(FVector(1.5f, 1.5f, 12.7f));
		FTCLRod->SetHiddenInGame(false); // Debug
	}

	FTCLGrab = CreateDefaultSubobject<UCapsuleComponent>("FuelTankGrab_Col");
	if (FTCLGrab)
	{
		FTCLGrab->SetupAttachment(FTMainBody);
		FTCLGrab->SetRelativeRotation(FRotator(90.0f, 0.f, 0.f));
		FTCLGrab->SetCapsuleRadius(2.0f);
		FTCLGrab->SetCapsuleHalfHeight(8.0f);
		FTCLGrab->SetHiddenInGame(false); // Debug
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialFinder_Main(TEXT("/Game/VRContent/Material/SRS_STAGE_Main.SRS_STAGE_Main"));
	if (MaterialFinder_Main.Succeeded())
	{
		ActorBaseMesh->SetMaterial(0, MaterialFinder_Main.Object);
		FTMainBody->SetMaterial(0, MaterialFinder_Main.Object);
	}

	FTMoveRoute = CreateDefaultSubobject<USplineComponent>("SplineComp");
	if (FTMoveRoute)
	{
		FTMoveRoute->SetupAttachment(FTMainRoot);
		FTMoveRoute->SetLocationAtSplinePoint(0, FVector(0.f, 0.f, 0.f), ESplineCoordinateSpace::Local, true);
		FTMoveRoute->SetRotationAtSplinePoint(0, FRotator(0.f, 0.f, 0.f), ESplineCoordinateSpace::Local, true);
		FTMoveRoute->SetTangentsAtSplinePoint(0, FVector(0.0f, 0.0f, 0.0f), FVector(0.0f, 0.0f, 0.0f), ESplineCoordinateSpace::Local, true);


		FTMoveRoute->SetLocationAtSplinePoint(1, FVector(0.f, 0.f, 50.f), ESplineCoordinateSpace::Local, true);
		FTMoveRoute->SetRotationAtSplinePoint(1, FRotator(0.f, 180.0f, 0.f), ESplineCoordinateSpace::Local, true);
		FTMoveRoute->SetTangentsAtSplinePoint(1, FVector(0.0f, 0.0f, 0.0f), FVector(0.0f, 0.0f, 0.0f), ESplineCoordinateSpace::Local, true);
	}

	FTCLGrab->OnComponentBeginOverlap.AddDynamic(this, &AAFuelTank::GrabColOverlapBegin);
	FTCLGrab->OnComponentEndOverlap.AddDynamic(this, &AAFuelTank::GrabColOverlapEnd);

	FTCLRod->OnComponentBeginOverlap.AddDynamic(this, &AAFuelTank::FuelRodColOverlapBegin);
	FTCLRod->OnComponentEndOverlap.AddDynamic(this, &AAFuelTank::FuelRodColOverlapEnd);

	static ConstructorHelpers::FObjectFinder<USoundBase> SoundFinder_FuelTankMove(TEXT("/Game/VRContent/Sound/Wavs/FuelRelated/sfx_radiation_move.sfx_radiation_move"));
	static ConstructorHelpers::FObjectFinder<USoundBase> SoundFinder_FuelRodIn(TEXT("/Game/VRContent/Sound/Wavs/FuelRelated/sfx_radiation_in.sfx_radiation_in"));
	static ConstructorHelpers::FObjectFinder<USoundBase> SoundFinder_FuelRodOut(TEXT("/Game/VRContent/Sound/Wavs/FuelRelated/sfx_radiation_out.sfx_radiation_out"));

	if (SoundFinder_FuelTankMove.Succeeded() && SoundFinder_FuelRodIn.Succeeded() && SoundFinder_FuelRodOut.Succeeded())
	{
		SFXFuelTankMove = SoundFinder_FuelTankMove.Object;
		SFXFuelRodIn = SoundFinder_FuelRodIn.Object;
		SFXFuelRodOut = SoundFinder_FuelRodOut.Object;
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

void AAFuelTank::BeginPlay()
{
	Super::BeginPlay();

	this->EquipmentRegistrable(this);

	// Debug
	EquipFuelRod = GetWorld()->SpawnActor<AAFuelRod>(AAFuelRod::StaticClass(), this->GetRootComponent()->GetComponentTransform());
	EquipFuelRod->SetFRIsAttaching(false);
	EquipFuelRod->HandleGaugeOperation(true);
	// Debug
}

void AAFuelTank::Tick(float DeltaTimes)
{
	Super::Tick(DeltaTimes);
}

void AAFuelTank::OnGrabbed(UMotionControllerComponent& InMCRef, const FVector& HandGrabPos, class AVRHand* InGrabbingHand)
{
	if (bIsHanding)
	{
		TempMCRef = &InMCRef;
		//mSoundPlayer->PlaySoundEffect(this, SFXFuelTankMove, FTMeshRoot->GetComponentLocation());
		GetWorldTimerManager().SetTimer(
			FuelTankMoveTimer,
			this,
			&AAFuelTank::MoveOperateFuelTank,
			0.01f,
			true
		);
	}
}

void AAFuelTank::OnDropped()
{
	TempMCRef = nullptr;
	GetWorldTimerManager().PauseTimer(FuelTankMoveTimer);
	GetWorldTimerManager().ClearTimer(FuelTankMoveTimer);
}

void AAFuelTank::EquipmentRegistrable(AActor* InActor)
{
	Super::EquipmentRegistrable(InActor);
}

void AAFuelTank::GrabColOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->ComponentHasTag(FName("HandCLBox")))
	{
		bIsHanding = true;
	}
}

void AAFuelTank::GrabColOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherComp->ComponentHasTag(FName("HandCLBox")))
	{
		bIsHanding = false;
	}
}

void AAFuelTank::FuelRodColOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->ComponentHasTag(FName("FuelRod")))
	{
		if (bIsOpen && !bIsAttachObjExist)
		{
			EquipFuelRod = Cast<AAFuelRod>(OtherActor);
			if (EquipFuelRod)
			{
				mSoundPlayer->PlaySoundEffect(this, SFXFuelRodIn, FTCLRod->GetComponentLocation());
				EquipFuelRod->SetFRIsAttaching(1);
				EquipFuelRod->OnDropped();
				EquipFuelRod->GetRootComponent()->AttachToComponent(FTCLRod, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

				EquipFuelRod->GetRootComponent()->SetRelativeScale3D(FVector(1.f, 1.f, 1.f));

				EquipFuelRod->GetRootComponent()->SetRelativeLocation(FVector(0.f, 0.f, -11.75f));
				bIsAttachObjExist = true;

				EquipFuelRod->HandleGaugeOperation(true);
			}
			else
			{
				return;
			}
		}
	}

	
}

void AAFuelTank::FuelRodColOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (EquipFuelRod == Cast<AAFuelRod>(OtherActor) && OtherComp->ComponentHasTag(FName("FuelRod")))
	{
		mSoundPlayer->PlaySoundEffect(this, SFXFuelRodOut, FTCLRod->GetComponentLocation());
		bIsAttachObjExist = false;
		EquipFuelRod->SetFRIsAttaching(0);
		EquipFuelRod->HandleGaugeOperation(false);
		EquipFuelRod = nullptr;
	}
}

void AAFuelTank::MoveOperateFuelTank()
{
	FVector TempVec(
		FTMeshRoot->GetRelativeLocation().X, 
		FTMeshRoot->GetRelativeLocation().Y, 
		FTMeshRoot->GetRelativeLocation().Z + TempMCRef->GetComponentTransform().GetRelativeTransform(FTMeshRoot->GetComponentTransform()).GetLocation().Z
	);

	float FinalTempZ = FMath::Clamp(
		TempVec.Z, 
		FTMoveRoute->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::Local).Z, 
		FTMoveRoute->GetLocationAtSplinePoint(1, ESplineCoordinateSpace::Local).Z
	);

	FRotator FinalTempRot(0.f, 
		FMath::GetMappedRangeValueClamped(
		FVector2D(0.0f, 50.0f),
		FVector2D(0.0f, 180.0f),
		FinalTempZ
		),
		0.f
	);

	FTMeshRoot->SetRelativeLocation(FVector(TempVec.X, TempVec.Y, FinalTempZ));
	FTMeshRoot->SetRelativeRotation(FinalTempRot);

	uint32 bWasOpen = 1;

	if (FTMeshRoot->GetRelativeLocation().Z >= 25.0f)
	{
		bWasOpen = bIsOpen;
		bIsOpen = 1;
	}
	else
	{
		bWasOpen = bIsOpen;
		bIsOpen = 0;
	}

	if (bWasOpen != bIsOpen)
	{
		mSoundPlayer->PlaySoundEffect(this, SFXFuelTankMove, FTMeshRoot->GetComponentLocation());
	}
}
