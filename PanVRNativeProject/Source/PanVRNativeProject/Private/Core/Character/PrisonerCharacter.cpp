// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Character/PrisonerCharacter.h"
#include "Core/Character/PrisonerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

APrisonerCharacter::APrisonerCharacter()
{
	// Setting Prisoner Character SkeletalMesh And Material
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> ModelingFinder_PrisonerBody(TEXT("/Game/VRContent/Prisoner/PrisonerModeling/TargetModeling/Meshes/SKM_Prisoner.SKM_Prisoner"));
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialFinder_PrisonerSkin(TEXT("/Game/VRContent/Material/SRS_STAGE_PrisonerSkin.SRS_STAGE_PrisonerSkin"));

	if (ModelingFinder_PrisonerBody.Succeeded() && MaterialFinder_PrisonerSkin.Succeeded())
	{
		this->GetMesh()->SetSkeletalMesh(ModelingFinder_PrisonerBody.Object);
		this->GetMesh()->SetMaterial(0, MaterialFinder_PrisonerSkin.Object);
		this->GetMesh()->SetCanEverAffectNavigation(false);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_Moustache(TEXT("/Game/VRContent/Prisoner/PrisonerModeling/TargetModeling/Meshes/SM_Moustache.SM_Moustache"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_Beard(TEXT("/Game/VRContent/Prisoner/PrisonerModeling/TargetModeling/Meshes/SM_Beard.SM_Beard"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_Hair(TEXT("/Game/VRContent/Prisoner/PrisonerModeling/TargetModeling/Meshes/SM_Hair.SM_Hair"));
	if (ModelingFinder_Moustache.Succeeded() && ModelingFinder_Beard.Succeeded() && ModelingFinder_Hair.Succeeded())
	{
		this->SMMoustache->SetStaticMesh(ModelingFinder_Moustache.Object);
		this->SMBeard->SetStaticMesh(ModelingFinder_Beard.Object);
		this->SMHair->SetStaticMesh(ModelingFinder_Hair.Object);

		this->SMMoustache->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		this->SMBeard->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		this->SMHair->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialFinder_PrisonerHair(TEXT("/Game/VRContent/Material/SRS_STAGE_PrisonerHair.SRS_STAGE_PrisonerHair"));
	if (MaterialFinder_PrisonerHair.Succeeded())
	{
		this->SMMoustache->SetMaterial(0, MaterialFinder_PrisonerHair.Object);
		this->SMBeard->SetMaterial(0, MaterialFinder_PrisonerHair.Object);
		this->SMHair->SetMaterial(0, MaterialFinder_PrisonerHair.Object);
		this->SMMoustache->SetCanEverAffectNavigation(false);
		this->SMBeard->SetCanEverAffectNavigation(false);
		this->SMHair->SetCanEverAffectNavigation(false);
	}

	if (this->GetCharacterMovement())
	{
		this->GetCharacterMovement()->MaxStepHeight = 150.0f;
		this->GetCharacterMovement()->SetWalkableFloorAngle(60.0f);
		this->GetCharacterMovement()->SetWalkableFloorZ(0.5f);
		this->GetCharacterMovement()->MaxWalkSpeed = 0.0f;
	}

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = APrisonerController::StaticClass();

	static ConstructorHelpers::FClassFinder<UAnimInstance> ABPFinder_Prisoner(TEXT("/Game/VRContent/Prisoner/PrisonerAnimation/ABP_Prisoner.ABP_Prisoner_C"));
	if (ABPFinder_Prisoner.Succeeded())
	{
		this->GetMesh()->SetAnimClass(ABPFinder_Prisoner.Class);
	}

	static ConstructorHelpers::FObjectFinder<UPhysicalMaterial> MatFinder_PrisonerPhysics(TEXT("/Game/VRContent/Prisoner/PrisonerModeling/TargetModeling/Materials/PrisonerPhysicsMaterial.PrisonerPhysicsMaterial"));
	if (MatFinder_PrisonerPhysics.Succeeded())
	{
		this->GetMesh()->SetPhysMaterialOverride(MatFinder_PrisonerPhysics.Object);
		this->GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));
		this->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}

	MeshDefaultRelativePos = this->GetMesh()->GetRelativeLocation();
	MeshDefaultRelativeRot = this->GetMesh()->GetRelativeRotation();
}

void APrisonerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APrisonerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APrisonerCharacter::HandleSMHiddenActivation(bool bIsActivateFlag)
{
	this->SMMoustache->SetHiddenInGame(bIsActivateFlag);
	this->SMBeard->SetHiddenInGame(bIsActivateFlag);
	this->SMHair->SetHiddenInGame(bIsActivateFlag);
}
