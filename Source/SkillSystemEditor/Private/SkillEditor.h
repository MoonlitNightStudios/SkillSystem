// Copyright Moonlit Night Studios Ltd. All rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/WorkflowCentricApplication.h"

class SGraphView;

class SKILLSYSTEMEDITOR_API FSkillEditor : public FWorkflowCentricApplication, public FNotifyHook
{
public:

	void InitAssetEditor(UObject* Asset, TSharedPtr<IToolkitHost> EditWithinLevelEditor, TMap<FName, TSharedRef<FApplicationMode>>& Modes);

	void FillEditorModesToolbar(FToolBarBuilder& ToolbarBuilder);

	//IToolkit implementation
	FName GetToolkitFName() const override { return "SkillEditor"; }
	FText GetBaseToolkitName() const override { return INVTEXT("Skill Editor"); }
	FString GetWorldCentricTabPrefix() const override { return "Skill "; }
	FLinearColor GetWorldCentricTabColorScale() const override { return FLinearColor::Red; }
	//~IToolkit Implementation

private:

	TArray<FName> ModeNames;

};
