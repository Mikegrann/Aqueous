#include "CGUIVolumeControlWidget.h"

#include "CProgramContext.h"
#include "CSite.h"
#include "CWorldTime.h"
#include "CGlyphNodeManager.h"
#include "CGUIContext.h"
#include "CMainState.h"

#include <Gwen/Controls.h>
#include <Gwen/Controls/ComboBox.h>
#include "HorizontalSliderTooltip.h"


CGUIVolumeControlWidget::CGUIVolumeControlWidget()
	: VolumeControl(CProgramContext::Get().Scene.Volume->Control), MainState(CMainState::Get())
{
	CProgramContext * Context = &CProgramContext::Get();
	f64 minVal = Context->CurrentSite->GetCurrentDataSet()->GetMinColorValue(), maxVal = Context->CurrentSite->GetCurrentDataSet()->GetMaxColorValue();

	Window = new Gwen::Controls::WindowControl(GUIManager->GetCanvas());
	Window->SetDeleteOnClose(false);
	Window->SetBounds(1200, 10, 330, 800);
	Window->SetTitle("Volume Controls");
	Window->SetHidden(true);

	EnableButton = new Gwen::Controls::Button(Window);
	EnableButton->SetBounds(15, 10, 290, 25);
	EnableButton->SetText("Disable Volume View");
	EnableButton->onPress.Add(this, & CGUIVolumeControlWidget::OnToggleVolume);

	// Slider Panel
	{
		Gwen::Controls::Label * SliderLabel = new Gwen::Controls::Label(Window);
		SliderLabel->SetFont(GUIManager->GetRegularFont());
		SliderLabel->SetText(L"Selection Value:");
		SliderLabel->SetBounds(10, 10 + 45, 300, 40);
		SliderLabel->SetTextColor(Gwen::Color(50, 20, 20, 215));

		Gwen::Controls::HorizontalSliderTooltip * EmphasisSlider = new Gwen::Controls::HorizontalSliderTooltip(Window);
		EmphasisSlider->SetBounds(10, 30 + 45, 300, 40);
		//EmphasisSlider->SetRange(0.f, 1.f);
		EmphasisSlider->SetRange(minVal, maxVal);
		EmphasisSlider->SetFloatValue((maxVal + minVal) / 2.0);

		SliderLabel = new Gwen::Controls::Label(Window);
		SliderLabel->SetFont(GUIManager->GetRegularFont());
		SliderLabel->SetText(L"Volume Opacity:");
		SliderLabel->SetBounds(10, 70 + 45, 300, 40);
		SliderLabel->SetTextColor(Gwen::Color(50, 20, 20, 215));

		IntensitySlider = new Gwen::Controls::HorizontalSliderTooltip(Window);
		IntensitySlider->SetBounds(10, 90 + 45, 300, 40);
		IntensitySlider->SetRange(0.5f, 10.f);
		IntensitySlider->SetFloatValue(1.f);

		SliderLabel = new Gwen::Controls::Label(Window);
		SliderLabel->SetFont(GUIManager->GetRegularFont());
		SliderLabel->SetText(L"Selection Range:");
		SliderLabel->SetBounds(10, 130 + 45, 300, 40);
		SliderLabel->SetTextColor(Gwen::Color(50, 20, 20, 215));

		Gwen::Controls::HorizontalSliderTooltip * LocalRangeSlider = new Gwen::Controls::HorizontalSliderTooltip(Window);
		LocalRangeSlider->SetBounds(10, 150 + 45, 300, 40);
		LocalRangeSlider->SetRange((maxVal - minVal) * 0.05f, (maxVal - minVal) * 0.5f);
		LocalRangeSlider->SetFloatValue((maxVal - minVal) * 0.1f);

		SliderLabel = new Gwen::Controls::Label(Window);
		SliderLabel->SetFont(GUIManager->GetRegularFont());
		SliderLabel->SetText(L"Selection Difference:");
		SliderLabel->SetBounds(10, 190 + 45, 300, 40);
		SliderLabel->SetTextColor(Gwen::Color(50, 20, 20, 215));

		Gwen::Controls::HorizontalSliderTooltip * MinimumAlphaSlider = new Gwen::Controls::HorizontalSliderTooltip(Window);
		MinimumAlphaSlider->SetBounds(10, 210 + 45, 300, 40);
		MinimumAlphaSlider->SetRange(0.0f, 0.5f);
		MinimumAlphaSlider->SetFloatValue(0.1f);

		// Wire Up Events
		EmphasisSlider->onValueChanged.Add(this,		& CGUIVolumeControlWidget::OnEmphasisSlider);
		IntensitySlider->onValueChanged.Add(this,		& CGUIVolumeControlWidget::OnIntensitySlider);
		MinimumAlphaSlider->onValueChanged.Add(this,	& CGUIVolumeControlWidget::OnMinimumAlphaSlider);
		LocalRangeSlider->onValueChanged.Add(this,		& CGUIVolumeControlWidget::OnLocalRangeSlider);
	}

	
	// Other Controls Panel
	{
		Gwen::Controls::Label * FieldLabel = new Gwen::Controls::Label(Window);
		FieldLabel->SetFont(GUIManager->GetRegularFont());
		FieldLabel->SetText(L"Data:");
		FieldLabel->SetBounds(37, 285 + 10 + 6, 90, 40);
		FieldLabel->SetTextColor(Gwen::Color(50, 50, 20, 215));

		Gwen::Controls::Label * InterpLabel = new Gwen::Controls::Label(Window);
		InterpLabel->SetFont(GUIManager->GetRegularFont());
		InterpLabel->SetText(L"Interp:");
		InterpLabel->SetBounds(27, 320 + 10 + 6, 90, 40);
		InterpLabel->SetTextColor(Gwen::Color(50, 50, 20, 215));

		// Interpolation Parameters
		{
			// Radial
			FuncLabel = new Gwen::Controls::Label(Window);
			FuncLabel->SetFont(GUIManager->GetRegularFont());
			FuncLabel->SetText(L"Func:");
			FuncLabel->SetBounds(37, 320 + 10 + 35 + 6, 90, 40);
			FuncLabel->SetTextColor(Gwen::Color(50, 50, 20, 215));

			FuncMode = new Gwen::Controls::ComboBox(Window);
			FuncMode->SetBounds(80, 120 + 120 + 35 + 45 + 45, 200, 25);
			for (int i = 0; i < Interp::RadialFunc::NumFuncs; i++) {
				std::string name = Interp::GetFuncName((Interp::RadialFunc)i);
				Gwen::Controls::MenuItem *newItem = FuncMode->AddItem(Gwen::UnicodeString(name.begin(), name.end()));
			}

			// Connor
			InvLabel = new Gwen::Controls::Label(Window);
			InvLabel->SetFont(GUIManager->GetRegularFont());
			InvLabel->SetText(L"Falloff:");
			InvLabel->SetBounds(27, 320 + 10 + 35 + 6, 90, 40);
			InvLabel->SetTextColor(Gwen::Color(50, 50, 20, 215));

			InvExponent = new Gwen::Controls::HorizontalSliderTooltip(Window);
			InvExponent->SetBounds(80, 120 + 120 + 35 + 45 + 45, 100, 25);
			InvExponent->SetRange(1.0f, 4.0f);
			InvExponent->SetFloatValue(2.0f);
			InvExponent->SetNotchCount(3);
			InvExponent->SetClampToNotches(true);

			LogLabel = new Gwen::Controls::Label(Window);
			LogLabel->SetFont(GUIManager->GetRegularFont());
			LogLabel->SetText(L"Log:");
			LogLabel->SetBounds(80 + 100 + 10, 320 + 10 + 35 + 6, 90, 40);
			LogLabel->SetTextColor(Gwen::Color(50, 50, 20, 215));

			LogCheckBox = new Gwen::Controls::CheckBox(Window);
			LogCheckBox->SetBounds(80 + 100 + 10 + 60, 320 + 10 + 35 + 6, 15, 15);
		}

		Gwen::Controls::Label * ControlLabel = new Gwen::Controls::Label(Window);
		ControlLabel->SetFont(GUIManager->GetRegularFont());
		ControlLabel->SetText(L"Draw:");
		ControlLabel->SetBounds(33, 320 + 35 + 45 + 6, 90, 40);
		ControlLabel->SetTextColor(Gwen::Color(50, 50, 20, 215));
		
		Gwen::Controls::Label * DebugLabel = new Gwen::Controls::Label(Window);
		DebugLabel->SetFont(GUIManager->GetRegularFont());
		DebugLabel->SetText(L"Debug:");
		DebugLabel->SetBounds(22, 320 + 35 + 45 + 70 + 6, 90, 40);
		DebugLabel->SetTextColor(Gwen::Color(50, 50, 20, 215));

		Gwen::Controls::Label * ShadingLabel = new Gwen::Controls::Label(Window);
		ShadingLabel->SetFont(GUIManager->GetRegularFont());
		ShadingLabel->SetText(L"Shading:");
		ShadingLabel->SetBounds(10, 320 + 35 + 45 + 70 + 35 + 6, 90, 40);
		ShadingLabel->SetTextColor(Gwen::Color(50, 50, 20, 215));

		Gwen::Controls::ComboBox * DrawField = new Gwen::Controls::ComboBox(Window);
		DrawField->SetBounds(80, 120 + 120 + 10 + 45, 200, 25);
		for (auto field : Context->CurrentSite->GetCurrentDataSet()->dataFields) {
			Gwen::Controls::MenuItem *newItem = DrawField->AddItem(Gwen::UnicodeString(field.begin(), field.end()));
		}
		
		Gwen::Controls::ComboBox * InterpMode = new Gwen::Controls::ComboBox(Window);
		InterpMode->SetBounds(80, 120 + 120 + 45 + 45, 200, 25);
		for (int i = 0; i < Interp::Mode::NumModes; i++) {
			std::string name = Interp::GetInterpName((Interp::Mode)i);
			Gwen::Controls::MenuItem *newItem = InterpMode->AddItem(Gwen::UnicodeString(name.begin(), name.end()));
		}
		OnInterpMode(InterpMode);

		Gwen::Controls::ComboBox * VolumeMode = new Gwen::Controls::ComboBox(Window);
		VolumeMode->SetBounds(80, 120 + 120 + 45 + 35 + 35 + 45, 200, 25);
		VolumeMode->AddItem(L"Constant");
		VolumeMode->AddItem(L"Sample Alpha");
		VolumeMode->AddItem(L"Plane Slices");
		VolumeMode->AddItem(L"Isosurface");

		Gwen::Controls::ComboBox * DebugMode = new Gwen::Controls::ComboBox(Window);
		DebugMode->SetBounds(80, 120 + 120 + 45 + 35 + 45 + 35 + 70, 100, 25);
		DebugMode->AddItem(L"Disabled");
		DebugMode->AddItem(L"Front");
		DebugMode->AddItem(L"Cutoff");
		DebugMode->AddItem(L"Accumulator");
		DebugMode->AddItem(L"Iterations");
		DebugMode->AddItem(L"Box Depth");
		DebugMode->AddItem(L"Gradient");

		Gwen::Controls::ComboBox * ShadingMode = new Gwen::Controls::ComboBox(Window);
		ShadingMode->SetBounds(80, 120 + 120 + 45 + 35 + 45 + 70 + 35 + 35, 200, 25);
		ShadingMode->AddItem(L"No Shading");
		ShadingMode->AddItem(L"Orbit Camera");
		ShadingMode->AddItem(L"Fly Camera");

		Gwen::Controls::Label * DepthLabel = new Gwen::Controls::Label(Window);
		DepthLabel->SetFont(GUIManager->GetRegularFont());
		DepthLabel->SetText(L"Depth:");
		DepthLabel->SetBounds(80 + 100 + 10, 320 + 45 + 70 + 35 + 6, 90, 40);
		DepthLabel->SetTextColor(Gwen::Color(50, 50, 20, 215));

		Gwen::Controls::CheckBox * DepthMode = new Gwen::Controls::CheckBox(Window);
		DepthMode->SetBounds(80 + 100 + 10 + 60, 120 + 120 + 45 + 35 + 45 + 70 + 35 + 6, 15, 15);
	
		ControlLabel = new Gwen::Controls::Label(Window);
		ControlLabel->SetFont(GUIManager->GetRegularFont());
		ControlLabel->SetText(L"Axis:");
		ControlLabel->SetBounds(43, 355 + 45 + 35 + 6, 90, 40);
		ControlLabel->SetTextColor(Gwen::Color(50, 50, 20, 215));

		Gwen::Controls::Button * pButtonX = new Gwen::Controls::Button(Window);
		pButtonX->SetBounds(80, 120 + 120 + 10 + 45 + 25 + 35 + 35 + 45, 40, 25);
		pButtonX->SetText("X");

		Gwen::Controls::Button * pButtonY = new Gwen::Controls::Button(Window);
		pButtonY->SetBounds(130, 120 + 120 + 10 + 45 + 25 + 35 + 35 + 45, 40, 25);
		pButtonY->SetText("Y");

		Gwen::Controls::Button * pButtonZ = new Gwen::Controls::Button(Window);
		pButtonZ->SetBounds(180, 120 + 120 + 10 + 45 + 25 + 35 + 35 + 45, 40, 25);
		pButtonZ->SetText("Z");

		Gwen::Controls::Label * SliderLabel = new Gwen::Controls::Label(Window);
		SliderLabel->SetFont(GUIManager->GetRegularFont());
		SliderLabel->SetText(L"Volume Detail:");
		SliderLabel->SetBounds(10, 35 + 530, 300, 40);
		SliderLabel->SetTextColor(Gwen::Color(50, 20, 20, 215));

		Gwen::Controls::HorizontalSliderTooltip * StepSizeSlider = new Gwen::Controls::HorizontalSliderTooltip(Window);
		StepSizeSlider->SetBounds(10, 35 + 545, 300, 40);
		StepSizeSlider->SetRange(10.f, 300.f);
		StepSizeSlider->SetFloatValue(VolumeControl.StepSize);

		// Resolution Panel
		Gwen::Controls::HorizontalSliderTooltip *ResolutionSliderX, *ResolutionSliderY, *ResolutionSliderZ;
		{
			Gwen::Controls::Label * ResolutionLabel = new Gwen::Controls::Label(Window);
			ResolutionLabel->SetFont(GUIManager->GetRegularFont());
			ResolutionLabel->SetText(L"Volume Resolution:");
			ResolutionLabel->SetBounds(10, 35 + 580 + 5, 300, 40);
			ResolutionLabel->SetTextColor(Gwen::Color(50, 20, 20, 215));

			Gwen::Controls::Label * ResolutionLabelX = new Gwen::Controls::Label(Window);
			ResolutionLabelX->SetFont(GUIManager->GetRegularFont());
			ResolutionLabelX->SetText(L"X:");
			ResolutionLabelX->SetBounds(15, 35 + 585 + 10 + 15, 300, 40);
			ResolutionLabelX->SetTextColor(Gwen::Color(50, 20, 20, 215));

			ResolutionSliderX = new Gwen::Controls::HorizontalSliderTooltip(Window);
			ResolutionSliderX->SetBounds(30, 35 + 585 + 10, 280, 40);
			ResolutionSliderX->SetRange(1.f, 30.f);
			ResolutionSliderX->SetFloatValue(CProgramContext::Get().Scene.Volume->Resolution.X);
			ResolutionSliderX->SetNotchCount(29);
			ResolutionSliderX->SetClampToNotches(true);

			Gwen::Controls::Label * ResolutionLabelY = new Gwen::Controls::Label(Window);
			ResolutionLabelY->SetFont(GUIManager->GetRegularFont());
			ResolutionLabelY->SetText(L"Y:");
			ResolutionLabelY->SetBounds(15, 35 + 620 + 10 + 15, 300, 40);
			ResolutionLabelY->SetTextColor(Gwen::Color(50, 20, 20, 215));

			ResolutionSliderY = new Gwen::Controls::HorizontalSliderTooltip(Window);
			ResolutionSliderY->SetBounds(30, 35 + 620 + 10, 280, 40);
			ResolutionSliderY->SetRange(1.f, 30.f);
			ResolutionSliderY->SetFloatValue(CProgramContext::Get().Scene.Volume->Resolution.Y);
			ResolutionSliderY->SetNotchCount(29);
			ResolutionSliderY->SetClampToNotches(true);

			Gwen::Controls::Label * ResolutionLabelZ = new Gwen::Controls::Label(Window);
			ResolutionLabelZ->SetFont(GUIManager->GetRegularFont());
			ResolutionLabelZ->SetText(L"Z:");
			ResolutionLabelZ->SetBounds(15, 35 + 655 + 10 + 15, 300, 40);
			ResolutionLabelZ->SetTextColor(Gwen::Color(50, 20, 20, 215));

			ResolutionSliderZ = new Gwen::Controls::HorizontalSliderTooltip(Window);
			ResolutionSliderZ->SetBounds(30, 35 + 655 + 10, 280, 40);
			ResolutionSliderZ->SetRange(1.f, 30.f);
			ResolutionSliderZ->SetFloatValue(CProgramContext::Get().Scene.Volume->Resolution.Z);
			ResolutionSliderZ->SetNotchCount(29);
			ResolutionSliderZ->SetClampToNotches(true);
		}

		// Wire Up Events
		pButtonX->onPress.Add(this,					& CGUIVolumeControlWidget::OnSetXAxis);
		pButtonY->onPress.Add(this,					& CGUIVolumeControlWidget::OnSetYAxis);
		pButtonZ->onPress.Add(this,					& CGUIVolumeControlWidget::OnSetZAxis);
		VolumeMode->onSelection.Add(this,			& CGUIVolumeControlWidget::OnVolumeMode);
		InterpMode->onSelection.Add(this,			& CGUIVolumeControlWidget::OnInterpMode);
		DrawField->onSelection.Add(this,			& CGUIVolumeControlWidget::OnDrawField);

		FuncMode->onSelection.Add(this,				& CGUIVolumeControlWidget::OnFuncMode);
		InvExponent->onValueChanged.Add(this,		& CGUIVolumeControlWidget::OnExponentSlider);
		LogCheckBox->onCheckChanged.Add(this,		& CGUIVolumeControlWidget::OnLogCheck);

		DebugMode->onSelection.Add(this,			& CGUIVolumeControlWidget::OnDebugMode);
		ShadingMode->onSelection.Add(this,			& CGUIVolumeControlWidget::OnShadingMode);
		StepSizeSlider->onValueChanged.Add(this,	& CGUIVolumeControlWidget::OnStepSizeSlider);
		DepthMode->onCheckChanged.Add(this,			& CGUIVolumeControlWidget::OnDepthMode);

		ResolutionSliderX->onValueChanged.Add(this, &CGUIVolumeControlWidget::OnSetXRes);
		ResolutionSliderY->onValueChanged.Add(this, &CGUIVolumeControlWidget::OnSetYRes);
		ResolutionSliderZ->onValueChanged.Add(this, &CGUIVolumeControlWidget::OnSetZRes);
	}
}

