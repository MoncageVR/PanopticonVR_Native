// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameEngine.h"

// For Debugging Environment When Executing In UE Editor, Macro that Part Of "Code"
// Macro Definition : DEBUG_EXEC(Code)
#define DEBUG_EXEC(Code) if (GIsEditor && GEngine && GEngine->IsEditor()) { Code; }

#define DEBUG_PRINTSCREEN(Duration, Color, Content) if (GEngine) {GEngine->AddOnScreenDebugMessage(-1, Duration, Color, Content); }

class PANVRNATIVEPROJECT_API FDebugLib { };

// HeaderFile Definition Path
// #include "Core/Debug/FDebugLib.h"