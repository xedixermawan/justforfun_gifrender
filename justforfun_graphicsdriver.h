/*
*  (C) 2013 XediXermawan <edi.ermawan@gmail.com>
*   justforfun
*/
#ifndef _JFF_GRAPHICS_DRIVER_H_
#define _JFF_GRAPHICS_DRIVER_H_

#include <Windows.h>
#include <d3d11.h>
#include <assert.h>

namespace jff   {
	class graphicsdriver   {
	public:
		graphicsdriver(HWND& hwindow,const unsigned int sw, const unsigned int sh);
		~graphicsdriver();
		void clearscreen();
		void present();
		ID3D11DeviceContext* getdevicecontext() const {
			return m_devcontext;
		};
		ID3D11Device* getdevice() const {
			return m_device;
		};
	private:
		void init();
		unsigned int m_screenwidth;
		unsigned int m_screenheight;
		HWND* m_hwindow;
		ID3D11Device* m_device;                  
		ID3D11DeviceContext* m_devcontext;       
		IDXGISwapChain* m_swapchain;
		ID3D11RenderTargetView* m_rendertarget;

		ID3D11Texture2D*            g_pDepthStencil;
ID3D11DepthStencilView*     g_pDepthStencilView;
	};
}; //namespace jff

#endif	// _JFF_GRAPHICS_DRIVER_H_