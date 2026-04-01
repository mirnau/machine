#include "Renderer.h"
#include "DX11.h"
#include <d3d11.h>
#include <d3dcommon.h>
#include <dxgi.h>
#include <winerror.h>
#include <winnt.h>
#include "MiniMath.h"
#include "Vertex.h"

Graphics::Renderer::Renderer(Graphics::DX11& dx) : m_dx(dx) {}

HRESULT Graphics::Renderer::Init(uint2 size) {
  HRESULT hr = CreateRVT();
  if(FAILED(hr))return hr;
  hr = CompileShaders();
  if(FAILED(hr)) return hr;
  hr = CreateInputLayout();
  if(FAILED(hr)) return hr;
  hr = CreateVertexBuffer();
  if(FAILED(hr)) return hr;

  SetViewport(size);

  return hr;
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
  m_dx.GetContext()->IASetInputLayout(m_inputLayout.Get());
  m_dx.GetContext()->VSSetShader(m_vertexShader.Get(), nullptr, 0);
  m_dx.GetContext()->PSSetShader(m_pixelShader.Get(), nullptr, 0);
  UINT stride = sizeof(Vertex);
  UINT offset = 0;
  m_dx.GetContext()->IASetVertexBuffers(0,1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
  m_dx.GetContext()->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
  m_dx.GetContext()->Draw(3,0);
  Present();
}

HRESULT Graphics::Renderer::Resize(uint2 size) {
  m_targetView.Reset();
  HRESULT hr = m_dx.GetSwapChain()->ResizeBuffers(0, size.w, size.h, DXGI_FORMAT_UNKNOWN, 0);
  if(FAILED(hr))
    return hr;
  SetViewport(size);
  return CreateRVT();
}

void Graphics::Renderer::SetViewport(uint2 size) {
  m_viewport = D3D11_VIEWPORT{
    .TopLeftX = 0.0f,
    .TopLeftY = 0.0f,
    .Width    = static_cast<float>(size.w),
    .Height   = static_cast<float>(size.h),
    .MinDepth = 0.0f,
    .MaxDepth = 1.0f,
  };
  m_dx.GetContext()->RSSetViewports(1, &m_viewport);
}

HRESULT Graphics::Renderer::CompileShaders() {

  ComPtr<ID3DBlob> errorBlob;

  HRESULT hr = D3DCompileFromFile(vxPath,
                                  nullptr, 
                                  nullptr,
                                  "main", 
                                  "vs_5_0",
                                  0,0,
                                  &m_vsBlob, 
                                  &errorBlob
                                  );
    if(FAILED(hr)) {
      if(errorBlob)
          MessageBoxA(nullptr, (char*)errorBlob->GetBufferPointer(), "Shader Error", MB_OK);
      else
          MessageBoxA(nullptr, "errorBlob is null - file not found?", "Shader Error", MB_OK);
      return hr;
  }
  if(FAILED(hr)) return hr;

  hr = m_dx.GetDevice()->CreateVertexShader(
    m_vsBlob->GetBufferPointer(),
    m_vsBlob->GetBufferSize(),
    nullptr, &m_vertexShader
  );

  if(FAILED(hr)) return hr;

  ComPtr<ID3DBlob> psBlob;
  hr = D3DCompileFromFile(pxPath,
                          nullptr, 
                          nullptr,
                          "main", 
                          "ps_5_0",
                          0,0,
                          &psBlob, 
                          &errorBlob
                          );
  if(FAILED(hr)) return hr;


  return m_dx.GetDevice()->CreatePixelShader(
    psBlob->GetBufferPointer(),
    psBlob->GetBufferSize(),
    nullptr,
    &m_pixelShader
  );
}

HRESULT Graphics::Renderer::CreateInputLayout() {
  D3D11_INPUT_ELEMENT_DESC desc[] = {
    {"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0,
      D3D11_INPUT_PER_VERTEX_DATA, 0 
    }
  };

  return m_dx.GetDevice()->CreateInputLayout(
    desc, 1,
    m_vsBlob->GetBufferPointer(),
    m_vsBlob->GetBufferSize(),
    &m_inputLayout
  );
}

HRESULT Graphics::Renderer::CreateVertexBuffer() {
  D3D11_BUFFER_DESC desc{};
  desc.ByteWidth = sizeof(triangleVertices);
  desc.Usage = D3D11_USAGE_IMMUTABLE;
  desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

  D3D11_SUBRESOURCE_DATA data {};
  data.pSysMem = triangleVertices;

  return m_dx.GetDevice()->CreateBuffer(&desc, &data, m_vertexBuffer.ReleaseAndGetAddressOf());
}
