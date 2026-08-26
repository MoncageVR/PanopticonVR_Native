#pragma once

#include "CoreMinimal.h"
#include "Sound/AmbientSound.h"
#include "AAmbientSound.generated.h"

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API AAAmbientSound : public AAmbientSound
{
	GENERATED_BODY()
	
public:
	AAAmbientSound();
	virtual void BeginPlay() override;
	
	// 0 : MainMap , 1 : LobbyMap 
	void SetPlayingSound(uint8 InGMFlag);

protected:

private:
	UPROPERTY()
	TObjectPtr<USoundBase> AmbiSound_Lobby;

	UPROPERTY()
	TObjectPtr<USoundBase> AmbiSound_Main;
};
