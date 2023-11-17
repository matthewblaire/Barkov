// (c) Matthew Blaire 2023

#pragma once

#include "CoreMinimal.h"
#include "InteractiveObjectBase.h"
#include "Item.generated.h"

/**
 * 
 */
UCLASS()
class BARKOV_API AItem : public AInteractiveObjectBase
{
	GENERATED_BODY()
public:
	virtual void Interact(AActor* Interactor) override;
	AItem();
private:
	
};
