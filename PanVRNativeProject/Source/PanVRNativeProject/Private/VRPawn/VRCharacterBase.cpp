// Fill out your copyright notice in the Description page of Project Settings.


#include "VRPawn/VRCharacterBase.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"

AVRCharacterBase::AVRCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>("MainRoot");
	Root->SetupAttachment(GetMesh());

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(Root);
	Camera->bCameraMeshHiddenInGame = false; // Debug
}

void AVRCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	PlayerMoveUpNDownInStage(true);
}