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
#include <stl2/detail/basic_iterator.hpp>

#include <stl2/detail/counted_iterator.hpp>

namespace
{
  template<typename I>
  struct cursor
  {
    I it_;
    struct mixin : ranges::basic_mixin<cursor>
    {
      mixin() = default;
      mixin(cursor c) : ranges::basic_mixin<cursor>(c) {}
      mixin(I i) : ranges::basic_mixin<cursor>(cursor{i}) {}
    };
    cursor() = default;
    explicit cursor(I i) : it_(i) {}
    template<class J, CONCEPT_REQUIRES_(ranges::ConvertibleTo<J, I>())>
    cursor(cursor<J> that) : it_(std::move(that.it_)) {}

    auto get() const -> decltype(*it_) { return *it_; }
    void next() { ++it_; }
  };

  template<class I>
  using iterator = ranges::basic_iterator<cursor<I>>;

}  // unnamed namespace

int main()
{
  std::cout << "\nTesting basic_iterator\n";
  using I = iterator<char const *>;

  static const char sz[] = "hello world";
  I i{sz};
  assert(*i == 'h');
  ++i;
  assert(*i == 'e');
}
