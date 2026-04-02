#include "Renderer.h"
#include "DX11.h"
#include <DirectXMath.h>
#include <d3d11.h>
#include <d3dcommon.h>
#include <dxgi.h>
#include <dxgiformat.h>
#include <winerror.h>
#include <winnt.h>
#include "Failure.h"
#include "MiniMath.h"
#include "Vertex.h"

Graphics::Renderer::Renderer(Graphics::DX11& dx) : m_dx(dx) {}

void Graphics::Renderer::Init(uint2 size) {
  CreateRVT();
  CreateDepthBuffer(size);
  CompileShaders();
  CreateInputLayout();
  CreateVertexBuffer();
  CreateConstantBuffer();
  SetViewport(size);
}

void Graphics::Renderer::CreateRVT() {
  ComPtr<ID3D11Texture2D> backBuffer;
  HRESULT hr = m_dx.GetSwapChain()->GetBuffer(
    0, __uuidof(ID3D11Texture2D),
    reinterpret_cast<void**>(backBuffer.GetAddressOf())
  );
  if (FAILED(hr))
    throw Machine::Failure::Graphics(hr);

  hr = m_dx.GetDevice()->CreateRenderTargetView(
    backBuffer.Get(), nullptr, m_targetView.GetAddressOf()
  );

  if(FAILED(hr))
    throw Machine::Failure::Graphics(hr);
}

void Graphics::Renderer::Clear() {
  m_dx.GetContext()->OMSetRenderTargets(1, m_targetView.GetAddressOf(), nullptr);
  m_dx.GetContext()->ClearRenderTargetView(m_targetView.Get(), m_color);
  m_dx.GetContext()->ClearDepthStencilView(m_depthView.Get(),D3D11_CLEAR_DEPTH, 1.0f, 0);
  m_dx.GetContext()->OMSetDepthStencilState(m_depthState.Get(),0);
} 

void Graphics::Renderer::Present() {
  m_dx.GetSwapChain()->Present(1, 0);
}

void Graphics::Renderer::Render() {

  m_angle += 0.01f;
  TransformCB cb;
  cb.world = DirectX::XMMatrixRotationZ(m_angle);
  D3D11_MAPPED_SUBRESOURCE mapped{};
  m_dx.GetContext()->Map(m_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
  memcpy(mapped.pData, &cb, sizeof(cb));
  m_dx.GetContext()->Unmap(m_constantBuffer.Get(), 0);
  m_dx.GetContext()->VSSetConstantBuffers(0, 1, m_constantBuffer.GetAddressOf());

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

void Graphics::Renderer::Resize(uint2 size) {
  m_targetView.Reset();
  m_depthView.Reset();
  HRESULT hr = m_dx.GetSwapChain()->ResizeBuffers(0, size.w, size.h, DXGI_FORMAT_UNKNOWN, 0);
  if(FAILED(hr))
    throw Machine::Failure::Graphics(hr);
  SetViewport(size);
  CreateRVT();
  CreateDepthBuffer(size);
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

void Graphics::Renderer::CompileShaders() {

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
   throw Machine::Failure::Shader(hr, errorBlob.Get()); 
  }

  hr = m_dx.GetDevice()->CreateVertexShader(
    m_vsBlob->GetBufferPointer(),
    m_vsBlob->GetBufferSize(),
    nullptr, &m_vertexShader
  );

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

  if(FAILED(hr)) {
      throw Machine::Failure::Shader(hr, errorBlob.Get());
  }

  hr = m_dx.GetDevice()->CreatePixelShader(
    psBlob->GetBufferPointer(),
    psBlob->GetBufferSize(),
    nullptr,
    &m_pixelShader
  );

  if(FAILED(hr))
    throw Machine::Failure::Graphics(hr);
}

void Graphics::Renderer::CreateInputLayout() {
  D3D11_INPUT_ELEMENT_DESC desc[] = {
    {"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0,
      D3D11_INPUT_PER_VERTEX_DATA, 0 },
    {"COLOR", 0,DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
      D3D11_INPUT_PER_VERTEX_DATA, 0}
  };

  HRESULT hr = m_dx.GetDevice()->CreateInputLayout(
    desc, 2,
    m_vsBlob->GetBufferPointer(),
    m_vsBlob->GetBufferSize(),
    &m_inputLayout
  );
  if(FAILED(hr))
    throw Machine::Failure::Graphics(hr);
}

void Graphics::Renderer::CreateVertexBuffer() {

  D3D11_BUFFER_DESC desc{};
  desc.ByteWidth = sizeof(triangleVertices);
  desc.Usage = D3D11_USAGE_IMMUTABLE;
  desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

  D3D11_SUBRESOURCE_DATA data {};
  data.pSysMem = triangleVertices;

  HRESULT hr = m_dx.GetDevice()->CreateBuffer(&desc, &data, m_vertexBuffer.ReleaseAndGetAddressOf());
  if(FAILED(hr))
    throw Machine::Failure::Graphics(hr);
}

void Graphics::Renderer::CreateConstantBuffer() {
  D3D11_BUFFER_DESC desc {};
  desc.ByteWidth = sizeof(TransformCB);
  desc.Usage = D3D11_USAGE_DYNAMIC;
  desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

  HRESULT hr = m_dx.GetDevice()->CreateBuffer(&desc, nullptr, m_constantBuffer.GetAddressOf());

  if(FAILED(hr))
    throw Machine::Failure::Graphics(hr);

}

void Graphics::Renderer::CreateDepthBuffer(uint2 size) {
  D3D11_TEXTURE2D_DESC texDesc{};
  texDesc.Width = size.w;
  texDesc.Height = size.h;
  texDesc.MipLevels = 1;
  texDesc.ArraySize = 1;
  texDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
  texDesc.SampleDesc.Count = 1;
  texDesc.Usage = D3D11_USAGE_DEFAULT;
  texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

  ComPtr<ID3D11Texture2D> depthTexture;
  HRESULT hr = m_dx.GetDevice()->CreateTexture2D(&texDesc,
                                                 nullptr,
                                                 depthTexture.GetAddressOf()
                                                 );
  if(FAILED(hr)) 
    throw Machine::Failure::Graphics(hr);

  hr = m_dx.GetDevice()->CreateDepthStencilView(depthTexture.Get(),
                                                nullptr,
                                                m_depthView.GetAddressOf()
                                                );
  if(FAILED(hr))
    throw Machine::Failure::Graphics(hr);

  D3D11_DEPTH_STENCIL_DESC dsDesc{};
  dsDesc.DepthEnable = TRUE;
  dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
  dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

  hr = m_dx.GetDevice()->CreateDepthStencilState(&dsDesc, m_depthState.GetAddressOf());
  
  if(FAILED(hr))
    throw Machine::Failure::Graphics(hr);
}

