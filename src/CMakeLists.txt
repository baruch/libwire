add_library (wire
    coro.c
    http_parser.c
    wire.c
    wire_channel.c
    wire_fd.c
    wire_io.c
    wire_io_gen.c.inc
    wire_lock.c
    wire_log.c
    wire_net.c
    wire_pool.c
    wire_semaphore.c
    wire_stack.c
    wire_timeout.c
    wire_wait.c
)
add_definitions(-D_GNU_SOURCE)
target_compile_options(wire PRIVATE -Wno-deprecated-declarations)
target_include_directories (wire PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)
target_link_libraries(wire PUBLIC pthread)
