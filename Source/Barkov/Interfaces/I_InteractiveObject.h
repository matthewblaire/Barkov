// (c) Matthew Blaire 2023

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "I_InteractiveObject.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UI_InteractiveObject : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BARKOV_API II_InteractiveObject
{
	GENERATED_BODY()
public:
	virtual void SetGlow(bool bShouldGlow) = 0;
	
	/**
	 *	Base interaction method, called by Interactors
	 * @param Interactor the actor that called Interact()
	 */
	virtual void Interact(AActor* Interactor) = 0;
	
	/**
	 * @return the interaction range of this object 
	 */
	virtual float GetInteractionRange() const = 0;
	
	/**
	 * @return the prompt text of this object
	 */
	virtual FText GetPromptText() const = 0;
	
	virtual UStaticMeshComponent* GetObjectVisual() const = 0;


	
	
public:
};
