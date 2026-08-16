#include "CoreObj/Manager/WorldSubSystem/VREquipmentWorldSubsystem.h"
#include "CoreCommon/Interface/IEquipmentInitInterface.h"

void UVREquipmentWorldSubsystem::PostInitialize()
{
	// UE_LOG(LogTemp, Log, TEXT("VREquipmentWorldSubSystem Initialize Success!"));
}

void UVREquipmentWorldSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	// UE_LOG(LogTemp, Log, TEXT("VREquipmentWorldSubSystem OnWorldBeginPlay Call Success!"));
}

void UVREquipmentWorldSubsystem::HandleAddEquipmentArrs(UObject* InActor)
{
	if (InActor && InActor->Implements<UIEquipmentInitInterface>())
	{
		EquipmentArrs.Add(TScriptInterface<IIEquipmentInitInterface>(InActor));
	}
}

//void UVREquipmentWorldSubsystem::ClearEquipmentArrs()
//{
//	EquipmentArrs.Empty();
//}

//void UVREquipmentWorldSubsystem::NotifyGameStartBroadCast()
//{
//	FGameStartSignature.ExecuteIfBound(true);
//}

void UVREquipmentWorldSubsystem::NotifyPunchStartBroadCast()
{
	FPunchStartSignature.ExecuteIfBound();
}

void UVREquipmentWorldSubsystem::NotifyCloseDoorBroadCast()
{
	FCloseDoorSignature.ExecuteIfBound();
}

void UVREquipmentWorldSubsystem::NotifyGameStartInLobbyBroadCast()
{
	FLobbyGameStartSignature.ExecuteIfBound();
}

void UVREquipmentWorldSubsystem::NotifyKeyPadArrClearBroadCast()
{
	FKeyPadArrClearSignature.ExecuteIfBound();
}

void UVREquipmentWorldSubsystem::NotifyPumpHorseOneStepBroadCast()
{
	FPumpHorseOneStepSignature.ExecuteIfBound();
}

void UVREquipmentWorldSubsystem::NotifyEBOperationControlBroadCast(uint8 InControlFlag)
{
	FEBOperateControlSignature.ExecuteIfBound(InControlFlag);
}

void UVREquipmentWorldSubsystem::NotifyJailOperationControlByABBroadCast(FName InOperationTargetObjectName)
{
	FJailOperationControlByABSignature.ExecuteIfBound(InOperationTargetObjectName);
}

void UVREquipmentWorldSubsystem::NotifyInputValuePassingByKeyPadBroadCast(bool bIsClearFlag, uint32 InPassingValue)
{
	FInputValuePassingByKeyPadSignature.ExecuteIfBound(bIsClearFlag, InPassingValue);
}

void UVREquipmentWorldSubsystem::NotifyKeyPadOperationBroadCast(uint8 InBodyMoveFlag)
{
	FToggleToKeyPadSignature.ExecuteIfBound(InBodyMoveFlag);
}

void UVREquipmentWorldSubsystem::NotifyToggleSwitchOperationBroadCast(uint8 InSwitchFlag)
{
	FKeyPadToToggleSignature.ExecuteIfBound(InSwitchFlag);
}

void UVREquipmentWorldSubsystem::NotifyPumpOperationBroadCast(float InOperTime)
{
	FBinToPumpSignature.ExecuteIfBound(InOperTime);
}

void UVREquipmentWorldSubsystem::NotifyTowerSubdueBroadCast(uint8 InSubdueFlag)
{
	FToiletToTowerSignature.ExecuteIfBound(InSubdueFlag);
}

void UVREquipmentWorldSubsystem::NotifyGameStartBroadCast(bool bIsGameStartFlag)
{
	if (!FGameStartSignature.IsBound())
	{
		UE_LOG(LogTemp, Error, TEXT("GameStart Signature Not Valid,"));
		return;
	}
	else
	{
		FGameStartSignature.Broadcast(bIsGameStartFlag);
	}
}

void UVREquipmentWorldSubsystem::NotifyMoveOrderBroadCast(FName InTag, int32 InTargetFloor)
{
	if (!FEBMoveOrderSignature.IsBound())
	{
		UE_LOG(LogTemp, Log, TEXT("EB Signature Not Binding!"));
		return;
	}
	else
	{
		FEBMoveOrderSignature.Broadcast(InTag, InTargetFloor);
	}
}
