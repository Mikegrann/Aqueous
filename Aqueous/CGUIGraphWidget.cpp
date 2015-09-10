#include "CGUIGraphWidget.h"

#include "CProgramContext.h"
#include "CSite.h"
#include "CPlot.h"


CGUIGraphWidget::CGUIGraphWidget()
{
	Window = new Gwen::Controls::WindowControl(GUIManager->GetCanvas());
	Window->SetDeleteOnClose(false);
	Window->SetBounds(1200, 670, 330, 330);
	Window->SetTitle("Graph Display");
	Window->SetHidden(true);
	Window->onResize.Add(this, &CGUIGraphWidget::OnResizeWindow);

	Graph = new Gwen::Controls::GraphPanel(Window);
	Graph->SetBounds(10, 45, 300, 300);
	Graph->onPress.Add(this, &CGUIGraphWidget::OnClickGraph);
	plot.setData(CProgramContext::Get().CurrentSite->GetCurrentDataSet()->GetGraphData());
	plot.setTimes(CProgramContext::Get().CurrentSite->GetCurrentDataSet()->GetGraphTimes());

	Gwen::Controls::Label *ModeLabel = new Gwen::Controls::Label(Window);
	ModeLabel->SetBounds(10, 10, 40, 25);
	ModeLabel->SetFont(GUIManager->GetRegularFont());
	ModeLabel->SetText(L"Type:");
	ModeLabel->SetTextColor(Gwen::Color(50, 50, 20, 215));

	Mode = new Gwen::Controls::ComboBox(Window);
	Mode->SetBounds(55, 10, 245, 25);
	for (int i = 0; i < CPlot::Mode::NumModes; i++) {
		std::string name = CPlot::GetModeName((CPlot::Mode)i);
		Gwen::Controls::MenuItem *newItem = Mode->AddItem(Gwen::UnicodeString(name.begin(), name.end()));
	}
	Mode->onSelection.Add(this, &CGUIGraphWidget::OnModeChange);
}

void CGUIGraphWidget::OnResizeWindow(Gwen::Controls::Base * Control) {
	double width = Window->Width() - 30;

	Graph->SizeToContents();
	Graph->SetBounds(10, 45, width, width * Graph->TextureHeight() / Graph->TextureWidth());
}

void CGUIGraphWidget::OnClickGraph(Gwen::Event::Info info) {
	SVector3f location = plot.getPosXYZ((double)info.Point.x * Graph->TextureWidth() / Graph->Width(), (double)info.Point.y * Graph->TextureHeight() / Graph->Height());
	stringstream str;
	str << "Location: " << location.X << ", " << location.Y << ", " << location.Z << std::endl;
	GUIContext->GetConsole()->AddMessage(str.str());

	double val = plot.getValAt(location.X);
	stringstream str2;
	str2 << "Value: " << val << std::endl;
	GUIContext->GetConsole()->AddMessage(str2.str());
}

void CGUIGraphWidget::OnModeChange(Gwen::Controls::Base * Control) {
	Gwen::Controls::ComboBox * Box = (Gwen::Controls::ComboBox *) Control;

	for (int i = 0; i < CPlot::Mode::NumModes; i++) {
		std::string name = CPlot::GetModeName((CPlot::Mode)i);
		if (Box->GetSelectedItem()->GetText() == Gwen::UnicodeString(name.begin(), name.end())) {
			plot.graph((CPlot::Mode)i);
			Graph->SetImage(Gwen::TextObject("Media/graph.png"));
			break;
		}
	}
}

void CGUIGraphWidget::toggle()
{
	Window->SetHidden(Window->Visible());

	if (Window->Visible()) {
		if (plot.isInvalid()) {
			OnModeChange(Mode);
			Graph->SetImage(Gwen::TextObject("Media/graph.png"));
			OnResizeWindow(Window);
		}
	}
}