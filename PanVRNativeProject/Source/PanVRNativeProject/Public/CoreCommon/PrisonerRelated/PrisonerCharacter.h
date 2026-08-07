#pragma once

#include "CoreMinimal.h"
#include "CoreCommon/PrisonerCharacterBase/CPrisonerCharacterBase.h"
#include "PrisonerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API APrisonerCharacter : public ACPrisonerCharacterBase
{
	GENERATED_BODY()

public:
	APrisonerCharacter();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void HandleSMHiddenActivation(bool bIsActivateFlag);

	void SetPrisonerAppearanceByDT(bool bIsHairVisible, bool bIsBeardVisible, bool bIsMoustacheVisible);
	void HandleSetPrisonerNewSkin();

#pragma region Getter
	FORCEINLINE FVector GetMeshDefaultPos() const { return MeshDefaultRelativePos; }
	FORCEINLINE FRotator GetMeshDefaultRot() const { return MeshDefaultRelativeRot; }
#pragma endregion


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class APrisonerController* PrisonerControllerComp;

	UPROPERTY()
	class UPhysicalMaterial* PrisonerPhysMat;

	UPROPERTY()
	TObjectPtr <UMaterialInstance> SecondTypePrisonerMat;

private:
	FVector MeshDefaultRelativePos;
	FRotator MeshDefaultRelativeRot;
};
