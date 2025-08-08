// Copyright Moonlit Night Studios Ltd. All rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SkillTree.generated.h"


class USkill;


UCLASS(BlueprintType)
class SKILLSYSTEM_API USkillTree : public UObject
{
	GENERATED_BODY()
	
public:

#if WITH_EDITOR
	virtual void PostDuplicate(bool bDuplicateForPIE) override;
	virtual void PostEditChangeProperty( struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

#if WITH_EDITORONLY_DATA
	UPROPERTY()
	class UEdGraph* Graph;
#endif

	USkill* AddSkill();
	void RemoveSkill(USkill* Skill);

	UFUNCTION(BlueprintCallable, Category = "SkillTree")
	const TArray<USkill*>& GetSkills() const {return Skills;}

	UFUNCTION(BlueprintCallable, Category = "SkillTree")
	UUserWidget* CreateWidget();

	UFUNCTION(BlueprintCallable, Category = "SkillTree")
	TSubclassOf<UUserWidget> GetSkillNodeWidgetClass(USkill* Skill) const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SkillTree")
	FText SkillTreeName;
	
public:
	
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Node", meta = (MustImplement = "SkillTreeWidgetInterface"))
	//TSubclassOf<UUserWidget> SkillTreeWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Node")
	TSubclassOf<UUserWidget> RootWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Node", meta = (MustImplement = "SkillNodeWidgetInterface"))
	TSubclassOf<UUserWidget> DefaultSkillWidget;

	UPROPERTY(EditAnywhere, Instanced, Category = "Debug")
	TArray<USkill*> Skills;

	UPROPERTY(EditAnywhere, Instanced, Category = "Graph")
	class USkillConnectionDrawingPolicy* ConnectionDrawingPolicy = nullptr;

private:

#if WITH_EDITOR
	friend class USkillTreeAssetFactory;
#endif //WITH_EDITOR

	//A reference to the asset that defines this item
	UPROPERTY(VisibleAnywhere, Category = "Debug")
	TSoftObjectPtr<USkillTree> AssetRef;

};
