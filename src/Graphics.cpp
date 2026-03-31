#include "Graphics.h"
#include <winerror.h>
#include <winnt.h>

int Graphics::DX11::Init(HWND hwnd) {
  this->CreateSwapChainDesc(hwnd);
  auto hr = this->CreateDeviceAndSwapChain();
  if(FAILED(hr)){
    wchar_t buf[64];
    swprintf_s(buf, L"0x%08X", (unsigned)hr);
    MessageBoxW(nullptr, buf, L"Device and Swapchain creation failed", MB_OK);
    return -1;
  }
  return 0;
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

HRESULT Graphics::DX11::CreateDeviceAndSwapChain() {
  return  D3D11CreateDeviceAndSwapChain(
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
