#include "EquipmentActor/Spawned_Actors/ATape.h"
#include "Components/BoxComponent.h"

AATape::AATape()
{
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_TapeBody(TEXT("/Game/VRContent/Modeling/14_Lobby/SM_Tape.SM_Tape"));
	if (ModelingFinder_TapeBody.Succeeded())
	{
		ActorBaseMesh->SetStaticMesh(ModelingFinder_TapeBody.Object);
		ActorBaseMesh->SetRelativeScale3D(FVector(0.8f));
		ActorBaseMesh->ComponentTags.Add(FName("Tape"));
		ActorBaseMesh->SetCollisionProfileName(FName("PhysicsActor"));
	}

	TapeType = 0;
	TapeNum = 0;

	TapeMats.Empty();
	TapeMats.Reserve(3);

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MatFinder_Default(TEXT("/Game/VRContent/Material/SRS_Lobby_Tape_Tutorial_02.SRS_Lobby_Tape_Tutorial_02"));
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MatFinder_Tuto3(TEXT("/Game/VRContent/Material/SRS_Lobby_Tape_Tutorial_03.SRS_Lobby_Tape_Tutorial_03"));
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MatFinder_Main1(TEXT("/Game/VRContent/Material/SRS_Lobby_Tape_Stage01.SRS_Lobby_Tape_Stage01"));
	if (MatFinder_Default.Succeeded() && MatFinder_Tuto3.Succeeded() && MatFinder_Main1.Succeeded())
	{
		TapeMats.Add(MatFinder_Default.Object); // Tuto 2 Material
		TapeMats.Add(MatFinder_Tuto3.Object);   // Tuto 3 Material 
		TapeMats.Add(MatFinder_Main1.Object);   // Mian 1 Material

		ActorBaseMesh->SetMaterial(0, MatFinder_Default.Object);
	}
}

void AATape::BeginPlay()
{
	Super::BeginPlay();
}

void AATape::OnGrabbed(UMotionControllerComponent& InMCRef, const FVector& HandGrabPos, AVRHand* InGrabbingHand)
{
	HVRSoundPlayer::PlaySoundEffect(this, SFX_LightGrab, this->GetRootComponent()->GetComponentLocation());
}

void AATape::OnDropped() 
{
	this->GC->SetPrimitiveCompPhysics(true);
}

void AATape::HandleDontGrabPhysics(uint8 bIsDontGrabFlag)
{
	if (bIsDontGrabFlag)
	{
		//ActorBaseMesh->SetCollisionProfileName(FName("NoCollision"));
		ActorBaseMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	}
	else
	{
		ActorBaseMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		ActorBaseMesh->SetCollisionProfileName(FName("PhysicsActor"));
	}
}

void AATape::SetTapeGrabOn()
{
	this->ActorBaseMesh->SetVisibility(true);
	this->ActorBaseMesh->SetHiddenInGame(false);
	this->GC->SetPrimitiveCompPhysics(true);
	HandleDontGrabPhysics(false);
}
