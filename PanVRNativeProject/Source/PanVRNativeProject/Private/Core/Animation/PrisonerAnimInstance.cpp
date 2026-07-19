// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Animation/PrisonerAnimInstance.h"

UPrisonerAnimInstance::UPrisonerAnimInstance()
{
	PrisonerUpperStateArrs.Init(false, 6);
	PrisonerLowerStateArrs.Init(false, 25);
}
