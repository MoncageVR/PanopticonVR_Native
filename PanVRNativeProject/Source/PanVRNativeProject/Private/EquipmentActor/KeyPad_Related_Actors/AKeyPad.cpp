#include "EquipmentActor/KeyPad_Related_Actors/AKeyPad.h"
#include "CoreCommon/VRPawn/AScopeCamera.h"
#include "CoreObj/Manager/WorldSubSystem/VREquipmentWorldSubsystem.h"
#include "CoreCommon/VRPawn/CVRPawn.h"
#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/TimelineComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"

AAKeyPad::AAKeyPad()
{
	// This actor does not need a per-frame update
	PrimaryActorTick.bCanEverTick = true;

	// Create the root scene component and set it as the actor's root
	SC_MainRoot = CreateDefaultSubobject<USceneComponent>("MainSceneRootComp");
	if (SC_MainRoot)
	{
		this->SetRootComponent(SC_MainRoot);
		//SC_MainRoot->SetRelativeScale3D(FVector(0.7f));
	}

	// Load the keypad main body mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_MainBody(TEXT("/Game/VRContent/Modeling/13_Keypad/SM_KeyPad_MainBody.SM_KeyPad_MainBody"));
	if (ActorBaseMesh && ModelingFinder_MainBody.Succeeded())
	{
		// Attach the main body to the root and set its mesh
		ActorBaseMesh->SetupAttachment(SC_MainRoot);
		ActorBaseMesh->SetStaticMesh(ModelingFinder_MainBody.Object);
		ActorBaseMesh->SetRelativeLocation(FVector(10.0f, 30.0f, 165.0f));
		ActorBaseMesh->SetRelativeRotation(FRotator(0.f));
		ActorBaseMesh->SetRelativeScale3D(FVector(1.0f));
	}

	// Prepare the array that holds the 9 number-key meshes
	SM_Keys.Empty();
	SM_Keys.Reserve(9);

	// Load the shared material used by every input key (loaded once)
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MatFinder_InputKey(TEXT("/Game/VRContent/Material/SRS_Stage_KeypadIN_Key.SRS_Stage_KeypadIN_Key"));
	// Build 9 number keys (Key01~Key09): each gets a mesh, material, and box collision
	for (int i = 1; i < 10; i++)
	{
		// Make a unique component name, e.g. Key01_SMComp
		FString KeyName = FString::Printf(TEXT("Key0%d_SMComp"), i);
		UStaticMeshComponent* TempKey = CreateDefaultSubobject<UStaticMeshComponent>(*KeyName);
		SM_Keys.Add(TempKey);

		// Build this key's mesh path (e.g. SM_KeyPad_Key01) and load it
		FString KeyModelingPath = FString::Printf(TEXT("/Game/VRContent/Modeling/13_Keypad/SM_KeyPad_Key0%d.SM_KeyPad_Key0%d"), i, i);

		ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_Key(*KeyModelingPath);
		if (ModelingFinder_Key.Succeeded())
		{
			// Set the loaded mesh and attach the key under the main body mesh
			SM_Keys[i - 1]->SetStaticMesh(ModelingFinder_Key.Object);
			SM_Keys[i - 1]->SetupAttachment(ActorBaseMesh);
		}

		// Apply the shared input-key material
		if (MatFinder_InputKey.Succeeded())
		{
			SM_Keys[i - 1]->SetMaterial(0, MatFinder_InputKey.Object);
		}

		// Create a box collision so this key can detect a press
		FString KeyColName = FString::Printf(TEXT("Key0%d_CLComp"), i);
		UBoxComponent* TempKeyCollision = CreateDefaultSubobject<UBoxComponent>(*KeyColName);
		CL_Keys.Add(TempKeyCollision);

		// Attach the collision to its matching key mesh
		CL_Keys[i - 1]->SetupAttachment(SM_Keys[i - 1]);
	}

	// Setting Modeling And Material Key : Pickle 
	SM_PickleKey = CreateDefaultSubobject<UStaticMeshComponent>("PickleKey_SMComp");
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_PickleKey(TEXT("/Game/VRContent/Modeling/13_Keypad/SM_KeyPad_Key10_Pickle.SM_KeyPad_Key10_Pickle"));
	if (SM_PickleKey && ModelingFinder_PickleKey.Succeeded())
	{
		SM_PickleKey->SetupAttachment(ActorBaseMesh);
		SM_PickleKey->SetStaticMesh(ModelingFinder_PickleKey.Object);
		SM_PickleKey->SetMaterial(0, MatFinder_InputKey.Object);
	}

	// Setting Collison Key : Pickle
	CL_PickleKey = CreateDefaultSubobject<UBoxComponent>("PickleKey_ColComp");
	if (CL_PickleKey)
	{
		CL_PickleKey->SetupAttachment(SM_PickleKey);
		CL_PickleKey->SetRelativeLocation(FVector(9.7f, 1.58f, 0.f));
		CL_PickleKey->SetBoxExtent(FVector(6.0f, 2.0f, 3.0f));
	}

	// Setting Modeling And Material Key : BackSpace
	SM_BackSpaceKey = CreateDefaultSubobject<UStaticMeshComponent>("BackSpaceKey_SMComp");
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_BackspaceKey(TEXT("/Game/VRContent/Modeling/13_Keypad/SM_KeyPad_Key11_Backspace.SM_KeyPad_Key11_Backspace"));
	if (SM_BackSpaceKey && ModelingFinder_BackspaceKey.Succeeded())
	{
		SM_BackSpaceKey->SetupAttachment(ActorBaseMesh);
		SM_BackSpaceKey->SetStaticMesh(ModelingFinder_BackspaceKey.Object);
		SM_BackSpaceKey->SetMaterial(0, MatFinder_InputKey.Object);
	}

	// Setting Collison Key : BackSpace
	CL_BackSpaceKey = CreateDefaultSubobject<UBoxComponent>("BackspaceKey_ColComp");
	if (CL_BackSpaceKey)
	{
		CL_BackSpaceKey->SetupAttachment(SM_BackSpaceKey);
		CL_BackSpaceKey->SetRelativeLocation(FVector(9.7f, 1.58f, -6.1f));
		CL_BackSpaceKey->SetBoxExtent(FVector(6.0f, 2.0f, 3.0f));
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_Monitor(TEXT("/Game/VRContent/Modeling/13_Keypad/SM_KeyPad_Monitor.SM_KeyPad_Monitor"));
	SM_Monitor = CreateDefaultSubobject<UStaticMeshComponent>("Monitor_SMComp");
	if (SM_Monitor && ModelingFinder_Monitor.Succeeded())
	{
		SM_Monitor->SetupAttachment(ActorBaseMesh);
		SM_Monitor->SetStaticMesh(ModelingFinder_Monitor.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MatFinder_Main(TEXT("/Game/VRContent/Material/SRS_STAGE_Main.SRS_STAGE_Main"));
	if (MatFinder_Main.Succeeded())
	{
		// Apply the main material to the body mesh
		ActorBaseMesh->SetMaterial(0, MatFinder_Main.Object);
	}

	// Setting KeyPad ScoreBoard
	SC_ScoreBoardRoot = CreateDefaultSubobject<USceneComponent>("KeyPad_ScoreBoard_RootComp");
	if (SC_ScoreBoardRoot)
	{
		// Attach the scoreboard root and position it on the keypad
		SC_ScoreBoardRoot->SetupAttachment(SC_MainRoot);
		SC_ScoreBoardRoot->SetRelativeLocation(FVector(12.24f, 32.6f, 15.0f));
		SC_ScoreBoardRoot->SetRelativeScale3D(FVector(1.0f));
	}

	// Setting KeyPad ScoreBoard : Bar
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_Bar(TEXT("/Game/VRContent/Modeling/13_Keypad/SM_KeyPad_Bar.SM_KeyPad_Bar"));
	SM_KeyPad_Bar = CreateDefaultSubobject<UStaticMeshComponent>("KeyPadBar_SMComp");
	if (SM_KeyPad_Bar && ModelingFinder_Bar.Succeeded())
	{
		SM_KeyPad_Bar->SetupAttachment(SC_ScoreBoardRoot);
		// Place the scoreboard bar and set its mesh and material
		SM_KeyPad_Bar->SetRelativeLocation(FVector(0.f, 60.0f, -13.0f));
		SM_KeyPad_Bar->SetStaticMesh(ModelingFinder_Bar.Object);
		SM_KeyPad_Bar->SetMaterial(0, MatFinder_Main.Object);
	}

	// Setting KeyPad ScoreBoard : ScoreBoard Bodys
	// Prepare arrays for the scoreboard bodies, glasses, and text (4 each)
	SM_ScoreBoard_Bodys.Empty();
	SM_ScoreBoard_Bodys.Reserve(4);

	SM_ScoreBoard_Glasses.Empty();
	SM_ScoreBoard_Glasses.Reserve(4);

	TR_ScoreBoard_Text.Empty();
	TR_ScoreBoard_Text.Reserve(4);

	// Load the shared scoreboard glass mesh and material (loaded once)
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_Glass(TEXT("/Game/VRContent/Modeling/13_Keypad/SM_KeyPad_Out_Glass.SM_KeyPad_Out_Glass"));
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MatFinder_Glass(TEXT("/Game/VRContent/Material/SRS_Stage_KeyPadGlass.SRS_Stage_KeyPadGlass"));

	// Build 4 scoreboard digits: each has a body, a glass cover, and a text label
	for (int j = 1; j < 5; j++)
	{
		// Create the scoreboard body and attach it under the scoreboard root
		FString ScoreBoardBodyName = FString::Printf(TEXT("ScoreBoardBody0%d_SMComp"), j);
		UStaticMeshComponent* TempScoreBoardBody = CreateDefaultSubobject<UStaticMeshComponent>(*ScoreBoardBodyName);
		SM_ScoreBoard_Bodys.Add(TempScoreBoardBody);
		SM_ScoreBoard_Bodys[j - 1]->SetupAttachment(SC_ScoreBoardRoot);

		// Create the glass cover and attach it on top of the body
		FString ScoreBoardGlassName = FString::Printf(TEXT("ScoreBoardGlass0%d_SMComp"), j);
		UStaticMeshComponent* TempScoreBoardGlass = CreateDefaultSubobject<UStaticMeshComponent>(*ScoreBoardGlassName);
		SM_ScoreBoard_Glasses.Add(TempScoreBoardGlass);
		SM_ScoreBoard_Glasses[j - 1]->SetupAttachment(SM_ScoreBoard_Bodys[j - 1]);

		// Build this body's mesh path (e.g. SM_KeyPad_Out001) and load it
		FString ScoreBoardBodyPath = FString::Printf(TEXT("/Game/VRContent/Modeling/13_Keypad/SM_KeyPad_Out00%d.SM_KeyPad_Out00%d"), j, j);
		static ConstructorHelpers::FObjectFinder<UStaticMesh> ModelingFinder_Body(*ScoreBoardBodyPath);
		if (ModelingFinder_Body.Succeeded())
		{
			// Set the body mesh and apply the main material
			SM_ScoreBoard_Bodys[j - 1]->SetStaticMesh(ModelingFinder_Body.Object);
			SM_ScoreBoard_Bodys[j - 1]->SetMaterial(0, MatFinder_Main.Object);
		}

		// Apply the shared glass mesh and material to the cover
		if (ModelingFinder_Glass.Succeeded())
			SM_ScoreBoard_Glasses[j - 1]->SetStaticMesh(ModelingFinder_Glass.Object);

		if (MatFinder_Glass.Succeeded())
			SM_ScoreBoard_Glasses[j - 1]->SetMaterial(0, MatFinder_Glass.Object);

		// Create the text label component for this digit
		FString ScoreBoardTextName = FString::Printf(TEXT("ScoreBoardText0%d_TextRenderComp"), j);
		UTextRenderComponent* TempScoreBoardText = CreateDefaultSubobject<UTextRenderComponent>(*ScoreBoardTextName);
		TR_ScoreBoard_Text.Add(TempScoreBoardText);

		// Attach the text and set its color, alignment, and size
		TR_ScoreBoard_Text[j - 1]->SetupAttachment(SM_ScoreBoard_Glasses[j - 1]);
		TR_ScoreBoard_Text[j - 1]->SetTextRenderColor(FColor::White);
		TR_ScoreBoard_Text[j - 1]->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
		TR_ScoreBoard_Text[j - 1]->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);
		TR_ScoreBoard_Text[j - 1]->SetWorldSize(10.0f);

		// Debug
		FString TempNum = FString::Printf(TEXT("%d"), j);
		TR_ScoreBoard_Text[j - 1]->SetText(FText::FromString(TempNum));
		// Debug
	}

	// Arrange the 9 key collisions as a 3x3 grid: (i%3)->column X, (i/3)->row Z
	if (CL_Keys.Num() > 0)
	{
		for (int32 i = 0; i < CL_Keys.Num(); i++)
		{
			const float X = -12.0f + (i % 3) * 6.0f;
			const float Z = 6.0f - (i / 3) * 6.0f;
			CL_Keys[i]->SetRelativeLocation(FVector(X, 1.5f, Z));
			CL_Keys[i]->SetBoxExtent(FVector(2.5f, 1.75f, 2.5f));
		}
	}

	CL_Keys.Add(CL_PickleKey);
	CL_Keys.Add(CL_BackSpaceKey);

	// Place the 4 scoreboard bodies side by side
	if (SM_ScoreBoard_Bodys.Num() > 0)
	{
		SM_ScoreBoard_Bodys[0]->SetRelativeLocation(FVector(-21.1f, -68.9f, 0.0f));
		SM_ScoreBoard_Bodys[1]->SetRelativeLocation(FVector(-7.0f, -68.9f, 0.0f));
		SM_ScoreBoard_Bodys[2]->SetRelativeLocation(FVector(7.1f, -68.9f, 0.0f));
		SM_ScoreBoard_Bodys[3]->SetRelativeLocation(FVector(21.2f, -68.9f, 0.0f));
	}

	// Center each scoreboard text and face it forward
	if (TR_ScoreBoard_Text.Num() > 0)
	{
		for (int32 k = 0; k < TR_ScoreBoard_Text.Num(); k++)
		{
			TR_ScoreBoard_Text[k]->SetRelativeLocation(FVector(0.f, 14.1f, 3.0f));
			TR_ScoreBoard_Text[k]->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
		}
	}

	if (MatFinder_Glass.Succeeded())
		SM_Monitor->SetMaterial(0, MatFinder_Glass.Object);

	if (GC)
	{
		GC->SetupAttachment(ActorBaseMesh);
		GC->EEGrabType = EGrabType::HandToObj;
	}

	for (UBoxComponent* KeyCol : CL_Keys)
	{
		if (KeyCol)
		{
			KeyCol->OnComponentBeginOverlap.AddDynamic(this, &AAKeyPad::OnKeyOverlapBegin);
		}
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> SFXFinder_Button(TEXT("/Game/VRContent/Sound/Wavs/KeyPad/sfx_keypad_button.sfx_keypad_button"));
	static ConstructorHelpers::FObjectFinder<USoundBase> SFXFinder_Return(TEXT("/Game/VRContent/Sound/Wavs/KeyPad/sfx_keypad_return.sfx_keypad_return"));
	if (SFXFinder_Button.Succeeded() && SFXFinder_Return.Succeeded())
	{
		ButtonPressSFX = SFXFinder_Button.Object;
		ReturnButtonPressSFX = SFXFinder_Return.Object;
	}

	FinalOutputTextLength = 4;
	bIsScopeFlag = false;

	CL_PerisScope = CreateDefaultSubobject<UBoxComponent>("PerisScope_ColComp");
	if (CL_PerisScope)
	{
		CL_PerisScope->SetupAttachment(ActorBaseMesh);
		CL_PerisScope->SetRelativeLocation(FVector(0.0f, 14.0f, -18.0f));
		CL_PerisScope->SetBoxExtent(FVector(10.0f, 20.0f, 10.0f));
		CL_PerisScope->SetGenerateOverlapEvents(false);
		CL_PerisScope->OnComponentBeginOverlap.AddDynamic(this, &AAKeyPad::OnCameraOverlapBegin);
		CL_PerisScope->OnComponentEndOverlap.AddDynamic(this, &AAKeyPad::OnCameraOverlapEnd);
	}

	KeyPadMoveTimelineComp = CreateDefaultSubobject<UTimelineComponent>("MoveTLComp");
	KeyPadMoveTimelineComp->SetLooping(false);
	KeyPadMoveTimelineComp->SetTimelineLength(3.01f);

	static ConstructorHelpers::FObjectFinder<UCurveFloat> CurveFinder_KeyPadMove(TEXT("/Game/VRContent/Blueprints/TimelineCurve/KeyPadBody_UpNDown_Curve.KeyPadBody_UpNDown_Curve"));
	if (CurveFinder_KeyPadMove.Succeeded())
	{
		KeyPadMove_CurveF = CurveFinder_KeyPadMove.Object;
	}

	// Debug
	TArray<UBoxComponent*> AllCollisionComps;
	GetComponents<UBoxComponent>(AllCollisionComps);
	for (UBoxComponent* AllColComp : AllCollisionComps)
	{
		if (!AllColComp) continue;

		if (AllColComp->bHiddenInGame)
			AllColComp->SetHiddenInGame(false);
		else
			continue;
	}
	// Debug

	// Disable navigation on all components so they do not affect the NavMesh
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

void AAKeyPad::BeginPlay()
{
	Super::BeginPlay();
	FinalOutputIntArrays.Empty();
	FinalOutputTextArrays.Empty();

	this->EquipmentRegistrable(this);

	EquipmentWorldSubSystem->FKeyPadArrClearSignature.BindUObject(this, &AAKeyPad::HandleKeyPadReceiveByEmergencyButton);

	EquipmentWorldSubSystem->FToggleToKeyPadSignature.BindUObject(this, &AAKeyPad::HandleKeyPadMove);

	FOnTimelineFloat MoveProgressFunc;
	FOnTimelineEvent MoveFinishedEvent;

	MoveProgressFunc.BindUFunction(this, FName("KeyPadMovePlayEvent"));
	MoveFinishedEvent.BindUFunction(this, FName("KeyPadMoveFinishedEvent"));

	KeyPadMoveTimelineComp->AddInterpFloat(KeyPadMove_CurveF, MoveProgressFunc);
	KeyPadMoveTimelineComp->SetTimelineFinishedFunc(MoveFinishedEvent);

	// For Initialize, Clear Output Array 
	for (UTextRenderComponent* TextRender : TR_ScoreBoard_Text)
	{
		TextRender->SetText(FText::GetEmpty());
	}
	CurrTextLength = 0;
	FinalOutputIntArrays.Empty();
	FinalOutputTextArrays.Empty();
	GetWorld()->GetTimerManager().PauseTimer(ArrayClearTimer);
	GetWorld()->GetTimerManager().ClearTimer(ArrayClearTimer);

	bIsInScopeView = false;
	bArmed = true;
	EnterDist = 2.0f;
	ExitDist = 8.0f;
	ReArmDist = 12.0f;
}

void AAKeyPad::Tick(float DeltaTimes)
{
	Super::Tick(DeltaTimes);

	if (bIsScopeFlag && mVRPlayerPawn)
	{
		FVector HMDWorld = GetHMDWorldLocation();
		double Dist = FMath::Abs(TargetPos.X - HMDWorld.X);
		float TempOpacity = (1.0f - FMath::Clamp((Dist - 2.0f) / (MaxPeekDist - 2.0f), 0.0f, 1.0f));
		mVRPlayerPawn->HandleMaskOpacity(TempOpacity);

		APlayerController* TempPC = Cast<APlayerController>(mVRPlayerPawn->GetController());
		checkf(TempPC, TEXT("PC Not Valid!"));
		if (!TempPC) return;

		if (!bIsInScopeView)
		{
			if (bArmed && Dist <= EnterDist)
			{
				TempPC->SetViewTargetWithBlend(ScopeCameraRef, 0.f);
				bIsInScopeView = true;
				bArmed = false;        
			}
		}
		else
		{
			if (Dist >= ExitDist)
			{
				TempPC->SetViewTargetWithBlend(mVRPlayerPawn, 0.f);
				bIsInScopeView = false;
			}
		}

		if (!bArmed && !bIsInScopeView && Dist >= ReArmDist)
		{
			bArmed = true;
		}
	}
}

void AAKeyPad::OnGrabbed(UMotionControllerComponent& InMCRef, const FVector& HandGrabPos, AVRHand* InGrabbingHand)
{
	TempMCRef = &InMCRef;
	if (TempMCRef->ComponentHasTag(FName("RightMC")))
	{
		GC->SetRelativeLocation(FVector(27.5f, -3.0f, -33.5f));
		GC->SetRelativeRotation(FRotator(0.f, 180.0f, -90.0f));
	}
	else
	{
		GC->SetRelativeLocation(FVector(-27.5f, -3.0f, -33.5f));
		GC->SetRelativeRotation(FRotator(0.f, 180.0f, -90.0f));
	}
}

void AAKeyPad::OnDropped()
{
	TempMCRef = nullptr;
}

void AAKeyPad::EquipmentRegistrable(AActor* InActor)
{
	Super::EquipmentRegistrable(InActor);
}

void AAKeyPad::HandleKeyPadMove(uint8 InMoveFlag)
{
	if (InMoveFlag)
	{
		if (ActorBaseMesh->GetRelativeLocation().Z <= 66.0f)
		{
			KeyPadMoveTimelineComp->Reverse();
		}
		else if (ActorBaseMesh->GetRelativeLocation().Z >= 164.0f)
		{
			KeyPadMoveTimelineComp->PlayFromStart();
		}
	}
}

void AAKeyPad::OnKeyOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp && OtherComp->ComponentHasTag(FName("HandIndex")))
	{
		if (!bIsOverlapping)
		{
			mSoundPlayer->PlaySoundEffect(this, ButtonPressSFX, ActorBaseMesh->GetComponentLocation());
			CheckOverlapColToInt(Cast<UBoxComponent>(OverlappedComp));
		}
	}
}

void AAKeyPad::OnKeyOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherComp && OtherComp->ComponentHasTag(FName("HandIndex")))
	{
		if (bIsOverlapping)
		{
			bIsOverlapping = false;
		}
	}
}

