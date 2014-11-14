#include "CGUITerrainControlWidget.h"

#include "CProgramContext.h"
#include "CMainState.h"
#include "CTerrainNodeManager.h"
#include "CWaterSceneObject.h"
#include "CGUIContext.h"

#include <Gwen/Controls.h>
#include <Gwen/Controls/ComboBox.h>


CGUITerrainControlWidget::CGUITerrainControlWidget()
	: Terrain(CProgramContext::Get().Scene.Terrain), Water(CProgramContext::Get().Scene.Water), MainState(CMainState::Get())
{
	Window = new Gwen::Controls::WindowControl(GUIManager->GetCanvas());
	Window->SetDeleteOnClose(false);
	Window->SetBounds(1200, 500, 330, 235);
	Window->SetTitle("Terrain Controls");
	Window->SetHidden(true);

	TerrainButton = new Gwen::Controls::Button(Window);
	TerrainButton->SetBounds(15, 10, 290, 25);
	TerrainButton->SetText(Terrain->GetNode()->IsVisible() ? "Disable Terrain" : "Enable Terrain");
	TerrainButton->onPress.Add(this, & CGUITerrainControlWidget::OnToggleTerrain);

	WaterButton = new Gwen::Controls::Button(Window);
	WaterButton->SetBounds(15, 10 + 35, 290, 25);
	WaterButton->SetText("Enable Water");
	//WaterButton->SetText(Water->isVisible() ? "Disable Water" : "Enable Water");
	WaterButton->onPress.Add(this, & CGUITerrainControlWidget::OnToggleWater);

	// Panel
	{
		Gwen::Controls::Label * SliderLabel = new Gwen::Controls::Label(Window);
		SliderLabel->SetFont(GUIManager->GetRegularFont());
		SliderLabel->SetText(L"Texture:");
		SliderLabel->SetBounds(10, 10 + 30 + 45, 300, 40);
		SliderLabel->SetTextColor(Gwen::Color(50, 20, 20, 215));

		Gwen::Controls::Button * ElevationButton = new Gwen::Controls::Button(Window);
		ElevationButton->SetBounds(15, 10 + 30 + 45 + 20, 140, 25);
		ElevationButton->SetText("Elevation");
		ElevationButton->onPress.Add(this, & CGUITerrainControlWidget::OnSelectElevation);

		Gwen::Controls::Button * ColorButton = new Gwen::Controls::Button(Window);
		ColorButton->SetBounds(140 + 15 + 10, 10 + 30 + 45 + 20, 140, 25);
		ColorButton->SetText("GoogleMaps");
		ColorButton->onPress.Add(this, & CGUITerrainControlWidget::OnSelectColor);

		Gwen::Controls::Label * ModeLabel = new Gwen::Controls::Label(Window);
		ModeLabel->SetFont(GUIManager->GetRegularFont());
		ModeLabel->SetText(L"Mode:");
		ModeLabel->SetBounds(10, 10 + 15 + 45 + 75, 300, 40);
		ModeLabel->SetTextColor(Gwen::Color(50, 20, 20, 215));

		Gwen::Controls::ComboBox * ModeBox = new Gwen::Controls::ComboBox(Window);
		ModeBox->SetBounds(15, 10 + 15 + 45 + 20 + 75, 200, 25);
		ModeBox->AddItem(L"Solid");
		ModeBox->AddItem(L"Wireframe");
		ModeBox->AddItem(L"AO1");
		ModeBox->AddItem(L"AO2");
		ModeBox->AddItem(L"AO3");
		ModeBox->AddItem(L"AO4");
		ModeBox->AddItem(L"AO5");
		ModeBox->AddItem(L"AO6");
		ModeBox->AddItem(L"AO7");
		ModeBox->AddItem(L"AO8");
		ModeBox->AddItem(L"AO9");
		ModeBox->onSelection.Add(this, & CGUITerrainControlWidget::OnModeSelect);
	}
}

void CGUITerrainControlWidget::OnToggleTerrain(Gwen::Controls::Base * Control)
{
	CProgramContext * Context = & CProgramContext::Get();

	if (Context->Scene.Terrain->GetNode()->IsVisible())
	{
		Context->Scene.Terrain->GetNode()->SetVisible(false);
		GUIContext->GetConsole()->AddMessage("Terrain View Disabled");
		TerrainButton->SetText("Enable Terrain");
	}
	else
	{
		Context->Scene.Terrain->GetNode()->SetVisible(true);
		GUIContext->GetConsole()->AddMessage("Terrain View Enabled");
		TerrainButton->SetText("Disable Terrain");
	}
}

