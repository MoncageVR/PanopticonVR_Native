#include "ActorBase/VRGrabActorBase.h"
#include "CoreCommon/Component/UGrabComp.h"

AVRGrabActorBase::AVRGrabActorBase()
{
	PrimaryActorTick.bCanEverTick = false;

	GC = CreateDefaultSubobject<UUGrabComp>("GrabComponent");
	GC->SetupAttachment(ActorBaseMesh);
}

void AVRGrabActorBase::BeginPlay()
{
	Super::BeginPlay();

}

void AVRGrabActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
