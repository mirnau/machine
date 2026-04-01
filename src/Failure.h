#pragma once
#include <exception>
#include <string>
#include <source_location>
#include <windows.h>
#include <d3dcommon.h>

namespace Machine {

class Failure : public std::exception {
public:
  Failure(
    const std::string& message,
    std::source_location loc = std::source_location::current()
  );

  const char* what() const noexcept override;

  class Graphics;
  class Shader;

private:
  std::string m_message;
};

class Failure::Graphics : public Failure {
public:
  Graphics(
    HRESULT hr,
    std::source_location loc = std::source_location::current()
  );

  HRESULT GetHResult() const noexcept;

private:
  static std::string FormatHResult(HRESULT hr);
  HRESULT m_hr;
};

class Failure::Shader : public Failure {
public:
  Shader(
    ID3DBlob* errorBlob,
    std::source_location loc = std::source_location::current()
  );
};

} // namespace Machine
