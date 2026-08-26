#include "MainActor/AGameResultScreen.h"
#include "CoreObj/GameMode/VRLobbyGameMode.h"
#include "CoreObj/Manager/VRGameInstance.h"

AAGameResultScreen::AAGameResultScreen()
{
	PrimaryActorTick.bCanEverTick = false;

	SC_MainRoot = CreateDefaultSubobject<USceneComponent>("MainSceneComp");
	if (SC_MainRoot)
	{
		this->SetRootComponent(SC_MainRoot);
	}

	SM_Success = CreateDefaultSubobject<UStaticMeshComponent>("SuccessStaticMeshComp");
	if (SM_Success)
	{
		SM_Success->SetupAttachment(SC_MainRoot);
		SM_Success->SetVisibility(false);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SMFinder_Success(TEXT("/Game/VRContent/Modeling/29_GameResultPrinter/SM_GameResult_Success_Signal.SM_GameResult_Success_Signal"));
	if (SMFinder_Success.Succeeded())
		SM_Success->SetStaticMesh(SMFinder_Success.Object);

	if (ActorBaseMesh)
	{
		ActorBaseMesh->SetupAttachment(SC_MainRoot);
		ActorBaseMesh->SetVisibility(false);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SMFinder_Fail(TEXT("/Game/VRContent/Modeling/29_GameResultPrinter/SM_GameResult_Fail_Signal.SM_GameResult_Fail_Signal"));
	if (SMFinder_Fail.Succeeded())
		ActorBaseMesh->SetStaticMesh(SMFinder_Fail.Object);

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MatFinder_blue(TEXT("/Game/VRContent/Material/MI_Hologram_Blue_Success.MI_Hologram_Blue_Success"));
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MatFinder_red(TEXT("/Game/VRContent/Material/MI_Hologram_Red_Fail.MI_Hologram_Red_Fail"));
	if (MatFinder_blue.Succeeded() && MatFinder_red.Succeeded())
	{
		SM_Success->SetMaterial(0, MatFinder_blue.Object);
		ActorBaseMesh->SetMaterial(0, MatFinder_red.Object);
	}
}

void AAGameResultScreen::BeginPlay()
{
	Super::BeginPlay();

	AVRLobbyGameMode* TempVRLGM = Cast<AVRLobbyGameMode>(GetWorld()->GetAuthGameMode());
	check(TempVRLGM);

	TempVRLGM->FGameResultPrintSignature.BindUObject(this, &AAGameResultScreen::GameResultDisplay);
}

void AAGameResultScreen::GameResultDisplay(bool bIsResult)
{
	if (bIsResult)
	{
		SM_Success->SetVisibility(true);
		ActorBaseMesh->SetVisibility(false);
	}
	else
	{
		SM_Success->SetVisibility(false);
		ActorBaseMesh->SetVisibility(true);
	}

	GetWorld()->GetTimerManager().SetTimer(
		RotateSelfTimer,
		this,
		&AAGameResultScreen::RotationSelf,
		0.01f,
		true
	);

	GetWorld()->GetTimerManager().SetTimer(
		RotatePauseSelfTimer,
		this,
		&AAGameResultScreen::PauseRotationSelf,
		5.0f,
		false
	);
}

void AAGameResultScreen::RotationSelf()
{
	SC_MainRoot->AddLocalRotation(FRotator(0.0f, 0.5f, 0.0f));
}

void AAGameResultScreen::PauseRotationSelf()
{
	UVRGameInstance* TempVRGI = Cast<UVRGameInstance>(GetGameInstance());
	check(TempVRGI);
	TempVRGI->ClearAllFlag();

	GetWorld()->GetTimerManager().PauseTimer(RotateSelfTimer);
	GetWorld()->GetTimerManager().ClearTimer(RotateSelfTimer);
	SM_Success->SetVisibility(false);
	ActorBaseMesh->SetVisibility(false);
}