void CGUIVolumeControlWidget::resetVolumeRange()
{
	if (VolumeControl.Mode)
		GUIContext->GetTitleLabels()->resetVolumeRangeIndicator(CProgramContext::Get().CurrentSite);
}

void CGUIVolumeControlWidget::OnEmphasisSlider(Gwen::Controls::Base * Control)
{
	Gwen::Controls::Slider * Bar = (Gwen::Controls::Slider *) Control;
	VolumeControl.EmphasisLocation = Bar->CalculateValue();
	resetVolumeRange();

	//Update Tooltip
	Bar->GetFloatValue();
}

void CGUIVolumeControlWidget::OnIntensitySlider(Gwen::Controls::Base * Control)
{
	Gwen::Controls::Slider * Bar = (Gwen::Controls::Slider *) Control;
	VolumeControl.AlphaIntensity = Bar->GetFloatValue();
}

void CGUIVolumeControlWidget::OnMinimumAlphaSlider(Gwen::Controls::Base * Control)
{
	Gwen::Controls::Slider * Bar = (Gwen::Controls::Slider *) Control;
	VolumeControl.MinimumAlpha = Bar->GetFloatValue();
}

void CGUIVolumeControlWidget::OnLocalRangeSlider(Gwen::Controls::Base * Control)
{
	CProgramContext * Context = &CProgramContext::Get();
	f64 minVal = Context->CurrentSite->GetCurrentDataSet()->GetMinColorValue(), maxVal = Context->CurrentSite->GetCurrentDataSet()->GetMaxColorValue();

	Gwen::Controls::Slider * Bar = (Gwen::Controls::Slider *) Control;
	VolumeControl.LocalRange = Bar->GetFloatValue() / (maxVal - minVal);
	resetVolumeRange();
}

