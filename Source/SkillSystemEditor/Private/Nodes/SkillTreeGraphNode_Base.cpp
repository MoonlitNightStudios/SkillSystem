// Copyright Moonlit Night Studios Ltd. All rights Reserved.

#include "Nodes/SkillTreeGraphNode_Base.h"
#include "Graphs/SkillTreeGraphSchema.h"
#include "Graphs/SkillTreeGraph.h"
#include "SkillTree.h"

void USkillTreeGraphNode_Base::PinConnectionListChanged(UEdGraphPin* Pin)
{
	//Only handle output pin changes
	if (Pin->Direction != EEdGraphPinDirection::EGPD_Output) return;

	
}

void USkillTreeGraphNode_Base::DestroyNode()
{
	TArray<UEdGraphPin*> PinsToNotify;
	for (UEdGraphPin* Pin : Pins)
	{
		PinsToNotify.Append(Pin->LinkedTo);
	}

	Super::DestroyNode();
	//Pin connection list changed is not called by destroy node. Call it manually
	for (UEdGraphPin* Pin : PinsToNotify)
	{
		Pin->GetOwningNode()->PinConnectionListChanged(Pin);
	}
}

void USkillTreeGraphNode_Base::ReconstructNode()
{
	Modify();
	// Break any links to 'orphan' pins
	/*for (int32 PinIndex = 0; PinIndex < Pins.Num(); ++PinIndex)
	{
		UEdGraphPin* Pin = Pins[PinIndex];
		TArray<class UEdGraphPin*>& LinkedToRef = Pin->LinkedTo;
		for (int32 LinkIdx = 0; LinkIdx < LinkedToRef.Num(); LinkIdx++)
		{
			UEdGraphPin* OtherPin = LinkedToRef[LinkIdx];
			// If we are linked to a pin that its owner doesn't know about, break that link
			if (!OtherPin->GetOwningNode()->Pins.Contains(OtherPin))
			{
				Pin->LinkedTo.Remove(OtherPin);
			}
		}
	}*/

	// Move the existing pins to a saved map
	TMap<FName, UEdGraphPin*> OldPins;
	for (UEdGraphPin* Pin : Pins)
	{
		OldPins.Add(Pin->PinName, Pin);
	}
	Pins.Reset();

	// Recreate the new pins
	AllocateDefaultPins();

	for (int32 PinIndex = 0; PinIndex < Pins.Num(); PinIndex++)
	{
		if (OldPins.Contains(Pins[PinIndex]->PinName))
		{
			Pins[PinIndex]->MovePersistentDataFromOldPin(*OldPins[Pins[PinIndex]->PinName]);
		}
	}

	for (TPair<FName, UEdGraphPin*>& Pin : OldPins)
	{
		Pin.Value->MarkAsGarbage();
	}

	GetGraph()->NotifyGraphChanged();
}

FLinearColor USkillTreeGraphNode_Base::GetNodeTitleColor() const
{
	return FColor::Purple;
}

FLinearColor USkillTreeGraphNode_Base::GetNodeCommentColor() const
{
	return FLinearColor::White;
}

FLinearColor USkillTreeGraphNode_Base::GetNodeBodyTintColor() const
{
	return FLinearColor::Gray;
}

USkillTree* USkillTreeGraphNode_Base::GetSkillTree() const
{
	return Cast<USkillTreeGraph>(GetGraph())->SkillTree;
}
