// Copyright Moonlit Night Studios Ltd. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SkillConnectionDrawingPolicy.generated.h"


UCLASS(Blueprintable, BlueprintType, EditInlineNew)
class SKILLSYSTEM_API USkillConnectionDrawingPolicy : public UObject
{
	GENERATED_BODY()

public:

	virtual void DrawConnection(FSlateWindowElementList& DrawElementsList, int32 LayerId, const FPaintGeometry& PaintGeometry, const FVector2f& Start, const FVector2f& End);
	
};
