<table>
<tr>
  <td align="left">Doc. no.:</td>
  <td align="left">D0186R0</td>
</tr>
<tr>
  <td align="left">Date:</td>
  <td align="left">2015-12-20</td>
</tr>
<tr>
  <td align="left">Project:</td>
  <td align="left">To be decided</td>
</tr>
<tr>
  <td align="left" valign="top">Reply to:</td>
  <td align="left">Beman Dawes &lt;bdawes at acm dot org&gt;</br>
    Eric Niebler &lt;....&gt;</br>
	Casey Carter &lt;....&gt;</td>
</tr>
</table>

# Iterator Facade Library Proposal for Ranges

<span style="background-color:lightyellow">***This is the "no-access" branch, an experiment to see if removing mention of class ```access``` (i.e. considering it an implementation detail) results in a less confusing specification.***</span>

*"We are what we pretend to be, so we must be careful about what we pretend to be." - Kurt Vonnegut*

>**Summary:** Proposes a library component for easily creating conforming iterators. Based on existing practice. Depends only on the C++17 working paper plus Concepts TS and Ranges TS. Breaks no existing code or ABI's. Two open-source implementations with test suites available. Draft wording provided.

## Table of Contents

*TBS*

## Introduction

### Problem

Iterators that conform to the requirements of the C++ standard library are tedious to write and difficult to write correctly. They are tedious to write because although they need only a few core functions, they also need subsidiary types, functions, and other boilerplate. Conforming iterators are difficult to write correctly because each iterator category has a subtly differing set of requirements, making it all too easy to get subsidiary types, functions, or other boilerplate wrong.

### Solution

