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
  string word_;
  static constexpr const char* alpha =
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
 public:
  cursor() noexcept
    : str_(nullptr), begin_(string::npos), word_() {}
  cursor(const string& str)
    : str_(&str), begin_(0), word_() { next(); }
  const string& read() const noexcept {
    assert(str_);                    // fails if uninitialized
    assert(begin_ != string::npos);  // fails on dereference end
    return word_;
  }
  bool equal(const cursor& rhs) const noexcept
    { return begin_ == rhs.begin_; }
  void next() {
    assert(str_);                    // fails if uninitialized
    assert(begin_ != string::npos);  // fails on increment past end
    if ((begin_ += word_.size()) != string::npos
        && (begin_ = str_->find_first_of(alpha, begin_)) != string::npos)
      word_.assign(*str_, begin_,
        str_->find_first_not_of(alpha, begin_) - begin_);
  }
};

using word_iterator = ranges::basic_iterator<cursor>;
// <!-- end snippet -->

// <!-- snippet=word_iterator_use -->
int main()
{
  std::string s
    ("now is 2016 the  time   when\nall good programmers should-party.");

  for (word_iterator it(s); it != word_iterator(); ++it)
    std::cout << *it << " (" << it->size() << ")\n";
}
// <!-- end snippet -->
