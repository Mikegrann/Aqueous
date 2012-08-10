#include "CGUIManager.h"

#include <Gwen/Renderers/SFML.h>
#include <Gwen/Skins/TexturedBase.h>
#include <Gwen/Controls.h>


Gwen::Font * LoadFont(Gwen::UnicodeString const & File, float const Size)
{
	Gwen::Font * Font = new Gwen::Font();
	Font->facename = File;
	Font->size = Size;

	return Font;
}


CGUIManager::CGUIManager()
	: Application(CApplication::get())
{
}

void CGUIManager::init()
{
	Gwen::Renderer::SFML * pRenderer = new Gwen::Renderer::SFML(* Application.App);

	Gwen::Skin::TexturedBase * skin = new Gwen::Skin::TexturedBase();
	skin->SetRender(pRenderer);
	skin->Init("DefaultSkin.png");
	skin->SetDefaultFont(L"OpenSans.ttf", 14.f);
	LargeFont = LoadFont(L"OpenSans.ttf", 48.f);
	MediumFont = LoadFont(L"OpenSans.ttf", 32.f);
	RegularFont = LoadFont(L"OpenSans.ttf", 14.f);

	Canvas = new Gwen::Controls::Canvas(skin);
	Canvas->SetSize(Application.getWindowSize().X, Application.getWindowSize().Y);
}

void CGUIManager::draw(f32 const Elapsed, bool const ClearAll)
{
	for (auto it = Widgets.begin(); it != Widgets.end(); ++ it)
	{
		(* it)->update(Elapsed);
	}

	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		if (ClearAll)
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		else
			glClear(GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION);

		glLoadIdentity();
			int left = 0, top = 0;
			int right = 1600, bottom = 900;
			glOrtho( left, right, bottom, top, -1.0, 1.0);

		glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glViewport(0, 0, right - left, bottom - top);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	Canvas->RenderCanvas();
}

Gwen::Controls::Canvas * CGUIManager::getCanvas()
{
	return Canvas;
}
	
Gwen::Font * CGUIManager::getLargeFont()
{
	return LargeFont;
}

Gwen::Font * CGUIManager::getMediumFont()
{
	return MediumFont;
}

Gwen::Font * CGUIManager::getRegularFont()
{
	return RegularFont;
}

void CGUIManager::addWidget(CGUIWidget * Widget)
{
	Widgets.push_back(Widget);
}

void CGUIManager::removeWidget(CGUIWidget * Widget)
{
	Widgets.erase(std::remove(Widgets.begin(), Widgets.end(), Widget), Widgets.end());
}

void CGUIManager::removeAllWidgets()
{
	Widgets.clear();
}
