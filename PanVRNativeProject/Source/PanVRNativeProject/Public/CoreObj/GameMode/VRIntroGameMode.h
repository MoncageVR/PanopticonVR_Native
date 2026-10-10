#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "VRIntroGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API AVRIntroGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AVRIntroGameMode();

	virtual void StartPlay() override;

private:
	FTimerHandle IntroTimer;

private:
	void GoToLobbyMap();
};
