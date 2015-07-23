
#pragma once
#ifndef GWEN_CONTROLS_HORIZONTALSLIDER_TOOLTIP_H
#define GWEN_CONTROLS_HORIZONTALSLIDER_TOOLTIP_H

#include "Gwen/Controls/Base.h"
#include "Gwen/Controls/Button.h"
#include "Gwen/Controls/Dragger.h"
#include "Gwen/Gwen.h"
#include "Gwen/Skin.h"
#include "Gwen/Controls/Slider.h"
#include "Gwen/Controls/HorizontalSlider.h"

namespace Gwen
{
	namespace Controls
	{
		class GWEN_EXPORT HorizontalSliderTooltip : public HorizontalSlider
		{
			GWEN_CONTROL(HorizontalSliderTooltip, HorizontalSlider);

			virtual float GetFloatValue();
			virtual void SetFloatValue(float val, bool forceUpdate = true);
		};
	}
}
#endif