/*
*  (C) 2013 XediXermawan <edi.ermawan@gmail.com>
*   justforfun
*/
#ifndef _JFF_APP_H_
#define _JFF_APP_H_

#include "justforfun_graphicsdriver.h"
#include "justforfun_texture.h"

#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "justforfun_gifsprite.h"

using namespace DirectX;

namespace jff   {
	class application   {
	public:
		application();
		application(wchar_t* appname, const unsigned int appw, const unsigned int apph);
		~application();
		void initialize(HWND& hwindow);
	    void update(const double delta_time);
		void render();
		wchar_t* getappname();
		unsigned int getwidthwindow();
		unsigned int getheightwindow();
	private:
		wchar_t* m_appname;
		unsigned int m_app_width;
		unsigned int m_app_height;
		graphicsdriver* m_gdriver;

		//paint
		DirectX::SpriteBatch* m_spritebatch;
		DirectX::SpriteFont* m_spritefont;
		texture* m_texture;
		gifsprite* m_gifsprite;
	};
};

#endif	// _JFF_APP_H_ 