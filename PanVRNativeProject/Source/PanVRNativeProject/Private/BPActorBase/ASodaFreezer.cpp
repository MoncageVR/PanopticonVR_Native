// Fill out your copyright notice in the Description page of Project Settings.


#include "BPActorBase/ASodaFreezer.h"
#include "Components/BoxComponent.h"
#include "Core/Component/UGrabComp.h"
#include "MotionControllerComponent.h"
#include "BPActorBase/ASodaBullet.h"

AASodaFreezer::AASodaFreezer()
{
	PrimaryActorTick.bCanEverTick = false;

	// Setting Main Root
	FreezerMainRoot = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	SetRootComponent(FreezerMainRoot);
	FreezerMainRoot->SetRelativeScale3D(FVector(0.8f, 0.8f, 0.8f));

	// Setting Freezer Main Collision
	CL_MainFreezer = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	CL_MainFreezer->SetupAttachment(ActorBaseMesh);
	CL_MainFreezer->SetRelativeLocation(FVector(0.0f, 4.0f, 40.0f));
	CL_MainFreezer->SetBoxExtent(FVector(20.0f, 12.0f, 22.0f));

	// Setting Freezer Body Mesh
	ActorBaseMesh->SetupAttachment(FreezerMainRoot);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Finder_FreezerBodyModeling(TEXT("/Game/VRContent/Modeling/SodaFreezer/SodaFreezerBody.SodaFreezerBody"));
	if (Finder_FreezerBodyModeling.Succeeded())
	{
		ActorBaseMesh->SetStaticMesh(Finder_FreezerBodyModeling.Object);
	}

	// Setting GC
	GC->SetupAttachment(ActorBaseMesh);
	GC->SetRelativeLocation(FVector(0.0f, 4.0f, 40.0f));
	GC->EEGrabType = EGrabType::SpawnObj;

	// Setting Upward Door Related
	UpwardFreezerDoorMesh = CreateDefaultSubobject<UStaticMeshComponent>("SM_Door1");
	UpwardFreezerDoorMesh->SetupAttachment(FreezerMainRoot);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Finder_FreezerUpDoorModeling(TEXT("/Game/VRContent/Modeling/SodaFreezer/SodaFreezerUpDoor.SodaFreezerUpDoor"));
	if (Finder_FreezerUpDoorModeling.Succeeded())
	{
		UpwardFreezerDoorMesh->SetStaticMesh(Finder_FreezerUpDoorModeling.Object);
	}
	
	UpwardFreezerDoorMesh->SetRelativeLocation(FVector(23.0f, 1.2f, 76.1f));
	UpwardFreezerDoorMesh->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));

	UpwardDoorGC = CreateDefaultSubobject<UUGrabComp>("GC_Door1");
	UpwardDoorGC->SetupAttachment(UpwardFreezerDoorMesh);
	UpwardDoorGC->EEGrabType = EGrabType::AutomaticObj;
	UpwardDoorGC->SetWorldLocation(FVector(18.5f, 36.0f, 25.0f));

	// Setting Downward Door Related
	DownwardFreezerDoorMesh = CreateDefaultSubobject<UStaticMeshComponent>("SM_Door2");
	DownwardFreezerDoorMesh->SetupAttachment(FreezerMainRoot);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Finder_FreezerDownDoorModeling(TEXT("/Game/VRContent/Modeling/SodaFreezer/SodaFreezerDownDoor.SodaFreezerDownDoor"));
	if (Finder_FreezerDownDoorModeling.Succeeded())
	{
		DownwardFreezerDoorMesh->SetStaticMesh(Finder_FreezerDownDoorModeling.Object);
	}

	DownwardFreezerDoorMesh->SetRelativeLocation(FVector(26.0f, 16.2f, 13.3f));
	DownwardFreezerDoorMesh->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
	// Debug : Default Value FRotator(0.0f, 90.0f, 0.0f)
	
	DownwardDoorGC = CreateDefaultSubobject<UUGrabComp>("GC_Door2");
	DownwardDoorGC->SetupAttachment(DownwardFreezerDoorMesh);
	DownwardDoorGC->EEGrabType = EGrabType::AutomaticObj;
	DownwardDoorGC->SetRelativeLocation(FVector(15.0f, 41.0f, 30.0f));

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialFinder_Main(TEXT("/Game/VRContent/Material/SRS_STAGE_Main.SRS_STAGE_Main"));
	if (MaterialFinder_Main.Succeeded())
	{
		ActorBaseMesh->SetMaterial(1, MaterialFinder_Main.Object);
		UpwardFreezerDoorMesh->SetMaterial(0, MaterialFinder_Main.Object);
		DownwardFreezerDoorMesh->SetMaterial(0, MaterialFinder_Main.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialFinder_SodaBullet(TEXT("/Game/VRContent/Material/SRS_Stage_Sodagun_Bullet.SRS_Stage_Sodagun_Bullet"));
	if (MaterialFinder_SodaBullet.Succeeded())
	{
		ActorBaseMesh->SetMaterial(0, MaterialFinder_SodaBullet.Object);
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> SoundFinder_Door(TEXT("/Game/VRContent/Sound/Wavs/SodaGunRelated/sfx_frg_door.sfx_frg_door"));
	if (SoundFinder_Door.Succeeded())
	{
		SFXDoor = SoundFinder_Door.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> SoundFinder_PickUp(TEXT("/Game/VRContent/Sound/Wavs/SodaGunRelated/sfx_frg_pickup.sfx_frg_pickup"));
	if (SoundFinder_PickUp.Succeeded())
	{
		SFXCanPickUp = SoundFinder_PickUp.Object;
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

void AASodaFreezer::BeginPlay()
{
	Super::BeginPlay();
}

void AASodaFreezer::Tick(float DeltaTime) 
{
	Super::Tick(DeltaTime);
}

void AASodaFreezer::OnGrabbed(UMotionControllerComponent& InMCRef, const FVector& HandGrabPos, class AVRHand* InGrabbingHand)
{
	const float CurrentYaw = 90.0f; // Rotation Of Door Close State
	const float TargetYaw = CurrentYaw - 170.0f; // Rotation Of Door Open State

	UWorld* TempMyWorld = GetWorld(); // Current World
	FTransform TempSpawnTransform(FTransform::Identity); // Default Spawn Trnasform

	// Main GC Operating : SpawnObj GrabType
	// Spawn SodaBullet Part
	if (GC->MCRef  == &InMCRef)
	{
		switch (GC->EEGrabType)
		{
		case EGrabType::SpawnObj:
			// Actually Spawn SodaBullet Actor In World
			NewSodaBullet = TempMyWorld->SpawnActor<AASodaBullet>(AASodaBullet::StaticClass(), TempSpawnTransform);
			NewSodaBullet->GC->SetPrimitiveCompPhysics(false); // SodaBullet Physics Turn Off
			NewSodaBullet->AttachToComponent(&InMCRef, FAttachmentTransformRules::SnapToTargetNotIncludingScale); // SodaBullet Attahcing MC
			// (SodaBullet -> MC)
			NewSodaBullet->OnGrabbed(InMCRef, HandGrabPos, InGrabbingHand); // SodaBullet On Grabbed Function Call
			// SodaBullet Attached MC, After Setting SodaBullet Relative Location And Rotation
			NewSodaBullet->SetActorRelativeLocation(FVector(-5.0f, -5.0f, 2.0f));
			NewSodaBullet->SetActorRelativeRotation(FRotator(-110.0f, 0.0f, 0.0f));
			mSoundPlayer->PlaySoundEffect(this, SFXCanPickUp, CL_MainFreezer->GetComponentLocation());

			break;
		default:
			break;
		}
	}
	// UpwardDoor GC Operating : AutomaticObj GrabType
	// Open And Close Movement Part
	else if (UpwardDoorGC->MCRef == &InMCRef)
	{
		switch (UpwardDoorGC->EEGrabType)
		{
		case EGrabType::AutomaticObj:
			if (UpwardFreezerDoorMesh->GetRelativeRotation().Yaw < 0)
			{
				UpwardFreezerDoorMesh->SetRelativeRotation(FRotator(0.0f, CurrentYaw, 0.0f));
				mSoundPlayer->PlaySoundEffect(this, SFXDoor, UpwardFreezerDoorMesh->GetComponentLocation());
			}
			else
			{
				UpwardFreezerDoorMesh->SetRelativeRotation(FRotator(0.0f, TargetYaw, 0.0f));
				mSoundPlayer->PlaySoundEffect(this, SFXDoor, UpwardFreezerDoorMesh->GetComponentLocation());
			}
			break;
		default:
			break;
		}
	}
	// DownwardDoor GC Operating : AutomaticObj GrabType
	// Open And Close Movement Part
	else if(DownwardDoorGC->MCRef == &InMCRef)
	{
		switch (DownwardDoorGC->EEGrabType)
		{
		case EGrabType::AutomaticObj:
			if (DownwardFreezerDoorMesh->GetRelativeRotation().Yaw < 0)
			{
				DownwardFreezerDoorMesh->SetRelativeRotation(FRotator(0.0f, CurrentYaw, 0.0f));
				mSoundPlayer->PlaySoundEffect(this, SFXDoor, DownwardFreezerDoorMesh->GetComponentLocation());
			}
			else
			{
				DownwardFreezerDoorMesh->SetRelativeRotation(FRotator(0.0f, TargetYaw, 0.0f));
				mSoundPlayer->PlaySoundEffect(this, SFXDoor, DownwardFreezerDoorMesh->GetComponentLocation());
			}
			break;
		default:
			break;
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("GC Wrong Operating!"));
		return;
	}
}

void AASodaFreezer::OnDropped()
{
	// All Gc In MCRef Initialize nullptr
	if (GC->MCRef)
	{
		GC->MCRef = nullptr;
	}
	else if (DownwardDoorGC->MCRef)
	{
		DownwardDoorGC->MCRef = nullptr;
	}
	else if (UpwardDoorGC->MCRef)
	{
		UpwardDoorGC->MCRef = nullptr;
	}

	// Spawn SodaBullet Exist, SodaBullet OnDropped Fucntion  Call And Other Function Call
	if (NewSodaBullet != nullptr)
	{
		if (NewSodaBullet->GetSBIsAttaching())
		{
			NewSodaBullet = nullptr;
			return;
		}
		else
		{
			NewSodaBullet->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform); // Detach SodaBullet
			NewSodaBullet->GC->SetPrimitiveCompPhysics(true); // SodaBullet Physics Turn On
			NewSodaBullet->OnDropped();
			NewSodaBullet = nullptr;
		}
	}
}