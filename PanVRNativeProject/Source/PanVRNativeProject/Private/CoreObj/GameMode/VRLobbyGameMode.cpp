#include "CoreObj/GameMode/VRLobbyGameMode.h"
#include "CoreObj/Manager/VRGameInstance.h"
#include "CoreObj/Manager/WorldSubSystem/VREquipmentWorldSubsystem.h"
#include "CoreObj/Manager/GameInstanceSubSystem/VRDialogueManagerSubsystem.h"
#include "CoreCommon/VRPawn/CVRPawn.h"
#include "CoreCommon/AmbientSound/AAmbientSound.h"
#include "Kismet/GameplayStatics.h"
#include "MainActor/AGameResultScreen.h"
#include "EquipmentActor/Spawned_Actors/ATape.h"
#include "Components/WidgetComponent.h"

AVRLobbyGameMode::AVRLobbyGameMode()
{
	DefaultPawnClass = ACVRPawn::StaticClass();
}

void AVRLobbyGameMode::StartPlay()
{
	Super::StartPlay();

	mVRGameInstanceRef = Cast<UVRGameInstance>(GetWorld()->GetGameInstance());
	check(mVRGameInstanceRef);

	ACVRPawn* TempVRPawnPlayer = Cast<ACVRPawn>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (TempVRPawnPlayer)
	{
		TempVRPawnPlayer->GetVRPawnWidgetComp()->SetVisibility(false);
	}

	if (UWorld* MyWorld = GetWorld())
	{
		FTransform TempSpawnTransform(FTransform::Identity);
		AAAmbientSound* TempAmbientSoundActor = MyWorld->SpawnActor<AAAmbientSound>(AAAmbientSound::StaticClass(), TempSpawnTransform);
		if (TempAmbientSoundActor)
		{
			TempAmbientSoundActor->SetPlayingSound(1);
		}
	}

	if (UWorld* mWorld = GetWorld())
	{
		TapeArrs.Empty();
		TArray<AActor*> TempFoundTapes;
		UGameplayStatics::GetAllActorsOfClass(mWorld, AATape::StaticClass(), TempFoundTapes);

		for (AActor* Actor : TempFoundTapes)
		{
			if (AATape* TempCastTape = Cast<AATape>(Actor))
			{
				TempCastTape->GetRootComponent()->SetVisibility(false);
				TempCastTape->GetRootComponent()->SetHiddenInGame(true);
				TapeArrs.Add(TempCastTape);
			}
		}

		if (TapeArrs.Num() == 3)
		{
			TapeArrs.Sort([](const AATape& A, const AATape& B)
				{
					if (A.GetTapeTypeNum() != B.GetTapeTypeNum())
						return A.GetTapeTypeNum() < B.GetTapeTypeNum();
					return A.GetTapeNum() < B.GetTapeNum();
				});
		}
	}

	TArray<bool> TempArrs = mVRGameInstanceRef->GetTapeUnlockFlagArr();

	for (int32 i = 0; i < TempArrs.Num(); i++)
	{
		if (TempArrs[i])
		{
			TapeArrs[i]->SetTapeGrabOn();
		}

		UE_LOG(LogTemp, Warning, TEXT("Tape Unlock Flag Arr[%d] : [%d]"), i, TempArrs[i]);
	}

	mVRGameInstanceRef->ClearCurrTapeRelatedInfo();
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
