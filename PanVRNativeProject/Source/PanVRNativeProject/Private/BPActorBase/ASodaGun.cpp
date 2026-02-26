// Fill out your copyright notice in the Description page of Project Settings.


#include "BPActorBase/ASodaGun.h"
#include "BPActorBase/ASodaBullet.h"
#include "Components/BoxComponent.h"
#include "Core/Component/UGrabComp.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "InputAction.h"

AASodaGun::AASodaGun()
{
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Finder_SodaBodyModeling(TEXT("/Game/VRContent/Modeling/SodaGun/Mesh_SodaGun.Mesh_SodaGun"));
	if (Finder_SodaBodyModeling.Succeeded())
	{
		ActorBaseMesh->SetStaticMesh(Finder_SodaBodyModeling.Object);
	}
	ActorBaseMesh->SetRelativeScale3D(FVector(0.8f, 0.8f, 0.8f));

	// In Constructor Setting GC Related Info
	//GC->SetRelativeLocation(FVector(0.0f, -2.8f, 0.0f));
	GC->SetRelativeRotation(FRotator(90.0f, 0.0f, 180.0f));
	GC->EEGrabType = EGrabType::ObjToHand;

	// In Constructor Setting BoxCollision Info
	CL_SodaCanLoadBox = CreateDefaultSubobject<UBoxComponent>("SodaGunLoad_Col");
	CL_SodaCanLoadBox->SetupAttachment(ActorBaseMesh);
	CL_SodaCanLoadBox->SetRelativeLocation(FVector(-7.3f, 0.0f, 14.5f));
	CL_SodaCanLoadBox->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
	CL_SodaCanLoadBox->SetBoxExtent(FVector(4.6f, 7.6f, 4.6f));
	CL_SodaCanLoadBox->SetHiddenInGame(false); // Debug

	// In Constructor Setting Related SceneComponent(Muzzle, Ejaculation)
	MuzzleStandardLocation = CreateDefaultSubobject<USceneComponent>("MuzzleLocation");
	MuzzleStandardLocation->SetupAttachment(ActorBaseMesh);
	MuzzleStandardLocation->SetRelativeLocation(FVector(-38.0f, 0.0f, 14.5f));
	MuzzleStandardLocation->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));

	EjaculationStandardLocation = CreateDefaultSubobject<USceneComponent>("EjaculationLocatiopn");
	EjaculationStandardLocation->SetupAttachment(ActorBaseMesh);
	EjaculationStandardLocation->SetRelativeLocation(FVector(-7.3f, -8.0f, 14.5f));

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialFinder_Main(TEXT("/Game/VRContent/Material/SRS_STAGE_Main.SRS_STAGE_Main"));
	if (MaterialFinder_Main.Succeeded())
	{
		ActorBaseMesh->SetMaterial(0, MaterialFinder_Main.Object);
	}

	CL_SodaCanLoadBox->OnComponentBeginOverlap.AddDynamic(this, &AASodaGun::LoadCheckColOverlapBegin);
	CL_SodaCanLoadBox->OnComponentEndOverlap.AddDynamic(this, &AASodaGun::LoadCheckColOverlapEnd);

	// After Find Reload SFX Wav asset , Setting SFX Sound 
	static ConstructorHelpers::FObjectFinder<USoundBase> SoundFinder_Reload(TEXT("/Game/VRContent/Sound/Wavs/SodaGunRelated/sfx_frg_reload.sfx_frg_reload"));
	if (SoundFinder_Reload.Succeeded())
	{
		SFXReloadCan = SoundFinder_Reload.Object;
	}

	// After Find Shot SFX Wav Asset , Setting SFX Sound
	static ConstructorHelpers::FObjectFinder<USoundBase> SoundFinder_Shoot(TEXT("/Game/VRContent/Sound/Wavs/SodaGunRelated/sfx_frg_shoot.sfx_frg_shoot"));
	if (SoundFinder_Shoot.Succeeded())
	{
		SFXShoot = SoundFinder_Shoot.Object;
	}

	// After Find Right And Left InputMappingContext Asset , Setting IMC 
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMCFinder_Right(TEXT("/Game/VRTemplate/Input/IMC_Weapon_Right.IMC_Weapon_Right"));
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMCFinder_Left(TEXT("/Game/VRTemplate/Input/IMC_Weapon_Left.IMC_Weapon_Left"));

	if (IMCFinder_Right.Succeeded() && IMCFinder_Left.Succeeded())
	{
		IMCRight = IMCFinder_Right.Object;
		IMCLeft = IMCFinder_Left.Object;
	}

	// After Find Right And Left InputAction Asset , Setting IA
	static ConstructorHelpers::FObjectFinder<UInputAction> IAFinder_RightShoot(TEXT("/Game/VRTemplate/Input/Actions/IA_Shoot_Right.IA_Shoot_Right"));
	static ConstructorHelpers::FObjectFinder<UInputAction> IAFinder_LeftShoot(TEXT("/Game/VRTemplate/Input/Actions/IA_Shoot_Left.IA_Shoot_Left"));
	if (IAFinder_RightShoot.Succeeded() && IAFinder_LeftShoot.Succeeded())
	{
		IARightShoot = IAFinder_RightShoot.Object;
		IALeftShoot = IAFinder_LeftShoot.Object;
	}

	static ConstructorHelpers::FClassFinder<AActor> ClassFinder_GunSmoke(TEXT("/Game/VRTemplate/Blueprints/Projectile.Projectile_C"));
	if (ClassFinder_GunSmoke.Succeeded())
	{
		SodaGunBulletSmoke = ClassFinder_GunSmoke.Class;
	}
}

