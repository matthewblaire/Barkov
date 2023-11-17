// (c) Matthew Blaire 2023

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractWidget.generated.h"

/**
 * 
 */
UCLASS()
class BARKOV_API UInteractWidget : public UUserWidget
{
	GENERATED_BODY()
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* PromptTextBlock;

	virtual void NativeConstruct() override;
public:
	void DisplayPrompt(FString PromptText);
	void HidePrompt();
	UFUNCTION(BlueprintCallable)
	void Setup();
	
	
};
