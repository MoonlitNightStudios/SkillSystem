// Copyright Moonlit Night Studios Ltd. All rights Reserved.

#include "SkillSystemEditor.h"
#include "AssetToolsModule.h"
#include "EdGraphUtilities.h"
#include "AssetTypeActions/SkillTreeAssetTypeActions.h"
#include "Graphs/SkillTreeConnectionDrawingPolicy.h"

#define LOCTEXT_NAMESPACE "FSkillSystemEditorModule"

EAssetTypeCategories::Type SkillAssetsCategory = EAssetTypeCategories::Misc;

void FSkillSystemEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	SkillAssetsCategory = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("SkillAssetsCategory")), LOCTEXT("SkillAssetsCategory", "Skill system"));

	AssetTools.RegisterAssetTypeActions(MakeShareable(new FSkillTreeAssetTypeActions));

	SkillTreeGraphPinConnectionFactory = MakeShareable(new FSkillTreeGraphPinConnectionFactory());
	FEdGraphUtilities::RegisterVisualPinConnectionFactory(SkillTreeGraphPinConnectionFactory);
}

void FSkillSystemEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	FEdGraphUtilities::UnregisterVisualPinConnectionFactory(SkillTreeGraphPinConnectionFactory);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSkillSystemEditorModule, SkillSystemEditor)