A class template that is given a few implementation functions can generate the facade (i.e. public interface and private implementation) for a fully conforming iterator, including all boilerplate. Boost ```iterator_facade``` pioneered this approach and has been in wide use since 2001<sup>&lsqb;[1](#1)&rsqb;</sup>. It eases writing conforming iterators and has proven less error prone than hand-coded iterators. The generated iterator conforms to an extended set of requirements based on the C++98 iterator requirements. Others, such as Chandler Carruth's LLVM ```iterator_facade_base```<sup>&lsqb;[2](#2)&rsqb;</sup>, have provided similar classes inspired by the Boost library.

### Proposal

This paper proposes an iterator facade class template for the standard library, useful by any programmer (novice, experienced, or expert) wishing to easily create a conforming iterator. The proposal uses C++11/14/17 with concepts<sup>&lsqb;[3](#3)&rsqb;</sup> and ranges<sup>&lsqb;[4](#4)&rsqb;</sup> to allow straightforward specification and implementation, and to ensure that the generated iterator is actually conforming. The proposal breaks no existing code and breaks no existing ABI's. Two open-source implementations with test suites are available on GitHub<sup>&lsqb;[5](#5)&rsqb;&lsqb;[6](#6)&rsqb;</sup>.

The proposal is suitable for C++17 if C++17 includes concepts and ranges. Otherwise, the proposal is suitable for the Ranges TS. No other core language or library changes are required.

### History

A 2004 proposal<sup>&lsqb;[7](#7)&rsqb;</sup> based on Boost ```iterator_facade``` failed because it depended on an iterator update proposal<sup>&lsqb;[8](#8)&rsqb;</sup> that failed because without concepts the language was not rich enough to express the necessary iterator requirements.

## Examples

*TBS*

## Design Decisions

#### Use Boost ```iterator_facade``` as a role model

This ensures that the proposal represents existing practice in widespread use.

#### Base the interface on features in the Concepts TS and the Ranges TS working paper

This eliminates the difficulties with specification that bedeviled the 2004 proposal. It allows an interface specification that ensures that the resulting iterator is actually conforming. It improves error message quality when a user makes a mistake.

#### Use Cursor mixins to supply implementation details
 
Cursor mixins have proven themselves useful time and again. That said, it's a curiously indirect way of defining an iterator's interface. The alternative of inheriting directly from the Cursor leads to interface pollution. Cursors are implementation details and they should stay hidden.

## Proposed wording

<span style="background-color:lightgrey">*Editorial comments are shown in italics with a light grey background.*</span>

<span style="background-color:lightgrey">*Proposed wording is relative to the Working Draft, C++ Extensions for Ranges*</span>

<span style="background-color:lightgrey">*For brevity, clarity, and reduced specification errors, add the following at a location to be determined by the project editor:*</span>

#### Method of description (Informative)

Simple concepts that require no further description are defined entirely in the appropriate synopsis and no further description is provided.

Namespaces with names reserved to the implementation are for the sake of exposition  only. Implementations are not required to provide the concepts declared in these namespaces. Implementations are permitted to elide use of such exposition only concepts as long as the requirements the concepts describe are met by some other mechanism.

>[Note: This constitutes an "as if" rule for exposition-only concepts that allows implementations freedom to refactor such concepts or use other mechanisms, such as template metaprogramming, as long as the requirements imposed are met. -- end note]

<span style="background-color:lightgrey">*Add to 24.8, Iterator adaptors [iterators.predef]:*</span>

Iterator adaptors generate new iterator types from existing types.

<span style="background-color:lightgrey">*Add a new iterator adapter at the end of 24.8, Iterator adaptors [iterators.predef]*</span>

#### Basic iterators [iterator.basic]

Class template ```basic_iterator``` is an iterator adaptor that iterates over the sequence described by a cursor type. [Namespace ```cursor```](#namespace-cursor) provides classes, types, concepts, and traits used to create cursor types.

A cursor ```C``` may extend the interface of ```basic_iterator<C>``` by defining a nested mixin type ```C::mixin```. In that way, the author of a cursor can non-intrusively add members and constructors to ```basic_iterator```.

[Class template ```basic_mixin```](#iterator-mixin) supports the creation of mixin types.

>[Note: Mixin types add interface to types that inherit from them. They can also hold an object -- in this case a cursor. By publicly inheriting from a mixin type, ```basic_iterator``` gets: (a) a cursor data member, and, optionally, (b) additional members and constructors. -- end note]

#### Namespace cursor [<a name="namespace-cursor">namespace.cursor</a>]

Namespace ```cursor``` provides a scope for the class, type, concept, and trait identifiers used to create cursor types.

#### Namespace cursor synopsis

```
  namespace cursor {

    // types
    template <class C>
      using reference_t = %!{see below}!%;
    template <class C>
      using rvalue_reference_t = %!{see below}!%;
    template <class C>
      using value_type_t = %!{see below}!%;
    template <class C>
      using difference_type_t = %!{see below}!%;
      
    // concepts
    template <class C>
      concept bool Cursor();
    template <class C>
      concept bool Input()
        { return Cursor<C>() && Readable<C>() && Next<C>(); }
    template <class C>
      concept bool Forward();
    template <class C>
      concept bool Bidirectional()
        { return Forward<C>() && Prev<C>(); }
    template <class C>
      concept bool RandomAccess()
        { return Bidirectional<C>() && Advance<C>() && Distance<C, C>(); }
    template <class C>
      concept bool Contiguous();
    template <class C>
      concept bool Readable();
    template <class C, class T>
      concept bool Writable();
    template <class C>
      concept bool Arrow();
    template <class C>
      concept bool Next();
    template <class C>
      concept bool Prev();
    template <class C>
      concept bool Advance();
    template <class C, class O>
      concept bool Distance();
    template <class C, class O>
      concept bool HasEqual();

    // category traits 
    template <class> struct category {};
    template <Input C>
      struct category<C> { using type = input_iterator_tag; };
    template <Forward C>
      struct category<C> { using type = forward_iterator_tag; };
    template <Bidirectional C>
      struct category<C> { using type = bidirectional_iterator_tag; };
    template <RandomAccess C>
      struct category<C> { using type = random_access_iterator_tag; };
    template <Contiguous C>
      struct category<C> { using type = ext::contiguous_iterator_tag; };
    template <class C>
      using category_t = typename category<C>::type;
  }  // namespace cursor
```

#### Namespace cursor semantics


```
template <class C>
concept bool Cursor();
```
```Cursor<C>()``` is satisfied if and only if ```Semiregular<C>() && Semiregular<Mx>()``` is ```true```, where type ```Mx``` is deduced by the implementation to be ```C::mixin``` if member ```C::mixin``` is present, otherwise is deduced to be ```basic_mixin<C>```.

```
template <class C>
concept bool Forward();
```
 
```Forward<C>``` is satisfied if and only if ```Input<C>()``` ```&& EqualityComparable<C, C>()``` ```&& !%!{imp}!%::single_pass<C>::value``` where ```single_pass``` is defined as if:

<blockquote>
<pre>
template &lt;class&gt;
constexpr bool single_pass = false;
template &lt;class C&gt;
  requires requires {
    typename C::single_pass;
    requires bool(C::single_pass::value);
  }
constexpr bool single_pass = true;</pre>
</blockquote>

```
    template <class C>
      concept bool Contiguous();
```
```Contiguous<C>``` is satisfied if and only if ```RandomAccess<C>()```, type ```C::contiguous``` exists, ```C::contiguous::value == true```, and.

```
    template <class C>
      concept bool Readable();
```
```Readable<C>``` is satisfied if and only if

```
    template <class C, class T>
      concept bool Writable();
```
```Writable<C, T>``` is satisfied if and only if

```
    template <class C>
      concept bool Arrow();
```
```Arrow<C>``` is satisfied if and only if

```
    template <class C>
      concept bool Next();
```
```Next<C>``` is satisfied if and only if

```
    template <class C>
      concept bool Prev();
```
```Prev<C>``` is satisfied if and only if

```
    template <class C>
      concept bool Advance();
```
```Advance<C>``` is satisfied if and only if

```
    template <class C, class O>
      concept bool Distance();
```
```Distance<C>``` is satisfied if and only if

```
    template <class C, class O>
      concept bool HasEqual();
```
```HasEqual<C>``` is satisfied if and only if

<span style="background-color:lightgrey">*Add to 24.6, Header ```<experimental/ranges/iterator>``` synopsis [iterator.synopsis] or some other synopsis:*</span>

```
  // basic_mixin
  template <Destructible T>
  class basic_mixin;
``` 

<span style="background-color:lightgrey">*Add somewhere:*</span>
        
####  Class template <code>basic_mixin</code> [<a name="iterator-mixin">iterator.mixin</a>]

Class template <code>basic_mixin</code> describes an empty mixin type.

```
  template <Destructible T>
  class basic_mixin {
  public:
    constexpr basic_mixin()
      noexcept(is_nothrow_default_constructible<T>::value)
      requires DefaultConstructible<T>();
    constexpr basic_mixin(const T& t)
      noexcept(is_nothrow_copy_constructible<T>::value)
      requires CopyConstructible<T>();
    constexpr basic_mixin(T&& t)
      noexcept(is_nothrow_move_constructible<T>::value)
      requires MoveConstructible<T>();
  };
```
 
##### <code>basic_mixin</code> constructors [mixin.cons]

```
constexpr basic_mixin()
  noexcept(is_nothrow_default_constructible<T>::value)
  requires DefaultConstructible<T>();
```

>*Effects:* Default constructs an object of type <code>basic_mixin</code>.

```
constexpr basic_mixin(const T& t)
  noexcept(is_nothrow_copy_constructible<T>::value)
  requires CopyConstructible<T>();
```

>*Effects:* Copy constructs an object of type <code>basic_mixin</code>.

```
constexpr basic_mixin(T&& t)
  noexcept(is_nothrow_move_constructible<T>::value)
  requires MoveConstructible<T>();
```

>*Effects:* Move constructs an object of type <code>basic_mixin</code>.

#### Class template ```basic_iterator```

Class template ```basic_iterator``` describes an iterator over a sequence. A type satisfying the Cursor requirements provides the sequence. 

```
namespace std { namespace experimental { namespace ranges_v1 { inline namespace v1 {
  
  template <Cursor C>  
  class basic_iterator
  : public cursor::mixin_t<C>  // TODO: move mixin_t<> from cursor::access to cursor
  {
    // types
    using difference_type = cursor::difference_type_t<C>;
    
    // constructors, assignments, and moves
    basic_iterator() = default;
    using mixin_t::mixin_t;
    template <ConvertibleTo<C> O>
    constexpr basic_iterator(basic_iterator<O> that)
      noexcept(is_nothrow_constructible<mixin_t, O&&>::value);
    friend constexpr decltype(auto) iter_move(const basic_iterator& i)
      noexcept(cur().move(i.cur()))
      requires cursor::Readable<C>() && cursor::Move<C>();
    
    // dereferences
    constexpr decltype(auto) operator*() const
      noexcept(noexcept(declval<const C&>().read()))
    constexpr decltype(auto) operator*() noexcept
      requires cursor::is_writable<C>;
    constexpr decltype(auto) operator*() const noexcept
      requires cursor::is_writable<C>;
    constexpr decltype(auto) operator->() const
      noexcept(noexcept(declval<const C&>().arrow()))
      requires cursor::Arrow<const C>();
         
    // modifiers
    constexpr basic_iterator& operator++() & noexcept;
    constexpr basic_iterator& operator++() &
      noexcept(noexcept(declval<C&>().next()))
      requires cursor::Next<C>();
    constexpr basic_iterator& operator++(int) & noexcept;
    constexpr postfix_increment_result_t operator++(int) &
      noexcept(is_nothrow_constructible<postfix_increment_result_t,
        basic_iterator&>::value
        && is_nothrow_move_constructible<postfix_increment_result_t>::value &&
             noexcept(++declval<basic_iterator&>()))
      requires cursor::Next<C>();
    constexpr basic_iterator& operator--() &
      noexcept(noexcept(declval<C&>().prev()))
      requires cursor::Bidirectional<C>();
    constexpr basic_iterator operator--(int) &
      noexcept(is_nothrow_copy_constructible<basic_iterator>::value &&
             is_nothrow_move_constructible<basic_iterator>::value &&
             noexcept(--declval<basic_iterator&>()))
      requires cursor::Bidirectional<C>();
    constexpr basic_iterator& operator+=(difference_type n) &
      noexcept(noexcept(declval<C&>().advance(n)))
      requires cursor::RandomAccess<C>();
    constexpr basic_iterator& operator-=(difference_type n) &
      noexcept(noexcept(declval<C&>().advance(-n)))
      requires cursor::RandomAccess<C>();  
    friend constexpr basic_iterator
      operator+(const basic_iterator& i, difference_type n)
        noexcept(is_nothrow_copy_constructible<basic_iterator>::value &&
             is_nothrow_move_constructible<basic_iterator>::value &&
             noexcept(declval<C&>().advance(n)))
        requires cursor::RandomAccess<C>();
    friend constexpr basic_iterator
      operator+(difference_type n, const basic_iterator& i)
        noexcept(noexcept(i + n))
        requires cursor::RandomAccess<C>();
    friend constexpr basic_iterator
      operator-(const basic_iterator& i, difference_type n)
        noexcept(noexcept(i + -n))
        requires cursor::RandomAccess<C>();    
    constexpr decltype(auto) operator[](difference_type n) const
      noexcept(noexcept(*(declval<basic_iterator&>() + n)))
      requires cursor::RandomAccess<C>();
```  
  <span style="background-color:lightgrey"><blockquote style="background-color:lightgrey">*We wish to hide implementation details, but since we wish to utilize the cursor type C that provides ```basic_iterator```'s customization types and functions as a mixin, it becomes a practical impossibility to describe ```basic_iterator``` without describing at least a few implementation types and functions. These conflicting needs are resolved by supplying the types and functions as private "exposition only" members.*</blockquote></span>
```  
  private:  %!{// all private members are exposition only}!% 
    // types   
    using mixin_t = cursor::mixin_t<C>;
    using assoc_t = %!{see below}!%;
   
    // constructors
    using typename assoc_t::postfix_increment_result_t;
    using typename assoc_t::reference_t;
    using typename assoc_t::const_reference_t;
 
    // mixin cursor object access
    constexpr C& cur() &
      noexcept(noexcept(declval<mixin_t&>().get()))
        { return mixin_t::get(); }
    constexpr const C& cur() const&
      noexcept(noexcept(declval<const mixin_t&>().get()))
        { return mixin_t::get(); }
    constexpr C&& cur() &&
      noexcept(noexcept(declval<mixin_t&>().get()))
        { return mixin_t::get(); }
  };
  
  // basic_iterator nonmember traits
  
  template <cursor::Input C>
  struct iterator_category<basic_iterator<C>>
    { using type = cursor::category_t<C>; };

  template <cursor::Input C>
  struct value_type<basic_iterator<C>>
    { using type = cursor::value_type_t<C>; };
   
  // basic_iterator nonmember functions

  template <class C>
    constexpr C& get_cursor(basic_iterator<C>& i)
  STL2_NOEXCEPT_RETURN(                             // TODO
    cursor::access::cursor(i)
  )

  template <class C>
  constexpr const C& get_cursor(const basic_iterator<C>& i)
  STL2_NOEXCEPT_RETURN(                             // TODO
    cursor::access::cursor(i)
  )

  template <class C>
  constexpr C&& get_cursor(basic_iterator<C>&& i)   // TODO
  STL2_NOEXCEPT_RETURN(
    cursor::access::cursor(__stl2::move(i))
  )
  
  template <class C>
    requires cursor::Sentinel<C, C>()
  constexpr bool operator==(
    const basic_iterator<C>& lhs, const basic_iterator<C>& rhs)
    noexcept(static_cast<bool>(get_cursor(lhs).equal(get_cursor(rhs))))
      { return get_cursor(lhs).equal(get_cursor(rhs)); }
  
  template <class C, class S>
    requires cursor::Sentinel<S, C>()
  constexpr bool operator==(
    const basic_iterator<C>& lhs, const S& rhs)
    noexcept(get_cursor(lhs).equal(rhs))
      { return get_cursor(lhs).equal(rhs); }

  template <class C, class S>
    requires cursor::Sentinel<S, C>()
  constexpr bool operator==(
    const S& lhs, const basic_iterator<C>& rhs)
    noexcept(rhs == lhs)
      { return rhs == lhs; }

  template <class C>
    requires cursor::Sentinel<C, C>()
  constexpr bool operator!=(
    const basic_iterator<C>& lhs, const basic_iterator<C>& rhs)
    noexcept(!(lhs == rhs))
      { return !(lhs == rhs); }

  template <class C, class S>
    requires cursor::Sentinel<S, C>()
  constexpr bool operator!=(
    const basic_iterator<C>& lhs, const S& rhs)
    noexcept(!get_cursor(lhs).equal(rhs))
      { return !get_cursor(lhs).equal(rhs); }

  template <class C, class S>
    requires cursor::Sentinel<S, C>()
  constexpr bool operator!=(
    const S& lhs, const basic_iterator<C>& rhs)
    noexcept(rhs != lhs)
      { return rhs != lhs; }

  template <class C>
    requires cursor::SizedSentinel<C, C>()
  constexpr cursor::access::difference_type_t<C> operator-(
    const basic_iterator<C>& lhs, const basic_iterator<C>& rhs)
    noexcept(noexcept(get_cursor(rhs).distance_to(get_cursor(lhs))))
      { return get_cursor(rhs).distance_to(get_cursor(lhs)); }

  template <class C, class S>
    requires cursor::SizedSentinel<S, C>()
  constexpr cursor::access::difference_type_t<C> operator-(
    const S& lhs, const basic_iterator<C>& rhs)
    noexcept(noexcept(get_cursor(rhs).distance_to(lhs)))
      { return get_cursor(rhs).distance_to(lhs); }
      
  template <class C, class S>
    requires cursor::SizedSentinel<S, C>()
  constexpr cursor::access::difference_type_t<C> operator-(
    const basic_iterator<C>& lhs, const S& rhs)
    noexcept(noexcept(-(rhs - lhs)))
      { return -(rhs - lhs); }

  template <class C>
    requires cursor::SizedSentinel<C, C>()
  constexpr bool operator<(
    const basic_iterator<C>& lhs, const basic_iterator<C>& rhs)
    noexcept(noexcept(0 < get_cursor(rhs).distance_to(lhs)))
      { return 0 < get_cursor(rhs).distance_to(lhs); }

  template <class C>
    requires cursor::SizedSentinel<C, C>()
  constexpr bool operator>(
    const basic_iterator<C>& lhs, const basic_iterator<C>& rhs)
    noexcept(noexcept(rhs < lhs))
      { return rhs < lhs; }

  template <class C>
    requires cursor::SizedSentinel<C, C>()
  constexpr bool operator<=(
    const basic_iterator<C>& lhs, const basic_iterator<C>& rhs)
    noexcept(noexcept(!(rhs < lhs)))
      { return !(rhs < lhs); } 

  template <class C>
    requires cursor::SizedSentinel<C, C>()
  constexpr bool operator>=(
    const basic_iterator<C>& lhs, const basic_iterator<C>& rhs)
    noexcept(noexcept(!(lhs < rhs)))
      { return !(lhs < rhs); } 
}}}}
```

#### Requirements [basic_iterator.require]

Class template ```basic_iterator``` publicly inherits from ```C::mixin``` if ```C``` defines a type named ```mixin```, otherwise it publicly inherits from ```basic_mixin<C>```. 

Private members of class ```basic_iterator``` are for exposition only (17.5.2.3 Private members [objects.within.classes]).

<span style="background-color:lightgrey">*Reference is made to 17.5.2.3 because implementations will need to rely on paragraph 3, "An implementation may use any technique that provides equivalent external behavior" since specifications like ```operator*()``` "Returns: ```cur().read()``` may require some form of indirection in actual implementations to meet requirements."*</span>

#### Types [basic_iterator.types]

```
difference_type
```

```difference_type``` is defined as:

* ```C::difference_type``` if ```C``` has a member type ```difference_type```, 
* otherwise ```decltype(declval<const C&>().distance_to(declval<const C&>()))``` if ```C``` has such a function,
* otherwise ```std::ptrdiff_t```.

#### Constructors, assignments, and moves [basic_iterator.cons]

%!{TBS}!%

#### Dereferences [basic_iterator.deref]

```
constexpr decltype(auto) operator*() const
  noexcept(noexcept(declval<const C&>().read()));
```
>*Returns:* ```cur().read()```.
 
>*Remarks:* ```cur().read()``` requires ```requires(C& c) { c.read(); }```.

```
constexpr decltype(auto) operator*() noexcept
  requires cursor::is_writable<C>;
```
>*Returns:* ```reference_t{cur()}```.
 
```
constexpr decltype(auto) operator*() const noexcept
  requires cursor::is_writable<C>;
```
>*Returns:* ```const_reference_t{cur()}```.  
 
```
constexpr decltype(auto) operator->() const
  noexcept(noexcept(declval<const C&>().arrow()))
  requires cursor::Arrow<const C>();
```
>*Returns:* ```cur().arrow()```.

#### Modifiers [basic_iterator.mods]

```
constexpr basic_iterator& operator++() & noexcept;
```
>*Returns:* ```*this```.

```
constexpr basic_iterator& operator++() &
  noexcept(noexcept(cursor::access::next(declval<C&>())))
  requires cursor::Next<C>();
```
>*Effects:* ```cursor::access::next(pos())```.

>*Returns:* ```*this```.

```
constexpr basic_iterator& operator++(int) & noexcept;
```
>*Returns:* ```*this```.

```
constexpr postfix_increment_result_t operator++(int) &
  noexcept(is_nothrow_constructible<postfix_increment_result_t,
    basic_iterator&>::value
    && is_nothrow_move_constructible<postfix_increment_result_t>::value &&
         noexcept(++declval<basic_iterator&>()))
  requires cursor::Next<C>();
```
>*Effects:*
  ```postfix_increment_result_t tmp(*this);```
  ```++*this;```
  ```return tmp;```

```
constexpr basic_iterator& operator--() &
  noexcept(noexcept(declval<C&>().prev()))
  requires cursor::Bidirectional<C>();
```
>*Effects:* ```cur().prev();```.

>*Returns:* ```*this```.

```
constexpr basic_iterator operator--(int) &
  noexcept(is_nothrow_copy_constructible<basic_iterator>::value &&
         is_nothrow_move_constructible<basic_iterator>::value &&
         noexcept(--declval<basic_iterator&>()))
  requires cursor::Bidirectional<C>();
```
>*Effects:*
  ```auto tmp = *this;```
  ```--*this;```
  ```return tmp;```

```
constexpr basic_iterator& operator+=(difference_type n) &
  noexcept(noexcept(declval<C&>().advance(n)))
  requires cursor::RandomAccess<C>();
```
>*Effects:* ```cur().advance(-n)```.

>*Returns:* ```*this```.

```
constexpr basic_iterator& operator-=(difference_type n) &
  noexcept(noexcept(declval<C&>().advance(-n)))
  requires cursor::RandomAccess<C>();  
```
>*Effects:* ```cur().advance(-n)```.

>*Returns:* ```*this```.

```
friend constexpr basic_iterator
  operator+(const basic_iterator& i, difference_type n)
    noexcept(is_nothrow_copy_constructible<basic_iterator>::value &&
         is_nothrow_move_constructible<basic_iterator>::value &&
         noexcept(declval<C&>().advance(n)))
    requires cursor::RandomAccess<C>();
```
>*Effects:*
  ```auto tmp = i;```
  ```tmp.cur().advance(n);```
  ```return tmp;```

```
friend constexpr basic_iterator
  operator+(difference_type n, const basic_iterator& i)
    noexcept(noexcept(i + n))
    requires cursor::RandomAccess<C>();
```
>*Returns:* ```i + n```.

```
friend constexpr basic_iterator
  operator-(const basic_iterator& i, difference_type n)
    noexcept(noexcept(i + -n))
    requires cursor::RandomAccess<C>();    
```
>*Returns:* ```i + -n```.

```
constexpr decltype(auto) operator[](difference_type n) const
  noexcept(noexcept(*(declval<basic_iterator&>() + n)))
  requires cursor::RandomAccess<C>();
```
>*Returns:* ```*(*this + n)```.

#### ```basic_iterator nonmember``` functions [basic_iterator.nonmem]

## Acknowledgements

*TBS*

## References
[1]: https://www.boost.org/doc/libs/1_59_0/libs/iterator/doc/iterator_facade.html
[2]: https://github.com/llvm-mirror/llvm/blob/master/include/llvm/ADT/iterator.h
[3]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4553.pdf
[4]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4560.pdf
[5]: https://github.com/ericniebler/range-v3
[6]: https://github.com/CaseyCarter/cmcstl2
[7]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2004/n1641.html
[8]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2004/n1640.html


&lsqb;<a name="1">1</a>&rsqb; David Abrahams, Jeremy Siek, Thomas Witt, [Boost Iterator Facade][1], 2003.

&lsqb;<a name="2">2</a>&rsqb; Chandler Carruth, [LLVM ```iterator_facade_base``` iterator.h][2], 2014.

&lsqb;<a name="3">3</a>&rsqb; Andrew Sutton, [N4553, Working Draft, C++ extensions for Concepts][3], 2015.

&lsqb;<a name="4">4</a>&rsqb; Eric Niebler, Casey Carter, [N4650, Working Draft, C++ Extensions for Ranges][4], 2015.

&lsqb;<a name="5">5</a>&rsqb; Eric Niebler, Casey Carter, [Experimental range library for C++11/14/17][5], 2015. Requires a C++14 compiler. Simulates concepts with macros and templates.

&lsqb;<a name="6">6</a>&rsqb; Casey Carter, Eric Niebler, [An implementation of C++ Extensions for Ranges][6], 2015. Requires a C++14 compiler supporting concepts (e.g. GCC trunk).

&lsqb;<a name="7">7</a>&rsqb; David Abrahams, Jeremy Siek, Thomas Witt, [N1641, Iterator Facade and Adaptor][7], 2004.

&lsqb;<a name="8">8</a>&rsqb; David Abrahams, Jeremy Siek, Thomas Witt, [N1640, New Iterator Concepts][8], 2004.

------
