#pragma once
#include <type_traits>

union uint2 {
  struct { unsigned x, y; };
  struct { unsigned w, h; };

  template<typename A, typename B>
    requires std::is_arithmetic_v<A> && std::is_arithmetic_v<B>
    uint2(A a, B b)
    : x(static_cast<unsigned>(a)),
    y(static_cast<unsigned>(b)) {};

  uint2(unsigned a, unsigned b) : x(a), y(b) {}
  uint2(uint2&&) = default;
  uint2(const uint2&) = default;
  uint2& operator=(const uint2&) = default;
  uint2& operator=(uint2&&) = default;
};
