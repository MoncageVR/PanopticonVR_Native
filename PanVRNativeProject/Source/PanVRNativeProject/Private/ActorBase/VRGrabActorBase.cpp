#include "ActorBase/VRGrabActorBase.h"
#include "CoreCommon/Component/UGrabComp.h"

AVRGrabActorBase::AVRGrabActorBase()
{
	PrimaryActorTick.bCanEverTick = false;

	GC = CreateDefaultSubobject<UUGrabComp>("GrabComponent");
	GC->SetupAttachment(ActorBaseMesh);

	static ConstructorHelpers::FObjectFinder<USoundBase> SFXFinder_HeavyGrab(TEXT("/Game/VRContent/Sound/Wavs/CommonSFX/sfx_grab_heavy.sfx_grab_heavy"));
	static ConstructorHelpers::FObjectFinder<USoundBase> SFXFinder_LightGrab(TEXT("/Game/VRContent/Sound/Wavs/CommonSFX/sfx_grab_light.sfx_grab_light"));
	if (SFXFinder_HeavyGrab.Succeeded() && SFXFinder_LightGrab.Succeeded())
	{
		SFX_HeavyGrab = SFXFinder_HeavyGrab.Object;
		SFX_LightGrab = SFXFinder_LightGrab.Object;
	}
}

void AVRGrabActorBase::BeginPlay()
{
	Super::BeginPlay();

}

void AVRGrabActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
