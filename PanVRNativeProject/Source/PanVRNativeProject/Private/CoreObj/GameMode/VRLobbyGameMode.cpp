


#include "CoreObj/GameMode/VRLobbyGameMode.h"
#include "CoreObj/Manager/VRGameInstance.h"
#include "CoreObj/Manager/WorldSubSystem/VREquipmentWorldSubsystem.h"
#include "CoreCommon/VRPawn/CVRPawn.h"
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
}

void AVRLobbyGameMode::HandleOpenMainMap()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("MainMap"));
}

void AVRLobbyGameMode::CheckGameResult()
{
	UVREquipmentWorldSubsystem* TempVREquipmentWorldSubSystemRef = GetWorld()->GetSubsystem<UVREquipmentWorldSubsystem>();
	AAGameResultScreen* TempGameResultScreen = nullptr;

	/*check(TempVREquipmentWorldSubSystemRef);
	for (IIEquipmentInitInterface* Var : TempVREquipmentWorldSubSystemRef->GetEquipmentArr())
	{
		TempGameResultScreen = Cast<AAGameResultScreen>(Var);
		if (TempGameResultScreen)
			break;
		else
			continue;
	}*/

	if (!mVRGameInstanceRef->GetIsFirstLobbyEntryFlag())
	{
		UE_LOG(LogTemp, Error, TEXT("Player is Lobby First Entry!!"));
	}

	if (mVRGameInstanceRef->GetIsGameResultSuccess())
	{
		//UE_LOG(LogTemp, Error, TEXT("Game Result Success Print Logic Execute!"));
		TempGameResultScreen->GameResultDisplay(true);
	}

	if (mVRGameInstanceRef->GetIsGameResultFail())
	{
		//UE_LOG(LogTemp, Error, TEXT("Game Result Fail Print Logic Execute!"));
		TempGameResultScreen->GameResultDisplay(false);
	}
}
