#include "CoreObj/Manager/GameInstanceSubSystem/VRGameInstanceSubsystem.h"
#include "ActorBase/VRGrabActorBase.h"
#include "CoreCommon/Component/UGrabComp.h"
#include "EquipmentActor/AGTWLever.h"

void UVRGameInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	
}

void UVRGameInstanceSubsystem::SetWorldVolumeMultiplier(float TempVPlaySize) { World_VolumeMultiplier = TempVPlaySize; }
void UVRGameInstanceSubsystem::SetWorldVolumePitch(float TempVPlaySpeed) { World_VolumePitch = TempVPlaySpeed; }
float UVRGameInstanceSubsystem::GetWorldVolumeMultiplier() const { return World_VolumeMultiplier; }
float UVRGameInstanceSubsystem::GetWorldVolumePitch() const { return World_VolumePitch; }
