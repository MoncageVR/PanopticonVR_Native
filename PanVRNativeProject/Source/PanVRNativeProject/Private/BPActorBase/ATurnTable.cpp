// Fill out your copyright notice in the Description page of Project Settings.


#include "BPActorBase/ATurnTable.h"
#include "Core/Component/UGrabComp.h"
#include "Components/BoxComponent.h"
#include "Components/AudioComponent.h"
#include "Core/Debug/FDebugLib.h"
#include "BPActorBase/ALP.h"
#include "CoreObj/VREquipmentWorldSubsystem.h"

AATurnTable::AATurnTable()
{
	PrimaryActorTick.bCanEverTick = false;

	// Main Root Setting
	TTMainRoot = CreateDefaultSubobject<USceneComponent>("SceneComp");
	if (TTMainRoot)
	{
		this->SetRootComponent(TTMainRoot);
		ActorBaseMesh->SetupAttachment(TTMainRoot);
		TTMainRoot->SetMobility(EComponentMobility::Static);
		TTMainRoot->SetWorldScale3D(FVector(0.8f, 0.8f, 0.8f));
	}

	// Main Body Setting
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_MainBody(TEXT("/Game/VRContent/Modeling/TurnTable/TurnTableBody.TurnTableBody"));
	if (ModelingFinder_MainBody.Succeeded())
	{
		ActorBaseMesh->SetStaticMesh(ModelingFinder_MainBody.Object);
		ActorBaseMesh->SetMobility(EComponentMobility::Static);
	}

	// Glass Mesh Setting
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_Glass(TEXT("/Game/VRContent/Modeling/TurnTable/TurnTableGlass.TurnTableGlass"));
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialFinder_Glass_SW(TEXT("/Game/VRContent/Material/MI_Stage_TurnTableGlass1_SunnyWeather.MI_Stage_TurnTableGlass1_SunnyWeather"));
	TTGlass = CreateDefaultSubobject<UStaticMeshComponent>("SM_Glass");
	if (TTGlass)
	{
		TTGlass->SetupAttachment(ActorBaseMesh);
		TTGlass->SetMobility(EComponentMobility::Movable);
		if (ModelingFinder_Glass.Succeeded())
		{
			TTGlass->SetStaticMesh(ModelingFinder_Glass.Object);
		}
		if (MaterialFinder_Glass_SW.Succeeded())
		{
			TTGlass->SetMaterial(0, MaterialFinder_Glass_SW.Object);
		}
	}

	// Handle Lever And GrabComponent Setting
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_HandleLever(TEXT("/Game/VRContent/Modeling/TurnTable/TurnTableHandle.TurnTableHandle"));
	TTHandleLever = CreateDefaultSubobject<UStaticMeshComponent>("SM_HandleLever");
	if (TTHandleLever)
	{
		TTHandleLever->SetupAttachment(ActorBaseMesh);
		TTHandleLever->SetMobility(EComponentMobility::Movable);
		if (ModelingFinder_HandleLever.Succeeded())
		{
			TTHandleLever->SetStaticMesh(ModelingFinder_HandleLever.Object);
		}
		GC->SetupAttachment(TTHandleLever);
		GC->SetRelativeLocation(FVector(24.0f, 0.0f, 10.0f));
		GC->EEGrabType = EGrabType::HandToObj;
		GC->SetMobility(EComponentMobility::Movable);
	}

	LeverStandardSceneRoot = CreateDefaultSubobject<USceneComponent>("LeverRotateStandard");
	if (LeverStandardSceneRoot)
	{
		LeverStandardSceneRoot->SetupAttachment(TTHandleLever);
		LeverStandardSceneRoot->SetMobility(EComponentMobility::Movable);
		LeverStandardSceneRoot->SetRelativeLocation(FVector(24.0f, 0.0f, 10.0f));
	}

	// Left Button Setting
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_LB(TEXT("/Game/VRContent/Modeling/TurnTable/TurnTableLeftButton.TurnTableLeftButton"));
	TTLeftButton = CreateDefaultSubobject<UStaticMeshComponent>("SM_LeftButton");
	if (TTLeftButton)
	{
		TTLeftButton->SetupAttachment(ActorBaseMesh);
		TTLeftButton->SetMobility(EComponentMobility::Movable);
		if (ModelingFinder_LB.Succeeded())
		{
			TTLeftButton->SetStaticMesh(ModelingFinder_LB.Object);
		}
	}

	// Left Button Collision Setting
	TTCLLB = CreateDefaultSubobject<UBoxComponent>("LB_Collision");
	if (TTCLLB)
	{
		TTCLLB->SetupAttachment(TTLeftButton);
		TTCLLB->SetRelativeLocation(FVector(-8.7f, 5.0f, 23.25f));
		TTCLLB->SetBoxExtent(FVector(1.6f, 2.9f, 1.25f));
		TTCLLB->SetMobility(EComponentMobility::Movable);
		TTCLLB->SetHiddenInGame(false); // Debug
		TTCLLB->ShapeColor = FColor::Red; // Debug
	}

	// Right Button Setting
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_RB(TEXT("/Game/VRContent/Modeling/TurnTable/TurnTableRightButton.TurnTableRightButton"));
	TTRightButton = CreateDefaultSubobject<UStaticMeshComponent>("SM_RightButton");
	if (TTRightButton)
	{
		TTRightButton->SetupAttachment(ActorBaseMesh);
		TTRightButton->SetMobility(EComponentMobility::Movable);
		if (ModelingFinder_RB.Succeeded())
		{
			TTRightButton->SetStaticMesh(ModelingFinder_RB.Object);
		}
	}

	// Right Button Collision Setting
	TTCLRB = CreateDefaultSubobject<UBoxComponent>("RB_Collision");
	if (TTCLRB)
	{
		TTCLRB->SetupAttachment(TTRightButton);
		TTCLRB->SetRelativeLocation(FVector(5.6f, 5.0f, 23.25f));
		TTCLRB->SetBoxExtent(FVector(1.6f, 2.9f, 1.25f));
		TTCLRB->SetMobility(EComponentMobility::Movable);
		TTCLRB->SetHiddenInGame(false); // Debug
		TTCLRB->ShapeColor = FColor::Red; // Debug
	}

	CLLPTarget = CreateDefaultSubobject<UBoxComponent>("LPTarget_Collision");
	if (CLLPTarget)
	{
		CLLPTarget->SetupAttachment(ActorBaseMesh);
		CLLPTarget->SetRelativeLocation(FVector(0.0f, -11.5f, 37.4f));
		CLLPTarget->SetBoxExtent(FVector(3.0f, 3.0f, 3.0f));
		CLLPTarget->SetHiddenInGame(false); // Debug
	}

	CLLPSpawn = CreateDefaultSubobject<UBoxComponent>("LPSpawn_CheckCollision");
	if (CLLPSpawn)
	{
		CLLPSpawn->SetupAttachment(ActorBaseMesh);
		CLLPSpawn->SetRelativeLocation(FVector(0.0f, -8.0f, 5.7f));
		CLLPSpawn->SetBoxExtent(FVector(17.5f, 17.5f, 1.0f));
		CLLPSpawn->SetHiddenInGame(false); // Debug
	}

	// Left And Right Button Overlap Function Mapping Part
	TTCLLB->OnComponentBeginOverlap.AddDynamic(this, &AATurnTable::OverlapLBBoxBegin);
	TTCLLB->OnComponentEndOverlap.AddDynamic(this, &AATurnTable::OverlapLBBoxEnd);
	TTCLRB->OnComponentBeginOverlap.AddDynamic(this, &AATurnTable::OverlapRBBoxBegin);
	TTCLRB->OnComponentEndOverlap.AddDynamic(this, &AATurnTable::OverlapRBBoxEnd);
	CLLPTarget->OnComponentBeginOverlap.AddDynamic(this, &AATurnTable::OverlapLPBoxBegin);
	CLLPTarget->OnComponentEndOverlap.AddDynamic(this, &AATurnTable::OverlapLPBoxEnd);
	CLLPSpawn->OnComponentEndOverlap.AddDynamic(this, &AATurnTable::OverlapSpawnLPBoxEnd);

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialFinder_Glass_PE(TEXT("/Game/VRContent/Material/MI_Stage_TurnTableGlass2_PrisonerElevator.MI_Stage_TurnTableGlass2_PrisonerElevator"));
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialFinder_Glass_FJ(TEXT("/Game/VRContent/Material/MI_Stage_TurnTableGlass3_FastJazz.MI_Stage_TurnTableGlass3_FastJazz"));
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialFinder_Glass_EP(TEXT("/Game/VRContent/Material/MI_Stage_TurnTableGlass4_ElectricPickle.MI_Stage_TurnTableGlass4_ElectricPickle"));

	if (MaterialFinder_Glass_SW.Succeeded()) { TTGlassMats.Add(MaterialFinder_Glass_SW.Object); }
	if (MaterialFinder_Glass_PE.Succeeded()) { TTGlassMats.Add(MaterialFinder_Glass_PE.Object); }
	if (MaterialFinder_Glass_FJ.Succeeded()) { TTGlassMats.Add(MaterialFinder_Glass_FJ.Object); }
	if (MaterialFinder_Glass_EP.Succeeded()) { TTGlassMats.Add(MaterialFinder_Glass_EP.Object); }

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialFinder_TTPublicMat(TEXT("/Game/VRContent/Material/SRS_STAGE_Main.SRS_STAGE_Main"));
	if (MaterialFinder_TTPublicMat.Succeeded())
	{
		ActorBaseMesh->SetMaterial(0, MaterialFinder_TTPublicMat.Object);
		TTHandleLever->SetMaterial(0, MaterialFinder_TTPublicMat.Object);
		TTLeftButton->SetMaterial(0, MaterialFinder_TTPublicMat.Object);
		TTRightButton->SetMaterial(0, MaterialFinder_TTPublicMat.Object);
	}

	// Sound Asset Pointer Setting
	static ConstructorHelpers::FObjectFinder<USoundBase> SoundFinder_LPDropSound(TEXT("/Game/VRContent/Sound/Wavs/TurnTable/sfx_turntable_set.sfx_turntable_set"));
	if (SoundFinder_LPDropSound.Succeeded())
	{
		SFXLPDropSound = SoundFinder_LPDropSound.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> SoundFinder_ButtonPressSound(TEXT("/Game/VRContent/Sound/Wavs/TurnTable/sfx_turntable_buttonpress.sfx_turntable_buttonpress"));
	if (SoundFinder_ButtonPressSound.Succeeded())
	{
		SFXButtonPressSound = SoundFinder_ButtonPressSound.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> SoundFinder_LeverPullSound(TEXT("/Game/VRContent/Sound/Wavs/TurnTable/sfx_turntable_leverpull.sfx_turntable_leverpull"));
	if (SoundFinder_LeverPullSound.Succeeded())
	{
		SFXLeverPullSound = SoundFinder_LeverPullSound.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> SoundFinder_TTBGMCues(TEXT("/Game/VRContent/Sound/Ques/TTBGM/TTBGM_Cues.TTBGM_Cues"));
	if (SoundFinder_TTBGMCues.Succeeded())
	{
		SCSfxTTBGMCues = SoundFinder_TTBGMCues.Object;
	}

	TTSCAudioPlayer = CreateDefaultSubobject<UAudioComponent>("SC_AudioComp");
	if (TTSCAudioPlayer)
	{
		TTSCAudioPlayer->SetupAttachment(TTMainRoot);
		TTSCAudioPlayer->SetAutoActivate(false);
		TTSCAudioPlayer->bAllowSpatialization = false;
	}

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

void AATurnTable::BeginPlay()
{
	Super::BeginPlay();

	EquipmentRegistrable(this);
	if (EquipmentWorldSubSystem)
	{
		EquipmentWorldSubSystem->FGameStartSignature.BindUObject(this, &AATurnTable::HandleTTReceiveByGTWLever);
	}

	// TurnTable Default LP Spawn Function Call
	SpawnDefaultLP(CLLPTarget->GetComponentLocation());

	if (TTSCAudioPlayer && SCSfxTTBGMCues)
	{
		TTSCAudioPlayer->Stop();
		TTSCAudioPlayer->SetSound(SCSfxTTBGMCues);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Audio Component Not Exist!"));
	}
}

void AATurnTable::EquipmentRegistrable(AActor* InActor)
{
	Super::EquipmentRegistrable(InActor);
}

void AATurnTable::Tick(float DeletaTime)
{
	Super::Tick(DeletaTime);
}

// Game Start First Time, Default Equipped LP Spawn Function
void AATurnTable::SpawnDefaultLP(FVector SpawnPos)
{
	FRotator TempSpawnRot = FRotator(75.0f, -90.0f, 0.0f); // Temporary Attaching Rotation
	AttachingLP = GetWorld()->SpawnActor<AALP>(SpawnPos, FRotator(0.0f, 0.0f, 0.0f)); // LP Spawn Part
	AttachingLP->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale); // Spawn LP Attaching TurnTable
	AttachingLP->SetActorRelativeLocation(CLLPTarget->GetRelativeLocation()); // After Attaching, Setting Position
	AttachingLP->SetActorRelativeRotation(TempSpawnRot); // After Attaching, Setting Rotation
	AttachingLP->SetSongNum(CurrSongNum); // Default LP Song Num is 0(Zero)
	AttachingLP->SetIsAttaching(true); // Setting a Flag Variable to Check Attachment

	PlaySongNum = AttachingLP->GetSongNum();

	return;
}

void AATurnTable::AdjustGCPosNRotByGrabbed()
{
	if (TempMCRef->ComponentHasTag(FName("RightMC")))
	{
		FVector TempGCPos = FVector(26.0f, 7.0f, 10.0f);
		FRotator TempGCRot = FRotator(90.0f, 0.0f, 180.0f);
		GC->SetRelativeLocation(TempGCPos);
		GC->SetRelativeRotation(TempGCRot);
	}
	else
	{
		FVector TempGCPos = FVector(22.0f, 7.0f, 10.0f);
		FRotator TempGCRot = FRotator(-90.0f, 0.0f, -180.0f);
		GC->SetRelativeLocation(TempGCPos);
		GC->SetRelativeRotation(TempGCRot);
	}
}

void AATurnTable::OperateTTLeverMovement()
{
	FTransform Between_MCAndStandard_MakeRelativeTransform = TempMCRef->GetComponentTransform().GetRelativeTransform(LeverStandardSceneRoot->GetComponentTransform());
	FVector LeverMoveDirection = LeverStandardSceneRoot->GetRelativeLocation() - Between_MCAndStandard_MakeRelativeTransform.GetLocation();

	float FinalLeverRoll = FMath::Clamp(TTHandleLever->GetRelativeRotation().Roll - LeverMoveDirection.Y, 0.0f, 179.0f);

	TTHandleLever->SetRelativeRotation(FRotator(TTHandleLever->GetRelativeRotation().Pitch, TTHandleLever->GetRelativeRotation().Yaw, FinalLeverRoll));

	// When the End of the Lever Movement Range is Reached, Spawn New LP Function Call
	if (TTHandleLever->GetRelativeRotation().Roll >= 179.0f && TTHandleLever->GetRelativeRotation().Roll <= 180.0f)
	{
		if (!bIsAlreadySpawnLP) // Check Already Spawn LP Exist?
		{
			mSoundPlayer->PlaySoundEffect(this, SFXLeverPullSound, TTHandleLever->GetComponentLocation());
			SpawnNewLP();
			this->OnDropped();
		}
	}
}

void AATurnTable::UpdateNewSpawnLPLocation()
{
	if (NewSpawnLP)
	{
		NewSpawnLP->AddActorLocalOffset(FVector(0.0f, 0.05f, 0.0f));
	}

	// Check LP Correct Position, Correct Position LP Stop Move 
	if (NewSpawnLP->GetRootComponent()->GetRelativeLocation().Y >= 8.0f)
	{
		GetWorldTimerManager().PauseTimer(LPMovementTimer);
		GetWorldTimerManager().ClearTimer(LPMovementTimer);
		NewSpawnLP->SetMeshCollisionEnabled(true);
		bIsAlreadySpawnLP = true;
	}
}

// In Reality New LP Spawn Function
void AATurnTable::SpawnNewLP()
{
	if (!NewSpawnLP)
	{
		FVector TempSpawnPos = FVector(0.0f, -11.0f, 5.7f); // Temporary Variable, New LP Spawn Relative Position
		NewSpawnLP = GetWorld()->SpawnActor<AALP>(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f)); // In Reality Spawn Part

		NewSpawnLP->AttachToComponent(ActorBaseMesh, FAttachmentTransformRules::SnapToTargetIncludingScale); // Attach TurnTable Body
		NewSpawnLP->SetActorRelativeLocation(TempSpawnPos); // After Attach, Setting LP Position
		NewSpawnLP->SetSongNum(CurrSongNum); // Current Song Num Setting
		NewSpawnLP->SetMeshCollisionEnabled(false); // Don't Catch LP Setting Collsion Enabled
	}
}

void AATurnTable::OperateLPRotationInPlace()
{
	if (AttachingLP)
	{
		float TempRotationSpeed = 1.0f;

		FQuat TempQuat =
			(AttachingLP->GetRootComponent()->GetRelativeRotation().Quaternion()) *
			(FQuat(FVector(0.0f, 0.0f, 1.0f), FMath::DegreesToRadians(TempRotationSpeed)));

		AttachingLP->GetRootComponent()->SetRelativeRotation(TempQuat.Rotator());
	}

	return;
}

void AATurnTable::OnGrabbed(UMotionControllerComponent& InMCRef, const FVector& HandGrabPos, class AVRHand* InGrabbingHand)
{
	TempMCRef = &InMCRef;

	if (TempMCRef)
	{
		if (bIsLPSpawnOperation) // If Spawning New Lp, Don't Execute, Lever Movement Function
		{
			return;
		}
		else // If Not Spawning New LP, Execute Lever Movement Function
		{
			AdjustGCPosNRotByGrabbed(); // Grabbed Setting GC
			GetWorldTimerManager().SetTimer(
				TTLeverMovementTimer,
				this,
				&AATurnTable::OperateTTLeverMovement,
				0.01f,
				true
			);
		}
	}

	return;
}

void AATurnTable::OnDropped()
{
	TempMCRef = nullptr;
	GetWorldTimerManager().ClearTimer(TTLeverMovementTimer);
	TTHandleLever->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));

	if (NewSpawnLP != nullptr) // Don't Execute, if Spawn New Lp
	{
		if (!bIsAlreadySpawnLP) // Don't Execute, if Already Spawn new LP
		{
			bIsLPSpawnOperation = true;
			GetWorldTimerManager().SetTimer(
				LPMovementTimer,
				this,
				&AATurnTable::UpdateNewSpawnLPLocation,
				0.01f,
				true
			);
		}
	}

	return;
}

