// (c) Matthew Blaire 2023

#pragma once

#include "CoreMinimal.h"
#include "Barkov/Data/ItemDataStructs.h"
#include "ItemBase.generated.h"

/**
 * 
 */
UCLASS()
class BARKOV_API UItemBase : public UObject
{
	GENERATED_BODY()
public:

	// UInventoryComponent* OwningInventory;
	
	UPROPERTY(VisibleAnywhere, Category="Item Data")
	int32 Quantity;

	UPROPERTY(VisibleAnywhere, Category="Item Data")
	FName ID;
	
	UPROPERTY(VisibleAnywhere, Category="Item Data")
	EItemType Type;

	UPROPERTY(VisibleAnywhere, Category="Item Data")
	EItemQuality Quality;

	UPROPERTY(VisibleAnywhere, Category="Item Data")
	FItemStats Stats;

	UPROPERTY(VisibleAnywhere, Category="Item Data")
	FItemTextData TextData;

	UPROPERTY(VisibleAnywhere, Category="Item Data")
	FItemNumericData NumericData;

	UPROPERTY(VisibleAnywhere, Category="Item Data")
	FItemAssetData AssetData;

	UItemBase();

	UItemBase* CreateItemCopy() const;

	float GetItemStackWeight() const { return Quantity * NumericData.Weight; }
	float GetItemSingleWeight() const { return NumericData.Weight; }
	bool IsFullItemStackSize() const { return Quantity == NumericData.MaxStackSize; }

	void SetQuantity(const int32 NewQuantity);

	virtual void Use(class ABarkovCharacter* Character);

protected:
	bool operator==(const FName& OtherID)
	{
		return this->ID == OtherID;
	}
};
