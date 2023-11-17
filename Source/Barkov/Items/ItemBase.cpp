// (c) Matthew Blaire 2023


#include "ItemBase.h"

UItemBase::UItemBase()
{
}

UItemBase* UItemBase::CreateItemCopy() const 
{
	UItemBase* ItemCopy = NewObject<UItemBase>(StaticClass());
	ItemCopy->Quantity = this->Quantity;
	ItemCopy->ID = this->ID;
	ItemCopy->Type = this->Type;
	ItemCopy->Quality = this->Quality;
	ItemCopy->Stats = this->Stats;
	ItemCopy->TextData = this->TextData;
	ItemCopy->NumericData = this->NumericData;
	ItemCopy->AssetData = this->AssetData;

	return ItemCopy;
	
}

void UItemBase::SetQuantity(const int32 NewQuantity)
{
	if (NewQuantity != Quantity)
	{
		auto MaxSize = NumericData.bIsStackable ? NumericData.MaxStackSize : 1;
		Quantity = FMath::Clamp(NewQuantity, 0, NumericData.bIsStackable ? NumericData.MaxStackSize : 1);
		/*if (OwningInventory)
		{
			if (Quantity <= 0)
			{
				OwningINventory->RemoveItem(this);
			}
		}*/
	}
}

void UItemBase::Use(ABarkovCharacter* Character)
{
	
}
