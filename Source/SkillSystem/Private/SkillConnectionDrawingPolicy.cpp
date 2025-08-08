// Copyright Moonlit Night Studios Ltd. All Rights Reserved.


#include "SkillConnectionDrawingPolicy.h"

void USkillConnectionDrawingPolicy::DrawConnection(FSlateWindowElementList& DrawElementsList, int32 LayerId, const FPaintGeometry& PaintGeometry, const FVector2f& Start, const FVector2f& End)
{
	// Draw the spline itself
	FSlateDrawElement::MakeSpline(
		DrawElementsList,
		LayerId,
		PaintGeometry,
		Start, FVector2D::ZeroVector,
		End, FVector2D::ZeroVector,
		5.f,
		ESlateDrawEffect::None,
		FLinearColor::White
	);
}
