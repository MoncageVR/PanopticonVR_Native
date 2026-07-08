// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreObj/VREquipmentWorldSubsystem.h"

void UVREquipmentWorldSubsystem::PostInitialize()
{
	//UE_LOG(LogTemp, Log, TEXT("VREquipmentWorldSubSystem Initialize Success!"));
}

void UVREquipmentWorldSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
}

void UVREquipmentWorldSubsystem::AddEquipmentArrs(IIEquipmentInitInterface* InActor)
{
	EquipmentArrs.Add(InActor);
}

void UVREquipmentWorldSubsystem::NotifyGameStartBroadCast()
{
	FGameStartSignature.ExecuteIfBound(true);
}

void UVREquipmentWorldSubsystem::NotifyPunchStartBroadCast()
{
	FPunchStartSignature.ExecuteIfBound();
}

void UVREquipmentWorldSubsystem::NotifyCloseDoorBroadCast()
{
	FCloseDoorSignature.ExecuteIfBound();
}

void UVREquipmentWorldSubsystem::NotifyGloveOperateBroadCast()
{
	FGloveOperationSignature.ExecuteIfBound();
}

void UVREquipmentWorldSubsystem::NotifyEBOperationControlBroadCast(uint8 InControlFlag)
{
	FEBOperateControlSignature.ExecuteIfBound(InControlFlag);
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
