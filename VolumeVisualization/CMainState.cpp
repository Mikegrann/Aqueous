
#include "CMainState.h"

#include "CTerrainNodeManager.h"
#include "CGlyphSceneObject.h"
#include "CWaterSceneObject.h"
#include "CSite.h"
#include "CSpectrumColorMapper.h"
#include "CGUIContext.h"

#include <ionScience.h>


CMainState::CMainState()
	: Scale(1), Mode(0), ShowDepth(false), ProjectionMode(0), GUIEnabled(true), OrbitCameraTimer(0), gifWriter()
{}

void CMainState::Begin()
{
	Context->GUIContext->SetupMainState();

	Context->Scene.Timer = 0.f;
	//Context->Scene.Glyphs->BuildLines();

	//ReflectionRenderPass = sharedNew<CReflectionRenderPass>(new CReflectionRenderPass);
	//SceneManager->getEffectManager()->RenderPasses.push_back(ReflectionRenderPass);

	CalculateDataAlignment();
	OrbitCameraTimer = 0;

	Font = IFont::init("OpenSans.ttf", 18);
}

void CMainState::End()
{
	Context->GUIContext->Clear();
}

void CMainState::BeginGifDraw()
{
	SceneManager->GetScene()->SetActiveCamera(Context->Scene.OrbitCamera);
	gifWriter = new GifWriter(Context->Window->GetSize());
	OrbitCameraTimer = 0;
}

void CMainState::EndGifDraw()
{
	SceneManager->GetScene()->SetActiveCamera(Context->Scene.Camera);
	if (! gifWriter->Save("output.gif"))
	{
		std::cerr << "GIF writing failed" << std::endl;
		WaitForUser();
	}

	delete gifWriter;
	gifWriter = 0;
}

f64 GlobalMin, GlobalMax;