// Left Button Click Function
void AATurnTable::OverlapLBBoxBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bIsTouching)
	{
		if (OtherComp->ComponentHasTag(FName("HandIndex")))
		{
			mSoundPlayer->PlaySoundEffect(this, SFXButtonPressSound, TTLeftButton->GetComponentLocation());
			bIsTouching = 1;
			if (CurrSongNum == 0) { CurrSongNum = 3; }
			else { CurrSongNum -= 1; }

			TTGlass->SetMaterial(0, TTGlassMats[CurrSongNum]);
		}
	}
}

void AATurnTable::OverlapLBBoxEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (bIsTouching)
	{
		if (OtherComp->ComponentHasTag(FName("HandIndex")))
			bIsTouching = 0;
	}
}

// Right Button Click Function
void AATurnTable::OverlapRBBoxBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bIsTouching)
	{
		if (OtherComp->ComponentHasTag(FName("HandIndex")))
		{
			mSoundPlayer->PlaySoundEffect(this, SFXButtonPressSound, TTRightButton->GetComponentLocation());
			bIsTouching = 1;
			if (CurrSongNum == 3) { CurrSongNum = 0; }
			else { CurrSongNum += 1; }

			TTGlass->SetMaterial(0, TTGlassMats[CurrSongNum]);
		}
	}
}

