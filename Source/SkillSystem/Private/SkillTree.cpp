// Copyright Moonlit Night Studios Ltd. All rights Reserved.

#include "SkillTree.h"
#include "Skill.h"
#include "Blueprint/UserWidget.h"

#if WITH_EDITOR

void USkillTree::PostDuplicate(bool bDuplicateForPIE)
{
    //If the duplicate is an asset, set the Asset reference to this.
    //This can happen when an item asset is being duplicated in editor
    if (IsAsset())
    {
        AssetRef = this;
    }
}

void USkillTree::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
    UObject::PostEditChangeProperty(PropertyChangedEvent);
}

#endif

USkill* USkillTree::AddSkill()
{
    Modify();
    USkill* Skill = NewObject<USkill>(this);
    Skills.Add(Skill);
    return Skill;
}

void USkillTree::RemoveSkill(USkill* Skill)
{
    Modify();
    Skills.Remove(Skill);
}

UUserWidget* USkillTree::CreateWidget()
{
    return nullptr;
}

TSubclassOf<UUserWidget> USkillTree::GetSkillNodeWidgetClass(USkill* Skill) const
{
    return Skill && Skill->SkillWidgetOverride ? Skill->SkillWidgetOverride : DefaultSkillWidget;
}
