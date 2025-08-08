// Copyright Moonlit Night Studios Ltd. All rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SGraphNode.h"
#include "EdGraphUtilities.h"
#include "KismetNodes/SGraphNodeK2Base.h"


class USkillTreeGraphNode_Base;


class SSkillTreeGraphNode : public SGraphNode
{
public:
	
	SLATE_BEGIN_ARGS(SSkillTreeGraphNode) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, USkillTreeGraphNode_Base* InNode);
	// SNodePanel::SNode interface
	virtual void GetNodeInfoPopups(FNodeInfoContext* Context, TArray<FGraphInformationPopupInfo>& Popups) const override;
	// End of SNodePanel::SNode interface

	// SGraphNode interface
	virtual void UpdateGraphNode() override;
	virtual void CreatePinWidgets() override;
	virtual void AddPin(const TSharedRef<SGraphPin>& PinToAdd) override;
	virtual void MoveTo( const FVector2f& NewPosition, FNodeSet& NodeFilter, bool bMarkDirty = true ) override;
	virtual TSharedPtr<SToolTip> GetComplexTooltip() override;
	// End of SGraphNode interface

	// SWidget interface
	void OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	void OnMouseLeave(const FPointerEvent& MouseEvent) override;
	// End of SWidget interface

	static void GetTreeInfoPopup(UEdGraphNode* GraphNode, TArray<FGraphInformationPopupInfo>& Popups);
protected:
	FSlateColor GetBorderBackgroundColor() const;

	TSubclassOf<class UUserWidget> WidgetClass = nullptr;
	
};


struct FSkillTreeGraphNodeFactory : public FGraphPanelNodeFactory
{
	virtual TSharedPtr<SGraphNode> CreateNode(class UEdGraphNode* InNode) const override;
};
