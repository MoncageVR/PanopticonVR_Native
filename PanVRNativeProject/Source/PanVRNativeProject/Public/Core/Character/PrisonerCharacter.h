// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PrisonerCharacterBase/CPrisonerBase.h"
#include "PrisonerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API APrisonerCharacter : public ACPrisonerBase
{
	GENERATED_BODY()

public:
	APrisonerCharacter();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void HandleSMHiddenActivation(bool bIsActivateFlag);

#pragma region Getter
	FORCEINLINE FVector GetMeshDefaultPos() const { return MeshDefaultRelativePos; }
	FORCEINLINE FRotator GetMeshDefaultRot() const { return MeshDefaultRelativeRot; }
#pragma endregion


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class APrisonerController* PrisonerControllerComp;

private:
	FVector MeshDefaultRelativePos;
	FRotator MeshDefaultRelativeRot;
};
