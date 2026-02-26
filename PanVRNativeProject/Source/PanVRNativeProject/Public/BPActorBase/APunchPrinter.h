// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorBase/VRActorBase.h"
#include "APunchPrinter.generated.h"

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API AAPunchPrinter : public AVRActorBase
{
	GENERATED_BODY()
	
public:
	AAPunchPrinter();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTimes) override;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> PPSandbag;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBoxComponent> CLPPSandbag;

	UPROPERTY()
	TObjectPtr<class UTimelineComponent> PPMovementTimeline;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBoxComponent> CLPPPaper;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class AAPaper> NewSpawnPaper;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class AAPaper> AttachingPaper;

	//class AAPaper* NewSpawnPaper = nullptr;

protected:
	UFUNCTION()
	void CLPPOverlapBegin(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void CLPPFOverlapEnd(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

	UFUNCTION()
	void CLPPEntranceOverlapBegin(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void CLPPEntranceFOverlapEnd(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

	UFUNCTION()
	void PPMoveTLFunc(float Value);

	UFUNCTION()
	void PPMoveTLEndFunc();

private:
	UCurveFloat* PPMoveCurve;
	//class AAPaper* NewSpawnPaper = nullptr;
	FTimerHandle NewSpawnTimer;

private:
	void PPMoveEndFunc();

	// BeignPlay After Default Paper Spawn Function
	void DefaultSpawnPaper();
};
