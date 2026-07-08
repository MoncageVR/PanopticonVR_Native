// Fill out your copyright notice in the Description page of Project Settings.


#include "BPActorBase/AToilet.h"
#include "Components/CapsuleComponent.h"
#include "Core/Component/UGrabComp.h"
#include "VRPawn/Hand/VRHand.h"

AAToilet::AAToilet()
{
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_MainBody(TEXT("/Game/VRContent/Modeling/Toilet/ToiletMainBody.ToiletMainBody"));
	if (ModelingFinder_MainBody.Succeeded() && ActorBaseMesh)
	{
		ActorBaseMesh->SetStaticMesh(ModelingFinder_MainBody.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_RubberDuck(TEXT("/Game/VRContent/Modeling/Toilet/Rubberduck.Rubberduck"));
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialFinder_RubberDuck(TEXT("/Game/VRContent/Material/SRS_STAGE_duck.SRS_STAGE_duck"));

	TRubberDuck = CreateDefaultSubobject<UStaticMeshComponent>("SMCompRubberDuck");
	if (TRubberDuck)
	{
		TRubberDuck->SetupAttachment(ActorBaseMesh);
		TRubberDuck->SetRelativeLocation(FVector(9.f, -25.f, 38.f));
		if (ModelingFinder_RubberDuck.Succeeded() && MaterialFinder_RubberDuck.Succeeded())
		{
			TRubberDuck->SetStaticMesh(ModelingFinder_RubberDuck.Object);
			TRubberDuck->SetMaterial(0, MaterialFinder_RubberDuck.Object);
		}
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_Lever(TEXT("/Game/VRContent/Modeling/Toilet/ToiletLever.ToiletLever"));
	TLever = CreateDefaultSubobject<UStaticMeshComponent>("SMCompLever");
	if (TLever)
	{
		TLever->SetupAttachment(ActorBaseMesh);
		if (GC)
		{
			GC->SetupAttachment(TLever);
			GC->EEGrabType = EGrabType::HandToObj;
			GC->SetRelativeLocation(FVector(7.f, 0.f, 57.72f));
		}
		if (ModelingFinder_Lever.Succeeded())
		{
			TLever->SetStaticMesh(ModelingFinder_Lever.Object);
		}
	}

	TCLLever = CreateDefaultSubobject<UCapsuleComponent>("CLCompLever");
	if (TCLLever)
	{
		TCLLever->SetupAttachment(TLever);
		TCLLever->SetRelativeLocation(FVector(7.f, 0.f, 57.72f));
		TCLLever->SetCapsuleRadius(3.f);
		TCLLever->SetCapsuleHalfHeight(7.76f);
		TCLLever->SetHiddenInGame(false); // Debug
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_Door(TEXT("/Game/VRContent/Modeling/Toilet/ToiletDoor.ToiletDoor"));
	TOvenDoor = CreateDefaultSubobject<UStaticMeshComponent>("SMCompOvenDoor");
	if (TOvenDoor)
	{
		TOvenDoor->SetupAttachment(ActorBaseMesh);
		TOvenDoor->SetRelativeLocation(FVector(-18.f, -5.1f, 69.7f));
		if (ModelingFinder_Door.Succeeded())
		{
			TOvenDoor->SetStaticMesh(ModelingFinder_Door.Object);
		}
	}

	TCLOvenDoor = CreateDefaultSubobject<UCapsuleComponent>("CLCompOvenDoor");
	if (TCLOvenDoor)
	{
		TCLOvenDoor->SetupAttachment(TOvenDoor);
		TCLOvenDoor->SetRelativeLocation(FVector(8.f, -29.6f, 11.f));
		TCLOvenDoor->SetCapsuleRadius(1.53f);
		TCLOvenDoor->SetCapsuleHalfHeight(7.f);
		TCLOvenDoor->SetHiddenInGame(false); // Debug
	}

	OvenDoorGC = CreateDefaultSubobject<UUGrabComp>("OvenDoorGrabComp");
	if (OvenDoorGC)
	{
		OvenDoorGC->SetupAttachment(TOvenDoor);
		OvenDoorGC->SetRelativeLocation(FVector(8.f, -29.75f, 11.f));
		OvenDoorGC->SetRelativeRotation(FRotator(-90.f, -90.f, -180.f));
		OvenDoorGC->EEGrabType = EGrabType::HandToObj;
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialFinder_Main(TEXT("/Game/VRContent/Material/SRS_STAGE_Main.SRS_STAGE_Main"));
	if (MaterialFinder_Main.Succeeded())
	{
		ActorBaseMesh->SetMaterial(0, MaterialFinder_Main.Object);
		TLever->SetMaterial(0, MaterialFinder_Main.Object);
		TOvenDoor->SetMaterial(0, MaterialFinder_Main.Object);
	}

	TCLLever->OnComponentBeginOverlap.AddDynamic(this, &AAToilet::LeverGrabColOverlapBegin);
	TCLLever->OnComponentEndOverlap.AddDynamic(this, &AAToilet::LeverGrabColOverlapEnd);

	TCLOvenDoor->OnComponentBeginOverlap.AddDynamic(this, &AAToilet::OvenDoorGrabColOverlapBegin);
	TCLOvenDoor->OnComponentEndOverlap.AddDynamic(this, &AAToilet::OvenDoorGrabColOverlapEnd);

	static ConstructorHelpers::FObjectFinder<USoundBase> SoundFinder_GrabHeavy(TEXT("/Game/VRContent/Sound/Wavs/CommonSFX/sfx_grab_heavy.sfx_grab_heavy"));
	static ConstructorHelpers::FObjectFinder<USoundBase> SoundFinder_Flush(TEXT("/Game/VRContent/Sound/Wavs/Toilet/sfx_toilet_flush.sfx_toilet_flush"));
	if (SoundFinder_GrabHeavy.Succeeded() && SoundFinder_Flush.Succeeded())
	{
		SFXGrabHeavy = SoundFinder_GrabHeavy.Object;
		SFXOperateFlush = SoundFinder_Flush.Object;
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

void AAToilet::BeginPlay()
{
	Super::BeginPlay();
}

void AAToilet::Tick(float DeltaTimes)
{
	Super::Tick(DeltaTimes);
}

void AAToilet::OnGrabbed(UMotionControllerComponent& InMCRef, const FVector& HandGrabPos, class AVRHand* InGrabbingHand)
{

	TempGrabbingHandRed = InGrabbingHand;

	if (OvenDoorGC->MCRef == &InMCRef)
	{
		TempMCRef = &InMCRef;
		if (bIsOvenDoorHanding)
		{
			if (InMCRef.ComponentHasTag(FName("RightMC")))
			{
				AdjustOvenDoorGC(1);
			}
			else
			{
				AdjustOvenDoorGC(0);
			}

			GetWorldTimerManager().SetTimer(
				OvenDoorMovementTimer,
				this,
				&AAToilet::UpdateOvenDoorRotation,
				0.01f,
				true
			);
		}
	}

	if (GC->MCRef == &InMCRef)
	{
		TempMCRef = &InMCRef;
		if (bIsLeverHanding)
		{
			if (InMCRef.ComponentHasTag(FName("RightMC")))
			{
				AdjustLeverGC(1);
			}
			else
			{
				AdjustLeverGC(0);
			}
			mSoundPlayer->PlaySoundEffect(this, SFXGrabHeavy, TLever->GetComponentLocation()); // Play Grab Heavy Object Sound
			GetWorldTimerManager().SetTimer(
				LeverMovementTimer,
				this,
				&AAToilet::UpdateLeverRotation,
				0.01f,
				true
			);
		}
	}
}

void AAToilet::OnDropped()
{
	if (OvenDoorGC->MCRef == TempMCRef)
	{
		TempMCRef = nullptr;
		if (GetWorldTimerManager().IsTimerActive(OvenDoorMovementTimer))
		{
			GetWorldTimerManager().PauseTimer(OvenDoorMovementTimer);
			GetWorldTimerManager().ClearTimer(OvenDoorMovementTimer);
		}
	}

	if (GC->MCRef == TempMCRef)
	{
		TempMCRef = nullptr;
		if (GetWorldTimerManager().IsTimerActive(LeverMovementTimer))
		{
			GetWorldTimerManager().PauseTimer(LeverMovementTimer);
			GetWorldTimerManager().ClearTimer(LeverMovementTimer);
		}

		if (bIsLeverCoolDown)
		{
			UE_LOG(LogTemp, Log, TEXT("Lever CoolDown!"));

			FLatentActionInfo LatentInfo;
			LatentInfo.CallbackTarget = this;

			UKismetSystemLibrary::MoveComponentTo(
				TLever,
				FVector(0.f, 0.f, 0.f),
				FRotator(0.f, 0.f, 0.f),
				true,
				true,
				5.0f,
				false,
				EMoveComponentAction::Move,
				LatentInfo
			);
		}
		else
		{
			TLever->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
		}
	}

}

void AAToilet::LeverGrabColOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->ComponentHasTag(FName("HandCLBox")))
		bIsLeverHanding = 1;
}

void AAToilet::LeverGrabColOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherComp->ComponentHasTag(FName("HandCLBox")))
	{
		bIsLeverHanding = 0;
		GC->SetRelativeLocation(FVector(7.f, 0.f, 57.72f));
		GC->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
		if (GetWorldTimerManager().IsTimerActive(LeverMovementTimer))
		{
			GetWorldTimerManager().PauseTimer(LeverMovementTimer);
			GetWorldTimerManager().ClearTimer(LeverMovementTimer);
		}
	}
}

void AAToilet::OvenDoorGrabColOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->ComponentHasTag(FName("HandCLBox")))
		bIsOvenDoorHanding = 1;
}

void AAToilet::OvenDoorGrabColOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherComp->ComponentHasTag(FName("HandCLBox")))
	{
		bIsOvenDoorHanding = 0;
		OvenDoorGC->SetRelativeLocation(FVector(8.f, -29.75f, 11.f));
		OvenDoorGC->SetRelativeRotation(FRotator(-90.f, -90.f, -180.f));
		if (GetWorldTimerManager().IsTimerActive(OvenDoorMovementTimer))
		{
			GetWorldTimerManager().PauseTimer(OvenDoorMovementTimer);
			GetWorldTimerManager().ClearTimer(OvenDoorMovementTimer);
		}
	}
}

