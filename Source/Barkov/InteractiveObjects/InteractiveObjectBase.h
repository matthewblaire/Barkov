// (c) Matthew Blaire 2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractiveObjectBase.generated.h"

UCLASS()
class BARKOV_API AInteractiveObjectBase : public AActor
{
	GENERATED_BODY()
	
public:

	
	/* constructor */
	AInteractiveObjectBase();
	
	
	/**
	 *	Base interaction method, called by Interactors
	 * @param Interactor the actor that called Interact()
	 */
	virtual void Interact(AActor* Interactor);

	
	/**
	 * @return the interaction range of this object 
	 */
	float GetInteractionRange() const { return InteractionRange; }
	
	
	/**
	 * @return the prompt text of this object
	 */
	FString GetPromptText() { return PromptText; }
	
protected:
	

	virtual void BeginPlay() override;
	/* Text displayed when user looks at object while within interaction range */
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess))
	FString PromptText = TEXT("Interact");

	/* the static mesh */
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess))
	UStaticMeshComponent* ObjectVisual;
	
private:
	
	
	/* Distance at which a character can interact with this object. (directly affects trigger radius) */ 
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess))
	float InteractionRange = 100; 
	
	/* Sphere that detects if a player is within interaction range */
	UPROPERTY(VisibleAnywhere, meta=(AllowPrivateAccess))
	class USphereComponent* AreaSphere;


#if WITH_EDITOR
	/*
	 *	Ensures that the SphereRadius of AreaSphere gets updated when InteractionRange changes
	 */
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	

};
