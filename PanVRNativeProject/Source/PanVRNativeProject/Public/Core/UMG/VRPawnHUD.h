// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VRPawnHUD.generated.h"

class UPrisonerManagerSubsystem;

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API UVRPawnHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	int32 Minutes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	int32 Seconds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	int32 TotalTimes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	int32 PassingTimes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	TObjectPtr<UPrisonerManagerSubsystem> PrisonerManagerRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	bool bTimerRun;
public:


protected:

private:

private:
	
};
