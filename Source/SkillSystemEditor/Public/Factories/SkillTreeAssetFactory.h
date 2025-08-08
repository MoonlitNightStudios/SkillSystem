// Copyright Moonlit Night Studios Ltd. All rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "SkillTreeAssetFactory.generated.h"


UCLASS()
class SKILLSYSTEMEDITOR_API USkillTreeAssetFactory : public UFactory
{
	GENERATED_BODY()

public:

	USkillTreeAssetFactory();

	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;

};
