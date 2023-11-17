// (c) Matthew Blaire 2023


#include "InteractWidget.h"

#include "Components/TextBlock.h"

void UInteractWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
}

void UInteractWidget::DisplayPrompt(FString PromptText)
{
	PromptTextBlock->SetText(FText::FromString(PromptText));
	SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UInteractWidget::HidePrompt()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UInteractWidget::Setup()
{
	AddToViewport(-1);
}


