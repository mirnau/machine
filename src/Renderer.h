#pragma once 
#include <d3d11.h>
#include <winnt.h>
#include <wrl/client.h>
#include "DX11.h"

using Microsoft::WRL::ComPtr;

namespace Graphics {
    class Renderer {
        public:
            Renderer(DX11& dx);
            Renderer(const Renderer& r) = delete;
            Renderer(Renderer&& r) = delete;
            ~Renderer() = default;
            Renderer& operator=(const Renderer& r) = delete;

            HRESULT Init();
            void Clear();
            void Present();
            void Render();
            HRESULT Resize(UINT w, UINT h);
            DX11& GetDX11() {return m_dx; };

        private: 
            HRESULT CreateRVT();

        private:
            //INFO: in the case of other driver implementations, 
            //an interface will replace this reference.
            //for a Dx enginge, this is appropriate.
            DX11& m_dx;
            ComPtr<ID3D11RenderTargetView> m_targetView;
            float m_color[4] = { 0.392f, 0.584f, 0.929f, 1.0f };

    };
}
