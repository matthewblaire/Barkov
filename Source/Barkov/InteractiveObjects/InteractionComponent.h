// (c) Matthew Blaire 2023

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"

class ABarkovCharacter;
class UInteractWidget;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BARKOV_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInteractionComponent();
	void UpdateInteractiveObjectCount(bool bIncrement);
	
	UInteractWidget* GetInteractWidget() const { return InteractWidget; }
	UPROPERTY()
	ABarkovCharacter* Character;

protected:

	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void InteractButtonPressed();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
private:
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess))
	bool bDrawDebugLine = false;
	/* pointer to the InteractWidget that gets created at BeginPlayer in the BarkovCharacter Event Graph */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess))
	UInteractWidget* InteractWidget;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess))
	TSubclassOf<UUserWidget> InteractWidgetClass;
	/*
	 * pointer to the object that the player is currently looking at 
	 * if InteractiveObjectsNearby == 0 this should be nullptr
	 */
	UPROPERTY()
	class AInteractiveObjectBase* InteractTarget = nullptr;



	/*
	 * keeps track of how many IOs are nearby.
	 * above zero: ticking should be enabled to perform line traces
	 * zero: ticking disabled
	 * below zero: should not happen
	 */
	UPROPERTY()
	int32 InteractiveObjectsNearby = 0;

	
	/*
	 * Performs line trace from player camera to Interactive objects. Displays interaction prompt when player is looking at
	 * an interactive object.
	 */
	void HandleInteractiveObjectCheck();

	/*
	 * Validates interaction with a distance check then calls Interact() on the Target InteractiveObject
	 * passing in pointer to owning character
	 */
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerInteractButtonPressed(AInteractiveObjectBase* Target);

	virtual void OnRegister() override;
	
	/*
	 *	Tells us when to INCREMENT its "InteractiveObjectsNearby" property
	 *	Called when something comes within InteractRange. 
	 */
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
	
	/*
	 *	Tells us when to DECREMENT its "InteractiveObjectsNearby" property
	 *	Called when something leaves InteractRange. 
	 */
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

private:
	/* Ensures that local control is only checked one time */
	bool bLocalControlChecked = false;

	/* 
	 * if the character this component is attached to is locally controlled,
	 * it should bind overlap callbacks and create the interaction widget.
	 * This function should only run one time.
	 */
	void LocalControlCheck();
};
