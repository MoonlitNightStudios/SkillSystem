// Copyright Moonlit Night Studios Ltd. All rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphSchema.h"
#include "SkillTreeGraphSchema.generated.h"


class UDialogueEntity;


static const FName PC_PreRequisite = TEXT("PreRequisite");
static const FName PC_Next = TEXT("Next");


/** Action to add a node to the graph */
USTRUCT()
struct  FSkillTreeGraphSchemaAction_NewSkillNode : public FEdGraphSchemaAction
{
	GENERATED_BODY();

	FSkillTreeGraphSchemaAction_NewSkillNode()
		: FEdGraphSchemaAction()
	{}

	FSkillTreeGraphSchemaAction_NewSkillNode(FText InNodeCategory, FText InMenuDesc, FText InToolTip, const int32 InGrouping)
		: FEdGraphSchemaAction(MoveTemp(InNodeCategory), MoveTemp(InMenuDesc), MoveTemp(InToolTip), InGrouping)
	{}

	//~ Begin FEdGraphSchemaAction Interface
	virtual UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode = true) override;
	//~ End FEdGraphSchemaAction Interface
};


/** Action to create new comment */
USTRUCT()
struct FSkillTreeGraphSchemaAction_NewComment : public FEdGraphSchemaAction
{
	GENERATED_BODY();

	FSkillTreeGraphSchemaAction_NewComment()
		: FEdGraphSchemaAction()
	{}

	FSkillTreeGraphSchemaAction_NewComment(FText InNodeCategory, FText InMenuDesc, FText InToolTip, const int32 InGrouping)
		: FEdGraphSchemaAction(MoveTemp(InNodeCategory), MoveTemp(InMenuDesc), MoveTemp(InToolTip), InGrouping)
	{}

	//~ Begin FEdGraphSchemaAction Interface
	virtual UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode = true) override;
	//~ End FEdGraphSchemaAction Interface
};


UCLASS()
class SKILLSYSTEMEDITOR_API USkillTreeGraphSchema : public UEdGraphSchema
{
	GENERATED_BODY()
	
public:

	//EdGraphSchema Interface
	virtual void CreateDefaultNodesForGraph(UEdGraph& Graph) const override;
	virtual void GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const override;
	virtual void GetContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const override;
	virtual const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* PinA, const UEdGraphPin* PinB) const override;
	//virtual const FPinConnectionResponse CanMergeNodes(const UEdGraphNode* A, const UEdGraphNode* B) const override;
	virtual FLinearColor GetPinTypeColor(const FEdGraphPinType& PinType) const override;
	/*virtual int32 GetNodeSelectionCount(const UEdGraph* Graph) const override;
	virtual bool IsCacheVisualizationOutOfDate(int32 InVisualizationCacheID) const override;
	virtual int32 GetCurrentVisualizationCacheID() const override;
	virtual void ForceVisualizationCacheClear() const override;*/
	virtual TSharedPtr<FEdGraphSchemaAction> GetCreateCommentAction() const override;
	//~EdGraphSchema Interface

	void GetPaletteActions(FGraphActionMenuBuilder& ActionMenuBuilder) const;
	void GetCommentAction(FGraphActionMenuBuilder& ActionMenuBuilder) const;
	void GetAllNodeActions(FGraphActionMenuBuilder& ActionMenuBuilder) const;

	void AutowireNewNode(UEdGraphPin* FromPin, UEdGraphNode* ToNode) const;

};
