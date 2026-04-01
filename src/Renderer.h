#pragma once 
#include <d3d11.h>
#include <winnt.h>
#include <wrl/client.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
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

        HRESULT Init(uint2 size);
        void Clear();
        void Present();
        void Render();
        void SetViewport(uint2 size);
        HRESULT Resize(uint2 size);
        DX11& GetDX11() {return m_dx; };

private: 
        HRESULT CreateRVT();
        HRESULT CompileShaders();
        HRESULT CreateInputLayout();
        HRESULT CreateVertexBuffer();

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
        D3D11_VIEWPORT m_viewport{};
        float m_color[4] = { 0.392f, 0.584f, 0.929f, 1.0f };
        const wchar_t* vxPath = L"shaders/VertexShader.hlsl";
        const wchar_t* pxPath = L"shaders/PixelShader.hlsl";

};
}
