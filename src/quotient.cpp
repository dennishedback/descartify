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
typedef std::set<Generator> Quotient;

struct Options
{
    bool verbose;
};

static Options opts;

#define VPRINT(x) if (opts.verbose) { std::cerr << x << std::endl; }

void read_tuples(Product &prod)
{
    VPRINT("Reading input...");
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

unsigned int product_cardinality(Generator &gen)
{
    unsigned int cardinality = 1;

    for (Generator::iterator it = gen.begin(); it != gen.end(); it++)
        cardinality *= it->size();

    return cardinality;
}

Product cartesian_product(Generator &gen)
{
    Product prod;
    size_t gen_size = gen.size();
    unsigned int counters[gen_size];

    for (unsigned int i = 0; i < gen_size; i++)
        counters[i] = 0;

    for (unsigned int i = 0; i < product_cardinality(gen); i++)
    {
        Tuple current_tuple;

        for (unsigned int j = 0; j < gen_size; j++)
        {
            Set current_set = gen[j];
            unsigned int k = 0;

            for (Set::iterator it = current_set.begin(); it != current_set.end(); k++, it++)
                if (k == counters[j])
                    current_tuple.push_back(*it);
        }
        
        prod.insert(current_tuple);

        for (unsigned int j = 0; j < gen_size; j++)
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
            for (Set::iterator si = gi->begin(); si != gi->end(); si++)
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

    for (unsigned int num_generators = 1, inserted = 0; prod.size() > 0; num_generators++)
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
   
            Product tmp_product = cartesian_product(tmp);

            for (Product::iterator tmpi = tmp_product.begin(); tmpi != tmp_product.end(); tmpi++)
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
            VPRINT(++inserted << " tuples inserted into " << num_generators << " generators ");
        // FIXME: Ugly "expected primary-expression before '}' token" fix 
        foo:
            if (1 == 2) break;
        }
        print_generator(current_generator);
    }
}

int main(int argc, char *argv[])
{
    opts.verbose = false;
    Product prod;
    read_tuples(prod);
    generating_sets(prod);

    return 0;
}
