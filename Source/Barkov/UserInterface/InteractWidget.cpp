// (c) Matthew Blaire 2023


#include "InteractWidget.h"


#include "Barkov/Components/InteractionComponent.h"
#include "Barkov/Interfaces/I_InteractiveObject.h"
#include "Components/TextBlock.h"

void UInteractWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
}



void UInteractWidget::DisplayPrompt()
{
	PlayAnimation(PromptFadeIn,  GetAnimationCurrentTime(PromptFadeIn),1,EUMGSequencePlayMode::Forward);
	
	bPromptIsShowing = true;
}

void UInteractWidget::HidePrompt()
{
	PlayAnimation(PromptFadeIn, GetAnimationCurrentTime(PromptFadeIn),1,EUMGSequencePlayMode::Reverse);
	bPromptIsShowing = false;
}

void UInteractWidget::Setup()
{
	SetVisibility(ESlateVisibility::HitTestInvisible);
	PromptTextBlock->SetText(FText::GetEmpty());
	AddToViewport(-1);
}

void UInteractWidget::OnInteractTargetUpdated(AActor* InteractTarget)
{
	if (InteractTarget)
	{
		
		if (auto InteractiveObject = Cast<II_InteractiveObject>(InteractTarget))
		{
			PromptTextBlock->SetText(InteractiveObject->GetPromptText());
			if (!bPromptIsShowing)
			{
				DisplayPrompt();
			}
		}
		
	}
	else
	{
		if (bPromptIsShowing)
		{
			HidePrompt();
		}
		
	}
}

void UInteractWidget::BindDelegates(UInteractionComponent* InteractionComponent)
{
	InteractionComponent->InteractTargetUpdated.AddDynamic(this, &ThisClass::OnInteractTargetUpdated);
}