void AATurnTable::OverlapRBBoxEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (bIsTouching)
	{
		if (OtherComp->ComponentHasTag(FName("HandIndex")))
			bIsTouching = 0;
	}
	return;
}

void AATurnTable::OverlapLPBoxBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!AttachingLP)
	{
		if (OtherComp->ComponentHasTag("LP"))
		{
			AALP* TempOverlapEndLP = Cast<AALP>(OtherComp->GetAttachParentActor());

			if (TempOverlapEndLP)
			{
				AttachingLP = TempOverlapEndLP;
			}

			if (AttachingLP)
			{
				AttachingLP->SetIsAttaching(true);
				AttachingLP->GC->GCTryRelease();
				AttachingLP->GC->SetPrimitiveCompPhysics(false);

				FRotator TempSpawnRot = FRotator(75.0f, -90.0f, 0.0f); // Temporary Attaching Rotation
				AttachingLP->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
				AttachingLP->GetRootComponent()->SetRelativeLocationAndRotation(CLLPTarget->GetRelativeLocation(), TempSpawnRot);
				PlaySongNum = AttachingLP->GetSongNum();

				mSoundPlayer->PlaySoundEffect(this, SFXLPDropSound, CLLPTarget->GetComponentLocation());
				PlaySoundBGM(PlaySongNum);

				if (AttachingLP->GetIsAttaching() && bIsGameStarted)
				{
					GetWorldTimerManager().SetTimer(
						LPRotationInPlaceTimer,
						this,
						&AATurnTable::OperateLPRotationInPlace,
						0.01f,
						true
					);
				}
				else
				{
					GetWorldTimerManager().PauseTimer(LPRotationInPlaceTimer);
					GetWorldTimerManager().ClearTimer(LPRotationInPlaceTimer);
				}
			}
		}
	}
	else
	{
		return;
	}

	return;
}

