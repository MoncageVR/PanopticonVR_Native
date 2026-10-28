#include "CoreObj/GameMode/VRIntroGameMode.h"
#include "CoreCommon/VRPawn/CVRPawn.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"

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
		7.1f,
		false
	);

	ACVRPawn* TempVRPlayer = Cast<ACVRPawn>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (TempVRPlayer)
	{
		TempVRPlayer->GetVRPawnWidgetComp()->SetVisibility(false);
	}
}

void AVRIntroGameMode::GoToLobbyMap()
{
	UGameplayStatics::OpenLevel(this, FName("LobbyMap"));
}
