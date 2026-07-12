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

public:
#pragma region Getter
	FORCEINLINE class UBehaviorTree* GetBT() const { return BehaviorTreeAsset; }
	FORCEINLINE class UBlackboardData* GetBB() const { return BlackboardAsset; }
	FORCEINLINE class UBlackboardComponent* GetBBComp() const { return BlackboardComp; }
	FORCEINLINE class UPrisonerAnimInstance* GetPrisonerAnimInstance() const { return mPrisonerAnimInstancePtr; }
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

	UAnimInstance* AnimInstancePtr;
	class UPrisonerAnimInstance* mPrisonerAnimInstancePtr;

};
