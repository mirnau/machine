#include "Renderer.h"
#include "DX11.h"
#include <d3d11.h>
#include <dxgi.h>
#include <winerror.h>
#include <winnt.h>

Graphics::Renderer::Renderer(Graphics::DX11& dx) : m_dx(dx) {}

HRESULT Graphics::Renderer::Init() {
  return CreateRVT();
}

HRESULT Graphics::Renderer::CreateRVT() {
  ComPtr<ID3D11Texture2D> backBuffer;
  HRESULT hr = m_dx.GetSwapChain()->GetBuffer(
      0, __uuidof(ID3D11Texture2D),
      reinterpret_cast<void**>(backBuffer.GetAddressOf())
      );
  if (FAILED(hr)) return hr;

  hr = m_dx.GetDevice()->CreateRenderTargetView(
      backBuffer.Get(), nullptr, m_targetView.GetAddressOf()
      );
  return hr;
}

void Graphics::Renderer::Clear() {
  m_dx.GetContext()->OMSetRenderTargets(1, m_targetView.GetAddressOf(), nullptr);
  m_dx.GetContext()->ClearRenderTargetView(m_targetView.Get(), m_color);
} 

void Graphics::Renderer::Present() {
  m_dx.GetSwapChain()->Present(1, 0);
}

void Graphics::Renderer::Render() {
  Clear();
  Present();
}

HRESULT Graphics::Renderer::Resize(UINT w, UINT h) {
  m_targetView.Reset();
  HRESULT hr = m_dx.GetSwapChain()->ResizeBuffers(0, w, h, DXGI_FORMAT_UNKNOWN, 0);
  if(FAILED(hr))
    return hr;

  return CreateRVT();
} 