void CGUIVolumeControlWidget::OnStepSizeSlider(Gwen::Controls::Base * Control)
{
	Gwen::Controls::Slider * Bar = (Gwen::Controls::Slider *) Control;
	VolumeControl.StepSize = Bar->GetFloatValue();
}

void CGUIVolumeControlWidget::OnExponentSlider(Gwen::Controls::Base * Control)
{
	CProgramContext * Context = &CProgramContext::Get();
	Gwen::Controls::Slider * Bar = (Gwen::Controls::Slider *) Control;
	Context->Scene.Volume->SetInterpExp((int)Bar->GetFloatValue());
	Context->CurrentSite->GetCurrentDataSet()->GenerateVolume(Context->WorldTime->GetTime(), Context->Scene.Volume->GetInterp());
}

void CGUIVolumeControlWidget::OnLogCheck(Gwen::Controls::Base * Control)
{
	CProgramContext * Context = &CProgramContext::Get();
	Gwen::Controls::CheckBox * Box = (Gwen::Controls::CheckBox *) Control;
	Context->Scene.Volume->SetInterpLog(Box->IsChecked());
	Context->CurrentSite->GetCurrentDataSet()->GenerateVolume(Context->WorldTime->GetTime(), Context->Scene.Volume->GetInterp());
}

