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

unsigned int product_cardinality(Generator gen)
{
    unsigned int cardinality = 1;

    for (Generator::iterator it = gen.begin(); it != gen.end(); it++)
        cardinality *= (*it).size();

    return cardinality;
}

Product cartesian_product(Generator gen)
{
    Product prod;
    unsigned int counters[gen.size()];

    for (unsigned int i = 0; i < gen.size(); i++)
        counters[i] = 0;

    for (unsigned int i = 0; i < product_cardinality(gen); i++)
    {
        Tuple current_tuple;

        for (unsigned int j = 0; j < gen.size(); j++)
        {
            Set current_set = gen[j];
            unsigned int k = 0;

            for (Set::iterator it = current_set.begin(); it != current_set.end(); k++, it++)
                if (k == counters[j])
                    current_tuple.push_back(*it);
        }
        
        prod.insert(current_tuple);

        for (unsigned int j = 0; j < gen.size(); j++)
        {
            counters[j]++;
            if (counters[j] > gen[j].size() - 1)
                counters[j] = 0;
            else
                break;
        }
    }

    return prod;
}

void print_product(Product prod)
{
    for (Product::iterator pi = prod.begin(); pi != prod.end(); pi++)
    {
        for (Tuple::const_iterator ti = (*pi).begin(); ti != (*pi).end(); ti++)
        {
            std::cout << *ti;

            if (++ti != (*pi).end())
                std::cout << ',';

            ti--;
        }

        std::cout << std::endl;
    }
}

int main(int argc, char *argv[])
{
    Generator gen = read_sets();
    Product prod = cartesian_product(gen);
    print_product(prod);

    return 0;
}
