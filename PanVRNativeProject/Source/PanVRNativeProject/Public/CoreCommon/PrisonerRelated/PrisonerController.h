#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PrisonerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTaskFinishedVar);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPrisonerLowerStateChangedDelegateVar, APrisonerController*, PrisonerCon, uint8, NewLowerState);


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

	UPROPERTY()
	FOnPrisonerLowerStateChangedDelegateVar FOnPrisonerLowerStateChangedSignature;

public:
	APrisonerController();

	virtual void Tick(float DeltaTimes) override;

	UFUNCTION(BlueprintCallable)
	void State_based_ExecutionTasks_GiventoSomeone(TArray<uint8> InUpperStates, TArray<uint8> InLowerStates);

	UFUNCTION()
	void HandlePlayPrisonerLogic(int32 InRanNum);

	// Initializes the UpperState and LowerState based on the assigned logic DataTable.
	UFUNCTION()
	void InitializeStatesFromLogicDT();

	void HandleFlameTransitionColNTimer(uint8 InHandleFlag);

	UFUNCTION()
	void HandleRunBT();

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
	FORCEINLINE TObjectPtr<class UBehaviorTree> GetBT() const { return BehaviorTreeAsset; }
	FORCEINLINE TObjectPtr<class UBlackboardData> GetBB() const { return BlackboardAsset; }
	FORCEINLINE class UBlackboardComponent* GetBBComp() const { return BlackboardComp; }
	FORCEINLINE TObjectPtr<class UPrisonerAnimInstance > GetPrisonerAnimInstance() const { return mPrisonerAnimInstancePtr; }
	FORCEINLINE uint8 GetCurrLowerState() const { return BlackboardComp ? BlackboardComp->GetValueAsEnum(TEXT("CurrLowerState")) : 0; }

	FORCEINLINE TObjectPtr<class UDataTable> GetMyLogicDT() const { return mLogicDT; }
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
	TObjectPtr<class UBehaviorTree> BehaviorTreeAsset;

	UPROPERTY()
	TObjectPtr<class UBlackboardData> BlackboardAsset;

	UPROPERTY()
	class UBlackboardComponent* BlackboardComp;

	UFUNCTION()
	void FlameCLOverlapBegin(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

private:
	UPROPERTY()
	TObjectPtr<UAnimInstance> AnimInstancePtr;

	UPROPERTY()
	TObjectPtr<class UPrisonerAnimInstance> mPrisonerAnimInstancePtr;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USphereComponent> CL_FlameTransition;

	FTimerHandle FLameTransitionTimer;

private:
	UFUNCTION()
	void AttachSphereCollision();

	UFUNCTION()
	void ActuallyFlameTransition();
};