void CGUIVolumeControlWidget::OnResetVolume(Gwen::Controls::Base * Control)
{
	VolumeControl.Mode = 0;
}

void CGUIVolumeControlWidget::OnResetAlpha(Gwen::Controls::Base * Control)
{
	IntensitySlider->SetFloatValue(1.f);
	VolumeControl.AlphaIntensity = 1.f;
}

void CGUIVolumeControlWidget::OnSetXAxis(Gwen::Controls::Base * Control)
{
	VolumeControl.SliceAxis = SVector3f(1.f, 0.f, 0.f);
}

void CGUIVolumeControlWidget::OnSetYAxis(Gwen::Controls::Base * Control)
{
	VolumeControl.SliceAxis = SVector3f(0.f, 1.f, 0.f);
}

void CGUIVolumeControlWidget::OnSetZAxis(Gwen::Controls::Base * Control)
{
	VolumeControl.SliceAxis = SVector3f(0.f, 0.f, 1.f);
}

void CGUIVolumeControlWidget::OnSetXRes(Gwen::Controls::Base * Control)
{
	CProgramContext * Context = &CProgramContext::Get();
	Gwen::Controls::HorizontalSlider * Slider = (Gwen::Controls::HorizontalSlider *) Control;

	Context->Scene.Volume->Resolution.X = Slider->GetFloatValue();
	Context->CurrentSite->GetCurrentDataSet()->GenerateVolume(Context->WorldTime->GetTime(), Context->Scene.Volume->GetInterp());
}

