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

	//UE_LOG(LogTemp, Log, TEXT("ArrLength : %d"), EquipmentArrs.Num());
}

void UVREquipmentWorldSubsystem::OnGameStartBroadCast()
{
	OnGameStart.ExecuteIfBound(true);
}

void UVREquipmentWorldSubsystem::OnMoveByEBBroadCast(FName InTag, int32 InTargetFloor)
{
	if (!OnEBMoveOrder.IsBound())
	{
		UE_LOG(LogTemp, Log, TEXT("Not Binding!"));
		return;
	}
	else
	{
		OnEBMoveOrder.Broadcast(InTag, InTargetFloor);
	}
}
