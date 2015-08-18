
#pragma once
#ifndef GWEN_CONTROLS_GRAPHPANEL_H
#define GWEN_CONTROLS_GRAPHPANEL_H

#include "Gwen/Controls/Base.h"
#include "Gwen/Gwen.h"
#include "Gwen/Skin.h"
#include "Gwen/Controls/ImagePanel.h"

namespace Gwen
{
	namespace Controls
	{
		class GWEN_EXPORT GraphPanel : public ImagePanel
		{
			GWEN_CONTROL(GraphPanel, ImagePanel);

		public:
			virtual void OnMouseClickLeft(int x, int y, bool bDown) override;
			virtual void OnPress(int x, int y);

			Gwen::Event::Caller	onPress;
		};
	}
}
#endif