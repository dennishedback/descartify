descartify
==========


Table of Contents
-----------------

1. New C++ implementation
2. What is descartify?
3. Building descartify
4. Using descartify
5. Implementation
6. Copying


New C++ implementation
----------------------

This repository used to house a Python module, but its algorithm turned out
to be broken. The new algorithm is very much slower, but more clear in terms
of code, so it was deemed necessary to reimplement it in C++.


What is descartify?
-------------------

descartify is a tool (two tools actually), to compute the Cartesian product of
an arbitrary number of input sets. It can also take an arbitrary number of
tuples and calculate the fewest possible groups of sets whose combined Cartesian
product contains exactly those tuples.

### Cartesian products

Given two sets A and B, their Cartesian product is defined as:

    A xx B = {(a,b) | a in A ^^^ b in B}

The product is a set of all tuples (a,b) where a is in A and b is in B. For
example:

    {1,2} xx {3,4} = {(1,3),(1,4),(2,3),(2,4)}

This operation can be performed on an arbitrary number of input sets:

    A xx B xx C xx ... = {(a,b,c,...) | a in A ^^^ b in B ^^^ c in C ^^^ ...}

#### Not commutative

A Cartesian product is not commutative. Changing the order of the operands
will change the result:

    A xx B != B xx A

#### Cardinality

The cardinality of a Cartesian product (i.e. the number of tuples outputted)
is the same as the product of the cardinalities of all input sets. If input
sets A and B each contain 2 elements, the cardinality of the product A xx B is:

    2 * 2 = 4

### Determining the groups of generating sets

Given a set of an arbitrary number of tuples, the fewest possible groups of
sets whose combined Cartesian product contains exactly those tuples, can be
calculated. For example:

    {(1,3),(1,4),(2,3),(2,4)} = {1,2} xx {3,4}

Let's say we remove one tuple. What is an accurate result then?

    {(1,3),(1,4),(2,3)}

Of course the original sets used to calculate the product is not an accurate
representation of this new set of tuples. Instead, we need to find the _groups_
of sets whose _combined_ Cartesian product contains exactly those tuples:

    {(1,2),(2,3), (1,4)} = {1,2} xx {3} uuu {1} xx {4}


Building descartify
-------------------

descartify utilizes no build system as of now. On a Linux system, do:

    $ cd ~/path/to/descartify_source
    $ g++ -O3 -Wall -o product src/product.cpp
    $ g++ -O3 -Wall -o quotient src/quotient.cpp


Using descartify from the command line
--------------------------------------

These instructions assume descartify was successfully built using the above
instructions.

`sets` is a file with the following contents:

    1,2,3
    4,5,6

Invoking `./product` with `sets` as input will then produce the Cartesian
product of the sets found in said file:

    $ ./product <sets
    1,4
    2,4
    3,4
    1,5
    2,5
    3,5
    1,6
    2,6
    3,6

Invoking `./quotient` on this output will recreate the original generating sets.
Beware that the time complexity of this program is O(n^n). A typical dataset of
mine will usually take 3 seconds on my hardware (1080 input tuples). Note that
each group of generating sets is delimited with a double-percent (%%):

    $ ./product <sets | ./quotient
    1,2,3
    4,5,6
    %%

As previously explained, if _one_ group of generating sets cannot exactly
represent the input tuples, cart-quot will determine the groups of generating
sets whose combined cartesian product contains exactly those tuples. If we
were to store the output of `./product <sets` in a file called `prod` and
then edit it (or otherwise filter the output, using grep, awk or some other
tool), so that it looks like this:

    # The tuple (2,4) was removed
    1,4
    3,4
    1,5
    2,5
    3,5
    1,6
    2,6
    3,6

`./quotient <prod` would then output the following:

    1,3
    4,5,6
    %%
    2
    5,6
    %%


Implementation
--------------

A description of the implementation will be provided once the programs are
deemed not-so-unstable.


Copying
-------

See file 'LICENSE'.