void AASodaGun::BeginPlay()
{
	Super::BeginPlay();

	DefaultParentActor = GetAttachParentActor();

	DefaultTransform = FTransform(GetRootComponent()->GetRelativeRotation(), GetRootComponent()->GetRelativeLocation(), GetRootComponent()->GetRelativeScale3D());

	mPlayerCon = GetWorld()->GetFirstPlayerController(); // APlayerController Variable
	EIComp = Cast<UEnhancedInputComponent>(mPlayerCon->InputComponent); 
	// UEnhancedInputComponent Variable
	/*
	
	AController
	¦¦ APlayerController
		¦¦ UInputComponent
			¦¦ UEnhancedInputComponent

	- APlayerController has a UInputComponent Pointer
	- And above Inheritance structure looking , Casting UInputComponent To UEnhancedInputComponent is Possible.

	*/

	if (IsValid(EIComp)) // EnhancedInputComponent is Valid , IA Bind Execution Function
	{
		EIComp->BindAction(IARightShoot, ETriggerEvent::Started, this, &AASodaGun::OnShootFunc);
		EIComp->BindAction(IALeftShoot, ETriggerEvent::Started, this, &AASodaGun::OnShootFunc);
	}
}

void AASodaGun::OnGrabbed(UMotionControllerComponent& InMCRef, const FVector& HandGrabPos, class AVRHand* InGrabbingHand)
{
	if (InMCRef.ComponentHasTag(FName("RightMC")))
	{
		GC->SetRelativeLocation(FVector(0.0f, -2.8f, 0.0f));
	}
	else
	{
		GC->SetRelativeLocation(FVector(0.0f, 2.8f, 0.0f));
	}

	if (IsValid(mPlayerCon))
	{
		mLocalPlayer = mPlayerCon->GetLocalPlayer();
		if (IsValid(mLocalPlayer))
		{
			InputSubSystem = mLocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
			if (IsValid(InputSubSystem))
			{
				EnableInput(mPlayerCon);
				if (GC->GetHeldByHand() == EControllerHand::Right)
					InputSubSystem->AddMappingContext(IMCRight, 0);
				else
					InputSubSystem->AddMappingContext(IMCLeft, 0);
			}
		}
	}
}

void AASodaGun::OnDropped()
{
	if (!IsValid(DefaultParentActor))
	{
		//UE_LOG(LogTemp, Warning, TEXT("SodaGun DefaultParentActor invalid"));
		return;
	}
	else
	{
		GC->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f)); // GC Default Relative Location Back

		AttachToActor(DefaultParentActor, FAttachmentTransformRules::KeepWorldTransform);
		this->GetRootComponent()->SetRelativeTransform(DefaultTransform);

		DisableInput(mPlayerCon);
		if (GC->GetHeldByHand() == EControllerHand::Right)
		{
			InputSubSystem->RemoveMappingContext(IMCRight);
		}
		else
		{
			InputSubSystem->RemoveMappingContext(IMCLeft);
		}
		
		if (mPlayerCon)
		{
			mLocalPlayer = nullptr;
			InputSubSystem = nullptr;
		}
	}
}

void AASodaGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AASodaGun::LoadCheckColOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->ComponentHasTag(FName("SodaBullet")))
	{
		if (AttachingSodaBullet != 1)
		{
			EquipSodaBullet = Cast<AASodaBullet>(OtherActor);
			if (!IsValid(EquipSodaBullet))
			{
				return;
			}
			else
			{
				EquipSodaBullet->SetSBIsAttaching(1);
				EquipSodaBullet->OnDropped();
				EquipSodaBullet->GC->SetPrimitiveCompPhysics(false);
				EquipSodaBullet->GC->CurrentIGrabInterface = nullptr;
				EquipSodaBullet->AttachToComponent(CL_SodaCanLoadBox, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
				EquipSodaBullet->SetActorRelativeLocation(FVector(0.f, -6.f, 0.f));
				EquipSodaBullet->SetActorRelativeRotation(FRotator(0.f, 0.f, 90.f));

				mSoundPlayer->PlaySoundEffect(this, SFXReloadCan, CL_SodaCanLoadBox->GetComponentLocation());

				AttachingSodaBullet = 1;
				bIsLoading = 1;
			}
		}
		else
		{
			return;
		}
	}
}

void AASodaGun::LoadCheckColOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherComp->ComponentHasTag(FName("SodaBullet")))
	{
		if (OtherActor == EquipSodaBullet)
		{
			EquipSodaBullet = nullptr;
			AttachingSodaBullet = 0;
		}
	}
}

void AASodaGun::OnShootFunc()
{
	if (IsValid(EquipSodaBullet) && AttachingSodaBullet == 1)
	{
		mSoundPlayer->PlaySoundEffect(this, SFXShoot, MuzzleStandardLocation->GetComponentLocation());
		MuzzleStandardLocation->GetComponentTransform();

		GetWorld()->SpawnActor<AActor>(SodaGunBulletSmoke, MuzzleStandardLocation->GetComponentTransform());

		AttachingSodaBullet = 0;
		ShootAfterEjaculationFunc();
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Not Reloading SodaBullet"));
	}
}

void AASodaGun::ShootAfterEjaculationFunc()
{
	EquipSodaBullet->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	EquipSodaBullet->GetRootComponent()->SetWorldLocation(EjaculationStandardLocation->GetComponentLocation());

	EquipSodaBullet->GC->SetPrimitiveCompPhysics(true);
	EquipSodaBullet->CurveMove(EjaculationStandardLocation->GetRightVector() * -1.0f);
}