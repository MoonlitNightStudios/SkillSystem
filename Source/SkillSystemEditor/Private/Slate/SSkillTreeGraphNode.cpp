// Copyright Moonlit Night Studios Ltd. All rights Reserved.

#include "Slate/SSkillTreeGraphNode.h"

#include "SGraphPanel.h"
#include "Nodes/SkillTreeNodes.h"
#include "SGraphPin.h"
#include "SkillEditor.h"
#include "SkillNodeWidgetInterface.h"
#include "Blueprint/UserWidget.h"
#include "EditorModes/SkillTreeEditorMode.h"

class SSkillTreeInputPin : public SGraphPin
{
public:
	SLATE_BEGIN_ARGS(SSkillTreeInputPin){}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdGraphPin* InPin, TSharedPtr<SWidget> InWidget);
protected:
	// Begin SGraphPin interface
	virtual TSharedRef<SWidget>	GetDefaultValueWidget() override;
	// End SGraphPin interface

	TSharedPtr<SWidget> Widget;
};

void SSkillTreeInputPin::Construct(const FArguments& InArgs, UEdGraphPin* InPin, TSharedPtr<SWidget> InWidget)
{
	this->SetCursor( EMouseCursor::Default );

	bShowLabel = true;

	GraphPinObj = InPin;
	check(GraphPinObj != NULL);

	const UEdGraphSchema* Schema = GraphPinObj->GetSchema();
	check(Schema);

	Widget = InWidget;

	SBorder::Construct( SBorder::FArguments()
		.BorderBackgroundColor( FColor(0,0,0,0) )
		.Cursor( this, &SSkillTreeInputPin::GetPinCursor )
		[
			Widget.ToSharedRef()
		]
	);
}

TSharedRef<SWidget> SSkillTreeInputPin::GetDefaultValueWidget()
{
	return SNew(STextBlock);
}

class SSkillTreeOutputPin : public SGraphPin
{
public:
	SLATE_BEGIN_ARGS(SSkillTreeOutputPin){}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdGraphPin* InPin);
protected:
	// Begin SGraphPin interface
	virtual TSharedRef<SWidget>	GetDefaultValueWidget() override;
	// End SGraphPin interface

	const FSlateBrush* GetPinBorder() const;
};

void SSkillTreeOutputPin::Construct(const FArguments& InArgs, UEdGraphPin* InPin)
{
	this->SetCursor( EMouseCursor::Default );

	bShowLabel = true;

	GraphPinObj = InPin;
	check(GraphPinObj != NULL);

	const UEdGraphSchema* Schema = GraphPinObj->GetSchema();
	check(Schema);

	// Set up a hover for pins that is tinted the color of the pin.
	SBorder::Construct( SBorder::FArguments()
		.BorderImage( this, &SSkillTreeOutputPin::GetPinBorder )
		.BorderBackgroundColor( this, &SSkillTreeOutputPin::GetPinColor )
		.OnMouseButtonDown( this, &SSkillTreeOutputPin::OnPinMouseDown )
		.Cursor( this, &SSkillTreeOutputPin::GetPinCursor )
	);
}

TSharedRef<SWidget>	SSkillTreeOutputPin::GetDefaultValueWidget()
{
	return SNew(STextBlock);
}

const FSlateBrush* SSkillTreeOutputPin::GetPinBorder() const
{
	return ( IsHovered() )
		? FAppStyle::GetBrush( TEXT("Graph.StateNode.Pin.BackgroundHovered") )
		: FAppStyle::GetBrush( TEXT("Graph.StateNode.Pin.Background") );
}

void SSkillTreeGraphNode::Construct(const FArguments& InArgs, USkillTreeGraphNode_Base* InNode)
{
	GraphNode = InNode;
	SetCursor(EMouseCursor::CardinalCross);
	WidgetClass = InNode->GetNodeWidgetclass();
	UpdateGraphNode();
}

void SSkillTreeGraphNode::GetNodeInfoPopups(FNodeInfoContext* Context, TArray<FGraphInformationPopupInfo>& Popups) const
{
	GetTreeInfoPopup(GraphNode, Popups);
}

void SSkillTreeGraphNode::UpdateGraphNode()
{
	InputPins.Empty();
	OutputPins.Empty();

	// Reset variables that are going to be exposed, in case we are refreshing an already setup node.
	RightNodeBox.Reset();
	LeftNodeBox.Reset();

	FLinearColor TitleShadowColor(0.6f, 0.6f, 0.6f);
	
	UAssetEditorSubsystem* AssetEditorSS = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>();
	auto E = AssetEditorSS->FindEditorForAsset(GetNodeObj()->GetOutermostObject(), false);
	FSkillEditor* Editor = static_cast<FSkillEditor*>(E);
	FSkillTreeEditorMode* TreeEditorInstance = Editor ? static_cast<FSkillTreeEditorMode*>(Editor->GetCurrentModePtr().Get()) : nullptr;
	
	UUserWidget* SkillWidget = TreeEditorInstance ? CreateWidget<UUserWidget>(TreeEditorInstance->PreviewScene.GetWorld(), WidgetClass) : nullptr;
	if (SkillWidget && SkillWidget->Implements<USkillNodeWidgetInterface>())
	{
		if (USkillNode* SkillNode = Cast<USkillNode>(GraphNode))
		{
			ISkillNodeWidgetInterface::Execute_Init(SkillWidget, SkillNode->Skill);
		}
	}
	TSharedPtr<SWidget> Widget = SkillWidget ? SkillWidget->TakeWidget() : SNew(STextBlock).Text(FText::FromString(TEXT("Skill")));
	
	UEdGraphPin* InPin = nullptr;
	for (UEdGraphPin* Pin : GraphNode->Pins)
	{
		if (Pin->Direction == EGPD_Input)
		{
			InPin = Pin;
			break;
		}
	}
	TSharedPtr<SGraphPin> InputPin = nullptr;
	if (InPin)
	{
		InputPin = SNew(SSkillTreeInputPin, InPin, Widget);
		InputPin->SetOwner(SharedThis(this));
		InputPins.Add(InputPin.ToSharedRef());
	}
	
	ContentScale.Bind(this, &SGraphNode::GetContentScale);
	GetOrAddSlot(ENodeZone::Center)
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SNew(SBorder)
			.BorderImage(FAppStyle::GetBrush("Graph.StateNode.Body"))
			.Padding(0)
			.BorderBackgroundColor(FColor::Cyan)
			[
				SNew(SOverlay)
				// PIN AREA
				+SOverlay::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				[
					SAssignNew(RightNodeBox, SVerticalBox)
				]

				// STATE WIDGET AREA
				+SOverlay::Slot()
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				.Padding(10.0f)
				[
					SNew(SBorder)
					.BorderImage( FAppStyle::GetBrush("Graph.StateNode.ColorSpill") )
					.BorderBackgroundColor( TitleShadowColor )
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					.Visibility(EVisibility::SelfHitTestInvisible)
					[
						InPin ? InputPin.ToSharedRef() : Widget.ToSharedRef()
					]
				]
			]
		];

	CreatePinWidgets();
}

