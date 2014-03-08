libxcoro
--------

libxcoro is a coroutine library that is intended to provide in C some semblance
of the GoLang environment. Namely, lightweight user-space cooperative threading
and communication channels between them. Unlike Go this is squarely intended at
high performance system-level programming that cares enough about memory
allocations, zero-copy wherever possible and the least amount of overhead even
at the expense of safety and ease of programming.

The library is built in layers to make it more understandable and to make each
part reviewable to ensure correctness. It should be possible to rip and replace
some of the parts in order to support OS compatibility.

Dependencies
------------

It is currently supported on modern Linux only and essentially expects
features that were only available as of late 2.6.x where x >= 30
This stems mostly from the user of epoll, timerfd and signalfd.
This can be replaced probably with libev but will mandate some change to the
API.

The coroutine core was taken from the lthreads library which is x86-32 and
x86-64 only but can be easily replaced with libcoro which has more support
across architectures.

Author
------

Baruch Even <baruch@ev-en.org>
