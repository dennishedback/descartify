/*
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

#include "common.hpp"

#include <iostream>
#include <vector>

struct Counter {
    Set::const_iterator begin;
    Set::const_iterator end;
    Set::const_iterator current;
};

typedef std::vector<Counter> Counters;

// Initalizes the counters with iterators from the sets contained in gen

void init_counters(Counters &ctrs, Generator &gen, size_t sz)
{
    ctrs = Counters(sz);

    unsigned int i = 0;
    // Initialize the counters
    for (Generator::const_iterator set = gen.begin(); set != gen.end(); set++, i++)
    {
        ctrs[i].begin = set->begin();
        ctrs[i].end = set->end();
        ctrs[i].current = ctrs[i].begin;
    }
}

// Populates a tuple with elements from an input set, according to the state of
// the counters provided in ctrs

void populate_tuple(Tuple &tup, Counters &ctrs, size_t sz)
{
    tup = Tuple(sz);

    unsigned int i = 0;
    for(Counters::const_iterator ctr = ctrs.begin(); ctr != ctrs.end(); ctr++, i++)
    {
        tup[i] = *(ctr->current);   
    }
}

// Increments the counters contained in ctrs

int increment_counters(Counters &ctrs)
{
    // Increment the first counter, then the second, etc.
    for (Counters::iterator ctr = ctrs.begin(); ; )
    {
        // Increment this counter.
        (ctr->current)++;
        if (ctr->current == ctr->end)
        {
            // The counter is about to roll over
            if ((ctr + 1) == ctrs.end())
            {
                // This is the last counter, and it is about to roll over, so
                // we're done here.
                return 1;
            }
            else
            {
                // This is not the last counter, so we reset it
                ctr->current = ctr->begin;
                ctr++;
            }
        }
        else
        {
            // If the counter is not about to roll over, we won't increment
            // any more counters for now
            break;
        }
    }

    // There's still more tuples to add to this Cartesian product
    return 0;
}

// Prints a tuple. Each element is delimited by ','.

void print_tuple(Tuple &tup)
{
    for (Tuple::const_iterator ti = tup.begin(); ti != tup.end(); ti++)
    {
        std::cout << *ti;

        if (++ti != tup.end())
            std::cout << ',';

        ti--;
    }

    std::cout << '\n';
}

// Calculates the Cartesian products of the sets contained in gen,
// and stores it in prod. Alternatively prints the product, tuple
// by tuple.

void cartesian_product(Generator &gen, Product &prod, bool print)
{
    Counters ctrs;
    size_t gen_size = gen.size();

    init_counters(ctrs, gen, gen_size);

    while (true)
    {
        Tuple tup;
        
        populate_tuple(tup, ctrs, gen_size);

        if (print)
            print_tuple(tup);
        else
            prod.insert(tup);

        if (increment_counters(ctrs) != 0) {
            // The last counter was about to roll over, we're done
            return;
        }
    }
}

