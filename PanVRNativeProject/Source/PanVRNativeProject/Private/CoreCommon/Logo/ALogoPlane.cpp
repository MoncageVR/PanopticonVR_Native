#include "CoreCommon/Logo/ALogoPlane.h"
#include "CoreObj/Manager/GameInstanceSubSystem/VRGameInstanceSubsystem.h"
#include "Components/TimelineComponent.h"
#include "Kismet/GameplayStatics.h"

AALogoPlane::AALogoPlane()
{
	PrimaryActorTick.bCanEverTick = false;

	SM_Logo = CreateDefaultSubobject<UStaticMeshComponent>("LogoSMComp");
	if (SM_Logo)
	{
		SetRootComponent(SM_Logo);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SMFinder_Plane(TEXT("/Engine/BasicShapes/Plane.Plane"));
	if (SMFinder_Plane.Succeeded())
		SM_Logo->SetStaticMesh(SMFinder_Plane.Object);

	TL_FadeComp = CreateDefaultSubobject<UTimelineComponent>("FadeTimelineComp");

	static ConstructorHelpers::FObjectFinder<UCurveFloat> CurveFinder_Fade(TEXT("/Game/VRContent/Blueprints/TimelineCurve/LogoFade_Curve.LogoFade_Curve"));
	if (CurveFinder_Fade.Succeeded())
		FadeCurve = CurveFinder_Fade.Object;

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MatFinder_Logo(TEXT("/Game/VRContent/Material/BasePhotos/Logo/Moncage_ICON_Mat_Inst.Moncage_ICON_Mat_Inst"));
	if (MatFinder_Logo.Succeeded())
		SM_Logo->SetMaterial(0, MatFinder_Logo.Object);

	static ConstructorHelpers::FObjectFinder<USoundBase> SFXFinder_Logo(TEXT("/Game/VRContent/Sound/Wavs/Logo/logo_sfx.logo_sfx"));
	if (SFXFinder_Logo.Succeeded())
	{
		SFX_LogoPlay = SFXFinder_Logo.Object;
	}
}

void AALogoPlane::BeginPlay()
{
	Super::BeginPlay();
	
	if (SM_Logo)
	{
		LogoMID = SM_Logo->CreateDynamicMaterialInstance(0);
	}

	if (FadeCurve)
	{
		FOnTimelineFloat ProgressFunc;
		FOnTimelineEvent FinishedFunc;
		ProgressFunc.BindUFunction(this, FName("HandleFadeProgress"));
		FinishedFunc.BindUFunction(this, FName("HandleFadeFinished"));
		TL_FadeComp->AddInterpFloat(FadeCurve, ProgressFunc);
		TL_FadeComp->SetTimelineFinishedFunc(FinishedFunc);
	}

	this->ActuallyPlayFadeEffect();
}

void AALogoPlane::ActuallyPlayFadeEffect()
{
	if (UWorld* mWorld = GetWorld())
	{
		mWorld->GetTimerManager().SetTimer(
			TempSoundTimer,
			this,
			&AALogoPlane::PlayLogoSound,
			2.0f,
			false
		);
	}

	TL_FadeComp->PlayFromStart();
}

void AALogoPlane::PlayLogoSound()
{
	UVRGameInstanceSubsystem* TempGameInstSub = this->GetWorld()->GetGameInstance()->GetSubsystem<UVRGameInstanceSubsystem>();

	if (IsValid(TempGameInstSub) && SFX_LogoPlay)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			SFX_LogoPlay,
			this->GetRootComponent()->GetComponentLocation() + FVector(0.f, 0.f, 20.0f),
			TempGameInstSub->GetWorldVolumeMultiplier(),
			TempGameInstSub->GetWorldVolumePitch()
		);

		UE_LOG(LogTemp, Log, TEXT("Logo SFX Play Success!"));
	}
}

void AALogoPlane::HandleFadeProgress(float Value)
{
	if (LogoMID)
		LogoMID->SetScalarParameterValue(FName("FadeAmount"), Value);
}

void AALogoPlane::HandleFadeFinished()
{
	GetWorld()->GetTimerManager().PauseTimer(TempSoundTimer);
	GetWorld()->GetTimerManager().ClearTimer(TempSoundTimer);
}

