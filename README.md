libwire    {#mainpage}
========

[![Build Status](https://travis-ci.org/baruch/libwire.png?branch=master)](https://travis-ci.org/baruch/libwire)
[![Coverage Status](https://coveralls.io/repos/baruch/libwire/badge.png?branch=master)](https://coveralls.io/r/baruch/libwire?branch=master)

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

Layering
========

libcoro
-------

The underlying layer used to implement the entire library is libcoro which is
used to switch between the wires. libcoro is very very simplistic and only
knows how to create the context switch area and to switch between two different
contexts. It is portable and can work on almost any environment.

wire
----

The first libwire layer is that of the user-space threads themselves. It deals
with settings things up easily and with suspend/resume. It is a fairly thin
layer on top of the coroutine switching layer of libcoro.

    void wire_thread_init(wire_thread_t *wire);
    void wire_thread_run(void);
    wire_t *wire_init(wire_t *wire, const char *name, void (*entry_point)(void *), void *arg, void *stack, unsigned stack_size);
    void wire_yield(void);
    void wire_suspend(void);
    void wire_resume(wire_t *wire);

wire_stack
----------

The wire_stack provides a simple way to allocate safe stacks of page size
multiples with a page of guard space around them. This helps catching stack
overflow bugs easily. It also provides a sigsegv handler to catch and report
such failures in a simple to understand way to quickly home on the problematic
wire.

    void *wire_stack_alloc(unsigned stack_size);
    #define WIRE_STACK_ALLOC(size) wire_stack_alloc(size), size
    void wire_stack_fault_detector_install(void);

wire_pool
---------

A wire_pool is used to easily provide a pool of wires that can be used for some
task. These wires will release their wire state to the pool upon completion and
make it easier to have a pool of workers. The wires are released in LIFO order
to try and have the memory as hot in the cache as possible.

    int wire_pool_init(wire_pool_t *pool, wire_pool_entry_t *entries, unsigned size, unsigned stack_size);
    wire_t *wire_pool_alloc(wire_pool_t *pool, const char *name, void (*entry_point)(void*), void *arg);
    wire_t *wire_pool_alloc_block(wire_pool_t *pool, const char *name, void (*entry_point)(void*), void *arg);

wire_wait
---------

The next layer (wire_wait) is that of cleanly suspending and resuming upon a
wakeup call from another wire. This is intended as a simple way to wait for
multiple sources such as waiting on a socket with a timeout, in such a case you
want to wait for the first of socket readable or timeout expired wakeups. The
wakeups do not carry any data beyond the wakeup itself.

    void wire_wait_init(wire_wait_t *w);
    void wire_wait_resume(wire_wait_t *w);
    void wire_wait_reset(wire_wait_t *w);
    void wire_wait_stop(wire_wait_t *w);
    void wire_wait_single(wire_wait_t *w);

    void wire_wait_list_init(wire_wait_list_t *wl);
    void wire_wait_chain(wire_wait_list_t *wl, wire_wait_t *w);
    void wire_wait_unchain(wire_wait_t *w);
    wire_wait_t *wire_list_wait(wire_wait_list_t *wl);

wire_channel
------------

A channel is a construct on top wire_wait which is used to send data between
wires. The data is only a void pointer so it can hold anything in it and the
sender will block until the data is received and processed by the receiver.
This provides both data passing and synchronization.

    void wire_channel_init(wire_channel_t *c);
    void wire_channel_send(wire_channel_t *c, void *msg);
    int wire_channel_recv_block(wire_channel_t *c, void **msg);
    int wire_channel_recv_nonblock(wire_channel_t *c, void **msg);
    void wire_channel_recv_wait(wire_channel_t *c, wire_channel_receiver_t *receiver, wire_wait_t *wait);

wire_fd
-------

wire_fd is using epoll to make a wire wait for a file descriptor to become
readable or writable. It also provides basic timers by using timerfd. This part
is Linux specific but can be ported by using the appropriate feature of the
host OS (BSD kqueue and such).

    void wire_fd_init(void);
    void wire_fd_mode_init(wire_fd_state_t *state, int fd);
    int wire_fd_mode_read(wire_fd_state_t *fd_state);
    int wire_fd_mode_write(wire_fd_state_t *fd_state);
    int wire_fd_mode_none(wire_fd_state_t *fd_state);
    void wire_fd_wait(wire_fd_state_t *fd_state);
    void wire_fd_wait_list_chain(wire_wait_list_t *wl, wire_fd_state_t *fd_state);
    int wire_fd_wait_msec(int msecs);

wire_io
-------

wire_io provides a way to make non-async blocking system calls into an async
operation by delegating it to a thread that will perform it and block and then
return the reply through a socket back to the thread running the wires. This
uses pthread mutexes and conditions and may block but hopefully contention on
the locks will be minimal.

    void wire_io_init(int num_threads);
    int wio_open(const char *pathname, int flags, mode_t mode);
    int wio_close(int fd);
    ssize_t wio_pread(int fd, void *buf, size_t count, off_t offset);
    ssize_t wio_pwrite(int fd, const void *buf, size_t count, off_t offset);
    int wio_fstat(int fd, struct stat *buf);
    int wio_ftruncate(int fd, off_t length);
    int wio_fallocate(int fd, int mode, off_t offset, off_t len);
    int wio_fsync(int fd);

wire_lock
---------

wire_lock provides a way to lock a shared resource between the wires. This is
rarely needed but one such example is multiple senders over a TCP socket where
each can block but mostly won't. The lock is very low cost if no blocking
happens (only a few integer operations) and will suspend the wire and be
completely fair if blocking is needed.

    void wire_lock_init(wire_lock_t *l);
    void wire_lock_take(wire_lock_t *l);
    void wire_lock_release(wire_lock_t *l);
    void wire_lock_wait_clear(wire_lock_t *l);

There is no smart scheduling done, the scheduling is completely FIFO based and
is handled by the core wire layer. Also, only a single thread is assumed to run
the wires even though there is a vestige or two of a multi-thread assumption as
well, this can later be added by using Thread Local Storage to reduce the need
to pass the wire_thread construct around.

Dependencies
============

It is currently supported on modern Linux only and essentially expects
features that were only available as of late 2.6.x where x >= 30
This stems mostly from the user of epoll, timerfd and signalfd.
This can be replaced probably with libev but will mandate some change to the
API.

The coroutine core uses libcoro to provide support for multiple architectures.

Author
======

Baruch Even <baruch@ev-en.org>
