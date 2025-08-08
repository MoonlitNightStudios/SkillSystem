// Copyright Moonlit Night Studios Ltd. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/CanvasPanel.h"
#include "SkillTreeWidget.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSkillSelectedDelegate, USkill*, Skill);


UCLASS(Abstract)
class SKILLSYSTEM_API USkillTreeWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, Category = "SkillSystem")
	void SetSkillTree(USkillTree* Tree);

	UFUNCTION(BlueprintCallable, Category = "SkillSystem")
	USkillTree* GetSkillTree() const { return SkillTree; }

	virtual void NativePreConstruct() override;
	virtual void NativeDestruct() override;
	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled ) const override;

	UPROPERTY(BlueprintAssignable, Category = "SkillSystem")
	FSkillSelectedDelegate OnSkillSelected;
	
protected:
	
	FReply OnSkillButtonClicked(USkill* Skill);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "SkillSystem")
	void SkillSelected(USkill* Skill);

	UPROPERTY(EditAnywhere, Category = "SkillSystem")
	USkillTree* SkillTree = nullptr;

	TMap<USkill*, UWidget*> SkillWidgets;
	UWidget* RootWidget = nullptr;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* Canvas = nullptr;
	
};
