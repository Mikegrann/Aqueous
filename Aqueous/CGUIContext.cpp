
#include "CGUIContext.h"

#include "SciDataManager.h"
#include "CMainState.h"

#include "CGUIGraphWidget.h"
#include "CGUIVolumeControlWidget.h"
#include "CGUITerrainControlWidget.h"
#include "CGUIGlyphControlWidget.h"
#include "CGUISharkControlWidget.h"
#include "CGUISceneControlWidget.h"

void CGUIContext::Init()
{
	Manager->Init("Media/GUI/DefaultSkin.png", "Media/GUI/OpenSans.ttf");
}

void CGUIContext::SetupMainState()
{
	GetCanvas()->SetDrawBackground(false);

	// Widgets
	Console = new CGUIConsoleWidget(& Manager->Get());
	TitleLabels = new CGUITitleLabelsWidget(CProgramContext::Get().CurrentSite);
	VolumeControl = new CGUIVolumeControlWidget();
	TerrainControl = new CGUITerrainControlWidget();
	GlyphControl = new CGUIGlyphControlWidget();
	SharkControl = new CGUISharkControlWidget();
	SceneControl = new CGUISceneControlWidget();
	GraphDisplay = new CGUIGraphWidget();
	ControlPanel = new CGUIControlPanelWidget();
	
	Console->AddMessage("GUI Initialized.");
	Console->AddMessage("Starting program...", Colors::Red);
}

void CGUIContext::Clear()
{
	Console = 0;
	TitleLabels = 0;
	VolumeControl = 0;
	TerrainControl = 0;
	GlyphControl = 0;
	SharkControl = 0;
	SceneControl = 0;
	ControlPanel = 0;

	Manager->RemoveAllWidgets();
	GetCanvas()->RemoveAllChildren();
}

Gwen::Controls::Canvas * CGUIContext::GetCanvas()
{
	return Manager->GetCanvas();
}

CGUIConsoleWidget * CGUIContext::GetConsole()
{
	return Console;
}

CGUITitleLabelsWidget * CGUIContext::GetTitleLabels()
{
	return TitleLabels;
}

CGUIVolumeControlWidget * CGUIContext::GetVolumeControl()
{
	return VolumeControl;
}

CGUITerrainControlWidget * CGUIContext::GetTerrainControl()
{
	return TerrainControl;
}

CGUIGlyphControlWidget * CGUIContext::GetGlyphControl()
{
	return GlyphControl;
}

CGUISharkControlWidget * CGUIContext::GetSharkControl()
{
	return SharkControl;
}

CGUIGraphWidget * CGUIContext::GetGraphDisplay()
{
	return GraphDisplay;
}

CGUISceneControlWidget * CGUIContext::GetSceneControl()
{
	return SceneControl;
}

CGUIControlPanelWidget * CGUIContext::GetControlPanel()
{
	return ControlPanel;
}

void CGUIContext::Toggle(CGUIContextWidget *widget) {
	widget->toggle();
}