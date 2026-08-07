

#pragma once

#include "CoreMinimal.h"
#include "ActorBase/VRActorBase.h"
#include "AGameResultScreen.generated.h"

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API AAGameResultScreen : public AVRActorBase
{
	GENERATED_BODY()
	
public:
	AAGameResultScreen();

	virtual void BeginPlay() override;
	// True = GameResult Success / False = GameResult Fail
	UFUNCTION()
	void GameResultDisplay(bool bIsResult);

protected:


private:
	UPROPERTY()
	TObjectPtr<USceneComponent> SC_MainRoot;
	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> SM_Success;

	FTimerHandle RotateSelfTimer;
	FTimerHandle RotatePauseSelfTimer;

private:
	void RotationSelf();
	void PauseRotationSelf();
};
