// (c) Matthew Blaire 2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BarkovHUD.generated.h"

class UInteractWidget;
class UMainMenuWidget;

/**
 * 
 */
UCLASS()
class BARKOV_API ABarkovHUD : public AHUD
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
public:

	
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInteractWidget> InteractWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMainMenuWidget> MainMenuClass;
	
	void DisplayMainMenu();
	void HideMainMenu();
	void DisplayInteractWidget();
	void UpdateInteractWidget(FText NewInteractText);
	void HideInteractWidget();

private:
	UPROPERTY()
	UInteractWidget* InteractWidget;
	UPROPERTY()
	UMainMenuWidget* MainMenuWidget;
};
