// Copyright Moonlit Night Studios Ltd. All rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NativeGameplayTags.h"
#include "Skill.generated.h"


class UGameplayAbility;
class UWidget;


//Skill Status
SKILLSYSTEM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_SkillTree_SkillStatus_Obtained);
SKILLSYSTEM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_SkillTree_SkillStatus_Hidden);


UCLASS(EditInlineNew, BlueprintType)
class SKILLSYSTEM_API USkill : public UObject
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	TSubclassOf<UGameplayAbility> Ability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	FGameplayTagContainer Tags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	FText SkillName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	FText SkillDescription;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	TSoftObjectPtr<UStreamableRenderAsset> SkillIcon;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visual", meta = (MustImplement = "/Script/SkillSystem.SkillNodeWidgetInterface"))
	TSubclassOf<UUserWidget> SkillWidgetOverride;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Node")
	FVector2f NodeLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Node")
	TArray<USkill*> Parents;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Node")
	TArray<USkill*> Children;

};
