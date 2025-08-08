// Copyright Moonlit Night Studios Ltd. All rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

struct FSkillTreeGraphPinConnectionFactory;

class FSkillSystemEditorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

protected:

	TSharedPtr<FSkillTreeGraphPinConnectionFactory> SkillTreeGraphPinConnectionFactory;
	
};
