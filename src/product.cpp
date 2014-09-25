/*
 * Takes an arbitrary number of sets as input and calculates the Cartesian
 * product of those sets.
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

#include <iostream>

#include "common.hpp" // cartesian_product

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

int main(int argc, char *argv[])
{
    Generator gen = read_sets();
    Product dummy;
    cartesian_product(gen, dummy, true);

    return 0;
}