void CGUIVolumeControlWidget::OnSetYRes(Gwen::Controls::Base * Control)
{
	CProgramContext * Context = &CProgramContext::Get();
	Gwen::Controls::HorizontalSlider * Slider = (Gwen::Controls::HorizontalSlider *) Control;

	Context->Scene.Volume->Resolution.Y = Slider->GetFloatValue();
	Context->CurrentSite->GetCurrentDataSet()->GenerateVolume(Context->WorldTime->GetTime(), Context->Scene.Volume->GetInterp());
}

void CGUIVolumeControlWidget::OnSetZRes(Gwen::Controls::Base * Control)
{
	CProgramContext * Context = &CProgramContext::Get();
	Gwen::Controls::HorizontalSlider * Slider = (Gwen::Controls::HorizontalSlider *) Control;

	Context->Scene.Volume->Resolution.Z = Slider->GetFloatValue();
	Context->CurrentSite->GetCurrentDataSet()->GenerateVolume(Context->WorldTime->GetTime(), Context->Scene.Volume->GetInterp());
}

void CGUIVolumeControlWidget::OnDrawField(Gwen::Controls::Base * Control)
{
	CProgramContext * Context = &CProgramContext::Get();
	Gwen::Controls::ComboBox * Box = (Gwen::Controls::ComboBox *) Control;

	Context->CurrentSite->GetCurrentDataSet()->ColorField = Box->GetSelectedItem()->GetText().m_String;
	Context->CurrentSite->GetCurrentDataSet()->GenerateVolume(Context->WorldTime->GetTime(), Context->Scene.Volume->GetInterp());
}

