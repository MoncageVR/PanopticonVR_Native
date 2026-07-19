// Fill out your copyright notice in the Description page of Project Settings.


#include "PrisonerCharacterBase/CPrisonerBase.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ACPrisonerBase::ACPrisonerBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->GetMesh()->SetRelativeTransform(FTransform(FRotator(0.f, -90.f, 0.f), FVector(0.f, 0.f, -130.f), FVector(1.5f, 1.5f, 1.5f)));

	this->GetCapsuleComponent()->SetCapsuleHalfHeight(130.f);
	this->GetCapsuleComponent()->SetCapsuleRadius(22.f);
	this->GetCapsuleComponent()->SetHiddenInGame(false); // Debug

	SMMoustache = CreateDefaultSubobject<UStaticMeshComponent>("MoustacheParts");
	SMBeard = CreateDefaultSubobject<UStaticMeshComponent>("BeardParts");
	SMHair = CreateDefaultSubobject<UStaticMeshComponent>("HairParts");

	if (SMMoustache && SMBeard && SMHair && this->GetMesh())
	{
		SMMoustache->SetupAttachment(this->GetMesh(), TEXT("Moustache_Socket"));
		SMBeard->SetupAttachment(this->GetMesh(), TEXT("Beard_Socket"));
		SMHair->SetupAttachment(this->GetMesh(), TEXT("Hair_Socket"));
	}

	this->GetCapsuleComponent()->ComponentTags.Add(TEXT("PrisonerCharacter"));
	this->GetMesh()->ComponentTags.Add(TEXT("PrisonerCharacter"));
}

// Called when the game starts or when spawned
void ACPrisonerBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACPrisonerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACPrisonerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

