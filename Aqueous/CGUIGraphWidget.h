
#pragma once

#include "CGUIContextWidget.h"
#include <Gwen/Controls/WindowControl.h>
#include <Gwen/Controls/Button.h>
#include "GraphPanel.h"
#include "CPlot.h"

class CGUIGraphWidget : public CGUIContextWidget
{

	Gwen::Controls::WindowControl * Window;
	Gwen::Controls::GraphPanel * Graph;
	Gwen::Controls::ComboBox * Mode;

	void SetDateTime();

public:

	CGUIGraphWidget();
	
	void OnResizeWindow(Gwen::Controls::Base * Control);
	void OnClickGraph(Gwen::Event::Info info);
	void OnModeChange(Gwen::Controls::Base * Control);

	void toggle();

	CPlot plot;

};
