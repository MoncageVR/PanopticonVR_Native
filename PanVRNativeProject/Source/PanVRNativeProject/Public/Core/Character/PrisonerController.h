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

	UFUNCTION()
	void State_based_ExecutionTasks_GiventoSomeone(TArray<uint8> InUpperStates, TArray<uint8> InLowerStates);

	UFUNCTION()
	void HandlePrisonerLogic(int32 InRanNum);

#pragma region DebugVariables
public:
	UPROPERTY(VisibleAnywhere)
	TArray<uint8> Debug_Upper_State;

	UPROPERTY(VisibleAnywhere)
	TArray<uint8> Debug_Lower_State;

	UPROPERTY(VisibleAnywhere)
	int32 Debug_Length;

	UPROPERTY(VisibleAnywhere)
	int32 Debug_CurrStateIndex;
#pragma endregion

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UDataTable> mLogicDT;

public:
#pragma region Getter
	FORCEINLINE class UBehaviorTree* GetBT() const { return BehaviorTreeAsset; }
	FORCEINLINE class UBlackboardData* GetBB() const { return BlackboardAsset; }
	FORCEINLINE class UBlackboardComponent* GetBBComp() const { return BlackboardComp; }
	FORCEINLINE class UPrisonerAnimInstance* GetPrisonerAnimInstance() const { return mPrisonerAnimInstancePtr; }
	FORCEINLINE uint8 GetCurrLowerState() const { return Debug_Lower_State[Debug_CurrStateIndex-1]; }
#pragma endregion

#pragma region Setter
	void SetBT(class UBehaviorTree* InNewBT) { BehaviorTreeAsset = InNewBT; }
	void SetBB(class UBlackboardData* InNewBB) { BlackboardAsset = InNewBB; }
	void SetBBComp(class UBlackboardComponent* InNewBBComp) { BlackboardComp = InNewBBComp; }
	FORCEINLINE void SetMyLogicDT(TObjectPtr<UDataTable> InDT) { mLogicDT = InDT; }

#pragma endregion
	UFUNCTION()
	void HandleNextTask();

protected:


protected:
	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY()
	class UBehaviorTree* BehaviorTreeAsset;

	UPROPERTY()
	class UBlackboardData* BlackboardAsset;

	UPROPERTY()
	class UBlackboardComponent* BlackboardComp;

private:
	UAnimInstance* AnimInstancePtr;
	class UPrisonerAnimInstance* mPrisonerAnimInstancePtr;
};
