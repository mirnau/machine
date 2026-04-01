//INFO: Total 32 bytes per vertex, perfectly packed. No wasted padding, GPU reads it efficiently. 

struct Vertex {
      float x, y, z, w;
      float r, g, b, a;
  };

  static const Vertex triangleVertices[] = {
      {  0.0f,  0.5f, 0.0f, 1.0f,   1.0f, 0.0f, 0.0f, 1.0f },
      {  0.5f, -0.5f, 0.0f, 1.0f,   0.0f, 1.0f, 0.0f, 1.0f },
      { -0.5f, -0.5f, 0.0f, 1.0f,   0.0f, 0.0f, 1.0f, 1.0f },
  };
 
