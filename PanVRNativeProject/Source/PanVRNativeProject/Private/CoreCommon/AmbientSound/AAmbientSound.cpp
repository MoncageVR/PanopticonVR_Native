#include "CoreCommon/AmbientSound/AAmbientSound.h"
#include "Components/AudioComponent.h"

AAAmbientSound::AAAmbientSound()
{
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<USoundBase> AmbiSoundFinder_Lobby(TEXT("/Game/VRContent/Sound/Wavs/Ambient_Sounds/ambi_lobby.ambi_lobby"));
	static ConstructorHelpers::FObjectFinder<USoundBase> AmbiSoundFinder_Main(TEXT("/Game/VRContent/Sound/Wavs/Ambient_Sounds/ambi_main.ambi_main"));
	if (AmbiSoundFinder_Lobby.Succeeded() && AmbiSoundFinder_Main.Succeeded())
	{
		AmbiSound_Lobby = AmbiSoundFinder_Lobby.Object;
		AmbiSound_Main = AmbiSoundFinder_Main.Object;
	}

	//this->GetAudioComponent()->bAllowSpatialization = 0;
}

void AAAmbientSound::BeginPlay()
{
}

void AAAmbientSound::SetPlayingSound(uint8 InGMFlag)
{
	if (InGMFlag)
	{
		this->GetRootComponent()->SetWorldLocation(FVector(-10.0f, -10.0f, 120.0f), false, nullptr, ETeleportType::TeleportPhysics);
		this->GetAudioComponent()->SetSound(AmbiSound_Lobby);
		this->GetAudioComponent()->Play();
	}
	else
	{
		this->GetRootComponent()->SetWorldLocation(FVector(-10.0f, -10.0f, 120.0f), false, nullptr, ETeleportType::TeleportPhysics);
		this->GetAudioComponent()->SetSound(AmbiSound_Main);
		this->GetAudioComponent()->Play();
	}
}
