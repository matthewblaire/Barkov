// (c) Matthew Blaire 2023


#include "ItemPickup.h"

//#include "Barkov/Data/ItemDataStructs.h"
#include "Net/UnrealNetwork.h"

void AItemPickup::Interact(AActor* Interactor)
{
	Super::Interact(Interactor);
	// add item to player inventory
	// remove item from world

	//TEST
	if (HasAuthority())
	{
		/*TArray<FItemData*> AllRows;
		ItemDataTable->GetAllRows<FItemData>(TEXT("TEST GET ALL ROWS"), AllRows);
		auto NumberOfRows = AllRows.Num();
		auto RandomRow = FMath::RandRange(0, NumberOfRows - 1);
		ItemData = *AllRows[RandomRow];
		OnRep_ItemData();*/
	}
}

void AItemPickup::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//DOREPLIFETIME(ThisClass, ItemData);
}

void AItemPickup::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		// choose random item data
		// update
	}
}

AItemPickup::AItemPickup()
{
	//ItemData = FItemData();
	PromptText = BasePromptText;
}

void AItemPickup::OnRep_ItemData()
{
	/*if (ItemData.AssetData.Mesh)
	{
		ObjectVisual->SetStaticMesh(ItemData.AssetData.Mesh);
		PromptText = BasePromptText + TEXT(" \n") + ItemData.TextData.Name.ToString();
	}*/
}
