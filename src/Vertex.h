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

static const Vertex cubeVertices[] = {
  { -0.5f,  0.5f, -0.5f, 1.0f,  1.0f, 0.0f, 0.0f, 1.0f }, // 0
  {  0.5f,  0.5f, -0.5f, 1.0f,  0.0f, 1.0f, 0.0f, 1.0f }, // 1
  {  0.5f, -0.5f, -0.5f, 1.0f,  0.0f, 0.0f, 1.0f, 1.0f }, // 2
  { -0.5f, -0.5f, -0.5f, 1.0f,  1.0f, 1.0f, 0.0f, 1.0f }, // 3
  { -0.5f,  0.5f,  0.5f, 1.0f,  1.0f, 0.0f, 1.0f, 1.0f }, // 4
  {  0.5f,  0.5f,  0.5f, 1.0f,  0.0f, 1.0f, 1.0f, 1.0f }, // 5
  {  0.5f, -0.5f,  0.5f, 1.0f,  0.5f, 0.5f, 1.0f, 1.0f }, // 6
  { -0.5f, -0.5f,  0.5f, 1.0f,  1.0f, 0.5f, 0.0f, 1.0f }, // 7
};

static const unsigned short cubeIndices[] = {
  0,2,1, 0,3,2,  // front (-Z)
  4,5,6, 4,6,7,  // back (+Z)
  4,0,3, 4,3,7,  // left (-X)
  1,5,6, 1,6,2,  // right (+X)
  4,5,1, 4,1,0,  // top (+Y)
  3,2,6, 3,6,7   // bottom (-Y)
};
