#pragma once 
#include <d3d11.h>
#include <winnt.h>
#include <wrl/client.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <functional>
#include "DX11.h"
#include "MiniMath.h"

using Microsoft::WRL::ComPtr;

namespace Graphics {
class Renderer {
public:
        Renderer(DX11& dx);
        Renderer(const Renderer& r) = delete;
        Renderer(Renderer&& r) = delete;
        ~Renderer() = default;
        Renderer& operator=(const Renderer& r) = delete;

        void Init(uint2 size);
        void Clear();
        void Present();
        void Render();
        void SetViewport(uint2 size);
        void Resize(uint2 size);
        std::function<void(uint2)> GetResizeCallback() { return [this](uint2 size) { Resize(size);};};
        DX11& GetDX11() {return m_dx; };

private: 
        void CreateRVT();
        void CompileShaders();
        void CreateInputLayout();
        void CreateVertexBuffer();
        void CreateConstantBuffer();
        void CreateDepthBuffer(uint2 size);
        void CreateIndexBuffer();
        void CreateRasterizerState();
        struct TransformCB { DirectX::XMMATRIX world; };

private:
        //INFO: in the case of other driver implementations, 
        //an interface will replace this reference.
        //for a Dx enginge, this is appropriate.
        DX11& m_dx;
        ComPtr<ID3D11RenderTargetView> m_targetView;
        ComPtr<ID3D11VertexShader> m_vertexShader;
        ComPtr<ID3D11PixelShader> m_pixelShader;
        ComPtr<ID3DBlob> m_vsBlob;
        ComPtr<ID3D11InputLayout> m_inputLayout;
        ComPtr<ID3D11Buffer> m_vertexBuffer;
        ComPtr<ID3D11Buffer> m_indexBuffer;
        ComPtr<ID3D11Buffer> m_constantBuffer;
        ComPtr<ID3D11DepthStencilView> m_depthView;
        ComPtr<ID3D11DepthStencilState> m_depthState;
        ComPtr<ID3D11RasterizerState> m_rasterizerState;
        float m_angle{0.0f};
        D3D11_VIEWPORT m_viewport{};

        float m_color[4] = { 0.392f, 0.584f, 0.929f, 1.0f };
        const wchar_t* vxPath = L"shaders/VertexShader.hlsl";
        const wchar_t* pxPath = L"shaders/PixelShader.hlsl";
};
}
