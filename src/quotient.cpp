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

Product read_tuples(void)
{
    Product prod;
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

    return prod;
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
    size_t gen_size;

    gen_size = gen.size();

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

Generator init_generator(size_t size)
{
    Generator gen;
    for (unsigned int i = 0; i < size; i++)
        gen.push_back(Set());   
    return gen;
}

Generator reference_generator(Product prod)
{
    Generator ref_generator;
    
    if (prod.size() > 0)
        ref_generator = init_generator((*prod.begin()).size());

    for (Product::iterator pi = prod.begin(); pi != prod.end(); pi++)
    {
        unsigned int i = 0;
        
        for (Tuple::const_iterator ti = (*pi).begin(); ti != (*pi).end(); i++, ti++)
        {
            ref_generator[i].insert(*ti);
        }
    }

    return ref_generator;
}

bool product_contains(Product prod, Tuple tuple)
{
    bool found = false;
    for (Product::iterator it = prod.begin(); it != prod.end(); it++)
        if ((*it) == tuple)
            found = true;
    return found;
}

Product difference(Product prod_1, Product prod_2)
{
    Product diff;

    for (Product::iterator pi_1 = prod_1.begin(); pi_1 != prod_1.end(); pi_1++)
        if (!product_contains(prod_2, (*pi_1)))
            diff.insert(*pi_1);

    return diff;
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

Quotient generating_sets(Product prod)
{
    Quotient quot;
    Generator ref_generator = reference_generator(prod);
    Product ref_product = cartesian_product(ref_generator);
    Product diff = difference(ref_product, prod);

    for (unsigned int num_generators = 1, inserted = 0; prod.size() > 0; num_generators++)
    {
        Generator current_generator = init_generator((*prod.begin()).size());
        Product to_erase;
        for (Product::iterator pi = prod.begin(); pi != prod.end();)
        {
            Tuple current_tuple = (*pi);
            Generator tmp = current_generator;
            size_t tup_size = current_tuple.size();
              
            for (unsigned int i = 0; i < tup_size; i++)
            {
                tmp[i].insert(current_tuple[i]);
            }
   
            Product tmp_product = cartesian_product(tmp);

            if (difference(tmp_product, diff).size() == product_cardinality(tmp)) {
                current_generator = tmp;
                prod.erase(pi++);
                VPRINT(++inserted << " tuples inserted into " << num_generators << " generators ");
            }
            else
            {
                ++pi;
            }
        }
        quot.insert(current_generator);
    }

    return quot;
}

void print_quotient(Quotient quot)
{
    for (Quotient::iterator qi = quot.begin(); qi != quot.end(); qi++)
    {    
        for (Generator::const_iterator gi = (*qi).begin(); gi != (*qi).end(); gi++)
        {
            for (Set::iterator si = (*gi).begin(); si != (*gi).end(); si++)
            {
                std::cout << *si;

                if (++si != (*gi).end())
                    std::cout << ',';

                si--;
            }

            std::cout << std::endl;
        }

        std::cout << "%%" << std::endl;
    }
}

int main(int argc, char *argv[])
{
    opts.verbose = true;
    Product prod = read_tuples();
    Quotient quot = generating_sets(prod);
    print_quotient(quot);

    return 0;
}