void SSkillTreeGraphNode::CreatePinWidgets()
{
	UEdGraphNode* SkillNode = CastChecked<UEdGraphNode>(GraphNode);

	UEdGraphPin* CurPin = nullptr;
	for (UEdGraphPin* Pin : SkillNode->Pins)
	{
		if (Pin->Direction == EGPD_Output)
		{
			CurPin = Pin;
			break;
		}
	}
	if (!CurPin->bHidden)
	{
		TSharedPtr<SGraphPin> NewPin = SNew(SSkillTreeOutputPin, CurPin);

		this->AddPin(NewPin.ToSharedRef());
	}
}

void SSkillTreeGraphNode::AddPin(const TSharedRef<SGraphPin>& PinToAdd)
{
	PinToAdd->SetOwner(SharedThis(this));

	RightNodeBox->AddSlot()
	.HAlign(HAlign_Fill)
	.VAlign(VAlign_Fill)
	.FillHeight(1.0f)
	[
		PinToAdd
	];
	OutputPins.Add(PinToAdd);
}

void SSkillTreeGraphNode::MoveTo(const FVector2f& NewPosition, FNodeSet& NodeFilter, bool bMarkDirty)
{
	if (USkillNode* SkillNode = Cast<USkillNode>(GetNodeObj()))
	{
		SkillNode->Skill->NodeLocation = SkillNode->GetPosition();
	}
	SGraphNode::MoveTo(NewPosition, NodeFilter, bMarkDirty);
}

TSharedPtr<SToolTip> SSkillTreeGraphNode::GetComplexTooltip()
{
	return TSharedPtr<SToolTip>();
}

void SSkillTreeGraphNode::OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	// Add pins to the hover set so outgoing transitions arrows remains highlighted while the mouse is over the node
	if (const UEdGraphNode* SkillNode = Cast<UEdGraphNode>(GraphNode))
	{
		const UEdGraphPin* OutputPin = nullptr;
		for (UEdGraphPin* Pin : SkillNode->Pins)
		{
			if (Pin->Direction == EGPD_Output)
			{
				OutputPin = Pin;
				break;
			}
		}
		if (OutputPin)
		{
			TSharedPtr<SGraphPanel> OwnerPanel = GetOwnerPanel();
			check(OwnerPanel.IsValid());

			for (int32 LinkIndex = 0; LinkIndex < OutputPin->LinkedTo.Num(); ++LinkIndex)
			{
				OwnerPanel->AddPinToHoverSet(OutputPin->LinkedTo[LinkIndex]);
			}
		}
	}
	
	SGraphNode::OnMouseEnter(MyGeometry, MouseEvent);
}

void SSkillTreeGraphNode::OnMouseLeave(const FPointerEvent& MouseEvent)
{
	// Remove manually added pins from the hover set
	if (const UEdGraphNode* SkillNode = Cast<UEdGraphNode>(GraphNode))
	{
		const UEdGraphPin* OutputPin = nullptr;
		for (UEdGraphPin* Pin : SkillNode->Pins)
		{
			if (Pin->Direction == EGPD_Output)
			{
				OutputPin = Pin;
				break;
			}
		}
		if (OutputPin)
		{
			TSharedPtr<SGraphPanel> OwnerPanel = GetOwnerPanel();
			check(OwnerPanel.IsValid());

			for (int32 LinkIndex = 0; LinkIndex < OutputPin->LinkedTo.Num(); ++LinkIndex)
			{
				OwnerPanel->RemovePinFromHoverSet(OutputPin->LinkedTo[LinkIndex]);
			}
		}
	}

	SGraphNode::OnMouseLeave(MouseEvent);
}

void SSkillTreeGraphNode::GetTreeInfoPopup(UEdGraphNode* GraphNode, TArray<FGraphInformationPopupInfo>& Popups)
{
	
}

FSlateColor SSkillTreeGraphNode::GetBorderBackgroundColor() const
{
	return FColor(0,0,0,0);
}

TSharedPtr<SGraphNode> FSkillTreeGraphNodeFactory::CreateNode(UEdGraphNode* InNode) const
{
	if (USkillTreeGraphNode_Base* Node = Cast<USkillTreeGraphNode_Base>(InNode))
	{
		return SNew(SSkillTreeGraphNode, Node);
	}
	return nullptr;
}
