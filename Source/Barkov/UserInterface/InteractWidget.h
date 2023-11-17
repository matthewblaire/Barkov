// (c) Matthew Blaire 2023

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractWidget.generated.h"

class UInteractionComponent;
class ABarkovCharacter;
/**
 * 
 */
UCLASS()
class BARKOV_API UInteractWidget : public UUserWidget
{
	GENERATED_BODY()
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* PromptTextBlock;

	UPROPERTY(meta=(BindWidgetAnim), Transient)
	UWidgetAnimation* PromptFadeIn;

	bool bPromptIsShowing = false;
	
	virtual void NativeConstruct() override;

	
public:
	void DisplayPrompt();
	void HidePrompt();
	UFUNCTION(BlueprintCallable)
	void Setup();

	UFUNCTION()
	void OnInteractTargetUpdated(AActor* InteractTarget);

	void BindDelegates(UInteractionComponent* InteractionComponent);
	
};
