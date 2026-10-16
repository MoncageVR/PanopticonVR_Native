#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ALogoPlane.generated.h"

UCLASS()
class PANVRNATIVEPROJECT_API AALogoPlane : public AActor
{
	GENERATED_BODY()
	
public:	
	AALogoPlane();

	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> SM_Logo;

	UPROPERTY()
	TObjectPtr<UCurveFloat> FadeCurve;

	UPROPERTY()
	TObjectPtr<class UTimelineComponent> TL_FadeComp;

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> LogoMID;

	UPROPERTY()
	TObjectPtr<USoundBase> SFX_LogoPlay;

	FTimerHandle TempSoundTimer;

private:
	void ActuallyPlayFadeEffect();

	void PlayLogoSound();

	UFUNCTION()
	void HandleFadeProgress(float Value);
	
	UFUNCTION()
	void HandleFadeFinished();
};
