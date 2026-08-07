

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CPrisonerCharacterBase.generated.h"

UCLASS()
class PANVRNATIVEPROJECT_API ACPrisonerCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ACPrisonerCharacterBase();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> SMMoustache;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> SMBeard;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> SMHair;

};
