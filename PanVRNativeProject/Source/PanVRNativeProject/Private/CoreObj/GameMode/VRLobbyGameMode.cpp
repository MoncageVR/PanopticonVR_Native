#include "CoreObj/GameMode/VRLobbyGameMode.h"
#include "CoreObj/Manager/VRGameInstance.h"
#include "CoreObj/Manager/WorldSubSystem/VREquipmentWorldSubsystem.h"
#include "CoreCommon/VRPawn/CVRPawn.h"
#include "CoreCommon/AmbientSound/AAmbientSound.h"
#include "Kismet/GameplayStatics.h"
#include "MainActor/AGameResultScreen.h"

AVRLobbyGameMode::AVRLobbyGameMode()
{
	DefaultPawnClass = ACVRPawn::StaticClass();
}

void AVRLobbyGameMode::StartPlay()
{
	Super::StartPlay();

	mVRGameInstanceRef = Cast<UVRGameInstance>(GetWorld()->GetGameInstance());
	check(mVRGameInstanceRef);
	// Debug
	if (!mVRGameInstanceRef->GetIsFirstLobbyEntryFlag())
	{
		UE_LOG(LogTemp, Error, TEXT("Player is Lobby First Entry!"));
	}
	// Debug
	mVRGameInstanceRef->SetIsFirstLobbyEntryFlag(true);

	if (UWorld* MyWorld = GetWorld())
	{
		FTransform TempSpawnTransform(FTransform::Identity);
		AAAmbientSound* TempAmbientSoundActor = MyWorld->SpawnActor<AAAmbientSound>(AAAmbientSound::StaticClass(), TempSpawnTransform);
		if (TempAmbientSoundActor)
		{
			TempAmbientSoundActor->SetPlayingSound(1);
		}
	}
}

void AVRLobbyGameMode::HandleOpenMainMap()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("MainMap"));
}

void AVRLobbyGameMode::CheckGameResult()
{
	if (mVRGameInstanceRef->GetIsFirstLobbyEntryFlag())
	{
		UE_LOG(LogTemp, Log, TEXT("Player isn't Lobby First Entry!!"));
	}

	if (mVRGameInstanceRef->GetIsGameResultSuccess())
	{
		//UE_LOG(LogTemp, Error, TEXT("Game Result Success Print Logic Execute!"));
		//TempGameResultScreen->GameResultDisplay(true);
		this->NotifyGameResultPrint(true);
	}

	if (mVRGameInstanceRef->GetIsGameResultFail())
	{
		//UE_LOG(LogTemp, Error, TEXT("Game Result Fail Print Logic Execute!"));
		//TempGameResultScreen->GameResultDisplay(false);
		this->NotifyGameResultPrint(false);
	}
}

void AVRLobbyGameMode::NotifyGameResultPrint(bool bIsGameResultFlag)
{
	FGameResultPrintSignature.ExecuteIfBound(bIsGameResultFlag);
}
