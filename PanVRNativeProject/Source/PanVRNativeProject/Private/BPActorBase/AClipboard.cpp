// Fill out your copyright notice in the Description page of Project Settings.

#include "BPActorBase/AClipboard.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"

AClipboard::AClipboard()
{
	PrimaryActorTick.bCanEverTick = false;

	// Root
	SM_CB_Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_CB_Body"));
	RootComponent = SM_CB_Body;

	// Hierarchy node attached to body
	GC_CB = CreateDefaultSubobject<USceneComponent>(TEXT("GC_CB"));
	GC_CB->SetupAttachment(SM_CB_Body);

	// Paper mesh under GC_CB
	SM_CB_Paper = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_CB_Paper"));
	SM_CB_Paper->SetupAttachment(GC_CB);

	// Collision on paper surface
	CL_Box_Paper = CreateDefaultSubobject<UBoxComponent>(TEXT("CL_Box_Paper"));
	CL_Box_Paper->SetupAttachment(SM_CB_Paper);
	CL_Box_Paper->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CL_Box_Paper->SetGenerateOverlapEvents(true);
	CL_Box_Paper->SetHiddenInGame(false);

	TArray<UPrimitiveComponent*> AllComps;
	GetComponents<UPrimitiveComponent>(AllComps);
	for (UPrimitiveComponent* AllComp : AllComps)
	{
		if (!AllComp) continue;

		if (AllComp->CanEverAffectNavigation())
			AllComp->SetCanEverAffectNavigation(false);
		else
			continue;
	}
}
