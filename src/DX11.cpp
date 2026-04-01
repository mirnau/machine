#include "DX11.h"
#include "Failure.h"
#include <winerror.h>
#include <winnt.h>

void Graphics::DX11::Init(HWND hwnd) {
  this->CreateSwapChainDesc(hwnd);
  this->CreateDeviceAndSwapChain();
}

void Graphics::DX11::CreateSwapChainDesc(HWND hwnd) {
  m_swapchainDesc = DXGI_SWAP_CHAIN_DESC{
    .BufferDesc = {
      .Format = DXGI_FORMAT_B8G8R8A8_UNORM,
    },
      .SampleDesc = {
        .Count = 1,
        .Quality = 0,
      },
      .BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
      .BufferCount = 2,
      .OutputWindow = hwnd,
      .Windowed = TRUE,
      .SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL,
  };
}

void Graphics::DX11::CreateDeviceAndSwapChain() {
  HRESULT hr = D3D11CreateDeviceAndSwapChain(
      nullptr,
      D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE,
      nullptr,
      GetDeviceFlags(),
      m_levels.data(),
      static_cast<UINT>(m_levels.size()),
      D3D11_SDK_VERSION,
      &m_swapchainDesc,
      m_swapChain.GetAddressOf(),
      m_device.GetAddressOf(),
      &m_featureLevel,
      m_context.GetAddressOf()
      );

  if(FAILED(hr))
    throw Machine::Failure::Graphics(hr);

}

UINT Graphics::DX11::GetDeviceFlags() {

  UINT deviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(DEBUG)
  deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

  return deviceFlags;
}

Graphics::DX11::~DX11() {
  if(m_context) 
    m_context->ClearState();
  m_context.Reset();
  m_swapChain.Reset();
  m_device.Reset();
}
