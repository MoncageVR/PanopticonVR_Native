

#pragma once

#include "CoreMinimal.h"
#include "EPrisonerStates.generated.h"

UENUM(BlueprintType)
enum class EPrisonerUpperStateType : uint8
{
	Idle = 0 UMETA(DisplayName = "IDLE"),
	Stop = 1 UMETA(DisplayName = "STOP"),
	Move = 2 UMETA(DisplayName = "MOVE"),
	Interact = 3 UMETA(DisplayName = "INTERACT"),
	Dangerous = 4 UMETA(DisplayName = "DANGEROUS"),
	Confusion = 5 UMETA(DisplayName = "CONFUSION")
};

UENUM(BlueprintType)
enum class EPrisonerLowerStateType : uint8
{
	Default = 0 UMETA(DisplayName = "DEFAULT"),
	Subdue = 1 UMETA(DisplayName = "SUBDUE"),
	GoHome = 2 UMETA(DisplayName = "GOHOME"),
	RandomMove = 3 UMETA(DisplayName = "RANDOMMOVE"),
	Run = 4 UMETA(DisplayName = "RUN"),
	Floating = 5 UMETA(DisplayName = "FLOATING"),
	SpiderMan = 6 UMETA(DisplayName = "SPIDERMAN"),
	DoorPicking = 7 UMETA(DisplayName = "DOORPICKING"),
	Anger = 8 UMETA(DisplayName = "ANGER"),
	AssistNeighbor = 9 UMETA(DisplayName = "ASSISTNEIGHBOR"),
	Teleport = 10 UMETA(DisplayName = "TELEPORT"),
	TopEscape = 11 UMETA(DisplayName = "TOPESCAPE"),
	Escape = 12 UMETA(DisplayName = "ESCAPE"),
	TowerRaid = 13 UMETA(DisplayName = "TOWERRAID"),
	Radioactivity = 14 UMETA(DisplayName = "RADIOACTIVITY"),
	Flame = 15 UMETA(DisplayName = "FLAME"),
	Distract = 16 UMETA(DisplayName = "DISTRACT"),
	Roar = 17 UMETA(DisplayName = "ROAR"),
	Golf = 18 UMETA(DisplayName = "GOLF")
};

/*DANCE = 16 UMETA(DisplayName = "Dance"),
HELLO = 17 UMETA(DisplayName = "Hello"),
SLEEP = 18 UMETA(DisplayName = "Sleep"),
STRECHING = 19 UMETA(DisplayName = "Streching"),
POINTING = 20 UMETA(DisplayName = "Pointing"),*/