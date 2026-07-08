// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PrisonerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTaskFinishedVar);

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API APrisonerController : public AAIController
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	FOnTaskFinishedVar OnTaskFinished;

public:
	APrisonerController();

	virtual void Tick(float DeltaTimes) override;

public:
#pragma region Getter
	FORCEINLINE class UBehaviorTree* GetBT() { return BehaviorTreeAsset; }
	FORCEINLINE class UBlackboardData* GetBB() { return BlackboardAsset; }
	FORCEINLINE class UBlackboardComponent* GetBBComp() { return BlackboardComp; }
#pragma endregion

#pragma region Setter
	void SetBT(class UBehaviorTree* InNewBT) { BehaviorTreeAsset = InNewBT; }
	void SetBB(class UBlackboardData* InNewBB) { BlackboardAsset = InNewBB; }
	void SetBBComp(class UBlackboardComponent* InNewBBComp) { BlackboardComp = InNewBBComp; }
#pragma endregion

	UFUNCTION()
	void HandleNextTask();

	

protected:
	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY()
	class UBehaviorTree* BehaviorTreeAsset;

	UPROPERTY()
	class UBlackboardData* BlackboardAsset;

	UPROPERTY()
	class UBlackboardComponent* BlackboardComp;

private:
	TArray<uint8> Debug_Upper_State;
	TArray<uint8> Debug_Lower_State;
	int32 Debug_Length;
	int32 Debug_CurrStateIndex;

};
