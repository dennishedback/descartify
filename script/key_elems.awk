#! /bin/awk -f
 
# Copyright (c) 2012, Dennis Hedback 
# All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
#   * Redistributions of source code must retain the above copyright notice,
#     this list of conditions and the following disclaimer.
#   * Redistributions in binary form must reproduce the above copyright
#     notice, this list of conditions and the following disclaimer in the
#     documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

BEGIN {
    key_elems_len = 0;
    for (i = 1; i < ARGC; i++) {
        if (ARGV[i] == "-m") {
            min = ARGV[i+1];
            i++;
        } else if (ARGV[i] == "-M") {
            max = ARGV[i+1];
            i++;
        } else {
            key_elems[key_elems_len++] = ARGV[i]
        }
    }
    FS = ",";
    ARGC = 1;
}

{
    tot = 0;
    for (i = 1; i <= NF; i++)
    {
        for (j = 0; j < key_elems_len; j++) {
            split(key_elems[j], a, ":")
            if (i == a[1] && $i == a[2]) {
                ++tot;    
            }
        }    
    }
    if (tot >= min && tot <= max) {
        print $0
    }
}

END {
}
