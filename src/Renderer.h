#include "Graphics.h"

namespace Graphics {
  class Renderer {

    public: 
      

    public:
      Renderer(DX11& d) : dx11(d) {};
      Renderer(const Renderer& r) = delete;
      Renderer(Renderer&& r) = delete;
      Renderer& operator=(const Renderer& r) = delete;
      ~Renderer() = default;

    private:
      DX11& dx11;
  };
}
 