void CMainState::Update(f32 const Elapsed)
{
	CProgramContext::SScene & Scene = Context->Scene;

	Scene.CameraController->Update(Elapsed);

	f32 Radius = Scene.Camera->GetPosition().Length();
	if (Radius < 10.f)
	{
		Scene.Camera->SetNearPlane(0.001f);
		Scene.Camera->SetFarPlane(100.f);
	}
	else if (Radius < 50.f)
	{
		Scene.Camera->SetNearPlane(0.01f);
		Scene.Camera->SetFarPlane(150.f);
	}
	else if (Radius < 500.f)
	{
		Scene.Camera->SetNearPlane(10.f);
		Scene.Camera->SetFarPlane(1000.f);
	}
	else if (Radius < 5000.f)
	{
		Scene.Camera->SetNearPlane(100.f);
		Scene.Camera->SetFarPlane(10000.f);
	}
	else if (Radius < 15000.f)
	{
		Scene.Camera->SetNearPlane(4000.f);
		Scene.Camera->SetFarPlane(20000.f);
	}

	Scene.Timer += Elapsed * 0.16f;

	float const Distance = 2.5f;
	static f32 const Speed = 1.f;
	static f32 const Increment = 0.05f;
	Scene.OrbitCamera->SetPosition(SVector3f(sin(Speed*OrbitCameraTimer)*Distance, 0.4f, cos(Speed*OrbitCameraTimer)*Distance));
	Scene.OrbitCamera->SetLookAtTarget(vec3f(0, -0.5f, 0));
	OrbitCameraTimer += Increment;

	Scene.LightPosition = SceneManager->GetScene()->GetActiveCamera()->GetPosition() + SVector3f(0, 0, 0);

	//SceneManager->GetDefaultColorRenderPass()->onPreDraw();
	//glClearColor(1.f, 0.25f, 0.05f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glClearColor(0.15f, 0.45f, 0.5f, 1.0f);

	glDisable(GL_DEPTH_TEST);
	Context->Scene.SkyBox->SetPosition(SceneManager->GetScene()->GetActiveCamera()->GetPosition());
	Context->Scene.SkyBox->Update();
	Context->Scene.SkyBox->UpdateAbsoluteTransformation();

	//Context->Scene.SkyBox->load(SceneManager, SceneManager->getDefaultColorRenderPass());
	//Context->Scene.SkyBox->draw(SceneManager, SceneManager->getDefaultColorRenderPass(), false);
	glEnable(GL_DEPTH_TEST);

	Scene.Volume->Update();

	SceneManager->DrawAll();
	if (Context->Window->IsKeyDown(EKey::F1))
		CFrameBuffer::DrawTextureToScreen(Context->SceneColorTexture);
	else if (Context->Window->IsKeyDown(EKey::F2))
		CFrameBuffer::DrawTextureToScreen(Context->SceneRefractColor);
	else if (Context->Window->IsKeyDown(EKey::F3))
		CFrameBuffer::DrawTextureToScreen(Context->SceneDepthBuffer);
	else if (Context->Window->IsKeyDown(EKey::F4))
		CFrameBuffer::DrawTextureToScreen(Context->RefractMaskTexture);
	else
	{
		ion::GL::Context::Clear();

		CDrawConfig DrawConfig(SceneManager->GetShaderLibrary()->Get("Merge"), ion::GL::EPrimativeType::Quads);
		DrawConfig.AddVertexBuffer("aPosition", CFrameBuffer::GetQuadVertexBuffer());
		DrawConfig.SetIndexBuffer(CFrameBuffer::GetQuadIndexBuffer());
		DrawConfig.AddTexture("uSceneColor", Context->SceneColorTexture);
		DrawConfig.AddTexture("uRefractColor", Context->SceneRefractColor);

		ion::GL::DrawContext DrawContext;
		DrawContext.LoadProgram(SceneManager->GetShaderLibrary()->Get("Merge"));
		DrawContext.Draw(& DrawConfig);
	}

	if (! ShowDepth)
	{
		//Context->Scene.Volume->draw(SceneManager, SceneManager->getDefaultColorRenderPass(), false);

		//glEnable(GL_BLEND);
		//glDepthMask(false);
		//Context->Scene.Water->draw(SceneManager, SceneManager->getDefaultColorRenderPass(), false);
		//glDepthMask(true);

		//SceneManager->endDraw();
	}

	//if (ShowDepth)
	//{
	//	CFrameBufferObject::bindDeviceBackBuffer();
	//	glDisable(GL_DEPTH_TEST);
	//	{
	//		CShaderContext Context(* CShaderLoader::loadShader("FBO/QuadCopy"));

	//		Context.bindTexture("uTexColor", SceneManager->getSceneDepthTexture());
	//		Context.bindBufferObject("aPosition", CSceneManager::getQuadHandle(), 2);

	//		glDrawArrays(GL_QUADS, 0, 4);
	//	}
	//	glEnable(GL_DEPTH_TEST);
	//}

	if (GUIEnabled)
		Context->GUIContext->Manager->Draw(Elapsed, false);
	
	//auto GetValueAt = [](f32 const v)
	//{
	//	color4f Color = CSpectrumColorMapper::MapColor(v);
	//	glColor3f(Color.Red, Color.Green, Color.Blue);
	//	return GlobalMin * (1 - v) + GlobalMax * v;
	//};
	//int Counter = 10;
	//auto DrawColor = [this, & Counter, GetValueAt](c8 const * const Label, f32 const v)
	//{
	//	f32 Value = GetValueAt(v);
	//	freetype::print(Font, 10, Counter, "%s:", Label);
	//	freetype::print(Font, 120, Counter, "%.3f �C", Value);
	//	Counter += 25;
	//};
	//DrawColor("Black", 0.f);
	//DrawColor("Blue", 0.15f);
	//DrawColor("Green", 0.4f);
	//DrawColor("Yellow", 0.6f);
	//DrawColor("Orange", 0.7f);
	//DrawColor("Red", 0.8f);
	//DrawColor("White", 1.f);
	
    // Read screen colors
	if (gifWriter)
	{
		u32 const FrameWidth = Context->Window->GetSize().X;
		u32 const FrameHeight = Context->Window->GetSize().Y;
		unsigned char * ImageData = new unsigned char[FrameWidth * FrameHeight * 3];

		static u32 Counter = 0;
		glReadPixels(0, 0, FrameWidth, FrameHeight, GL_RGB, GL_UNSIGNED_BYTE, ImageData);
		gifWriter->AddFrame(ImageData, 0.12f);

		delete [] ImageData;

		if (Speed*OrbitCameraTimer >= 2*Constants32::Pi)
			EndGifDraw();
	}
}

void CMainState::AddConsoleMessage(std::string const & Message, color4i const & Color)
{
	Context->GUIContext->GetConsole()->AddMessage(Message, Color);
}

