#include "GraphPanel.h"

using namespace Gwen;
using namespace Gwen::Controls;

GWEN_CONTROL_CONSTRUCTOR(GraphPanel)
{
	SetUV(0, 0, 1, 1);
	SetMouseInputEnabled(true);
	m_DrawColor = Colors::White;

	SetStretch(true);
}

void GraphPanel::OnMouseClickLeft(int x, int y, bool bDown)
{
	if (IsDisabled()) return;

	if (bDown)
	{
		Gwen::MouseFocus = this;
		//onDown.Call(this);
	}
	else
	{
		if (IsHovered())
		{
			OnPress(x, y);
		}

		Gwen::MouseFocus = NULL;
		//onUp.Call(this);
	}
}

void GraphPanel::OnPress(int x, int y)
{
	Event::Information info;
	info.Point = CanvasPosToLocal(Point(x, y));
	onPress.Call(this, info);
}