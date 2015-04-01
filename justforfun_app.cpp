#include "justforfun_app.h"

namespace jff   {

application::application()
	:m_appname(L"xedi:render animated gif"),
	 m_app_height(400),
	 m_app_width(400) {
}

application::application(wchar_t* appname, const unsigned int appw, const unsigned int apph)
	:m_appname(appname),
	 m_app_height(appw),
	 m_app_width(apph) {
}

application::~application()   {
}

void application::initialize(HWND& hwindow)   {
	m_gdriver     = new graphicsdriver( hwindow, m_app_width, m_app_height );
	m_spritebatch = new DirectX::SpriteBatch( m_gdriver->getdevicecontext() );
	m_spritefont  = new DirectX::SpriteFont( m_gdriver->getdevice(), L"armalite.spritefont");
	//m_texture     = new texture("castle.dds", m_gdriver->getdevice() );
	extern std::string g_filegif_loaded;
	m_gifsprite = new gifsprite(g_filegif_loaded.c_str(), m_gdriver->getdevice(), m_spritebatch);
}

void application::update(const double delta_time)   {
	m_gifsprite->update( delta_time);
}

void application::render()   {
	m_gdriver->clearscreen();
	m_spritebatch->Begin();
	extern double g_fps_measure;
	wchar_t wfps[32];
	swprintf(wfps, L"fps: %.1f", g_fps_measure);
	
/*
	RECT rect1;
	rect1.top    = 0;
	rect1.left   = 0;
	rect1.bottom = m_gifsprite->m_vectextures[0]->height;
	rect1.right = m_gifsprite->m_vectextures[0]->width;
//	m_spritebatch->Draw(m_texture->gettexview(), XMFLOAT2(0, 0), &rect1, Colors::White, 0.0f,
//	                     XMFLOAT2(0.0f, 0.0f), XMFLOAT2( 1.0f, 1.0f ) , DirectX::SpriteEffects_None, 0.0f);
	m_spritebatch->Draw(m_gifsprite->m_vectextures[0]->ptrtex->gettexview() , XMFLOAT2(0, 0), &rect1, Colors::White, 0.0f,
		XMFLOAT2(0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f), DirectX::SpriteEffects_None, 0.0f);
*/	
	m_gifsprite->render();
	m_spritefont->DrawString(m_spritebatch, wfps, XMFLOAT2(5, 370), Colors::Red);
	m_spritebatch->End();
	m_gdriver->present();
}

wchar_t* application::getappname()   {
	return m_appname;
}

unsigned int application::getwidthwindow()   {
	return m_app_width;
}

unsigned int application::getheightwindow()   {
	return m_app_height;
}

} // namespace jff
