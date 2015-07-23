#include "HorizontalSliderTooltip.h"

using namespace Gwen;
using namespace Gwen::Controls;

GWEN_CONTROL_CONSTRUCTOR(HorizontalSliderTooltip)
{
	m_SliderBar->SetHorizontal(true);
}

float HorizontalSliderTooltip::GetFloatValue()
{
	float value = Slider::GetFloatValue();
	SetToolTip(TextObject(std::to_string(value)));
	return value;
}

void HorizontalSliderTooltip::SetFloatValue(float val, bool forceUpdate)
{
	Slider::SetFloatValue(val, forceUpdate);
	GetFloatValue();
}