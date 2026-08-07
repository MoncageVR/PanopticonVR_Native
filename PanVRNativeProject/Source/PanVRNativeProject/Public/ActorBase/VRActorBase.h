#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoreCommon/Interface/IEquipmentInitInterface.h"
#include "Kismet/GameplayStatics.h"
#include "VRActorBase.generated.h"

class UVREquipmentWorldSubsystem;
class UMapObjManagerSubsystem;

UCLASS()
class PANVRNATIVEPROJECT_API AVRActorBase : public AActor , public IIEquipmentInitInterface
{
	GENERATED_BODY()
	
public:	
	AVRActorBase();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void EquipmentRegistrable(AActor* InActor) override;

	class HVRSoundPlayer* mSoundPlayer;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Mesh")
	TObjectPtr<class UStaticMeshComponent> ActorBaseMesh;

	UPROPERTY()
	TObjectPtr<UVREquipmentWorldSubsystem> EquipmentWorldSubSystem;

	UPROPERTY()
	TObjectPtr<UMapObjManagerSubsystem> MapObjManagerGameInstSubsystemRef;

private:
};


class HVRSoundPlayer
{
public:
	HVRSoundPlayer() = default;

	void PlaySoundEffect(UObject* PlayEquipment, USoundBase* Sound, FVector TempPlayPos);
};