// Copyright Moonlit Night Studios Ltd. All rights Reserved.

#include "EditorModes/SkillTreeEditorMode.h"
#include "Graphs/SkillTreeGraph.h"
#include "Graphs/SkillTreeGraphSchema.h"
#include "Nodes/SkillTreeNodes.h"
#include "SkillTree.h"
#include "Framework/Commands/GenericCommands.h"
#include "IStructureDetailsView.h"

#define LOCTEXT_NAMESPACE "SkillTreeEditorMode"

FSkillTreeEditorMode::FSkillTreeEditorMode(FName InModeName) : FApplicationMode(InModeName),
	PreviewScene(FPreviewScene::ConstructionValues().AllowAudioPlayback(true).ShouldSimulatePhysics(false))
{
	PreviewScene.GetWorld()->SetBegunPlay(false);
	
	TabLayout = FTabManager::NewLayout("SkillTreeLayout")
		->AddArea
		(
			FTabManager::NewPrimaryArea()->SetOrientation(Orient_Horizontal)
			->Split
			(
				//DetailsPanel
				FTabManager::NewStack()
				->SetSizeCoefficient(0.2f)
				->AddTab("DetailsTab", ETabState::OpenedTab)
			)
			->Split
			(
				// Graph canvas
				FTabManager::NewStack()
				->SetSizeCoefficient(0.8f)
				->SetHideTabWell(true)
				->AddTab("GraphCanvas", ETabState::OpenedTab)
			)
		);
}

void FSkillTreeEditorMode::Init(USkillTree* InSkillTree)
{
	FDetailsViewArgs Args;
	Args.bHideSelectionTip = true;
	//Args.NotifyHook = this;

	FStructureDetailsViewArgs StructViewArgs;

	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertiesView = PropertyModule.CreateDetailView(Args);
	PropertiesView->OnFinishedChangingProperties().AddRaw(this, &FSkillTreeEditorMode::OnPropertiesChanged);
	PropertiesView->SetObject(InSkillTree);

	// Customize the appearance of the graph.
	FGraphAppearanceInfo AppearanceInfo;
	AppearanceInfo.CornerText = LOCTEXT("SkillTreeGraphCornerText", "SKILL TREE");
	AppearanceInfo.InstructionText = LOCTEXT("SkillTreeGraphInstructionText", "Right Click to add new nodes.");

	if (!GraphEditorCommands.IsValid())
	{
		GraphEditorCommands = MakeShareable(new FUICommandList);

		// Editing commands
		GraphEditorCommands->MapAction(FGenericCommands::Get().SelectAll,
			FExecuteAction::CreateSP(this, &FSkillTreeEditorMode::SelectAllNodes),
			FCanExecuteAction::CreateSP(this, &FSkillTreeEditorMode::CanSelectAllNodes)
		);

		GraphEditorCommands->MapAction(FGenericCommands::Get().Delete,
			FExecuteAction::CreateSP(this, &FSkillTreeEditorMode::RemoveSelectedNodes),
			FCanExecuteAction::CreateSP(this, &FSkillTreeEditorMode::CanRemoveNodes)
		);
	}

	SGraphEditor::FGraphEditorEvents InEvents;
	//InEvents.OnNodeDoubleClicked
	InEvents.OnSelectionChanged = SGraphEditor::FOnSelectionChanged::CreateSP(this, &FSkillTreeEditorMode::OnSelectedNodesChanged);

	if (!InSkillTree->Graph)
	{
		USkillTreeGraph* NewGraph = NewObject<USkillTreeGraph>(InSkillTree, USkillTreeGraph::StaticClass());
		NewGraph->SkillTree = InSkillTree;
		NewGraph->Schema = USkillTreeGraphSchema::StaticClass();
		NewGraph->GetSchema()->CreateDefaultNodesForGraph(*NewGraph);
		InSkillTree->Graph = NewGraph;
	}

	GraphCanvas = SNew(SGraphEditor)
		.AdditionalCommands(GraphEditorCommands)
		.IsEditable(true)
		.Appearance(AppearanceInfo)
		.GraphToEdit(InSkillTree->Graph)
		.GraphEvents(InEvents)
		.ShowGraphStateOverlay(false);
}

void FSkillTreeEditorMode::RegisterTabFactories(TSharedPtr<class FTabManager> InTabManager)
{
	FApplicationMode::RegisterTabFactories(InTabManager);

	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(LOCTEXT("SkillTreeEditorMenuCategoryName", "Skill Tree Editor"));

	InTabManager->RegisterTabSpawner("GraphCanvas", FOnSpawnTab::CreateLambda([this](const FSpawnTabArgs&)
	{
		return SNew(SDockTab)
		[
			GraphCanvas.ToSharedRef()
		];
	})
	)
		.SetDisplayName(LOCTEXT("GraphCanvasDisplayName", "Graph canvas"))
		.SetGroup(WorkspaceMenuCategory.ToSharedRef())
		;

	InTabManager->RegisterTabSpawner("DetailsTab", FOnSpawnTab::CreateLambda([this](const FSpawnTabArgs&)
	{
		return SNew(SDockTab)
		[
			PropertiesView.ToSharedRef()
		];
	})
	)
		.SetDisplayName(LOCTEXT("SkillTreeDetailsDisplayName", "Details"))
		.SetGroup(WorkspaceMenuCategory.ToSharedRef())
		;
}

bool FSkillTreeEditorMode::GetBoundsForSelectedNodes(FSlateRect& Rect, float Padding)
{
	return GraphCanvas->GetBoundsForSelectedNodes(Rect, Padding);
}

void FSkillTreeEditorMode::OnSelectedNodesChanged(const FGraphPanelSelectionSet& SelectionSet)
{
	TArray<UObject*> Entities;
	for (UObject* Object : SelectionSet)
	{
		if (USkillTreeGraphNode_Base* Node = Cast<USkillTreeGraphNode_Base>(Object))
		{
			Entities.Add(Node->GetWrappedObject());
		}
	}
	if (Entities.Num() > 0)
	{
		PropertiesView->SetObjects(Entities);
	}
	else
	{
		PropertiesView->SetObject(Cast<USkillTreeGraph>(GraphCanvas->GetCurrentGraph())->SkillTree);
	}
}

void FSkillTreeEditorMode::OnPropertiesChanged(const FPropertyChangedEvent& PropertyChangedEvent) const
{
	GraphCanvas->NotifyGraphChanged();
}

void FSkillTreeEditorMode::SelectAllNodes()
{
	GraphCanvas->SelectAllNodes();
}

bool FSkillTreeEditorMode::CanSelectAllNodes() const
{
	return true;
}

void FSkillTreeEditorMode::RemoveSelectedNodes()
{
	RemoveNodes(GraphCanvas->GetSelectedNodes());
	GraphCanvas->ClearSelectionSet();
}

void FSkillTreeEditorMode::RemoveNodes(const TSet<UObject*>& NodesToRemove)
{
	for (UObject* SelectedNode : NodesToRemove)
	{
		UEdGraphNode* Node = Cast<UEdGraphNode>(SelectedNode);
		if (Node && Node->CanUserDeleteNode())
		{
			Node->DestroyNode();
		}
	}
}

bool FSkillTreeEditorMode::CanRemoveNodes() const
{
	return GraphCanvas->GetSelectedNodes().Num() > 0;
}

#undef LOCTEXT_NAMESPACE
