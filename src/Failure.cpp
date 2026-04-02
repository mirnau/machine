#include <format>
#include <string>
#include <source_location>
#include <winerror.h>
#include <stacktrace>
#include "Failure.h"

namespace Machine {

Failure::Failure(const std::string& message, std::source_location loc)
  : m_message(std::format("[{}:{}] {}: {}\n{}",
                          loc.file_name(),
                          loc.line(),
                          loc.function_name(),
                          message,
                          std::to_string(std::stacktrace::current())))
{}

const char* Machine::Failure::what() const noexcept {
  return m_message.c_str();
}

Failure::Graphics::Graphics(HRESULT hr, std::source_location loc) 
: Failure(FormatHResult(hr), loc), m_hr(hr) 
{}

std::string Failure::Graphics::FormatHResult(HRESULT hr) {
  wchar_t* msg = nullptr;
  FormatMessageW(
    FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
    nullptr,
    hr,
    0,
    reinterpret_cast<LPWSTR>(&msg),
    0,
    nullptr
  );

  if (!msg)
    return std::format("HRESULT 0x{:08X}", static_cast<unsigned>(hr));

  std::wstring wide(msg);
  LocalFree(msg);
  return std::string(wide.begin(), wide.end());
}

HRESULT Failure::Graphics::GetHResult() const noexcept {
  return m_hr;
}

  Failure::Shader::Shader(HRESULT hr, ID3DBlob* errorBlob, std::source_location loc)
    : Failure(errorBlob
        ? std::string(static_cast<char*>(errorBlob->GetBufferPointer()), errorBlob->GetBufferSize())
        : Failure::Graphics::FormatHResult(hr), loc)
  {}
}
