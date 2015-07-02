
#pragma once

#include "CGUIContextWidget.h"
#include <Gwen/Controls/WindowControl.h>
#include <Gwen/Controls/HorizontalSlider.h>


class CGUIGlyphControlWidget : public CGUIContextWidget
{

	Gwen::Controls::WindowControl * Window;
	Gwen::Controls::Button * EnableButton;
	Gwen::Controls::Label * DateLabel;

	void SetButtonTitle();
	void SetDateTime();

public:

	CGUIGlyphControlWidget();

	void OnToggleGlyphs(Gwen::Controls::Base * Control);
	void OnSelectPoint(Gwen::Controls::Base * Control);
	void OnSelectGrid(Gwen::Controls::Base * Control);
	void OnSizeSlider(Gwen::Controls::Base * Control);
	void OnTimeDecrease(Gwen::Controls::Base * Control);
	void OnTimeIncrease(Gwen::Controls::Base * Control);

	void toggle();

};
