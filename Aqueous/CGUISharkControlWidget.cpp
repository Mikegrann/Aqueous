#include "CGUISharkControlWidget.h"

#include "CProgramContext.h"
#include "CMainState.h"
#include "CTerrainNodeManager.h"
#include "CSharkNodeManager.h"
#include "CSplineNodeManager.h"

#include <Gwen/Controls.h>
#include <Gwen/Controls/ComboBox.h>
#include <sstream>


CGUISharkControlWidget::CGUISharkControlWidget()
{
	Window = new Gwen::Controls::WindowControl(GUIManager->GetCanvas());
	Window->SetDeleteOnClose(false);
	Window->SetBounds(1200, 670, 330, 160);
	Window->SetTitle("Scene Controls");
	Window->SetHidden(true);

	SharkEnableButton = new Gwen::Controls::Button(Window);
	SharkEnableButton->SetBounds(15, 10, 290, 25);
	SharkEnableButton->SetText("Shark Visiblity Button");
	SharkEnableButton->onPress.Add(this, &CGUISharkControlWidget::OnToggleShark);

	SplineEnableButton = new Gwen::Controls::Button(Window);
	SplineEnableButton->SetBounds(15, 45, 290, 25);
	SplineEnableButton->SetText("Spline Visiblity Button");
	SplineEnableButton->onPress.Add(this, &CGUISharkControlWidget::OnToggleSpline);
	SetButtonTitles();
}

void CGUISharkControlWidget::SetButtonTitles()
{
	CProgramContext * Context = & CProgramContext::Get();
	
	if (Context->Scene.Shark->GetNode()->IsVisible())
		SharkEnableButton->SetText("Disable Shark Visual");
	else
		SharkEnableButton->SetText("Enable Shark Visual");

	if (Context->Scene.Spline->GetNode()->IsVisible())
		SplineEnableButton->SetText("Disable Spline Visual");
	else
		SplineEnableButton->SetText("Enable Spline Visual");
}

void CGUISharkControlWidget::OnToggleShark(Gwen::Controls::Base * Control)
{
	CProgramContext * Context = & CProgramContext::Get();

	if (! Context->Scene.Shark->GetNode()->IsVisible())
	{
		Context->Scene.Shark->GetNode()->SetVisible(true);
		GUIContext->GetConsole()->AddMessage("Shark View Enabled");
		SetButtonTitles();
	}
	else
	{
		Context->Scene.Shark->GetNode()->SetVisible(false);
		GUIContext->GetConsole()->AddMessage("Shark View Disabled");
		SetButtonTitles();
	}
}

void CGUISharkControlWidget::OnToggleSpline(Gwen::Controls::Base * Control)
{
	CProgramContext * Context = &CProgramContext::Get();

	if (!Context->Scene.Spline->GetNode()->IsVisible())
	{
		Context->Scene.Spline->GetNode()->SetVisible(true);
		GUIContext->GetConsole()->AddMessage("Spline View Enabled");
		SetButtonTitles();
	}
	else
	{
		Context->Scene.Spline->GetNode()->SetVisible(false);
		GUIContext->GetConsole()->AddMessage("Spline View Disabled");
		SetButtonTitles();
	}
}

void CGUISharkControlWidget::toggle()
{
	Window->SetHidden(Window->Visible());
}