void CGUITerrainControlWidget::OnToggleWater(Gwen::Controls::Base * Control)
{
	CProgramContext * Context = & CProgramContext::Get();

	//if (Context->Scene.Water->isVisible())
	//{
	//	Context->Scene.Water->setVisible(false);
	//	GUIContext->GetConsole()->AddMessage("Water View Disabled");
	//	WaterButton->SetText("Enable Water");
	//}
	//else
	//{
	//	Context->Scene.Water->setVisible(true);
	//	GUIContext->GetConsole()->AddMessage("Water View Enabled");
	//	WaterButton->SetText("Disable Water");
	//}
}

void CGUITerrainControlWidget::OnSelectElevation(Gwen::Controls::Base * Control)
{
	CProgramContext * Context = & CProgramContext::Get();
	Context->Scene.Terrain->SetDebugHeightEnabled(true);
}

void CGUITerrainControlWidget::OnSelectColor(Gwen::Controls::Base * Control)
{
	CProgramContext * Context = & CProgramContext::Get();
	Context->Scene.Terrain->SetDebugHeightEnabled(false);
}

void CGUITerrainControlWidget::OnModeSelect(Gwen::Controls::Base * Control)
{
	CProgramContext * Context = & CProgramContext::Get();
	Gwen::Controls::ComboBox * Box = (Gwen::Controls::ComboBox *) Control;
	
	if (Box->GetSelectedItem()->GetText() == Gwen::UnicodeString(L"Solid"))
	{
		Context->Scene.Terrain->DebugMode = 0;
		Context->Scene.Terrain->GetNode()->SetFeatureEnabled(ion::GL::EDrawFeature::Wireframe, false);
	}
	else if (Box->GetSelectedItem()->GetText() == Gwen::UnicodeString(L"Wireframe"))
	{
		Context->Scene.Terrain->DebugMode = 0;
		Context->Scene.Terrain->GetNode()->SetFeatureEnabled(ion::GL::EDrawFeature::Wireframe, true);
	}
	else if (Box->GetSelectedItem()->GetText() == Gwen::UnicodeString(L"AO1"))
	{
		Context->Scene.Terrain->DebugMode = 1;
		Context->Scene.Terrain->GetNode()->SetFeatureEnabled(ion::GL::EDrawFeature::Wireframe, false);
	}
	else if (Box->GetSelectedItem()->GetText() == Gwen::UnicodeString(L"AO2"))
	{
		Context->Scene.Terrain->DebugMode = 2;
		Context->Scene.Terrain->GetNode()->SetFeatureEnabled(ion::GL::EDrawFeature::Wireframe, false);
	}
	else if (Box->GetSelectedItem()->GetText() == Gwen::UnicodeString(L"AO3"))
	{
		Context->Scene.Terrain->DebugMode = 3;
		Context->Scene.Terrain->GetNode()->SetFeatureEnabled(ion::GL::EDrawFeature::Wireframe, false);
	}
	else if (Box->GetSelectedItem()->GetText() == Gwen::UnicodeString(L"AO4"))
	{
		Context->Scene.Terrain->DebugMode = 4;
		Context->Scene.Terrain->GetNode()->SetFeatureEnabled(ion::GL::EDrawFeature::Wireframe, false);
	}
	else if (Box->GetSelectedItem()->GetText() == Gwen::UnicodeString(L"AO5"))
	{
		Context->Scene.Terrain->DebugMode = 5;
		Context->Scene.Terrain->GetNode()->SetFeatureEnabled(ion::GL::EDrawFeature::Wireframe, false);
	}
	else if (Box->GetSelectedItem()->GetText() == Gwen::UnicodeString(L"AO6"))
	{
		Context->Scene.Terrain->DebugMode = 6;
		Context->Scene.Terrain->GetNode()->SetFeatureEnabled(ion::GL::EDrawFeature::Wireframe, false);
	}
	else if (Box->GetSelectedItem()->GetText() == Gwen::UnicodeString(L"AO7"))
	{
		Context->Scene.Terrain->DebugMode = 7;
		Context->Scene.Terrain->GetNode()->SetFeatureEnabled(ion::GL::EDrawFeature::Wireframe, false);
	}
	else if (Box->GetSelectedItem()->GetText() == Gwen::UnicodeString(L"AO8"))
	{
		Context->Scene.Terrain->DebugMode = 8;
		Context->Scene.Terrain->GetNode()->SetFeatureEnabled(ion::GL::EDrawFeature::Wireframe, false);
	}
	else if (Box->GetSelectedItem()->GetText() == Gwen::UnicodeString(L"AO9"))
	{
		Context->Scene.Terrain->DebugMode = 9;
		Context->Scene.Terrain->GetNode()->SetFeatureEnabled(ion::GL::EDrawFeature::Wireframe, false);
	}
}

void CGUITerrainControlWidget::toggle()
{
	Window->SetHidden(Window->Visible());
}
