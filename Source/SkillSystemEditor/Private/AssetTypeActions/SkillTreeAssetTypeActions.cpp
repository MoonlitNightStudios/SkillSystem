// Copyright Moonlit Night Studios Ltd. All rights Reserved.

#include "AssetTypeActions/SkillTreeAssetTypeActions.h"
#include "EditorModes/SkillTreeEditorMode.h"
#include "SkillTree.h"
#include "SkillEditor.h"

UClass* FSkillTreeAssetTypeActions::GetSupportedClass() const
{
	return USkillTree::StaticClass();
}

FText FSkillTreeAssetTypeActions::GetName() const
{
	return INVTEXT("Skill Tree");
}

FColor FSkillTreeAssetTypeActions::GetTypeColor() const
{
	return FColor::Magenta;
}

FText FSkillTreeAssetTypeActions::GetAssetDescription(const FAssetData& AssetData) const
{
	return NSLOCTEXT("SkillTreeAsset", "SkillTreeAssetDescription", "Skill tree asset.");
}

extern EAssetTypeCategories::Type SkillAssetsCategory;
uint32 FSkillTreeAssetTypeActions::GetCategories()
{
	return SkillAssetsCategory;
}

void FSkillTreeAssetTypeActions::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	for (auto ObjIt = InObjects.CreateConstIterator(); ObjIt; ++ObjIt)
	{
		USkillTree* SkillTree = Cast<USkillTree>(*ObjIt);
		if (SkillTree != NULL)
		{
			TMap<FName, TSharedRef<FApplicationMode>> EditorModes;

			TSharedRef<FSkillTreeEditorMode> SkillTreeEditorMode = MakeShareable<FSkillTreeEditorMode>(new FSkillTreeEditorMode(FName("Skill Tree Editor")));
			SkillTreeEditorMode->Init(SkillTree);
			EditorModes.Add(SkillTreeEditorMode->GetModeName(), SkillTreeEditorMode);

			MakeShared<FSkillEditor>()->InitAssetEditor(SkillTree, EditWithinLevelEditor, EditorModes);
		}
	}
}