void CGUIVolumeControlWidget::OnInterpMode(Gwen::Controls::Base * Control)
{
	CProgramContext * Context = &CProgramContext::Get();
	Gwen::Controls::ComboBox * Box = (Gwen::Controls::ComboBox *) Control;

	for (int i = 0; i < Interp::Mode::NumModes; i++) {
		std::string name = Interp::GetInterpName((Interp::Mode)i);
		if (Box->GetSelectedItem()->GetText() == Gwen::UnicodeString(name.begin(), name.end())) {
			ShowParameters((Interp::Mode)i);
			Context->Scene.Volume->SetInterpMode((Interp::Mode)i);
			Context->CurrentSite->GetCurrentDataSet()->GenerateVolume(Context->WorldTime->GetTime(), Context->Scene.Volume->GetInterp());
			return;
		}
	}
}

void CGUIVolumeControlWidget::OnFuncMode(Gwen::Controls::Base * Control)
{
	CProgramContext * Context = &CProgramContext::Get();
	Gwen::Controls::ComboBox * Box = (Gwen::Controls::ComboBox *) Control;

	for (int i = 0; i < Interp::RadialFunc::NumFuncs; i++) {
		std::string name = Interp::GetFuncName((Interp::RadialFunc)i);
		if (Box->GetSelectedItem()->GetText() == Gwen::UnicodeString(name.begin(), name.end())) {
			Context->Scene.Volume->SetInterpFunc((Interp::RadialFunc)i);
			Context->CurrentSite->GetCurrentDataSet()->GenerateVolume(Context->WorldTime->GetTime(), Context->Scene.Volume->GetInterp());
			return;
		}
	}
}

