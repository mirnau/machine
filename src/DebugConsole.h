#if defined DEBUG
#include <windows.h>
#include <cstdio>

inline void CreateDebugConsole() {
  AllocConsole();
  FILE* f;
  freopen_s(&f, "CONOUT$", "w", stdout);
  freopen_s(&f, "CONOUT$", "w", stderr);
  freopen_s(&f, "CONIN$", "r", stdin);
}

#else

inline void CreateDebugConsole() {};

#endif
