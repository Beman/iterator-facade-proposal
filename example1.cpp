//  example1.cpp 
//
//  Copyright Eric Niebler 2014
//  Copyright Beman Dawes 2015
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt)

#include <cassert>
#include <iostream>
#include <typeinfo>
#include <stl2/detail/iterator/basic_iterator.hpp>

namespace stl2 = __stl2;

namespace
{
    template<typename I>
    struct cursor
    {
        I it_;
        struct mixin : stl2::basic_mixin<cursor>
        {
            mixin() = default;
            mixin(cursor c) : stl2::basic_mixin<cursor>(c) {}
            mixin(I i) : stl2::basic_mixin<cursor>(cursor{i}) {}
        };
        cursor() = default;
        explicit cursor(I i) : it_(i) {}
//        template<class J, CONCEPT_REQUIRES_(stl2::ConvertibleTo<J, I>())>
//        cursor(cursor<J> that) : it_(std::move(that.it_)) {}
//        cursor(cursor that) : it_(std::move(that.it_)) {}
        auto read() const -> decltype(*it_) { return *it_; }
        void next() { ++it_; }
    };

    template<class I>
    using iterator = stl2::basic_iterator<cursor<I>>;

}  // unnamed namespace

int main()
{
  std::cout << "Testing basic_iterator" << std::endl;

  static const char sz[] = "hello world";
  typedef iterator<char const*> iterator_t;
  std::cout << typeid(iterator_t).name() << std::endl;

  iterator_t itr{sz};

  std::cout << typeid(decltype(*itr)).name() << std::endl;

  assert(*itr == 'h');
  ++itr;
  assert(*itr == 'e');

  std::cout << "Testing basic_iterator complete" << std::endl;
}
