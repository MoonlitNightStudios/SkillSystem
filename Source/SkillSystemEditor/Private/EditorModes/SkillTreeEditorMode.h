// Copyright Moonlit Night Studios Ltd. All rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/ApplicationMode.h"


class USkillTree;
class USkillNode;


class FSkillTreeEditorMode : public FApplicationMode
{

public:
	
	void Init(USkillTree* InSkillTree);

	FSkillTreeEditorMode(FName InModeName);
	virtual void RegisterTabFactories(TSharedPtr<class FTabManager> InTabManager) override;
	//virtual void PreDeactivateMode() override;
	//virtual void PostActivateMode() override;

	bool GetBoundsForSelectedNodes(class FSlateRect& Rect, float Padding);

protected:

	void OnSelectedNodesChanged(const FGraphPanelSelectionSet& SelectionSet);
	void OnPropertiesChanged(const FPropertyChangedEvent& PropertyChangedEvent) const;
	
	TSharedPtr<IDetailsView> PropertiesView;
	TSharedPtr<SGraphEditor> GraphCanvas;

	/** Command list for this editor */
	TSharedPtr<FUICommandList> GraphEditorCommands;

public:
	
	/** The preview scene that owns the preview GUI */
	FPreviewScene PreviewScene;
	
private:

	/** Select every node in the graph */
	void SelectAllNodes();

	/** Whether we can select every node */
	bool CanSelectAllNodes() const;

	/** Remove the currently selected nodes from editor view*/
	void RemoveSelectedNodes();
	void RemoveNodes(const TSet<UObject*>& NodesToRemove);

	/** Whether we are able to remove the currently selected nodes */
	bool CanRemoveNodes() const;

};
