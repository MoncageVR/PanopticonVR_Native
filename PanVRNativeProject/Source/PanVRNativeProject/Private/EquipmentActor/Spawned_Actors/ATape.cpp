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

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MatFinder_TapeTutorial(TEXT("/Game/VRContent/Material/SRS_Lobby_Tape_Tutorial01.SRS_Lobby_Tape_Tutorial01"));
	if (MatFinder_TapeTutorial.Succeeded())
	{
		ActorBaseMesh->SetMaterial(0, MatFinder_TapeTutorial.Object);
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

}

void AATape::HandleDontGrabPhysics(uint8 bIsGrabFlag)
{
	if (bIsGrabFlag)
	{
		ActorBaseMesh->SetCollisionProfileName(FName("NoCollision"));
	}
	else
	{
		ActorBaseMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		ActorBaseMesh->SetCollisionProfileName(FName("PhysicsActor"));
	}
}
