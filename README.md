# fastdiv

fastdiv is a snippet of C language code that can be used for improving
performance of division or modulo by rarely varying or non-compile-time
invariant integers. For compile-time invariant integers, the C compiler is most
likely already doing similar tricks.

The benefit is that modulo operation is about 1.3 ns instead of 2.3 ns on a
typical computer. Division performace improvement is even better: 1.0 ns vs 2.4
ns.

The drawback is that initializing the context to calculate modulo by a
particular integer takes about 3.2 ns. So, for modulo, 1.3 ns + 3.2 ns = 4.5
ns, slower than 2.3 ns for ordinary modulo.

The same is true for division: 1.0 ns + 3.2 ns = 4.2 ns, slower than 2.4 ns as
required for ordinary division.

The optimization thus makes sense only if multiple numbers are divided by the
same integer.

Applications would be expected to include non-power-of-2-sized hash tables and
fast generation of random numbers from an uniform integer distribution.

Note that currently only uint32\_t by uint32\_t division is supported, and the
divisor must actually fit into int32\_t, so highest bit must be zero.

# References

For the academic paper reference, see:
https://gmplib.org/~tege/divcnst-pldi94.pdf

Or directly from ACM: https://dl.acm.org/citation.cfm?id=178249

T. Granlund, and P. L. Montgomery, "Division by invariant integers using
multiplication," ACM SIGPLAN Notices, vol. 29, no. 6, pp. 61-72, 1994.
