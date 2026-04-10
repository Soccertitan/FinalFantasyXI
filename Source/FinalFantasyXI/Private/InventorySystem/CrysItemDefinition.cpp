// Copyright Soccertitan 2025


#include "InventorySystem/CrysItemDefinition.h"

#include "Item/Fragment/ItemFragment_UI.h"
#include "UI/ViewModel/InventorySystem/CrysItemViewModel.h"

UCrysItemDefinition::UCrysItemDefinition()
{
	TInstancedStruct<FItemFragment_UI> Fragment_UI;
	Fragment_UI.InitializeAsScriptStruct(FItemFragment_UI::StaticStruct());
	Fragment_UI.GetMutablePtr<FItemFragment_UI>()->ItemViewModelClass = UCrysItemViewModel::StaticClass();
	
	Fragments.Add(Fragment_UI);
}
