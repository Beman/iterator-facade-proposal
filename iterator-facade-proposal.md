## Table of Contents

## Introduction

### Problem

Iterators that conform to the requirements of the C++ standard library are tedious to write and difficult to write correctly. They are tedious to write because although they need only a few of core functions, they also require a larger number of subsidiary functions and other boilerplate. Conforming iterators are difficult to write correctly because each iterator category has a subtly differing set of requirements, so it is all too easy to forget subsidiary functions or other boilerplate.

### Solution

A class template that is given a few implementation functions can generate the facade (i.e. public interface and private implementation) for a fully conforming iterator, including all boilerplate. Boost ```iterator_facade``` pioneered this approach and has been in wide use since 2001<sup>&lsqb;[1](#1)&rsqb;</sup>. It eases writing conforming iterators and has proven less error prone than hand-coded iterators. The generated iterator conforms to an extended set of requirements based on the C++98 iterator requirements. Others, such as Chandler Carruth's LLVM ```iterator_facade_base```<sup>&lsqb;[2](#2)&rsqb;</sup>, have provided similar classes inspired by the Boost library.

### Proposal

This paper proposes a modern iterator facade class template for the standard library. The proposal uses C++11/14/17 with concepts<sup>&lsqb;[3](#3)&rsqb;</sup> and ranges<sup>&lsqb;[4](#4)&rsqb;</sup> to allow straightforward specification and implementation. The proposal will break no existing code and break no existing ABI. A public open-source implementation and test suite is available on GitHub<sup>&lsqb;[5](#5)&rsqb;</sup>.

The proposal is suitable for C++17 if C++17 includes concepts and ranges. Otherwise, the proposal is suitable for the Ranges TS. No other core language or library changes are required.

### History

A 2004 proposal<sup>&lsqb;[6](#6)&rsqb;</sup> based on Boost iterator_facade failed because it depended an iterator update proposal<sup>&lsqb;[7](#7)&rsqb;</sup> that failed because C++03 without concepts was not rich enough to express the necessary iterator requirements.

## Motivation and Scope

**Why is this important?**
+ It eases the task of creating standard library conforming iterators.

*What kinds of problems does it address? What is the intended user community?*

*What level of programmers (novice, experienced, expert) is it intended to support?* All levels.
*What existing practice is it based on? How widespread is its use? How long has it been in use? Is there a reference implementation and test suite available for inspection?*

## Examples

## Design Decisions


## Technical Specifications

## Acknowledgements

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
