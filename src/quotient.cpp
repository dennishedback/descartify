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

#include "common.hpp"

void read_tuples(Product &prod)
{
    Tuple current_tuple;
    std::string current_elem;

    for (std::istreambuf_iterator<char> it(std::cin), end; it != end; it++)
    {
        char c = *it;

        if (c == ',' || c == '\n')
        {
            current_tuple.push_back(current_elem);
            current_elem.clear();

            if (c == '\n')
            {
                prod.insert(current_tuple);
                current_tuple.clear();
            }
        }
        else
        {
            current_elem += c;
        }
    }
}

void init_generator(Generator &gen, size_t size)
{
    for (unsigned int i = 0; i < size; i++)
        gen.push_back(Set());   
}

bool product_contains(Product &prod, Tuple tuple)
{
    return prod.find(tuple) != prod.end();
}

void print_generator(Generator &gen)
{
        for (Generator::const_iterator gi = gen.begin(); gi != gen.end(); gi++)
        {
            for (Set::const_iterator si = gi->begin(); si != gi->end(); si++)
            {
                std::cout << *si;

                if (++si != gi->end())
                    std::cout << ',';

                si--;
            }

            std::cout << '\n';
        }

        std::cout << "%%\n";
}

void generating_sets(Product &prod)
{
    Product ref_prod = prod;

    while (prod.size() > 0)
    {
        Generator current_generator;
        init_generator(current_generator, prod.begin()->size());

        for (Product::iterator pi = prod.begin(); pi != prod.end();)
        {
            Tuple current_tuple = *pi;
            Generator tmp = current_generator;
            size_t tup_size = current_tuple.size();
              
            for (unsigned int i = 0; i < tup_size; i++)
            {
                tmp[i].insert(current_tuple[i]);
            }
   
            Product tmp_product;
            cartesian_product(tmp, tmp_product, false);

            for (Product::const_iterator tmpi = tmp_product.begin(); tmpi != tmp_product.end(); tmpi++)
            {
                if (!product_contains(ref_prod, *tmpi))
                {
                    ++pi;
                    // FIXME: Go away ugly goto, go away!
                    goto foo;
                }
            }

            current_generator = tmp;
            prod.erase(pi++);

        // FIXME: Ugly "expected primary-expression before '}' token" fix 
        foo:
            if (1 == 2) break;
        }
        print_generator(current_generator);
    }
}

int main(int argc, char *argv[])
{
    Product prod;
    read_tuples(prod);
    generating_sets(prod);

    return 0;
}
