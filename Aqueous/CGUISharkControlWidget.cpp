#include "CGUISharkControlWidget.h"

#include "CProgramContext.h"
#include "CMainState.h"
#include "CTerrainNodeManager.h"
#include "CSharkNodeManager.h"

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

	EnableButton = new Gwen::Controls::Button(Window);
	EnableButton->SetBounds(15, 10, 290, 25);
	EnableButton->SetText("Shark Visiblity Button");
	EnableButton->onPress.Add(this, &CGUISharkControlWidget::OnToggleShark);
	SetButtonTitle();
}

void CGUISharkControlWidget::SetButtonTitle()
{
	CProgramContext * Context = & CProgramContext::Get();
	
	if (Context->Scene.Shark->GetNode()->IsVisible())
		EnableButton->SetText("Disable Shark Visual");
	else
		EnableButton->SetText("Enable Shark Visual");
}

void CGUISharkControlWidget::OnToggleShark(Gwen::Controls::Base * Control)
{
	CProgramContext * Context = & CProgramContext::Get();

	if (! Context->Scene.Shark->GetNode()->IsVisible())
	{
		Context->Scene.Shark->GetNode()->SetVisible(true);
		GUIContext->GetConsole()->AddMessage("Shark View Enabled");
		SetButtonTitle();
	}
	else
	{
		Context->Scene.Shark->GetNode()->SetVisible(false);
		GUIContext->GetConsole()->AddMessage("Shark View Disabled");
		SetButtonTitle();
	}
}

void CGUISharkControlWidget::toggle()
{
	Window->SetHidden(Window->Visible());
}