void CGUIVolumeControlWidget::OnVolumeMode(Gwen::Controls::Base * Control)
{
	Gwen::Controls::ComboBox * Box = (Gwen::Controls::ComboBox *) Control;

	if (Box->GetSelectedItem()->GetText() == Gwen::UnicodeString(L"Plane Slices"))
	{
		VolumeControl.Mode = 1;
		CProgramContext::Get().GUIContext->GetTitleLabels()->clearVolumeRangeIndicator();
	}
	else if (Box->GetSelectedItem()->GetText() == Gwen::UnicodeString(L"Isosurface"))
	{
		VolumeControl.Mode = 2;
		resetVolumeRange();
	}
	else if (Box->GetSelectedItem()->GetText() == Gwen::UnicodeString(L"Constant"))
	{
		VolumeControl.Mode = 0;
		CProgramContext::Get().GUIContext->GetTitleLabels()->clearVolumeRangeIndicator();
	}
	else if (Box->GetSelectedItem()->GetText() == Gwen::UnicodeString(L"Sample Alpha"))
	{
		VolumeControl.Mode = 3;
		CProgramContext::Get().GUIContext->GetTitleLabels()->clearVolumeRangeIndicator();
	}
}

void CGUIVolumeControlWidget::OnShadingMode(Gwen::Controls::Base * Control)
{
	Gwen::Controls::ComboBox * Box = (Gwen::Controls::ComboBox *) Control;

	if (Box->GetSelectedItem()->GetText() == Gwen::UnicodeString(L"No Shading"))
		VolumeControl.UseShading = 0;
	else if (Box->GetSelectedItem()->GetText() == Gwen::UnicodeString(L"Orbit Camera"))
		VolumeControl.UseShading = 1;
	else if (Box->GetSelectedItem()->GetText() == Gwen::UnicodeString(L"Fly Camera"))
		VolumeControl.UseShading = 2;
}

