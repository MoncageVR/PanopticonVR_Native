// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorBase/VRActorBase.h"
#include "AABButton.generated.h"

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API AAABButton : public AVRActorBase 
{
	GENERATED_BODY()

public:
	AAABButton();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void EquipmentRegistrable(AActor* InActor) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component|StaticMesh")
	TObjectPtr<UStaticMeshComponent> ButtonA;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component|Collision")
	TObjectPtr<class UBoxComponent> CLButtonA;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component|StaticMesh")
	TObjectPtr<UStaticMeshComponent> ButtonB;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component|Collision")
	TObjectPtr<class UBoxComponent> CLButtonB;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Values")
	uint8 bIsButtonAPressed = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Values")
	uint8 bIsButtonBPressed = 0;

private:
	TArray<uint8> AnswerSaveList;
	FTimerHandle NextAnswerCheckTimer;
	FTimerHandle CooldownApplyTimer;

	FString CorrectAnswerFirstCase = TEXT("1010");
	FString CorrectAnswerSecondCase = TEXT("1001");

	USoundBase* AButtonSFX;
	USoundBase* BButtonSFX;

private:
	void ButtonMovement(UStaticMeshComponent* InActuallyMoveSMButton, FVector InTargetPos);
	void NextAnswerNonInputClear();
	void CheckCorrectAnswer();

	void CoolDownApplyFunc(uint8 InCoolTimeFlag);
	void CoolDownUnApplyFunc();

protected:
	UFUNCTION()
	void OverlapABoxBegin(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void OverlapABoxEnd(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

	UFUNCTION()
	void OverlapBBoxBegin(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void OverlapBBoxEnd(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

};
