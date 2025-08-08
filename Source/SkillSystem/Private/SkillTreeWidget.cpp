// Copyright Moonlit Night Studios Ltd. All Rights Reserved.


#include "SkillTreeWidget.h"

#include "Skill.h"
#include "SkillConnectionDrawingPolicy.h"
#include "SkillNodeWidgetInterface.h"
#include "SkillTree.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanelSlot.h"

void USkillTreeWidget::SetSkillTree(USkillTree* Tree)
{
	if (!Canvas)
	{
		Canvas = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("Canvas"));
		WidgetTree->RootWidget = Canvas;
	}
	Canvas->ClearChildren();
	Canvas->GetCanvasWidget()->ClearChildren();
	RootWidget = nullptr;
	SkillWidgets.Empty();

	SkillTree = Tree;
	
	if (!Tree) return;

	//Create Root Widget
	{
		RootWidget = CreateWidget<UUserWidget>(this, Tree->RootWidget);
		//UButton* Button = CreateWidget(UButton::StaticClass());
		//Button->AddChild(SkillWidget);
		UCanvasPanelSlot* CanvasSlot = Canvas->AddChildToCanvas(RootWidget);
		CanvasSlot->SetPosition(FVector2D::ZeroVector);
		CanvasSlot->SetAutoSize(true);
		CanvasSlot->SetAlignment(FVector2D(0.5,0.5));
		CanvasSlot->SetAnchors(FAnchors(0.5,1,0.5,1));
	}

	//Create Skill nodes
	for (USkill* Skill : Tree->Skills)
	{
		UUserWidget* SkillWidget = CreateWidget<UUserWidget>(this, Tree->GetSkillNodeWidgetClass(Skill));
		if (SkillWidget->Implements<USkillNodeWidgetInterface>())
		{
			ISkillNodeWidgetInterface::Execute_Init(SkillWidget, Skill);
		}
		Canvas->GetCanvasWidget()->AddSlot().Alignment(FVector2D(0.5,0.5))
		.Anchors(FAnchors(0.5,1,0.5,1))
		.AutoSize(true)
		.Offset(FVector2D(Skill->NodeLocation))
		[
			SNew(SButton)
			.OnClicked_UObject(this, &USkillTreeWidget::OnSkillButtonClicked, Skill)
			[
				SkillWidget->TakeWidget()
			]
		]
		;
		/*UButton* Button = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass());
		Button->AddChild(SkillWidget);
		Button->OnClicked.AddDynamic(this, &USkillTreeWidget::OnSkillButtonClicked);
		UCanvasPanelSlot* CanvasSlot = Canvas->AddChildToCanvas(Button);
		CanvasSlot->SetPosition(FVector2D(Skill->NodeLocation));
		CanvasSlot->SetAutoSize(true);
		CanvasSlot->SetAlignment(FVector2D(0.5,0.5));
		CanvasSlot->SetAnchors(FAnchors(0.5,1,0.5,1));*/

		SkillWidgets.Add(Skill, SkillWidget);
	}

	//Draw connections in OnPaint
}

void USkillTreeWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	SetSkillTree(SkillTree);
}

void USkillTreeWidget::NativeDestruct()
{
	SkillWidgets.Empty();
	RootWidget = nullptr;
	
	Super::NativeDestruct();
}

int32 USkillTreeWidget::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
                                    const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
                                    const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	if (USkillConnectionDrawingPolicy* DrawPolicy = SkillTree ? SkillTree->ConnectionDrawingPolicy : nullptr)
	{
		FVector2f RootLocation = FVector2f(RootWidget->GetPaintSpaceGeometry().GetAbsolutePositionAtCoordinates({0.5f,0.5f}));
		for (USkill* Skill : SkillTree->Skills)
		{
			if (Skill)
			{
				FVector2f SkillLocation = FVector2f(SkillWidgets[Skill]->GetPaintSpaceGeometry().GetAbsolutePositionAtCoordinates({0.5f,0.5f}));
				if (Skill->Parents.IsEmpty())
				{
					DrawPolicy->DrawConnection(OutDrawElements, LayerId, FPaintGeometry(), SkillLocation, RootLocation);
				}
				else
				{
					for (USkill* ParentSkill : Skill->Parents)
					{
						FVector2f ParentLocation = FVector2f(SkillWidgets[ParentSkill]->GetPaintSpaceGeometry().GetAbsolutePositionAtCoordinates({0.5f,0.5f}));
						DrawPolicy->DrawConnection(OutDrawElements, LayerId, FPaintGeometry(), SkillLocation, ParentLocation);
					}
				}
			}
		}
	}
	
	return Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle,
	                          bParentEnabled);
}

FReply USkillTreeWidget::OnSkillButtonClicked(USkill* Skill)
{
	SkillSelected(Skill);
	OnSkillSelected.Broadcast(Skill);
	return FReply::Handled();
}
