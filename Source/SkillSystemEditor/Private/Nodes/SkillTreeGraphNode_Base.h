// Copyright Moonlit Night Studios Ltd. All rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "SkillTreeGraphNode_Base.generated.h"


class USkillTree;
class USkill;


UCLASS(Abstract)
class USkillTreeGraphNode_Base : public UEdGraphNode
{
	GENERATED_BODY()

public:

	virtual void PinConnectionListChanged(UEdGraphPin* Pin) override;
	virtual void DestroyNode() override;
	virtual void ReconstructNode() override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual FLinearColor GetNodeCommentColor() const override;
	virtual FLinearColor GetNodeBodyTintColor() const override;

	USkillTree* GetSkillTree() const;

	virtual UObject* GetWrappedObject() const { return nullptr; }

	virtual TSubclassOf<class UUserWidget> GetNodeWidgetclass() const {return nullptr;}

};
