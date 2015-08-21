
#pragma once

#include <ionGUI.h>


class CGUIContext;

class CGUIContextWidget : public CGUIWidget
{

public:

	CGUIContextWidget();
	virtual void toggle() {}

protected:

	CGUIContext * GUIContext;

};
