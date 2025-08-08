// Copyright Moonlit Night Studios Ltd. All rights Reserved.

#include "SkillEditor.h"
#include "SkillTree.h"
#include "EditorModes/SkillTreeEditorMode.h"
#include "WorkflowOrientedApp/SModeWidget.h"

void FSkillEditor::InitAssetEditor(UObject* Asset, TSharedPtr<IToolkitHost> EditWithinLevelEditor, TMap<FName, TSharedRef<FApplicationMode>>& Modes)
{
	for (TPair<FName, TSharedRef<FApplicationMode>>& Mode : Modes)
	{
		AddApplicationMode(Mode.Key, Mode.Value);
	}

	FWorkflowCentricApplication::InitAssetEditor(EToolkitMode::Standalone, EditWithinLevelEditor, "SkillEditor", FTabManager::FLayout::NullLayout, true, true, Asset);

	TSharedPtr<FExtender> ToolBarExtender = MakeShareable(new FExtender());
	ToolBarExtender->AddToolBarExtension("Asset", EExtensionHook::After, GetToolkitCommands(),
		FToolBarExtensionDelegate::CreateSP(this, &FSkillEditor::FillEditorModesToolbar));
	AddToolbarExtender(ToolBarExtender);

	Modes.GetKeys(ModeNames);
	if (ModeNames.Num() > 0)
	{
		SetCurrentMode(ModeNames[0]);
	}
}

void FSkillEditor::FillEditorModesToolbar(FToolBarBuilder& ToolbarBuilder)
{
	/*for (FName& Mode : ModeNames)
	{
		AddToolbarWidget(
			SNew(SModeWidget, FText::FromName(Mode), Mode)
			.OnGetActiveMode(this, &FSkillEditor::GetCurrentMode)
			.OnSetActiveMode(this, &FSkillEditor::SetCurrentMode)
			.ToolTipText(FText::Format(NSLOCTEXT("QuestEditor", "DesignerModeButtonTooltip", "Switch to {0} Mode"), FText::FromName(Mode)))
			.IconImage(FAppStyle::GetBrush("UMGEditor.SwitchToScriptingMode"))
			.AddMetaData<FTagMetaData>(FTagMetaData(Mode))
		);

		AddToolbarWidget(SNew(SSpacer).Size(FVector2D(10.0f, 1.0f)));
	}*/
}