void CGUIVolumeControlWidget::OnDebugMode(Gwen::Controls::Base * Control)
{
	Gwen::Controls::ComboBox * Box = (Gwen::Controls::ComboBox *) Control;

	if (Box->GetSelectedItem()->GetText() == Gwen::UnicodeString(L"Disabled"))
		VolumeControl.DebugLevel = 0;
	else if (Box->GetSelectedItem()->GetText() == Gwen::UnicodeString(L"Front"))
		VolumeControl.DebugLevel = 1;
	else if (Box->GetSelectedItem()->GetText() == Gwen::UnicodeString(L"Cutoff"))
		VolumeControl.DebugLevel = 2;
	else if (Box->GetSelectedItem()->GetText() == Gwen::UnicodeString(L"Accumulator"))
		VolumeControl.DebugLevel = 3;
	else if (Box->GetSelectedItem()->GetText() == Gwen::UnicodeString(L"Iterations"))
		VolumeControl.DebugLevel = 4;
	else if (Box->GetSelectedItem()->GetText() == Gwen::UnicodeString(L"Box Depth"))
		VolumeControl.DebugLevel = 5;
	else if (Box->GetSelectedItem()->GetText() == Gwen::UnicodeString(L"Gradient"))
		VolumeControl.DebugLevel = 6;
}

void CGUIVolumeControlWidget::OnDepthMode(Gwen::Controls::Base * Control)
{
	Gwen::Controls::CheckBox * Box = (Gwen::Controls::CheckBox *) Control;

	CMainState::Get().ShowDepth = Box->IsChecked();
}

void CGUIVolumeControlWidget::OnToggleVolume(Gwen::Controls::Base * Control)
{
	CProgramContext * Context = & CProgramContext::Get();

	if (Context->Scene.Volume->GetNode()->IsVisible())
	{
		Context->Scene.Volume->GetNode()->SetVisible(false);
		GUIContext->GetConsole()->AddMessage("Volume View Disabled");
		EnableButton->SetText("Enable Volume View");
	}
	else
	{
		Context->Scene.Volume->GetNode()->SetVisible(true);
		GUIContext->GetConsole()->AddMessage("Volume View Enabled");
		EnableButton->SetText("Disable Volume View");
	}
}

void CGUIVolumeControlWidget::toggle()
{
	Window->SetHidden(Window->Visible());
}

void CGUIVolumeControlWidget::ShowParameters(Interp::Mode mode) {
	switch (mode) {
	case Interp::Mode::Radial:
		FuncMode->SetHidden(false);
		FuncLabel->SetHidden(false);

		InvExponent->SetHidden(true);
		InvLabel->SetHidden(true);
		LogCheckBox->SetHidden(true);
		LogLabel->SetHidden(true);
		break;

	case Interp::Mode::Connor:
		FuncMode->SetHidden(true);
		FuncLabel->SetHidden(true);

		InvExponent->SetHidden(false);
		InvLabel->SetHidden(false);
		LogCheckBox->SetHidden(false);
		LogLabel->SetHidden(false);
		break;

	case Interp::Mode::Density:
		FuncMode->SetHidden(true);
		FuncLabel->SetHidden(true);

		InvExponent->SetHidden(true);
		InvLabel->SetHidden(true);
		LogCheckBox->SetHidden(true);
		LogLabel->SetHidden(true);
		break;
	}
}