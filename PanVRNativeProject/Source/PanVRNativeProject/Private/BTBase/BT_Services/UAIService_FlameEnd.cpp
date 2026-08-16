#include "BTBase/BT_Services/UAIService_FlameEnd.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "PanVRNativeProject/PanVRNativeProject.h"

UUAIService_FlameEnd::UUAIService_FlameEnd()
{
	NodeName = TEXT("UAIService_FlameStateEnd");
	bCreateNodeInstance = true;
	bNotifyCeaseRelevant = true;
}

void UUAIService_FlameEnd::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnCeaseRelevant(OwnerComp, NodeMemory);

	if (APrisonerController* mPC = Cast<APrisonerController>(OwnerComp.GetAIOwner()))
	{
		//int32 TempNum = mPC->GetBBComp()->GetValueAsInt(FName("UniqueNum"));
		if (mPC->GetBBComp()->GetValueAsEnum(TEXT("CurrUpperState")) == 4 && mPC->GetBBComp()->GetValueAsEnum(TEXT("CurrLowerState")) == 15)
		{
			//UE_LOG(LogTemp, Log, TEXT("%d Prisoner is Still Flame Status"), TempNum);
			return;
		}
		else
		{
			mPC->HandleFlameTransitionColNTimer(0);
			//UE_LOG(LogTemp, Log, TEXT("%d Prisoner Flame State is Abort!"), TempNum);
			return;
		}

	}
}
