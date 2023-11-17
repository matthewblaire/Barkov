// (c) Matthew Blaire 2023


#include "Item.h"

void AItem::Interact(AActor* Interactor)
{
	Super::Interact(Interactor);
	// add item to player inventory
	// remove item from world
}

AItem::AItem()
{
	PromptText = TEXT("Pick up");
}
