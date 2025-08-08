// Copyright Moonlit Night Studios Ltd. All rights Reserved.

#include "Nodes/SkillTreeNodes.h"
#include "Graphs/SkillTreeGraphSchema.h"
#include "SkillTree.h"
#include "Skill.h"
#include "KismetNodes/SGraphNodeK2Base.h"
#include "Slate/SSkillTreeGraphNode.h"
#include "Blueprint/UserWidget.h"

void USkillTreeRootNode::AllocateDefaultPins()
{
	CreatePin(EGPD_Output, PC_Next, TEXT("Root"));
}

FText USkillTreeRootNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString("Root");
}

TSharedPtr<SGraphNode> USkillTreeRootNode::CreateVisualWidget()
{
	return SNew(SSkillTreeGraphNode, this);
}

UObject* USkillTreeRootNode::GetWrappedObject() const
{
	return GetSkillTree();
}

TSubclassOf<UUserWidget> USkillTreeRootNode::GetNodeWidgetclass() const
{
	USkillTree* Tree = GetSkillTree();
	return Tree ? Tree->RootWidget : nullptr;
}

void USkillNode::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, PC_PreRequisite, TEXT(""));
	CreatePin(EGPD_Output, PC_Next, TEXT(""));
}

FText USkillNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString("Skill");
}

void USkillNode::DestroyNode()
{
	USkillTree* Tree = GetSkillTree();
	if (Tree)
	{
		Tree->RemoveSkill(Skill);
	}
	
	Super::DestroyNode();
}

TSharedPtr<SGraphNode> USkillNode::CreateVisualWidget()
{
	return SNew(SSkillTreeGraphNode, this);
}

void USkillNode::PinConnectionListChanged(UEdGraphPin* Pin)
{
	Super::PinConnectionListChanged(Pin);

	Skill->Modify();
	
	if (Pin->Direction == EGPD_Output)
	{
		Skill->Children.Empty(Pin->LinkedTo.Num());
		for (UEdGraphPin* LinkedPin : Pin->LinkedTo)
		{
			if (USkillNode* OtherNode = Cast<USkillNode>(LinkedPin->GetOwningNode()))
			{
				Skill->Children.Add(OtherNode->Skill);
			}
		}
	}
	else
	{
		Skill->Parents.Empty(Pin->LinkedTo.Num());
		for (UEdGraphPin* LinkedPin : Pin->LinkedTo)
		{
			if (USkillNode* OtherNode = Cast<USkillNode>(LinkedPin->GetOwningNode()))
			{
				Skill->Parents.Add(OtherNode->Skill);
			}
		}
	}
}

UObject* USkillNode::GetWrappedObject() const
{
	return Skill;
}

TSubclassOf<UUserWidget> USkillNode::GetNodeWidgetclass() const
{
	USkillTree* Tree = GetSkillTree();
	return Tree ? Tree->GetSkillNodeWidgetClass(Skill) : nullptr;
}