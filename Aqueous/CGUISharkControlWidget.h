
#pragma once

#include "CGUIContextWidget.h"
#include <Gwen/Controls/WindowControl.h>
#include <Gwen/Controls/HorizontalSlider.h>


class CGUISharkControlWidget : public CGUIContextWidget
{

	Gwen::Controls::WindowControl * Window;
	Gwen::Controls::Button * SharkEnableButton;
	Gwen::Controls::Button * SplineEnableButton;

	void SetButtonTitles();

public:

	CGUISharkControlWidget();

	void OnToggleShark(Gwen::Controls::Base * Control);
	void OnToggleSpline(Gwen::Controls::Base * Control);

	void toggle();

};
