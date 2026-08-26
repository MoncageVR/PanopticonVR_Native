#include "CoreCommon/PrisonerRelated/PrisonerCharacter.h"
#include "CoreCommon/PrisonerRelated/PrisonerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Components/AudioComponent.h"

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

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MatFinder_PrisonerSkin2(TEXT("/Game/VRContent/Material/SRS_STAGE_PrisonerSkin_2.SRS_STAGE_PrisonerSkin_2"));
	if (MatFinder_PrisonerSkin2.Succeeded())
	{
		SecondTypePrisonerMat = MatFinder_PrisonerSkin2.Object;
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
		PrisonerPhysMat = MatFinder_PrisonerPhysics.Object;
		//this->GetMesh()->SetPhysMaterialOverride(MatFinder_PrisonerPhysics.Object);
		this->GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));
		this->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}

	MeshDefaultRelativePos = this->GetMesh()->GetRelativeLocation();
	MeshDefaultRelativeRot = this->GetMesh()->GetRelativeRotation();

	PrisonerAudioPlayer = CreateDefaultSubobject<UAudioComponent>("SoundPlayerComp");
	if (PrisonerAudioPlayer)
	{
		PrisonerAudioPlayer->SetupAttachment(this->GetRootComponent());
		PrisonerAudioPlayer->SetAutoActivate(false);
		PrisonerAudioPlayer->bAllowSpatialization = false;
	}
}

void APrisonerCharacter::BeginPlay()
{
	Super::BeginPlay();
	this->GetMesh()->SetPhysMaterialOverride(PrisonerPhysMat);

	if (PrisonerAudioPlayer)
	{
		PrisonerAudioPlayer->Stop();
	}
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

void APrisonerCharacter::SetPrisonerAppearanceByDT(bool bIsHairVisible, bool bIsBeardVisible, bool bIsMoustacheVisible)
{
	this->SMMoustache->SetVisibility(bIsMoustacheVisible);
	this->SMBeard->SetVisibility(bIsBeardVisible);
	this->SMHair->SetVisibility(bIsHairVisible);
}

void APrisonerCharacter::HandleSetPrisonerNewSkin()
{
	this->GetMesh()->SetMaterial(0, SecondTypePrisonerMat);
}

void APrisonerCharacter::HandlePlayAPSound(USoundBase* InSound)
{
	if (PrisonerAudioPlayer && InSound)
	{
		if (!PrisonerAudioPlayer->IsPlaying())
		{
			UE_LOG(LogTemp, Log, TEXT("%s : %s Sound - Playing!"), *this->GetName(), *InSound->GetName());
			PrisonerAudioPlayer->SetSound(InSound);
			PrisonerAudioPlayer->Play();
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Already Playing , Sound Play Noop"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Sound Play Noop"));
	}
}

void APrisonerCharacter::HandlePauseAPSound()
{
	if (PrisonerAudioPlayer->IsPlaying())
	{
		PrisonerAudioPlayer->Stop();
	}
}
