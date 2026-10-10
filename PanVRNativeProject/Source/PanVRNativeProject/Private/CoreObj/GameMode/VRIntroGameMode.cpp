#include "CoreObj/GameMode/VRIntroGameMode.h"
#include "CoreCommon/VRPawn/CVRPawn.h"
#include "Kismet/GameplayStatics.h"

AVRIntroGameMode::AVRIntroGameMode()
{
	DefaultPawnClass = ACVRPawn::StaticClass();
}

void AVRIntroGameMode::StartPlay()
{
	Super::StartPlay();

	FLatentActionInfo LatentInfo;
	UGameplayStatics::LoadStreamLevel(this, FName("LobbyMap"), true, false, LatentInfo);

	GetWorld()->GetTimerManager().SetTimer(
		IntroTimer,
		this,
		&AVRIntroGameMode::GoToLobbyMap,
		4.1f,
		false
	);
}

void AVRIntroGameMode::GoToLobbyMap()
{
	UE_LOG(LogTemp, Log, TEXT("[Debug] - Open the LobbyMap Success!"));
	UGameplayStatics::OpenLevel(this, FName("LobbyMap"));
}
