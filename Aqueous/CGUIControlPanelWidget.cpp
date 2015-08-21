#include "CGUIControlPanelWidget.h"

#include "CProgramContext.h"
#include "CMainState.h"
#include "CTerrainNodeManager.h"

#include <Gwen/Controls.h>
#include <Gwen/Controls/ComboBox.h>


CGUIControlPanelWidget::CGUIControlPanelWidget()
{
	Window = new Gwen::Controls::WindowControl(GUIManager->GetCanvas());
	Window->SetDeleteOnClose(false);
	Window->SetBounds(30, 600, 660 + 30 + 165 + 165, 85);
	Window->SetTitle("Control Panel");
	Window->SetClosable(false);

	Gwen::Controls::Button * EnableButton = new Gwen::Controls::Button(Window);
	EnableButton->SetBounds(15, 10, 150, 35);
	EnableButton->SetText("Volume Controls");
	EnableButton->onPress.Add(this, & CGUIControlPanelWidget::OnToggleVolume);

	EnableButton = new Gwen::Controls::Button(Window);
	EnableButton->SetBounds(180, 10, 150, 35);
	EnableButton->SetText("Terrain Controls");
	EnableButton->onPress.Add(this, & CGUIControlPanelWidget::OnToggleTerrain);

	EnableButton = new Gwen::Controls::Button(Window);
	EnableButton->SetBounds(345, 10, 150, 35);
	EnableButton->SetText("Glyph Controls");
	EnableButton->onPress.Add(this, & CGUIControlPanelWidget::OnToggleGlyph);

	EnableButton = new Gwen::Controls::Button(Window);
	EnableButton->SetBounds(510, 10, 150, 35);
	EnableButton->SetText("Scene Controls");
	EnableButton->onPress.Add(this, & CGUIControlPanelWidget::OnToggleScene);

	EnableButton = new Gwen::Controls::Button(Window);
	EnableButton->SetBounds(675, 10, 150, 35);
	EnableButton->SetText("Shark Controls");
	EnableButton->onPress.Add(this, &CGUIControlPanelWidget::OnToggleShark);

	EnableButton = new Gwen::Controls::Button(Window);
	EnableButton->SetBounds(840, 10, 150, 35);
	EnableButton->SetText("Graph Display");
	EnableButton->onPress.Add(this, &CGUIControlPanelWidget::OnToggleGraph);

}

void CGUIControlPanelWidget::OnToggleTerrain(Gwen::Controls::Base * Control)
{
	CProgramContext * Context = & CProgramContext::Get();
	Context->GUIContext->Toggle((CGUIContextWidget *)Context->GUIContext->GetTerrainControl());
}

void CGUIControlPanelWidget::OnToggleVolume(Gwen::Controls::Base * Control)
{
	CProgramContext * Context = & CProgramContext::Get();
	Context->GUIContext->Toggle((CGUIContextWidget *)Context->GUIContext->GetVolumeControl());
}

void CGUIControlPanelWidget::OnToggleGlyph(Gwen::Controls::Base * Control)
{
	CProgramContext * Context = & CProgramContext::Get();
	Context->GUIContext->Toggle((CGUIContextWidget *)Context->GUIContext->GetGlyphControl());
}

void CGUIControlPanelWidget::OnToggleScene(Gwen::Controls::Base * Control)
{
	CProgramContext * Context = & CProgramContext::Get();
	Context->GUIContext->Toggle((CGUIContextWidget *)Context->GUIContext->GetSceneControl());
}

void CGUIControlPanelWidget::OnToggleShark(Gwen::Controls::Base * Control)
{
	CProgramContext * Context = &CProgramContext::Get();
	Context->GUIContext->Toggle((CGUIContextWidget *)Context->GUIContext->GetSharkControl());
}

void CGUIControlPanelWidget::OnToggleGraph(Gwen::Controls::Base * Control)
{
	CProgramContext * Context = &CProgramContext::Get();
	Context->GUIContext->Toggle((CGUIContextWidget *)Context->GUIContext->GetGraphDisplay());
}
