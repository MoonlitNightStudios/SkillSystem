// Copyright Moonlit Night Studios Ltd. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SkillNodeWidgetInterface.generated.h"

class USkill;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USkillNodeWidgetInterface : public UInterface
{
	GENERATED_BODY()
};


class SKILLSYSTEM_API ISkillNodeWidgetInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SkillTree")
	void Init(USkill* Skill);
	
};
