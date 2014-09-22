#! /bin/awk -f

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
