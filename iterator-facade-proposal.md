<table>
<tr>
  <td align="left">Doc. no.:</td>
  <td align="left">DnnnnR0</td>
</tr>
<tr>
  <td align="left">Date:</td>
  <td align="left">2015-12-15</td>
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

# Iterator Facade Library Proposal

>**Summary:** Proposes a library component for easily creating conforming iterators. Based on existing practice. Depends only on the C++17 working paper plus Concepts TS and Ranges TS. Breaks no existing code or ABI's. Public open-source implementation and test suite available. Draft wording provided.

## Table of Contents

*TBS*

## Introduction

### Problem

Iterators that conform to the requirements of the C++ standard library are tedious to write and difficult to write correctly. They are tedious to write because although they need only a few of core functions, they also require a larger number of subsidiary functions and other boilerplate. Conforming iterators are difficult to write correctly because each iterator category has a subtly differing set of requirements, making it all too easy to get subsidiary functions or other boilerplate wrong.

### Solution

A class template that is given a few implementation functions can generate the facade (i.e. public interface and private implementation) for a fully conforming iterator, including all boilerplate. Boost ```iterator_facade``` pioneered this approach and has been in wide use since 2001<sup>&lsqb;[1](#1)&rsqb;</sup>. It eases writing conforming iterators and has proven less error prone than hand-coded iterators. The generated iterator conforms to an extended set of requirements based on the C++98 iterator requirements. Others, such as Chandler Carruth's LLVM ```iterator_facade_base```<sup>&lsqb;[2](#2)&rsqb;</sup>, have provided similar classes inspired by the Boost library.

### Proposal

This paper proposes an iterator facade class template for the standard library, useful by any programmer (novice, experienced, or expert) wishing to easily create a conforming iterator. The proposal uses C++11/14/17 with concepts<sup>&lsqb;[3](#3)&rsqb;</sup> and ranges<sup>&lsqb;[4](#4)&rsqb;</sup> to allow straightforward specification and implementation, and to ensure that the generated iterator is actually conforming. The proposal breaks no existing code and breaks no existing ABI's. A public open-source implementation and test suite is available on GitHub<sup>&lsqb;[5](#5)&rsqb;</sup>.

The proposal is suitable for C++17 if C++17 includes concepts and ranges. Otherwise, the proposal is suitable for the Ranges TS. No other core language or library changes are required.

### History

A 2004 proposal<sup>&lsqb;[6](#6)&rsqb;</sup> based on Boost ```iterator_facade``` failed because it depended an iterator update proposal<sup>&lsqb;[7](#7)&rsqb;</sup> that failed because C++03 without concepts was not rich enough to express the necessary iterator requirements.

## Examples

*TBS*

## Design Decisions

#### Use Boost ```iterator_facade``` as a role model

This ensures that the proposal represents existing practice in widespread use.

#### Base the interface on features in the Concepts TS and the Ranges TS working paper

This eliminates the difficulties with specification that bedeviled the 2004 proposal. It allows an interface specification that ensures that the resulting iterator is actually conforming. It improves error message quality when a user makes a mistake.

#### Use Cursor mixins to supply implementation details

Cursor mixins have proven themselves useful time and again. That said, it's a curiously indirect way of defining an iterator's interface. The alternative of inheriting directly from the Cursor leads to interface pollution. Cursors are implementation details and they should stay hidden.

## Technical Specifications

*TBS*

## Acknowledgements

*TBS*

## References
[1]: https://www.boost.org/doc/libs/1_59_0/libs/iterator/doc/iterator_facade.html
[2]: https://github.com/llvm-mirror/llvm/blob/master/include/llvm/ADT/iterator.h
[3]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4553.pdf
[4]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4560.pdf
[5]: https://github.com/ericniebler/range-v3
[6]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2004/n1641.html
[7]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2004/n1640.html


&lsqb;<a name="1">1</a>&rsqb; David Abrahams, Jeremy Siek, Thomas Witt, [Boost Iterator Facade][1], 2003.

&lsqb;<a name="2">2</a>&rsqb; Chandler Carruth, [LLVM ```iterator_facade_base``` iterator.h][2], 2014.

&lsqb;<a name="3">3</a>&rsqb; Andrew Sutton, [N4553, Working Draft,
C++ extensions for Concepts][3], 2015.

&lsqb;<a name="4">4</a>&rsqb; Eric Niebler, Casey Carter, [N4650, Working Draft,
C++ Extensions for Ranges][4], 2015.

&lsqb;<a name="5">5</a>&rsqb; Eric Niebler, Casey Carter, [Experimental range library for C++11/14/17][5], 2015.

&lsqb;<a name="6">6</a>&rsqb; David Abrahams, Jeremy Siek, Thomas Witt, [N1641, Iterator Facade and Adaptor][6], 2004.

&lsqb;<a name="7">7</a>&rsqb; David Abrahams, Jeremy Siek, Thomas Witt, [N1640, New Iterator Concepts][7], 2004.

------
