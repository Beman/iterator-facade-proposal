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

<span style="background-color:lightgrey">*Add to 24.6, Header ```<experimental/ranges/iterator>``` synopsis [iterator.synopsis] or some other synopsis:*</span>

```
  // basic_mixin
  template <Destructible T>
  class basic_mixin;
``` 

<span style="background-color:lightgrey">*Add somewhere:*</span>
        
####  Class template <code>basic_mixin</code> [???.mixin]

The class template <code>basic_mixin</code> describes objects that ???


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

<span style="background-color:lightgrey">*Add to 24.8, Iterator adaptors [iterators.predef]:*</span>

Iterator adaptors generate new iterator types from existing iterator types.

<span style="background-color:lightgrey">*Add a new sub-clause after 24.8, Iterator adaptors [iterators.predef]*</span>

#### Iterator facades [iterator.facades]

Iterator facades generate new iterator types from cursor types. Cursor types describe the minimum functionality required to generate fully compliant iterators.

<span style="background-color:lightgrey">*Class template ```basic_iterator``` is the only iterator facade currently proposed.*</span> 

#### Namespace cursor synopsis [cursor.synopsis]

Namespace ```cursor``` provides a scope for the class, type, concept, and trait identifiers needed to create cursor types.

```
  namespace cursor {
    class access;

    // types
    template <class C>
      using reference_t = access::reference_t<C>;
    template <class C>
      using rvalue_reference_t = access::rvalue_reference_t<C>;
    template <class C>
      using value_type_t = access::value_type_t<C>;
    template <class C>
      using difference_type_t = access::difference_type_t<C>;
      
    // for exposition only
    namespace __impl
    {  
      template <class C>
        concept bool Arrow()
          { return requires(C& c) { access::arrow(c); }; }
      template <class C>
        concept bool Next()
          { return requires(C& c) { access::next(c); }; }
      template <class C>
        concept bool Prev()
          { return requires(C& c) { access::prev(c); }; }
      template <class C>
        concept bool Advance()
          { return requires(C& c, difference_type_t<C> n)
            { access::advance(c, n); }; }
      template <class C, class O>
        concept bool Distance()
          { return requires(const C& l, const O& r)
            { access::distance(l, r); }; }
      template <class C, class O>
        concept bool HasEqual()
          { return requires(const C& l, const O& r) { access::equal(l, r); }; }
    }  // namespace __impl
  
    // concepts
    template <class C>
      concept bool Readable()
        { return requires(C& c) {
          typename value_type_t<remove_cv_t<C>>;
          access::read(c); }; }
    template <class C, class T>
      concept bool Writable()
        { return requires(C& c, T&& t)
          { access::write(c, forward<T>(t)); }; }
    template <class C>
      concept bool Cursor()
        { return Semiregular<C>() && Semiregular<access::mixin_t<C>>(); }
    template <class C>
      concept bool Input()
        { return Cursor<C>() && Readable<C>() && Next<C>(); }
    template <class C>
      concept bool Forward()
        { return Input<C>() && HasEqual<C, C>()
            && !access::single_pass<C>::value; }
    template <class C>
      concept bool Bidirectional()
        { return Forward<C>() && Prev<C>(); }
    template <class C>
      concept bool RandomAccess()
        { return Bidirectional<C>() && Advance<C>() && Distance<C, C>(); }
    template <class C>
      concept bool Contiguous()
        { return RandomAccess<C>() && access::contiguous<C>::value; }

    // category traits 
    template <class>  struct category {};
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

#### Class cursor::access [cursor.access]



```
  namespace cursor {
    class access {
      template <class T>
        struct mixin_base { using type = basic_mixin<T>; };
      template <class T>
        requires requires { typename T::mixin; }
        struct mixin_base<T> { using type = typename T::mixin; };

    public:
      template <class>
        struct reference_type {};
      template <class C>
        requires
          requires(const C& c) { STL2_DEDUCE_AUTO_REF_REF(c.read()); }
        struct reference_type<C> 
          { using type = decltype(declval<const C&>().read()); };
      template <class C>
        using reference_t = typename reference_type<C>::type;

      // Not a bool variable template due to GCC PR68666.
      template <class>
      struct single_pass : false_type {};
      template <class C>
      requires
        requires {
          typename C::single_pass;
          requires bool(C::single_pass::value);
        }
      struct single_pass<C> : true_type {};

      // Not a bool variable template due to GCC PR68666.
      template <class> struct contiguous : false_type {};
      template <class C>
      requires
        requires {
          typename C::contiguous;
          requires bool(C::contiguous::value);
          requires _Is<reference_t<C>, is_reference>;
        }
      struct contiguous<C> : true_type {};

      template <class T>
        using mixin_t = typename mixin_base<T>::type;

      template <class>
      struct difference_type {
        using type = std::ptrdiff_t;
      };
      template <detail::MemberDifferenceType C>
      struct difference_type<C> {
        using type = typename C::difference_type;
      };
      template <class C>
      requires
        !detail::MemberDifferenceType<C> &&
        requires(const C& lhs, const C& rhs) { rhs.distance_to(lhs); }
      struct difference_type<C> {
        using type = decltype(declval<const C&>().distance_to(declval<const C&>()));
      };
      template <class C>
      requires
        SignedIntegral<typename difference_type<C>::type>()
      using difference_type_t = typename difference_type<C>::type;

      template <class C>
      struct value_type {};
      template <detail::MemberValueType C>
      struct value_type<C> {
        using type = typename C::value_type;
      };
      template <class C>
      requires
        !detail::MemberValueType<C> &&
        requires { typename reference_t<C>; }
      struct value_type<C> {
        using type = decay_t<reference_t<C>>;
      };
      template <class C>
      requires
        Same<typename value_type<C>::type, decay_t<typename value_type<C>::type>>()
      using value_type_t = typename value_type<C>::type;

      template <class C>
      requires
        requires(C& c) { c.read(); }
      static constexpr reference_t<C> read(C& c)
      STL2_NOEXCEPT_RETURN(c.read())

      template <class C>
      requires
        requires(C& c) { c.arrow(); }
      static constexpr decltype(auto) arrow(C& c)
      STL2_NOEXCEPT_RETURN(c.arrow())

      template <class C, class T>
      requires
        requires(C& c, T&& t) { c.write(forward<T>(t)); }
      static constexpr void write(C& c, T&& t)
      STL2_NOEXCEPT_RETURN((void)c.write(forward<T>(t)))

      template <class C>
      requires
        requires(C& c) { c.next(); }
      static constexpr void next(C& c)
      STL2_NOEXCEPT_RETURN((void)c.next())

      template <class C>
      requires
        requires(C& c) { c.prev(); }
      static constexpr void prev(C& c)
      STL2_NOEXCEPT_RETURN((void)c.prev())

      template <class C, class Other>
      requires
        requires(const C& lhs, const Other& rhs) {
          { lhs.equal(rhs) } -> bool;
        }
      static constexpr bool equal(const C& lhs, const Other& rhs)
      STL2_NOEXCEPT_RETURN(static_cast<bool>(lhs.equal(rhs)))

      template <class C>
      requires
        requires(C& c, difference_type_t<C> n) { c.advance(n); }
      static constexpr void advance(C& c, difference_type_t<C> n)
      STL2_NOEXCEPT_RETURN((void)c.advance(n))

      template <class C, class Other>
      requires
        requires(const C& lhs, const Other& rhs) {
          STL2_EXACT_TYPE_CONSTRAINT(lhs.distance_to(rhs), difference_type_t<C>);
        }
      static constexpr difference_type_t<C>
      distance(const C& lhs, const Other& rhs)
      STL2_NOEXCEPT_RETURN(lhs.distance_to(rhs))

      template <class C>
      requires
        requires(const C& c) { c.read(); }
      static constexpr decltype(auto) move(const C& c)
      STL2_NOEXCEPT_RETURN(move(c.read()))

      template <class C>
      requires
        requires(const C& c) { c.read(); c.move(); }
      static constexpr decltype(auto) move(const C& c)
      STL2_NOEXCEPT_RETURN(c.move())

      template <class C>
      using rvalue_reference_t = decltype(access::move(declval<const C&>()));

      template <class I>
      requires
        requires(I&& i) { STL2_DEDUCE_AUTO_REF_REF(((I&&)i).pos()); }
      static constexpr auto&& cursor(I&& i)
      STL2_NOEXCEPT_RETURN(forward<I>(i).pos())
    };  // class access
```

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
