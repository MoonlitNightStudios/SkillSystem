// Copyright Moonlit Night Studios Ltd. All Rights Reserved.

#include "SkillTreeConnectionDrawingPolicy.h"

#include "SkillConnectionDrawingPolicy.h"
#include "SkillTreeGraph.h"

FSkillTreeConnectionDrawingPolicy::FSkillTreeConnectionDrawingPolicy(int32 InBackLayerID, int32 InFrontLayerID,
                                                                     float InZoomFactor, const FSlateRect& InClippingRect, FSlateWindowElementList& InDrawElements, UEdGraph* InGraphObj) :
	FConnectionDrawingPolicy(InBackLayerID, InFrontLayerID, InZoomFactor, InClippingRect, InDrawElements)
{
	USkillTreeGraph* Graph = Cast<USkillTreeGraph>(InGraphObj);
	SkillTree = Graph ? Graph->SkillTree : nullptr;
}

void FSkillTreeConnectionDrawingPolicy::DrawSplineWithArrow(const FGeometry& StartGeom, const FGeometry& EndGeom,
	const FConnectionParams& Params)
{
	const FVector2f StartPoint = FGeometryHelper::CenterOf(StartGeom);
	const FVector2f EndPoint = FGeometryHelper::CenterOf(EndGeom);

	FConnectionDrawingPolicy::DrawSplineWithArrow(StartPoint, EndPoint, Params);
}

void FSkillTreeConnectionDrawingPolicy::DrawConnection(int32 LayerId, const FVector2f& Start, const FVector2f& End,
                                                       const FConnectionParams& Params)
{
	if (SkillTree)
	{
		if (SkillTree->ConnectionDrawingPolicy)
		{
			SkillTree->ConnectionDrawingPolicy->DrawConnection(DrawElementsList, LayerId, FPaintGeometry(), Start, End);
			return;
		}
	}
	FConnectionDrawingPolicy::DrawConnection(LayerId, Start, End, Params);
}

FConnectionDrawingPolicy* FSkillTreeGraphPinConnectionFactory::CreateConnectionPolicy(
	const UEdGraphSchema* Schema, int32 InBackLayerID, int32 InFrontLayerID, float ZoomFactor,
	const FSlateRect& InClippingRect, FSlateWindowElementList& InDrawElements,
	UEdGraph* InGraphObj) const
{
	if (InGraphObj->IsA<USkillTreeGraph>())
	{
		return new FSkillTreeConnectionDrawingPolicy(InBackLayerID, InFrontLayerID, ZoomFactor, InClippingRect, InDrawElements, InGraphObj);
	}
	return nullptr;
}
