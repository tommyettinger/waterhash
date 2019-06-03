# waterhash
Variant on Wang Yi's wyhash with 32-bit output, using at most 64-bit math

This is a pretty closely-related hash to wyhash and thus to mumhash, but waterhash does some things differently.
As such, it can pass SMHasher in full despite only "mumming" 128 bits at a time instead of the stronger 256 bits that wyhash uses.
It gets a fairly-impressive hash rate, considering it uses 32-bit inputs and outputs instead of 64-bit:

(Results use MSVC 2017 on Windows 7; machine is a laptop with a 6th-gen i7 mobile processor)

```
[[[ Speed Tests ]]]

Bulk speed test - 262144-byte keys
Alignment  7 -  2.943 bytes/cycle - 8421.17 MiB/sec @ 3 ghz
Alignment  6 -  2.943 bytes/cycle - 8419.29 MiB/sec @ 3 ghz
Alignment  5 -  2.932 bytes/cycle - 8387.41 MiB/sec @ 3 ghz
Alignment  4 -  2.945 bytes/cycle - 8424.37 MiB/sec @ 3 ghz
Alignment  3 -  2.953 bytes/cycle - 8448.38 MiB/sec @ 3 ghz
Alignment  2 -  2.957 bytes/cycle - 8460.25 MiB/sec @ 3 ghz
Alignment  1 -  2.946 bytes/cycle - 8428.90 MiB/sec @ 3 ghz
Alignment  0 -  2.944 bytes/cycle - 8423.08 MiB/sec @ 3 ghz
Average      -  2.945 bytes/cycle - 8426.61 MiB/sec @ 3 ghz

Small key speed test -    1-byte keys -    26.40 cycles/hash
Small key speed test -    2-byte keys -    28.17 cycles/hash
Small key speed test -    3-byte keys -    28.44 cycles/hash
Small key speed test -    4-byte keys -    28.29 cycles/hash
Small key speed test -    5-byte keys -    31.13 cycles/hash
Small key speed test -    6-byte keys -    26.00 cycles/hash
Small key speed test -    7-byte keys -    29.22 cycles/hash
Small key speed test -    8-byte keys -    27.27 cycles/hash
Small key speed test -    9-byte keys -    29.18 cycles/hash
Small key speed test -   10-byte keys -    29.24 cycles/hash
Small key speed test -   11-byte keys -    28.95 cycles/hash
Small key speed test -   12-byte keys -    29.13 cycles/hash
Small key speed test -   13-byte keys -    29.11 cycles/hash
Small key speed test -   14-byte keys -    29.23 cycles/hash
Small key speed test -   15-byte keys -    27.24 cycles/hash
Small key speed test -   16-byte keys -    30.89 cycles/hash
Small key speed test -   17-byte keys -    37.48 cycles/hash
Small key speed test -   18-byte keys -    37.33 cycles/hash
Small key speed test -   19-byte keys -    38.05 cycles/hash
Small key speed test -   20-byte keys -    38.19 cycles/hash
Small key speed test -   21-byte keys -    38.43 cycles/hash
Small key speed test -   22-byte keys -    38.18 cycles/hash
Small key speed test -   23-byte keys -    36.74 cycles/hash
Small key speed test -   24-byte keys -    36.63 cycles/hash
Small key speed test -   25-byte keys -    39.09 cycles/hash
Small key speed test -   26-byte keys -    37.71 cycles/hash
Small key speed test -   27-byte keys -    37.68 cycles/hash
Small key speed test -   28-byte keys -    39.22 cycles/hash
Small key speed test -   29-byte keys -    39.35 cycles/hash
Small key speed test -   30-byte keys -    39.23 cycles/hash
Small key speed test -   31-byte keys -    37.95 cycles/hash
Average                                    33.199 cycles/hash
```

On my machine, this has almost identical bulk speed to the 32-bit Fast Positive Hash t1ha0_32le (it gets `3.037 bytes/cycle - 8689.51 MiB/sec @ 3 ghz`) and better small key speed
(it gets `37.794 cycles/hash`). It has better bulk speed and small key speed than 32-bit XXHash version 2 (it gets `2.508 bytes/cycle - 7176.47 MiB/sec @ 3 ghz` and `42.826 cycles/hash`);
XXH3 isn't a terribly fair comparison when the same featureset is permitted (no SSE, no AVX); its bulk speed without those machine-dependent boosts is
`1.979 bytes/cycle - 5661.42 MiB/sec @ 3 ghz` and its small key speed remains excellent, `26.234 cycles/hash`.

The mum function is almost identical to a bit-reduced version of wyhash's mum, except that it subtracts the top 32 bits of a 64-bit multiplication from the bottom 32 bits
(I believe this is called the Fermat residue); wyhash uses xor instead. Neither should be expected to be equidistributed for an RNG, so waterhash does not act as a random
number generator. Several of the other operations  in the large `switch` block in wyhash have been edited for the smaller size and to pass SMHasher.

The first version of waterhash passed an older version of SMHasher but not the current one; as of this commit on June 2, 2019, waterhash passes both the older version and
the current version of SMHasher. The results produced by running waterhash through the current SMHasher are in `waterhash_results.txt`.

This algorithm is extremely sensitive to the choice of constants; I'd suggest not changing the `_waterp0` through `_waterp5` constants, which use the upper 31 bits of wyhash'
`_wyp0` through `_wyp5` constants and always set the LSB.

I hope this can be useful!

Credits
===

[Wang Yi and the contributors to wyhash](https://github.com/wangyi-fudan/wyhash) deserve the lion's share of credit for this project.
Thanks to my family as well for letting me rubber-duck debug this algorithm as I yammered for extended periods of time.
