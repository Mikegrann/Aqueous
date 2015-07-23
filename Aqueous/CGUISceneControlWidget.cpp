#include "CGUISceneControlWidget.h"

#include "CProgramContext.h"
#include "CSite.h"
#include "CDataSet.h"
#include "CMainState.h"
#include "CWorldTime.h"
#include "CTerrainNodeManager.h"
#include "CGlyphNodeManager.h"

#include <Gwen/Controls.h>
#include <Gwen/Controls/ComboBox.h>

CGUISceneControlWidget::CGUISceneControlWidget()
{
	Window = new Gwen::Controls::WindowControl(GUIManager->GetCanvas());
	Window->SetDeleteOnClose(false);
	Window->SetBounds(1200, 670, 330, 160);
	Window->SetTitle("Scene Controls");
	Window->SetHidden(true);

	EnableButton = new Gwen::Controls::Button(Window);
	EnableButton->SetBounds(15, 10, 290, 25);
	EnableButton->SetText("Enable Backdrop");
	EnableButton->onPress.Add(this, & CGUISceneControlWidget::OnToggleBackdrop);

	DateLabel = new Gwen::Controls::Label(Window);
	DateLabel->SetFont(GUIManager->GetRegularFont());
	DateLabel->SetText(L"Date/Time:");
	DateLabel->SetBounds(10, 50, 300, 30);
	DateLabel->SetTextColor(Gwen::Color(50, 20, 20, 215));

	Gwen::Controls::Button * BackButton = new Gwen::Controls::Button(Window);
	BackButton->SetBounds(15, 85, 25, 25);
	BackButton->SetText("<");
	BackButton->onPress.Add(this, &CGUISceneControlWidget::OnTimeDecrease);

	Gwen::Controls::Button * ForwardButton = new Gwen::Controls::Button(Window);
	ForwardButton->SetBounds(330 - 25 - 15, 85, 25, 25);
	ForwardButton->SetText(">");
	ForwardButton->onPress.Add(this, &CGUISceneControlWidget::OnTimeIncrease);

	DateLabel = new Gwen::Controls::Label(Window);
	DateLabel->SetFont(GUIManager->GetRegularFont());
	DateLabel->SetText(L"Time Here");
	DateLabel->SetBounds(15 + 25 + 5, 90, 300 - 25 - 25 - 10, 25);
	DateLabel->SetTextColor(Gwen::Color(50, 20, 20, 215));
	SetDateTime();
}

void CGUISceneControlWidget::OnToggleBackdrop(Gwen::Controls::Base * Control)
{
	CProgramContext * Context = & CProgramContext::Get();
	
	Context->Scene.SkyBox->SetVisible(! Context->Scene.SkyBox->IsVisible());
	std::stringstream s;
	s << "Skybox object ";
	if (Context->Scene.SkyBox->IsVisible())
		s << "enabled.";
	else
		s << "disabled.";
	Context->GUIContext->GetConsole()->AddMessage(s.str());

	if (Context->Scene.SkyBox->IsVisible())
		((Gwen::Controls::Button *) Control)->SetText("Disable Backdrop");
	else
		((Gwen::Controls::Button *) Control)->SetText("Enable Backdrop");
}

void CGUISceneControlWidget::toggle()
{
	Window->SetHidden(Window->Visible());
}

void CGUISceneControlWidget::SetDateTime()
{
	CProgramContext * Context = &CProgramContext::Get();

	DateLabel->SetText(Context->WorldTime->GetTimeFormatted());
}

void CGUISceneControlWidget::OnTimeDecrease(Gwen::Controls::Base * Control)
{
	GUIContext->GetConsole()->AddMessage("Time Decreased");

	CProgramContext * Context = &CProgramContext::Get();
	Context->WorldTime->DecreaseTime(CWorldTime::GetTimeStep());
	SetDateTime();
}

void CGUISceneControlWidget::OnTimeIncrease(Gwen::Controls::Base * Control)
{
	GUIContext->GetConsole()->AddMessage("Time Increased");

	CProgramContext * Context = &CProgramContext::Get();
	Context->WorldTime->IncreaseTime(CWorldTime::GetTimeStep());
	SetDateTime();
}