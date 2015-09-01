/*
 * Takes an arbitrary number of tuples as input  and calculates the fewest
 * possible groups of sets whose combined Cartesian product contains exactly
 * those tuples.
 *
 * Copyright (c) 2012, Dennis Hedback
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <algorithm>
#include <iostream>
#include <iterator>
#include "common.hpp"

// Reads tuples from input stream and puts them in prod.
void read_tuples(Product &prod)
{
    Tuple tup;
    std::string elem;
    // Read the input stream character by character
    for (std::istreambuf_iterator<char> it(std::cin), end; it != end; it++)
    {
        char c = *it;
        // Tuple elements are delimited by ',' and tuples are delimited by '\n'
        if (c == ',' || c == '\n')
        {
            // This is the end of the current element, push it into the tuple
            // and finalize
            tup.push_back(elem);
            elem.clear();

            if (c == '\n')
            {
                // This is the end of the current tuple, insert it into the
                // product and finalize
                prod.insert(tup);
                tup.clear();
            }
        }
        else
        {
            elem += c; // Append character to the current element
        }
    }
}

// Populates a generator FIXME: This could easily be done in the call to the
// generator's constructor instead
void init_generator(Generator &gen, size_t size)
{
    for (unsigned int i = 0; i < size; i++)
        gen.push_back(Set());
}

// Does product contains Tuple 'tup'?
bool product_contains(Product &prod, Tuple const &tup)
{
    return prod.find(tup) != prod.end();
}

// Prints a generator
void print_generator(Generator &gen)
{
        for (Generator::const_iterator gi = gen.begin(); gi != gen.end(); gi++)
        {
            for (Set::const_iterator si = gi->begin(); si != gi->end(); si++)
            {
                std::cout << *si;
                if (++si != gi->end()) // This is not the last element
                    std::cout << ','; // Delimit elements with ','
                si--;
            }
            std::cout << '\n'; // Sets are delimited with newline
        }
        std::cout << "%%\n"; // Generators are delimited with "%%"
}

// Inserts a tuple into a generator
void insert_tuple(Generator &gen, Tuple const &tup)
{
    // FIXME: Handle cases where tup.size() != gen.size()
    Generator::iterator set = gen.begin();
    Tuple::const_iterator elem = tup.begin();
    // Insert tup[n] into gen[n]
    for (; set != gen.end() && elem != tup.end(); set++, elem++)
    {
        set->insert(*elem);
    }
}

// Is one product a subset of another?
bool is_subset(Product &subset, Product &superset)
{
#ifdef USE_HTAB
// We're using boost::unordered_set so the std::includes algo won't work
    for (Product::const_iterator it = subset.begin(); it != subset.end(); it++)
    {
        if (!product_contains(superset, *it))
        {
            // If superset does not contain any given tuple of subset, then
            // subset is not a subset of superset
            return false;
        }
    }
    return true;
#else
// We're using std::set so let's use the std::includes algo
    return std::includes(superset.begin(), superset.end(), subset.begin(), subset.end());
#endif // USE_HTAB
}

// Calculate, and print, the groups of generating sets whose combined Cartesian
// products contain exactly the tuples in 'prod'
void generating_sets(Product &prod)
{
    Product ref_prod = prod; // Our reference
    // Remove a tuple from prod once it's been inserted into a generator, so
    // we'll continue until all tuples' been inserted
    while (prod.size() > 0)
    {
        Generator gen; // Current generator
        init_generator(gen, prod.begin()->size());
        // Iterate over all tuples in prod and try to insert them into gen
        for (Product::iterator prod_it = prod.begin(); prod_it != prod.end();)
        {
            Generator tmp_gen = gen; // Temporary copy of current generator
            Product tmp_prod; // Cartesian product of temporary generator
            // Insert the tuple into the temporary generator instead of the
            // current generator so we can discard the generator if its product
            // is not a subset of the input product
            insert_tuple(tmp_gen, *prod_it);
            // Calculate the Cartesian product of the current generator and put
            // it in tmp_prod
            cartesian_product(tmp_gen, tmp_prod, false);
            // If the temporary product is a subset of the input product, the
            // generator is kept and the tuple is deleted from the input
            // input product. The goal is that all generators' combined
            // Cartesian products contain exactly the tuples in the input
            // product. If the Cartesian product of one generator is _not_ a
            // subset of the input product, then this is not the case.
            if (is_subset(tmp_prod, ref_prod))
            {
                gen = tmp_gen;
                prod.erase(prod_it++);
            }
            else
            {
                ++prod_it;
            }
        }
        print_generator(gen); // Generator is complete, print it
    }
}

int main(int argc, char *argv[])
{
    Product prod;
    read_tuples(prod);
    generating_sets(prod);
    return 0;
}
