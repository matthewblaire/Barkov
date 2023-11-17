// (c) Matthew Blaire 2023

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"

class ABarkovCharacter;
class UInteractWidget;
//class AInteractiveObjectBase; deprecated 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractTargetUpdated, AActor*, InteractTarget);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BARKOV_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInteractionComponent();
	void UpdateInteractiveObjectCount(bool bIncrement);

	
	UInteractWidget* GetInteractWidget() const { return InteractWidget; }
	
	/**
	 * Pointer to the character that owns this component
	 */
	UPROPERTY()
	ABarkovCharacter* Character;

	
	/**
	 * Delegate that should be broadcast whenever the InteractTarget changes
	 */
	FInteractTargetUpdated InteractTargetUpdated;

	UPROPERTY(EditAnywhere);
	float InteractionCooldownSeconds = 1.f;

	float LastInteractionTime = FLT_MIN;
	
protected:

	virtual void BeginPlay() override;

	
	/**
	 * Updates the InteractTarget then broadcasts the update
	 * @param NewInteractTarget The new interact target. Can be AActor* or nullptr
	 */
	void SetInteractTarget(AActor* NewInteractTarget);
	
public:
	UFUNCTION()
	void InteractButtonPressed();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
private:
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess))
	bool bDrawDebugLine = false;
	/* pointer to the InteractWidget that gets created once owner is posessed */
	UPROPERTY()
	UInteractWidget* InteractWidget;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess))
	TSubclassOf<UUserWidget> InteractWidgetClass;
	
	/*
	 * pointer to the object that the player is currently looking at 
	 * if InteractiveObjectsNearby == 0 this should be nullptr
	 */
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	AActor* InteractTarget = nullptr;



	/*
	 * keeps track of how many IOs are nearby.
	 * above zero: ticking should be enabled to perform line traces
	 * zero: ticking disabled
	 * below zero: should not happen
	 */
	UPROPERTY()
	int32 InteractiveObjectsNearby = 0;

	
	/*
	 * Performs line trace from player camera to Interactive objects. Displays interaction prompt when player is looking 
	 * at an interactive object.
	 */
	void HandleInteractiveObjectCheck();

	/*
	 * Validates interaction with a distance check then calls Interact() on the Target InteractiveObject
	 * passing in pointer to owning character
	 */
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerInteractButtonPressed(AActor* CurrentInteractTarget);

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
	

	/* 
	 * if the character this component is attached to is locally controlled,
	 * bind delegates and create interactwidget
	 */
	UFUNCTION()
	void LocalControlCheck(APlayerController* NewController);
};
