//  word_iterator.cpp  -----------------------------------------------------------------//

//  Copyright Beman Dawes 2016

//  Subject to the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

#include <iostream>
#include <string>
#include <cassert>
#include <stl2/detail/iterator/basic_iterator.hpp>

using namespace std::experimental;

// <!-- snippet=word_iterator -->
class cursor
{
  using string = std::string;
  const string* str_;        // nullptr if uninitialized
  string::size_type begin_;  // end iterator: begin_ == string::npos
  string::size_type end_;    // half open
  string word_;
 public:
  cursor() noexcept
    : str_(nullptr), begin_(string::npos), word_() {}
  cursor(const string& str)
    : str_(&str), begin_(0), end_(0) { next(); }
  const string& read() const noexcept
    { return word_; }
  bool equal(const cursor& rhs) const noexcept
    { return begin_ == rhs.begin_; }
  void next() {
    static const char* alpha =
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    assert(str_);                    // fails if uninitialized
    assert(begin_ != string::npos);  // fails on increment past end
    if ((begin_ = end_) != string::npos) {
      if ((begin_ = str_->find_first_of(alpha, begin_)) != string::npos) {
        end_ = str_->find_first_not_of(alpha, begin_);
        word_.assign(*str_, begin_, end_ - begin_);
      }
    }
  }
};

int main()
{
  std::string s
    ("now is 2016 the  time   when\nall good programmers should-party");
  using iterator = ranges::basic_iterator<cursor>;

  for (iterator it(s); it != iterator(); ++it)
    std::cout << *it << ' ' << it->size() << std::endl;
}
// <!-- end snippet -->
