// (c) Matthew Blaire 2023

#pragma once

#include "CoreMinimal.h"
#include "InteractiveObjectBase.h"
#include "Core/Actors/Parents/A_ItemActor.h"

#include "ItemPickup.generated.h"

/**
 * 
 */
UCLASS()
class BARKOV_API AItemPickup : public AInteractiveObjectBase
{
	GENERATED_BODY()
public:
	virtual void Interact(AActor* Interactor) override;


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
private:
	AItemPickup();
	//UPROPERTY(ReplicatedUsing=OnRep_ItemData)
	//FItemData ItemData;

	UFUNCTION()
	void OnRep_ItemData();

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess))
	UDataTable* ItemDataTable;

	FString BasePromptText = TEXT("Pick Up");
	
};