void AAToilet::AdjustOvenDoorGC(uint32 TempHandDir)
{
	// TempHandDir is 1 = RightHand
	// TempHandDir is 0 = LeftHand
	if (TempHandDir)
		OvenDoorGC->SetRelativeLocationAndRotation(FVector(14.f, -31.f, 11.f), FRotator(90.f, 0.f, -90.f), false, nullptr, ETeleportType::TeleportPhysics);
	else
		OvenDoorGC->SetRelativeLocationAndRotation(FVector(14.f, -28.f, 11.f), FRotator(-90.f, -90.f, -180.f), false, nullptr, ETeleportType::TeleportPhysics);
	return;
}

void AAToilet::UpdateOvenDoorRotation()
{
	FVector TempVec = (TCLOvenDoor->GetRelativeLocation()) - (TempMCRef->GetComponentTransform().GetRelativeTransform(TCLOvenDoor->GetComponentTransform()).GetLocation());

	TOvenDoor->SetRelativeRotation(
		FRotator(
		0.f, 
		FMath::Clamp(TOvenDoor->GetRelativeRotation().Yaw - TempVec.X, 0.0f, 120.f), 
		0.f
		)
	);
}

void AAToilet::AdjustLeverGC(uint32 TempHandDir)
{
	// TempHandDir is 1 = RightHand
	// TempHandDir is 0 = LeftHand
	if (TempHandDir)
		GC->SetRelativeLocationAndRotation(FVector(14.f, -2.f, 57.72f), FRotator(90.f, 0.f, -90.f), false, nullptr, ETeleportType::TeleportPhysics);
	else
		GC->SetRelativeLocationAndRotation(FVector(14.f, 2.f, 57.72f), FRotator(-90.f, 90.f, 0.f), false, nullptr, ETeleportType::TeleportPhysics);
	return;
}

