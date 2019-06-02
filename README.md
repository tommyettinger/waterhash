# waterhash
Variant on Wang Yi's wyhash with 32-bit output, using at most 64-bit math

This is a pretty closely-related hash to wyhash and thus to mumhash, but waterhash does some things differently.
As such, it can pass SMHasher in full despite only "mumming" 128 bits at a time instead of the stronger 256 bits that wyhash uses.
It gets a fairly-impressive hash rate, considering it uses 32-bit inputs and outputs instead of 64-bit:

(Results use MSVC 2017 on Windows 7; machine is a laptop with a 6th-gen i7 mobile processor)

```
Bulk speed test - 262144-byte keys
Alignment  7 -  3.024 bytes/cycle - 8652.01 MiB/sec @ 3 ghz
Alignment  6 -  3.026 bytes/cycle - 8657.89 MiB/sec @ 3 ghz
Alignment  5 -  3.005 bytes/cycle - 8596.62 MiB/sec @ 3 ghz
Alignment  4 -  3.018 bytes/cycle - 8633.40 MiB/sec @ 3 ghz
Alignment  3 -  3.035 bytes/cycle - 8683.70 MiB/sec @ 3 ghz
Alignment  2 -  3.030 bytes/cycle - 8668.42 MiB/sec @ 3 ghz
Alignment  1 -  3.001 bytes/cycle - 8585.48 MiB/sec @ 3 ghz
Alignment  0 -  2.939 bytes/cycle - 8409.34 MiB/sec @ 3 ghz
Average      -  3.010 bytes/cycle - 8610.86 MiB/sec @ 3 ghz

Small key speed test -    1-byte keys -    25.12 cycles/hash
Small key speed test -    2-byte keys -    24.46 cycles/hash
Small key speed test -    3-byte keys -    25.68 cycles/hash
Small key speed test -    4-byte keys -    26.25 cycles/hash
Small key speed test -    5-byte keys -    27.57 cycles/hash
Small key speed test -    6-byte keys -    24.72 cycles/hash
Small key speed test -    7-byte keys -    24.73 cycles/hash
Small key speed test -    8-byte keys -    24.55 cycles/hash
Small key speed test -    9-byte keys -    26.16 cycles/hash
Small key speed test -   10-byte keys -    25.89 cycles/hash
Small key speed test -   11-byte keys -    26.57 cycles/hash
Small key speed test -   12-byte keys -    26.14 cycles/hash
Small key speed test -   13-byte keys -    26.57 cycles/hash
Small key speed test -   14-byte keys -    26.37 cycles/hash
Small key speed test -   15-byte keys -    25.96 cycles/hash
Small key speed test -   16-byte keys -    30.45 cycles/hash
Small key speed test -   17-byte keys -    34.44 cycles/hash
Small key speed test -   18-byte keys -    35.26 cycles/hash
Small key speed test -   19-byte keys -    35.44 cycles/hash
Small key speed test -   20-byte keys -    35.67 cycles/hash
Small key speed test -   21-byte keys -    35.85 cycles/hash
Small key speed test -   22-byte keys -    35.15 cycles/hash
Small key speed test -   23-byte keys -    34.87 cycles/hash
Small key speed test -   24-byte keys -    34.91 cycles/hash
Small key speed test -   25-byte keys -    36.29 cycles/hash
Small key speed test -   26-byte keys -    35.50 cycles/hash
Small key speed test -   27-byte keys -    35.61 cycles/hash
Small key speed test -   28-byte keys -    35.57 cycles/hash
Small key speed test -   29-byte keys -    36.59 cycles/hash
Small key speed test -   30-byte keys -    36.36 cycles/hash
Small key speed test -   31-byte keys -    36.42 cycles/hash
Average                                    30.681 cycles/hash
```

On my machine, this has almost identical bulk speed to the 32-bit Fast Positive Hash t1ha0_32le (it gets `3.037 bytes/cycle - 8689.51 MiB/sec @ 3 ghz`) and better small key speed
(it gets `37.794 cycles/hash`). It has better bulk speed and small key speed than 32-bit XXHash version 2 (it gets `2.508 bytes/cycle - 7176.47 MiB/sec @ 3 ghz` and `42.826 cycles/hash`);
I haven't tested XXHash 3.

The mum function is almost identical to a bit-reduced version of wyhash's mum, except that it subtracts the top 32 bits of a 64-bit multiplication from the bottom 32 bits
(I believe this is called the Fermat residue); wyhash uses xor instead. Neither should be expected to be equidistributed for an RNG, so waterhash does not act as a random
number generator. Several of the other operations  in the large `switch` block in wyhash have been edited for the smaller size and to pass SMHasher.

This algorithm is extremely sensitive to the choice of constants; I'd suggest not changing the `_waterp0` through `_waterp5` constants, which use the upper 31 bits of wyhash'
`_wyp0` through `_wyp5` constants and always set the LSB.

I hope this can be useful!

Credits
===

[Wang Yi and the contributors to wyhash](https://github.com/wangyi-fudan/wyhash) deserve the lion's share of credit for this project.
Thanks to my family as well for letting me rubber-duck debug this algorithm as I yammered for extended periods of time.
