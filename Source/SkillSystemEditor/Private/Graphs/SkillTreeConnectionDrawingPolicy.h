// Copyright Moonlit Night Studios Ltd. All Rights Reserved.

#pragma once

#include "ConnectionDrawingPolicy.h"
#include "EdGraphUtilities.h"
#include "SkillTree.h"

class SKILLSYSTEMEDITOR_API FSkillTreeConnectionDrawingPolicy final : public FConnectionDrawingPolicy
{

	USkillTree* SkillTree = nullptr;
	
public:

	FSkillTreeConnectionDrawingPolicy(int32 InBackLayerID, int32 InFrontLayerID, float InZoomFactor, const FSlateRect& InClippingRect, FSlateWindowElementList& InDrawElements, UEdGraph* InGraphObj);

	virtual void DrawSplineWithArrow(const FGeometry& StartGeom, const FGeometry& EndGeom, const FConnectionParams& Params) override;
	virtual void DrawConnection(int32 LayerId, const FVector2f& Start, const FVector2f& End, const FConnectionParams& Params) override;

};

struct SKILLSYSTEMEDITOR_API FSkillTreeGraphPinConnectionFactory : public FGraphPanelPinConnectionFactory
{
public:
	virtual FConnectionDrawingPolicy* CreateConnectionPolicy(const UEdGraphSchema* Schema, int32 InBackLayerID, int32 InFrontLayerID, float ZoomFactor, const FSlateRect& InClippingRect, FSlateWindowElementList& InDrawElements, UEdGraph* InGraphObj) const override;
};
