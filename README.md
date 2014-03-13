libwire
--------

[![Build Status](https://travis-ci.org/baruch/libwire.png?branch=master)](https://travis-ci.org/baruch/libwire)

libwire is a user-space threading library that is intended to provide in C some semblance
of the GoLang environment. Namely, lightweight user-space cooperative threading
and communication channels between them. Unlike Go this is squarely intended at
high performance system-level programming that cares enough about memory
allocations, zero-copy wherever possible and the least amount of overhead even
at the expense of safety and ease of programming.

A guiding light for this library is to not allocate memory by itself if at all possible
and to let the user completely manage the memory and the allocation. A memory pool is
provided for when that is needed but even that memory can be provided by the user as a
static array instead of mallocing each part.

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

The coroutine core uses libcoro to provide support for multiple architectures.

Author
------

Baruch Even <baruch@ev-en.org>
