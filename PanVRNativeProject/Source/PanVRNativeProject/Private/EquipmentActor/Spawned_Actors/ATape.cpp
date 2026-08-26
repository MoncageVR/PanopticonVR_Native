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

	//CL_TapeBody = CreateDefaultSubobject<UBoxComponent>("TapeColComp");
	//if (CL_TapeBody)
	//{
	//	CL_TapeBody->SetupAttachment(ActorBaseMesh);
	//	CL_TapeBody->SetBoxExtent(FVector(20.2f, 10.0f, 3.1f));
	//	CL_TapeBody->SetHiddenInGame(false); // Debug
	//	CL_TapeBody->SetCollisionProfileName(FName("OverlapAll"));
	//	CL_TapeBody->ComponentTags.Add(FName("Tape"));
	//}
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