void AAToilet::UpdateLeverRotation()
{
	FVector TempVec = (TCLLever->GetRelativeLocation()) - (TempMCRef->GetComponentTransform().GetRelativeTransform(TCLLever->GetComponentTransform()).GetLocation());

	TLever->SetRelativeRotation(
		FRotator(
			FMath::Clamp(TLever->GetRelativeRotation().Pitch + TempVec.X, 
				-70.0f, 
				20.0f), 
			0.f, 
			0.f
		)
	);

	if (TLever->GetRelativeRotation().Pitch <= -69.0f)
	{
		bIsLeverCoolDown = 1;
		mSoundPlayer->PlaySoundEffect(this, SFXOperateFlush, TRubberDuck->GetComponentLocation());
		WaterFlushing();
	}
}

void AAToilet::WaterFlushing()
{
	TempGrabbingHandRed->ReleaseObject();

	/*
	TowerRaid Subdue Logic Part

	ElevatorButton Object Stop Operate Logic Part

	*/
	
	GetWorldTimerManager().SetTimer(
		RubberDuckMovementTimer,
		this,
		&AAToilet::UpdateRubberDuck,
		0.01f,
		true
	);

	TLever->SetCollisionProfileName(FName("NoCollision"));
	TCLLever->SetCollisionProfileName(FName("NoCollision"));

	GetWorldTimerManager().SetTimer(
		RubberDuckMovementStopTimer,
		this,
		&AAToilet::StopRubberDuck,
		5.0f,
		false
	);
	
}

void AAToilet::UpdateRubberDuck() { TRubberDuck->AddLocalRotation(FRotator(0.f, 5.f, 0.f)); }

void AAToilet::StopRubberDuck()
{
	if (GetWorldTimerManager().IsTimerActive(RubberDuckMovementTimer))
	{
		GetWorldTimerManager().PauseTimer(RubberDuckMovementTimer);
		GetWorldTimerManager().ClearTimer(RubberDuckMovementTimer);
	}

	TLever->SetCollisionProfileName(FName("BlockAllDynamic"));
	TCLLever->SetCollisionProfileName(FName("OverlapAllDynamic"));

	TRubberDuck->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));


	/*
	
	ElevatorButton Object Continue Operate Logic Part

	*/

	bIsLeverCoolDown = 0;
}
