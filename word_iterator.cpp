//  word_iterator.cpp  -----------------------------------------------------------------//

//  Copyright Beman Dawes 2016

//  Subject to the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

#include <iostream>
#include <string>
#include <cassert>
#include <stl2/detail/iterator/basic_iterator.hpp>

namespace stl2 = __stl2;

class cursor
{
  const std::string* str_;        // nullptr if uninitialized
  std::string::size_type begin_;  // end iterator: begin_ == std::string::npos
  std::string::size_type end_;    // half open
  std::string word_;
 public:
  cursor() noexcept
    : str_(nullptr), begin_(std::string::npos), word_() {}
  cursor(const std::string& str)
    : str_(&str), begin_(0), end_(0) { next(); }
  const std::string& read() const noexcept { return word_; }
  bool equal(const cursor& rhs) const noexcept { return begin_ == rhs.begin_; }
  void next() {
    static const char* alpha =
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    assert(str_);                        // fails if uninitialized
    assert(begin_ != std::string::npos); // fails on increment past end
    if ((begin_ = end_) != std::string::npos) {
      if ((begin_ = str_->find_first_of(alpha, begin_)) == std::string::npos)
        return;
      end_ = str_->find_first_not_of(alpha, begin_);
      word_.assign(*str_, begin_, end_ - begin_);
    }
  }
};

int main(int argc, char * argv[])
{
  using iterator = stl2::basic_iterator<cursor>;

  std::string s("now is the time  when\nall good people should-party");

  for (iterator it(s); it != iterator(); ++it)
    std::cout << *it << std::endl;
}