void AAKeyPad::OnCameraOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp && OtherComp->ComponentHasTag(FName("VRPawn")) && !bIsScopeFlag)
	{
		UE_LOG(LogTemp, Log, TEXT("Camera Overlap Begin"));
		mVRPlayerPawn = Cast<ACVRPawn>(OtherActor);
		TargetPos = CL_PerisScope->GetComponentLocation();
		UE_LOG(LogTemp, Log, TEXT("TargetPos : %s"), *TargetPos.ToString());
		bIsScopeFlag = true;

		if (!IsValid(ScopeCameraRef))
		{
			FVector HMDWorld = GetHMDWorldLocation();
			MaxPeekDist = FVector::Dist2D(TargetPos, HMDWorld);

			for (TScriptInterface<IIEquipmentInitInterface> Equip : EquipmentWorldSubSystem->GetEquipmentArr())
			{
				IIEquipmentInitInterface* IEquipPtr = Equip.GetInterface();
				ScopeCameraRef = Cast<AAScopeCamera>(IEquipPtr);
				if (ScopeCameraRef)
					break;
				else
					continue;
			}
			checkf(ScopeCameraRef, TEXT("ScopeCamera Not Valid"));
		}
	}
}

void AAKeyPad::OnCameraOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherComp && OtherComp->ComponentHasTag(FName("VRPawn")) && bIsScopeFlag)
	{
		bIsScopeFlag = false;
		mVRPlayerPawn->HandleMaskOpacity(0.0f);
	}
}

