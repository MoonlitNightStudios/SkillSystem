// Copyright Moonlit Night Studios Ltd. All rights Reserved.

#include "Graphs/SkillTreeGraphSchema.h"
#include "Graphs/SkillTreeGraph.h"
#include "EditorModes/SkillTreeEditorMode.h"
#include "SkillTree.h"
#include "Nodes/SkillTreeNodes.h"
#include "EdGraphNode_Comment.h"
#include "SkillEditor.h"
#include "Toolkits/ToolkitManager.h"
#include "Kismet2/KismetEditorUtilities.h"

#define LOCTEXT_NAMESPACE "DialogueSchema"

void USkillTreeGraphSchema::CreateDefaultNodesForGraph(UEdGraph& Graph) const
{
	FGraphNodeCreator<USkillTreeRootNode> NodeCreator(Graph);
	USkillTreeRootNode* RootNode = NodeCreator.CreateNode();
	NodeCreator.Finalize();
}

void USkillTreeGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	GetAllNodeActions(ContextMenuBuilder);
	GetCommentAction(ContextMenuBuilder);
}

void USkillTreeGraphSchema::GetContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
}

const FPinConnectionResponse USkillTreeGraphSchema::CanCreateConnection(const UEdGraphPin* PinA, const UEdGraphPin* PinB) const
{
	// Make sure the pins are not on the same node
	if (PinA->GetOwningNode() == PinB->GetOwningNode())
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, LOCTEXT("ConnectionSameNode", "Both are on the same node"));
	}

	// Compare the directions
	const UEdGraphPin* InputPin = PinB;
	const UEdGraphPin* OutputPin = PinA;

	if (!CategorizePinsByDirection(PinA, PinB, /*out*/ InputPin, /*out*/ OutputPin))
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, LOCTEXT("ConnectionIncompatible", "Directions are not compatible"));
	}
	
	return FPinConnectionResponse(CONNECT_RESPONSE_MAKE, TEXT(""));
}

FLinearColor USkillTreeGraphSchema::GetPinTypeColor(const FEdGraphPinType& PinType) const
{
	return FLinearColor::Red;
}

TSharedPtr<FEdGraphSchemaAction> USkillTreeGraphSchema::GetCreateCommentAction() const
{
	return TSharedPtr<FEdGraphSchemaAction>(static_cast<FEdGraphSchemaAction*>(new FSkillTreeGraphSchemaAction_NewComment));
}

void USkillTreeGraphSchema::GetPaletteActions(FGraphActionMenuBuilder& ActionMenuBuilder) const
{
	GetAllNodeActions(ActionMenuBuilder);
	GetCommentAction(ActionMenuBuilder);
}

void USkillTreeGraphSchema::GetCommentAction(FGraphActionMenuBuilder& ActionMenuBuilder) const
{
	if (!ActionMenuBuilder.FromPin)
	{
		const FText MenuDescription = LOCTEXT("AddCommentAction", "Add Comment...");
		const FText ToolTip = LOCTEXT("CreateCommentToolTip", "Creates a comment.");

		TSharedPtr<FSkillTreeGraphSchemaAction_NewComment> NewAction(new FSkillTreeGraphSchemaAction_NewComment(FText::GetEmpty(), MenuDescription, ToolTip, 0));
		ActionMenuBuilder.AddAction(NewAction);
	}
}

void USkillTreeGraphSchema::GetAllNodeActions(FGraphActionMenuBuilder& ActionMenuBuilder) const
{
	FText MenuDesc = LOCTEXT("AddSkillAction", "Add Skill");
	const FText AddToolTip = FText::Format(NSLOCTEXT("SkillTreeEditor", "NewSkillNodeTooltip", "Adds a new skill node here"), MenuDesc);
	FText Category = LOCTEXT("AddSkillAction", "Skills");
	TSharedPtr<FSkillTreeGraphSchemaAction_NewSkillNode> NewNodeAction(new FSkillTreeGraphSchemaAction_NewSkillNode(Category, MenuDesc, AddToolTip, 0));
	ActionMenuBuilder.AddAction(NewNodeAction);
}

void USkillTreeGraphSchema::AutowireNewNode(UEdGraphPin* FromPin, UEdGraphNode* ToNode) const
{
	for (UEdGraphPin* Pin : ToNode->Pins)
	{
		if (TryCreateConnection(FromPin, Pin))
		{
			FromPin->GetOwningNode()->PinConnectionListChanged(FromPin);
			ToNode->PinConnectionListChanged(Pin);
			return;
		}
	}
}

UEdGraphNode* FSkillTreeGraphSchemaAction_NewSkillNode::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode)
{
	USkillTree* SkillTree = Cast<USkillTreeGraph>(ParentGraph)->SkillTree;
	USkill* NewSkill = SkillTree->AddSkill();

	ParentGraph->Modify();

	FGraphNodeCreator<USkillNode> NodeCreator(*SkillTree->Graph);
	USkillNode* NewNode = NodeCreator.CreateNode();

	NewSkill->NodeLocation = FVector2f(Location.X, Location.Y);
	if (NewNode)
	{
		NewNode->Skill = NewSkill;
		NewNode->NodePosX = Location.X;
		NewNode->NodePosY = Location.Y;
	}
	NodeCreator.Finalize();

	if (FromPin)
	{
		Cast<USkillTreeGraphSchema>(ParentGraph->GetSchema())->AutowireNewNode(FromPin, NewNode);
	}

	return NewNode;
}

UEdGraphNode* FSkillTreeGraphSchemaAction_NewComment::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode)
{
	// Add menu item for creating comment boxes
	UEdGraphNode_Comment* CommentTemplate = NewObject<UEdGraphNode_Comment>();

	FVector2D SpawnLocation = Location;

	FSlateRect Bounds(FVector2f(Location.X - 25, Location.Y - 25), FVector2f(Location.X + 25, Location.Y + 25));
	TSharedPtr< IToolkit > FoundAssetEditor = FToolkitManager::Get().FindEditorForAsset(Cast<USkillTreeGraph>(ParentGraph)->SkillTree);
	TSharedPtr<FSkillEditor> Editor = nullptr;
	if (FoundAssetEditor.IsValid())
	{
		if ((Editor = StaticCastSharedPtr<FSkillEditor>(FoundAssetEditor)))
		{
			StaticCastSharedPtr<FSkillTreeEditorMode>(Editor->GetCurrentModePtr())->GetBoundsForSelectedNodes(Bounds, 50.f);
		}
	}

	CommentTemplate->SetBounds(Bounds);
	SpawnLocation.X = CommentTemplate->NodePosX;
	SpawnLocation.Y = CommentTemplate->NodePosY;

	return FEdGraphSchemaAction_NewNode::SpawnNodeFromTemplate<UEdGraphNode_Comment>(ParentGraph, CommentTemplate, SpawnLocation);
}

#undef LOCTEXT_NAMESPACE
