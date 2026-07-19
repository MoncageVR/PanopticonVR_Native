// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EPrisonerStates.generated.h"


UENUM(BlueprintType)
enum class EPrisonerUpperState : uint8
{
	IDLE = 0 UMETA(DisplayName = "Idle"),
	STOP = 1 UMETA(DisplayName = "Stop"),
	MOVE = 2 UMETA(DisplayName = "Move"),
	INTERACT = 3 UMETA(DisplayName = "Interact"),
	DANGEROUS = 4 UMETA(DisplayName = "Dangerous"),
	CONFUSION = 5 UMETA(DisplayName = "Confusion")
};


UENUM(BlueprintType)
enum class EPrisonerLowerState : uint8
{
	DEFAULT = 0 UMETA(DisplayName = "Default"),
	SUBDUE = 1 UMETA(DisplayName = "Subdue"),
	GOHOME = 2 UMETA(DisplayName = "GoHome"),
	RANDOMMOVE = 3 UMETA(DisplayName = "RandomMove"),
	RUN = 4 UMETA(DisplayName = "Run"),
	FLOATING = 5 UMETA(DisplayName = "Floating"),
	SPIDERMAN = 6 UMETA(DisplayName = "SpiderMan"),
	DOORPICKING = 7 UMETA(DisplayName = "DoorPicking"),
	ANGER = 8 UMETA(DisplayName = "Anger"),
	ASSISTNEIGHBOR = 9 UMETA(DisplayName = "AssistNeighbor"),
	TELEPORT = 10 UMETA(DisplayName = "Teleport"),
	TOPESCAPE = 11 UMETA(DisplayName = "TopEscape"),
	ESCAPE = 12 UMETA(DisplayName = "Escape"),
	TOWERRAID = 13 UMETA(DisplayName = "TowerRaid"),
	RADIOACTIVITY = 14 UMETA(DisplayName = "Radioactivity"),
	FLAME = 15 UMETA(DisplayName = "Flame"),
	DANCE = 16 UMETA(DisplayName = "Dance"),
	HELLO = 17 UMETA(DisplayName = "Hello"),
	SLEEP = 18 UMETA(DisplayName = "Sleep"),
	STRECHING = 19 UMETA(DisplayName = "Streching"),
	POINTING = 20 UMETA(DisplayName = "Pointing"),
	ROAR = 21 UMETA(DisplayName = "Roar"),
	EXPLOSION = 22 UMETA(DisplayName = "Explosion"),
	GOLF = 23 UMETA(DisplayName = "Golf")
};

/**
 * 
 */
//class PANVRNATIVEPROJECT_API EPrisonerStates
//{
//public:
//	EPrisonerStates();
//	~EPrisonerStates();
//};
