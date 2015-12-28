//  example1.cpp 
//
//  Copyright Eric Niebler 2014
//  Copyright Beman Dawes 2015
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt)

#include <cstring>
#include <cassert>
#include <tuple>
#include <iostream>
#include <stl2/detail/iterator/basic_iterator.hpp>

//#include <stl2/detail/iterator/counted_iterator.hpp>

namespace stl2 = __stl2;

namespace
{
  struct cursor
  {
  public:
    cursor() = default;

    struct mixin : protected stl2::basic_mixin<cursor>
    {
      mixin() = default;
      using stl2::basic_mixin<cursor>::basic_mixin;
      mixin(I p) : basic_mixin<cursor>(cursor{p}) {}
    };

    explicit cursor(const char* p) : p_(p) {}

    auto get() const -> decltype(*p_) { return *p_; }
    void next() { ++p_; }

  private:
    const char* p_;
  };

  using iterator = stl2::basic_iterator<cursor>;

}  // unnamed namespace

int main()
{
  std::cout << "\nTesting basic_iterator\n";

  static const char sz[] = "hello world";
  iterator itr{sz};
  assert(*itr == 'h');
  ++itr;
  assert(*itr == 'e');
}
