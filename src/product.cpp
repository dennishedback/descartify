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

#include <iostream>
#include <iterator>
#include <set>
#include <string>
#include <vector>

typedef std::set<std::string> Set;
typedef std::vector<std::string> Tuple;
typedef std::set<Tuple> Product;
typedef std::vector<Set> Generator;

Generator read_sets(void)
{
    Generator gen;
    Set current_set;
    std::string current_elem;

    for (std::istreambuf_iterator<char> it(std::cin), end; it != end; it++)
    {
        char c = *it;

        if (c == ',' || c == '\n')
        {
            current_set.insert(current_elem);
            current_elem.clear();

            if (c == '\n')
            {
                gen.push_back(current_set);
                current_set.clear();
            }
        }
        else
        {
            current_elem += c;
        }
    }

    return gen;
}

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

struct Counter {
    Set::const_iterator begin;
    Set::const_iterator end;
    Set::const_iterator current;
};

typedef std::vector<Counter> Counters;

void init_counters(Counters &ctrs, Generator &gen)
{
    for (Generator::const_iterator set = gen.begin(); set != gen.end(); set++)
    {
        Counter ctr = {
            set->begin(),
            set->end(),
            set->begin()
        };
        ctrs.push_back(ctr);
    }
}

void populate_tuple(Tuple &tup, Counters &ctrs)
{
    for(Counters::const_iterator ctr = ctrs.begin(); ctr != ctrs.end(); ctr++)
    {
        tup.push_back(*(ctr->current));   
    }
}

int increment_counters(Counters &ctrs)
{
    for (Counters::iterator ctr = ctrs.begin(); ; )
    {
        (ctr->current)++;
        if (ctr->current == ctr->end)
        {
            if (ctr+1 == ctrs.end())
            {
                return 1;
            }
            else
            {
                ctr->current = ctr->begin;
                ctr++;
            }
        }
        else
        {
            break;
        }
    }
    return 0;
}

void cartesian_product(Generator &gen)
{
    Counters ctrs;

    init_counters(ctrs, gen);

    while (true)
    {
        Tuple tup;
        
        populate_tuple(tup, ctrs);
        print_tuple(tup);

        if (increment_counters(ctrs) != 0)
            return;
    }
}

int main(int argc, char *argv[])
{
    Generator gen = read_sets();
    cartesian_product(gen);

    return 0;
}