void AAKeyPad::ClearOutputArrays()
{
	mSoundPlayer->PlaySoundEffect(this, ReturnButtonPressSFX, ActorBaseMesh->GetComponentLocation());
	UE_LOG(LogTemp, Log, TEXT("AKeyPad In Text Array Clear"));
	for (UTextRenderComponent* TextRender : TR_ScoreBoard_Text)
	{
		TextRender->SetText(FText::GetEmpty());
	}

	CurrTextLength = 0;
	FinalOutputIntArrays.Empty();
	FinalOutputTextArrays.Empty();

	EquipmentWorldSubSystem->NotifyInputValuePassingByKeyPadBroadCast(true, 1);

	GetWorld()->GetTimerManager().PauseTimer(ArrayClearTimer);
	GetWorld()->GetTimerManager().ClearTimer(ArrayClearTimer);
}

void AAKeyPad::CheckOverlapColToInt(TObjectPtr<class UBoxComponent> InBoxCol)
{
	int32 KeyIndex = CL_Keys.IndexOfByKey(Cast<UBoxComponent>(InBoxCol));
	GetWorld()->GetTimerManager().SetTimer( // KeyPad Not Input 5 Seconds After Clear Timer
		ArrayClearTimer,
		this,
		&AAKeyPad::ClearOutputArrays,
		5.0f,
		false
	);

	if (FinalOutputTextLength > CurrTextLength)
	{
		//UE_LOG(LogTemp, Log, TEXT("Key pressed Index : %d"), KeyIndex);
		if (CheckOverlapColToText(KeyIndex))
		{
			EquipmentWorldSubSystem->NotifyInputValuePassingByKeyPadBroadCast(false, KeyIndex + 1);
			TR_ScoreBoard_Text[CurrTextLength - 1]->SetText(FinalOutputTextArrays[CurrTextLength - 1]);
		}
	}
	else
	{
		if (KeyIndex == 10)
		{
			ClearOutputArrays();
		}
	}
}

