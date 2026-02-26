// Fill out your copyright notice in the Description page of Project Settings.


#include "BPActorBase/ALighter.h"
#include "Components/CapsuleComponent.h"
#include "Components/AudioComponent.h"

AALighter::AALighter()
{
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_ZippoBody(TEXT("/Game/VRContent/Modeling/Lighter_Zippo/Zippo_Body.Zippo_Body"));
	if (ActorBaseMesh && ModelingFinder_ZippoBody.Succeeded())
	{
		ActorBaseMesh->SetStaticMesh(ModelingFinder_ZippoBody.Object);
		ActorBaseMesh->SetRelativeScale3D(FVector(.9f, .9f, .9f));
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_ZippoHead(TEXT("/Game/VRContent/Modeling/Lighter_Zippo/Zippo_Head.Zippo_Head"));
	LHead = CreateDefaultSubobject<UStaticMeshComponent>("SM_LighterHead");
	if (LHead && ModelingFinder_ZippoHead.Succeeded())
	{
		LHead->SetupAttachment(ActorBaseMesh);
		LHead->SetStaticMesh(ModelingFinder_ZippoHead.Object);
	}

	if (GC)
	{
		GC->SetupAttachment(ActorBaseMesh);
		GC->EEGrabType = EGrabType::ObjToHand;
	}

	CLFire = CreateDefaultSubobject<UCapsuleComponent>("FireCollisionComp");
	if (CLFire)
	{
		CLFire->SetupAttachment(ActorBaseMesh);
		CLFire->SetRelativeLocation(FVector(3.35f, 0.f, 2.f));
		CLFire->SetCapsuleRadius(1.f);
		CLFire->SetCapsuleHalfHeight(2.f);
		CLFire->ComponentTags.Add(FName("Fire"));
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialFinder_Main(TEXT("/Game/VRContent/Material/SRS_STAGE_Main.SRS_STAGE_Main"));
	if (MaterialFinder_Main.Succeeded())
	{
		ActorBaseMesh->SetMaterial(0, MaterialFinder_Main.Object);
		LHead->SetMaterial(0, MaterialFinder_Main.Object);
	}

	LSCAudioPlayer = CreateDefaultSubobject<UAudioComponent>("SC_AudioComp");
	if (LSCAudioPlayer)
	{
		LSCAudioPlayer->SetupAttachment(ActorBaseMesh);
		LSCAudioPlayer->SetAutoActivate(false);
		LSCAudioPlayer->bAllowSpatialization = false;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> SoundFinder_LighterSound(TEXT("/Game/VRContent/Sound/Ques/Lighter/sfx_lighter_fire_Cue.sfx_lighter_fire_Cue"));
	if (SoundFinder_LighterSound.Succeeded())
	{
		SCSfxLighterCues = SoundFinder_LighterSound.Object;
	}
}

void AALighter::BeginPlay()
{
	Super::BeginPlay();

	DefaultParentActor = GetAttachParentActor();
	DefaultTransform = FTransform(GetRootComponent()->GetRelativeRotation(), GetRootComponent()->GetRelativeLocation(),GetRootComponent()->GetRelativeScale3D());

	if (LSCAudioPlayer && SCSfxLighterCues)
	{
		LSCAudioPlayer->Stop();
		LSCAudioPlayer->SetSound(SCSfxLighterCues);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Audio Component Not Exist!"));
	}
}

void AALighter::Tick(float DeltaTimes)
{
	Super::Tick(DeltaTimes);
}

void AALighter::OnGrabbed(UMotionControllerComponent& InMCRef, const FVector& HandGrabPos, class AVRHand* InGrabbingHand)
{
	TempMCRef = &InMCRef;
	AdjustGrabTransform();
	LighterHeadMovement(1);
	LSCAudioPlayer->Play();
}

void AALighter::OnDropped()
{
	TempMCRef = nullptr;
	LighterHeadMovement(0);
	LSCAudioPlayer->Stop();

	if (DefaultParentActor)
	{
		AttachToActor(DefaultParentActor, FAttachmentTransformRules::KeepWorldTransform);
		this->GetRootComponent()->SetRelativeTransform(DefaultTransform);
	}
}

void AALighter::AdjustGrabTransform()
{
	if (TempMCRef->ComponentHasTag(FName("RightMC")))
	{
		FVector TempVec = FVector(
			ActorBaseMesh->GetRelativeLocation().X + 2.f,
			ActorBaseMesh->GetRelativeLocation().Y - 8.f,
			ActorBaseMesh->GetRelativeLocation().Z - 3.f
		);
		ActorBaseMesh->SetRelativeLocation(TempVec);

		FRotator TempRot = FRotator(180.f, 260.f, 70.f);
		ActorBaseMesh->SetRelativeRotation(TempRot);
	}
	else
	{
		FVector TempVec = FVector(
			ActorBaseMesh->GetRelativeLocation().X + 2.f,
			ActorBaseMesh->GetRelativeLocation().Y + 8.f,
			ActorBaseMesh->GetRelativeLocation().Z - 3.f
		);
		ActorBaseMesh->SetRelativeLocation(TempVec);

		FRotator TempRot = FRotator(0.f, 280.f, 110.f);
		ActorBaseMesh->SetRelativeRotation(TempRot);
	}
}

void AALighter::LighterHeadMovement(uint32 TempDirection)
{
	if (TempDirection)
	{
		LHead->SetRelativeRotation(FRotator(170.f, 0.f, 0.f));
	}
	else
	{
		LHead->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
	}
	return;
}