void CMainState::CalculateDataAlignment()
{
	CProgramContext::SScene & Scene = Context->Scene;

	CSite * CurrentSite = Context->CurrentSite;
	STable & DataSet = CurrentSite->GetCurrentDataSet()->Points;

	SRange<f64> XRange = DataSet.GetFieldRange(CurrentSite->GetCurrentDataSet()->Traits.PositionXField, 15.0);
	SRange<f64> YRange = DataSet.GetFieldRange(CurrentSite->GetCurrentDataSet()->Traits.PositionYField, 15.0);
	SRange<f64> ZRange = DataSet.GetFieldRange(CurrentSite->GetCurrentDataSet()->Traits.PositionZField, 15.0);
	
	if (XRange.IsEmpty())
		XRange = SRange<f64>(-1, 1);
	if (YRange.IsEmpty())
		YRange = SRange<f64>(-1, 1);
	if (ZRange.IsEmpty())
		ZRange = SRange<f64>(-1, 1);

	printf("Longlat range is %g %g to %g %g\n", XRange.Minimum, ZRange.Minimum, XRange.Maximum, ZRange.Maximum);

	longlatd const DataLonLatMin(XRange.Minimum, ZRange.Minimum), DataLonLatMax(XRange.Maximum, ZRange.Maximum);
	longlatd const MapLonLatMin(CurrentSite->GetCurrentLocation()->LowerBound), MapLonLatMax(CurrentSite->GetCurrentLocation()->UpperBound);

	longlatd const DataLonLatCenter = longlatd(SLongitudeLatitude<f64>::DMStoDecimal(9, 32, 23.44 - 3.1), SLongitudeLatitude<f64>::DMStoDecimal(63, 35, 35.37 - 2.4));// (DataLonLatMin + DataLonLatMax) / 2.f;
	
	vec2d DataRangeMin, DataRangeMax, MapRangeMin, MapRangeMax;
	sharedPtr<longlatd::IProjectionSystem> Projection;
	if (ProjectionMode == 0)
		Projection = sharedNew(new longlatd::CHaversineProjection());
	else if (ProjectionMode == 1)
		Projection = sharedNew(new longlatd::CVincentyProjection());
	else if (ProjectionMode == 2)
		Projection = sharedNew(new longlatd::CEquirectangularProjection(DataLonLatCenter.Latitude));
	DataRangeMin = DataLonLatCenter.OffsetTo(DataLonLatMin, Projection);
	DataRangeMax = DataLonLatCenter.OffsetTo(DataLonLatMax, Projection);
	MapRangeMin = DataLonLatCenter.OffsetTo(MapLonLatMin, Projection);
	MapRangeMax = DataLonLatCenter.OffsetTo(MapLonLatMax, Projection);

	vec2d const DataRangeSize = DataLonLatMax - DataLonLatMin;// DataRangeMax - DataRangeMin;
	vec2d const DataRangeCenter = DataRangeSize / 2;// (DataLonLatMin + DataLonLatMax) / 2.f;// (DataRangeMin + DataRangeMax) / 2.f;
	f64 const DataDepth = YRange.Size();
	
	vec2d const MapRangeSize = MapRangeMax - MapRangeMin;
	vec2d const MapRangeCenter = (MapRangeMin + MapRangeMax) / 2.f;
	f64 const MapDepth = 180.0;
	
	printf("Data range is %f by %f meters,\n", DataRangeSize.X, DataRangeSize.Y);
	printf("Terrain range is %f by %f meters,\n", MapRangeSize.X, MapRangeSize.Y);
	
	vec2d const ActualOffset = MapRangeCenter - DataRangeCenter;
	vec2d const MapOffset = ActualOffset * 3.f / Maximum(DataRangeSize.X, DataRangeSize.Y);
	vec3d const DataScale = 3.0 * vec3d(DataRangeSize.X, DataDepth, DataRangeSize.Y) / Maximum(DataRangeSize.X, DataRangeSize.Y);
	vec3d const MapScale = DataScale * vec3d(MapRangeSize.X, MapDepth, MapRangeSize.Y) / vec3d(DataRangeSize.X, DataDepth, DataRangeSize.Y);

	static f64 const YExaggeration = 8.0;
	static vec3d const Multiplier = vec3d(1, YExaggeration, 1);
	
	//Scene.Glyphs->SetScale(DataScale * Multiplier);
	Scene.Volume->GetNode()->SetScale(DataScale * Multiplier);
	//Scene.Glyphs->SetTranslation(vec3f(0, -DataScale.Y * YExaggeration / 2, 0));
	Scene.Volume->GetNode()->SetTranslation(vec3f(0, -DataScale.Y * YExaggeration / 2, 0));
	//
	Scene.Terrain->GetNode()->SetScale(MapScale * Multiplier / CTerrainNodeManager::Size);
	//Scene.Water->SetScale(MapScale / CTerrainNodeManager::Size);
	////Scene.SkyBox->SetScale(SVector3f(MapScale.X, 30.f, MapScale.Z));

	Scene.Terrain->GetNode()->SetTranslation(vec3f(MapOffset.X, 0, -MapOffset.Y));
	//Scene.Water->SetTranslation(vec3f(MapOffset.X, 0, -MapOffset.Y));
	////Scene.SkyBox->setTranslation(vec3f(MapOffset.X, 0, -MapOffset.Y));
	//
	//// Flip for RHC->LHC
	//Scene.Glyphs->SetScale(Scene.Glyphs->GetScale() * vec3f(1, 1, -1));
	Scene.Volume->GetNode()->SetScale(Scene.Volume->GetNode()->GetScale() * vec3f(1, 1, -1));
	Scene.Terrain->GetNode()->SetScale(Scene.Terrain->GetNode()->GetScale() * vec3f(1, 1, -1));
	//Scene.Water->SetScale(Scene.Water->GetScale() * vec3f(1, 1, -1));
	////Scene.SkyBox->SetScale(Scene.SkyBox->GetScale() * vec3f(1, 1, -1));
	//
	//// Flip Height -> Depth
	Scene.Volume->GetNode()->SetScale(Scene.Volume->GetNode()->GetScale() * vec3f(1, -1, 1));
	//Scene.Glyphs->SetScale(Scene.Glyphs->GetScale() * vec3f(1, -1, 1));
}

void CMainState::SetSite(int site)
{
	CProgramContext::SScene & Scene = Context->Scene;
	//Scene.Terrain->SetSite(Site);
	CalculateDataAlignment();
}