bool AAKeyPad::CheckOverlapColToText(uint32 InBoxNum)
{
	bool TempResult = false;
	if (InBoxNum == 10) // BackSpace Key
	{
		ClearOutputArrays();
		TempResult = false;
		return TempResult;
	}
	else if (InBoxNum == 9) // Pickle Key
	{
		FinalOutputIntArrays.Add(0);
		FinalOutputTextArrays.Add(FText::FromString(FString::Printf(TEXT("%d"), 0)));
		CurrTextLength++;
		TempResult = true;
		return TempResult;
	}
	else
	{
		FinalOutputIntArrays.Add(InBoxNum + 1);
		FinalOutputTextArrays.Add(FText::FromString(FString::Printf(TEXT("%u"), InBoxNum + 1)));
		CurrTextLength++;
		TempResult = true;
		return TempResult;
	}
}

void AAKeyPad::HandleKeyPadReceiveByEmergencyButton()
{
	// KeyPad Out Array Clear
	for (UTextRenderComponent* TextRender : TR_ScoreBoard_Text)
	{
		TextRender->SetText(FText::GetEmpty());
	}

	CurrTextLength = 0;
	FinalOutputIntArrays.Empty();
	FinalOutputTextArrays.Empty();

	GetWorld()->GetTimerManager().PauseTimer(ArrayClearTimer);
	GetWorld()->GetTimerManager().ClearTimer(ArrayClearTimer);
}

FVector AAKeyPad::GetHMDWorldLocation() const
{
	if (!mVRPlayerPawn) 
		return FVector::ZeroVector;
	FVector HMDPos = FVector::ZeroVector;
	FRotator HMDRot = FRotator::ZeroRotator;
	UHeadMountedDisplayFunctionLibrary::GetOrientationAndPosition(HMDRot, HMDPos);
	return mVRPlayerPawn->GetActorTransform().TransformPosition(HMDPos);
}

void AAKeyPad::KeyPadMovePlayEvent(float Value)
{
	FVector TempBaseMeshVector = ActorBaseMesh->GetRelativeLocation();

	ActorBaseMesh->SetRelativeLocation(FVector(TempBaseMeshVector.X, TempBaseMeshVector.Y, Value * 100.0f));
}

void AAKeyPad::KeyPadMoveFinishedEvent()
{
	EquipmentWorldSubSystem->NotifyToggleSwitchOperationBroadCast(0);
	if (ActorBaseMesh->GetRelativeLocation().Z <= 66.0f)
	{
		CL_PerisScope->SetGenerateOverlapEvents(true);
	}
}
