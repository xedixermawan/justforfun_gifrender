#include "justforfun_graphicsdriver.h"
#include <wrl\client.h>

using namespace Microsoft::WRL;

namespace jff   {

graphicsdriver::graphicsdriver(HWND& hwindow,const unsigned int sw, const unsigned int sh)   {
	m_hwindow      = &hwindow;
	m_screenwidth  = sw;
	m_screenheight = sh;
	init();
}

graphicsdriver::~graphicsdriver()   {
}

void graphicsdriver::init()   {
	D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_10_1 };
	// create swapchain & device
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	swapChainDesc.BufferDesc.Width  = (UINT) m_screenwidth;
	swapChainDesc.BufferDesc.Height = (UINT) m_screenheight;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.OutputWindow = (*m_hwindow);
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
	                 NULL,
	                 D3D_DRIVER_TYPE_WARP, // original : D3D_DRIVER_TYPE_HARDWARE, D3D_DRIVER_TYPE_WARP
	                 NULL,
	                 D3D11_CREATE_DEVICE_SINGLETHREADED,
	                 featureLevels,
	                 _countof(featureLevels),
	                 D3D11_SDK_VERSION,
	                 &swapChainDesc,
	                 &m_swapchain,
	                 &m_device,
	                 NULL,
	                 &m_devcontext);
	assert(SUCCEEDED(hr));

	// Get swap chain's back buffer, create its render target view and set that view as render target
	ID3D11Texture2D* backbuffer;
	hr = m_swapchain->GetBuffer(0, __uuidof(*backbuffer), (void**)&backbuffer);
	assert(SUCCEEDED(hr));

	hr = m_device->CreateRenderTargetView(backbuffer, NULL, &m_rendertarget);
	assert(SUCCEEDED(hr));


	// Create depth stencil texture
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory( &descDepth, sizeof(descDepth) );
	descDepth.Width = m_screenwidth;
	descDepth.Height = m_screenheight;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = m_device->CreateTexture2D( &descDepth, NULL, &g_pDepthStencil );

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory( &descDSV, sizeof(descDSV) );
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = m_device->CreateDepthStencilView( g_pDepthStencil, &descDSV, &g_pDepthStencilView );

	m_devcontext->OMSetRenderTargets(1, &m_rendertarget, g_pDepthStencilView);

	// Set viewport
	D3D11_VIEWPORT viewport;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width  = (FLOAT) m_screenwidth;
	viewport.Height = (FLOAT) m_screenheight;
	viewport.MinDepth = 0.f;
	viewport.MaxDepth = 1.f;
	m_devcontext->RSSetViewports(1, &viewport);

	//setup for 2d
	//----- raster state setting--------//
	D3D11_RASTERIZER_DESC rasterizerState;
	rasterizerState.CullMode = D3D11_CULL_NONE;
	rasterizerState.FillMode = D3D11_FILL_SOLID;
	rasterizerState.FrontCounterClockwise = FALSE;
	rasterizerState.DepthBias = false;
	rasterizerState.DepthBiasClamp = 0;
	rasterizerState.SlopeScaledDepthBias = 0;
	rasterizerState.DepthClipEnable = true;
	rasterizerState.ScissorEnable = false;
	rasterizerState.MultisampleEnable = false;
	rasterizerState.AntialiasedLineEnable = true;
	ComPtr<ID3D11RasterizerState> pRS;
	m_device->CreateRasterizerState( &rasterizerState, &pRS );
	m_devcontext->RSSetState(pRS.Get());

	//----- depth stencil setting--------//
	D3D11_DEPTH_STENCIL_DESC dsDesc;
	// Depth test parameters
	dsDesc.DepthEnable = false;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL; // default is D3D11_COMPARISON_LESS

	// Stencil test parameters
	dsDesc.StencilEnable = true;
	dsDesc.StencilReadMask = 0xFF;
	dsDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing
	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing
	dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create depth stencil state
	ComPtr<ID3D11DepthStencilState> pDSState;
	m_device->CreateDepthStencilState(&dsDesc, &pDSState);

	m_devcontext->OMSetDepthStencilState(pDSState.Get(), 1);

	// blend setting
	D3D11_BLEND_DESC bDesc;
	ZeroMemory(&bDesc, sizeof(D3D11_BLEND_DESC));
	// Create an alpha enabled blend state description.
	bDesc.RenderTarget[0].BlendEnable = TRUE;
	bDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	ComPtr<ID3D11BlendState> pBState;
	m_device->CreateBlendState(&bDesc, &pBState);

	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	UINT sampleMask   = 0xffffffff;
	m_devcontext->OMSetBlendState(pBState.Get(), blendFactor, sampleMask);
}

void graphicsdriver::clearscreen()   {
	const float bg_color[] = { 0.8f, 0.6f, 1.0f, 1.0f };// {1.0f, 0.0f, 1.0f, 1.0f}; // {0.8f, 0.6f, 1.0f, 1.0f};
	m_devcontext->ClearRenderTargetView( m_rendertarget, bg_color );
}

void graphicsdriver::present()   {
	m_swapchain->Present(1, 0);
}


}