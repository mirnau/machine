#pragma once
#include <Windows.h>
#include <wrl/client.h>
#include <d3d11.h>
#include <dxgi1_3.h>
#include <array>

using Microsoft::WRL::ComPtr;

namespace Graphics {
        class DX11 {
                public:
                        DX11() = default;
                        DX11(const DX11& d) = delete;
                        DX11(DX11&& d) = delete;
                        ~DX11();
                        DX11& operator=(const DX11&) = delete;

                        int Init(HWND hwnd);
                        HRESULT CreateDeviceAndSwapChain();
                        void CreateSwapChainDesc(HWND hwnd);

                        ID3D11Device* GetDevice() const { return m_device.Get();};
                        ID3D11DeviceContext* GetContext() const { return m_context.Get();};
                        IDXGISwapChain* GetSwapChain() const { return m_swapChain.Get();};
                private:
                        ComPtr<ID3D11Device> m_device;
                        ComPtr<ID3D11DeviceContext> m_context;
                        ComPtr<IDXGISwapChain> m_swapChain;
                        DXGI_SWAP_CHAIN_DESC m_swapchainDesc;

                        const std::array<D3D_FEATURE_LEVEL, 7> m_levels {
                                D3D_FEATURE_LEVEL_11_1,
                                        D3D_FEATURE_LEVEL_11_0,
                                        D3D_FEATURE_LEVEL_10_1,
                                        D3D_FEATURE_LEVEL_10_0,
                                        D3D_FEATURE_LEVEL_9_3,
                                        D3D_FEATURE_LEVEL_9_2,
                                        D3D_FEATURE_LEVEL_9_1,
                        };
                        
                        UINT GetDeviceFlags();
                        D3D_FEATURE_LEVEL m_featureLevel{};
        };
}
