
#pragma once

#include "CGUIContextWidget.h"
#include <Gwen/Controls/WindowControl.h>
#include <Gwen/Controls/HorizontalSlider.h>

#include "CVolumeNodeManager.h"


class CMainState;

class CGUIVolumeControlWidget : public CGUIContextWidget
{

	CVolumeNodeManager::SControl & VolumeControl;
	CMainState & MainState;

	Gwen::Controls::WindowControl * Window;
	Gwen::Controls::Button * EnableButton;
	Gwen::Controls::HorizontalSlider * IntensitySlider;

	Gwen::Controls::ComboBox * FuncMode;
	Gwen::Controls::Label * FuncLabel;

	Gwen::Controls::HorizontalSlider * InvExponent;
	Gwen::Controls::Label * InvLabel;
	Gwen::Controls::CheckBox * LogCheckBox;
	Gwen::Controls::Label * LogLabel;

public:

	CGUIVolumeControlWidget();
	void resetVolumeRange();

	void OnEmphasisSlider(Gwen::Controls::Base * Control);
	void OnIntensitySlider(Gwen::Controls::Base * Control);
	void OnMinimumAlphaSlider(Gwen::Controls::Base * Control);
	void OnLocalRangeSlider(Gwen::Controls::Base * Control);
	void OnStepSizeSlider(Gwen::Controls::Base * Control);

	void OnResetVolume(Gwen::Controls::Base * Control);
	void OnResetAlpha(Gwen::Controls::Base * Control);

	void OnSetXAxis(Gwen::Controls::Base * Control);
	void OnSetYAxis(Gwen::Controls::Base * Control);
	void OnSetZAxis(Gwen::Controls::Base * Control);

	void OnSetXRes(Gwen::Controls::Base * Control);
	void OnSetYRes(Gwen::Controls::Base * Control);
	void OnSetZRes(Gwen::Controls::Base * Control);

	void OnVolumeMode(Gwen::Controls::Base * Control);
	void OnInterpMode(Gwen::Controls::Base * Control);
	void OnDrawField(Gwen::Controls::Base * Control);

	void OnFuncMode(Gwen::Controls::Base * Control);
	void OnExponentSlider(Gwen::Controls::Base * Control);
	void OnLogCheck(Gwen::Controls::Base * Control);

	void OnDebugMode(Gwen::Controls::Base * Control);
	void OnShadingMode(Gwen::Controls::Base * Control);
	void OnDepthMode(Gwen::Controls::Base * Control);
	void OnToggleVolume(Gwen::Controls::Base * Control);

	void toggle();

	void ShowParameters(Interp::Mode mode);
};
