// Copyright Moonlit Night Studios Ltd. All rights Reserved.

#include "Factories/SkillTreeAssetFactory.h"
#include "SkillTree.h"

USkillTreeAssetFactory::USkillTreeAssetFactory()
{
	SupportedClass = USkillTree::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* USkillTreeAssetFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	USkillTree* SkillTree = NewObject<USkillTree>(InParent, Class, Name, Flags, Context);
	SkillTree->AssetRef = SkillTree;
	return SkillTree;
}
