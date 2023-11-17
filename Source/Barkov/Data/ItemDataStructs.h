#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemDataStructs.generated.h"


UENUM()
enum class EItemQuality : uint8
{
	Civilian UMETA(DisplayName = "Civilian"),
	Professional UMETA(DisplayName = "Professional"),
	Elite UMETA(DisplayName = "Elite"),
	Sovereign UMETA(DisplayName = "Sovereign")
};

UENUM()
enum class EItemType : uint8
{
	Armor UMETA(DisplayName = "Armor"),
	Weapon UMETA(DisplayName = "Weapon"),
	Consumable UMETA(DisplayName = "Consumable"),
	Quest UMETA(DisplayName = "Quest")
};

USTRUCT()
struct FItemStats
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere)
	float ArmorRating;
	UPROPERTY(EditAnywhere)
	float DamageValue;
	UPROPERTY(EditAnywhere)
	float RestorationAmount;
	UPROPERTY(EditAnywhere)
	float SellValue;
};

USTRUCT()
struct FItemTextData
{
	GENERATED_USTRUCT_BODY()
	
	// Item name
	UPROPERTY(EditAnywhere)
	FText Name;
	
	// Item description
	UPROPERTY(EditAnywhere)
	FText Description;
	
	// in-world interaction text
	UPROPERTY(EditAnywhere)
	FText InteractionText;
	
	// In-inventory interaction text i.e. "use, consume, open"
	UPROPERTY(EditAnywhere)
	FText UsageText;
};

USTRUCT()
struct FItemNumericData
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere)
	float Weight;
	
	UPROPERTY(EditAnywhere)
	int32 MaxStackSize;
	
	UPROPERTY(EditAnywhere)
	bool bIsStackable;
};

USTRUCT()
struct FItemAssetData
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere)
	UTexture2D* Icon;
	
	UPROPERTY(EditAnywhere)
	UStaticMesh* Mesh;
		
};

USTRUCT()
struct FItemData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere, Category="Item Data")
	FName ID;
	
	UPROPERTY(EditAnywhere, Category="Item Data")
	EItemType ItemType;

	UPROPERTY(EditAnywhere, Category="Item Data")
	EItemQuality ItemQuality;

	UPROPERTY(EditAnywhere, Category="Item Data")
	FItemStats ItemStats;

	UPROPERTY(EditAnywhere, Category="Item Data")
	FItemTextData ItemTextData;

	UPROPERTY(EditAnywhere, Category="Item Data")
	FItemNumericData ItemNumericData;

	UPROPERTY(EditAnywhere, Category="Item Data")
	FItemAssetData ItemAssetData;
	
};