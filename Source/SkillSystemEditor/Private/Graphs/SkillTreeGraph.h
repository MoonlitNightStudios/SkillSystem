// Copyright Moonlit Night Studios Ltd. All rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraph.h"
#include "SkillTreeGraph.generated.h"


class USkillTree;


UCLASS()
class SKILLSYSTEMEDITOR_API USkillTreeGraph : public UEdGraph
{
	GENERATED_BODY()
	
public:

	UPROPERTY()
	TObjectPtr<USkillTree> SkillTree;

};