void AATurnTable::OverlapLPBoxEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AttachingLP)
	{
		if (OtherComp->ComponentHasTag("LP"))
		{
			AALP* TempOverlapEndLP = Cast<AALP>(OtherComp->GetAttachParentActor());
			if (TempOverlapEndLP)
			{
				if (AttachingLP == TempOverlapEndLP)
				{
					AttachingLP->SetIsAttaching(false);
					AttachingLP = nullptr;
					TTSCAudioPlayer->Stop();

					if (GetWorldTimerManager().IsTimerActive(LPRotationInPlaceTimer))
					{
						GetWorldTimerManager().PauseTimer(LPRotationInPlaceTimer);
						GetWorldTimerManager().ClearTimer(LPRotationInPlaceTimer);
					}

					return;
				}
			}
		}
	}

	return;
}

// New Spawn LP Overlpa End Check Function
void AATurnTable::OverlapSpawnLPBoxEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Check NewSpawnLP And Overlap End LP is Same
	if (NewSpawnLP)
	{
		AALP* TempOverlapEndLP = Cast<AALP>(OtherActor);

		if (NewSpawnLP == TempOverlapEndLP)
		{
			if (OtherComp->ComponentHasTag("LP"))
			{
				bIsAlreadySpawnLP = false;
				bIsLPSpawnOperation = false;
				NewSpawnLP = nullptr;
			}
		}

	}
	else
		return;

	return;
}

void AATurnTable::PlaySoundBGM(int TempCueNum)
{
	if (bIsGameStarted)
	{
		TTSCAudioPlayer->SetIntParameter(FName("IntCue"), TempCueNum);
		TTSCAudioPlayer->Play();
		return;
	}
	else
	{
		return;
	}
}

void AATurnTable::HandleTTReceiveByGTWLever(bool TempGameStartFlag)
{
	if (AttachingLP && TTSCAudioPlayer)
	{
		bIsGameStarted = TempGameStartFlag;
		PlaySoundBGM(PlaySongNum);

		GetWorldTimerManager().SetTimer(
			LPRotationInPlaceTimer,
			this,
			&AATurnTable::OperateLPRotationInPlace,
			0.01f,
			true
		);
	}
}
