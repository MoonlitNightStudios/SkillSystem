// Copyright Moonlit Night Studios Ltd. All rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Nodes/SkillTreeGraphNode_Base.h"
#include "Skill.h"
#include "SkillTreeNodes.generated.h"


UCLASS()
class USkillTreeRootNode : public USkillTreeGraphNode_Base
{
	GENERATED_BODY()

public:

	//UEdGraphNode Interface
	virtual void AllocateDefaultPins() override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual TSharedPtr<SGraphNode> CreateVisualWidget() override;
	//virtual void ReconstructNode() override;
	//~UEdGraphNode Interface

	virtual UObject* GetWrappedObject() const override;
	virtual TSubclassOf<UUserWidget> GetNodeWidgetclass() const override;

};


UCLASS()
class USkillNode : public USkillTreeGraphNode_Base
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, Category = "Skill")
	TObjectPtr<USkill> Skill;

	//UEdGraphNode Interface
	virtual void AllocateDefaultPins() override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual void DestroyNode() override;
	virtual TSharedPtr<SGraphNode> CreateVisualWidget() override;
	virtual void PinConnectionListChanged(UEdGraphPin* Pin) override;
	//virtual void ReconstructNode() override;
	//~UEdGraphNode Interface

	virtual UObject* GetWrappedObject() const override;
	virtual TSubclassOf<UUserWidget> GetNodeWidgetclass() const override;

};
