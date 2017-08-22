add_executable (base base.c)
target_link_libraries (base LINK_PUBLIC wire)

add_executable (echo_server echo_server.c utils.c)
target_link_libraries(echo_server LINK_PUBLIC wire)

add_executable (recurser recurser.c)
target_link_libraries (recurser LINK_PUBLIC wire)

add_executable(web web.c utils.c)
target_link_libraries (web LINK_PUBLIC wire)

add_executable(web_client web_client.c utils.c)
target_link_libraries (web_client LINK_PUBLIC wire)

add_executable(channel channel.c)
target_link_libraries (channel LINK_PUBLIC wire)

add_executable(waiters waiters.c)
target_link_libraries (waiters LINK_PUBLIC wire)

add_executable(bench bench.c)
target_link_libraries (bench LINK_PUBLIC wire)

add_executable(locks locks.c)
target_link_libraries (locks LINK_PUBLIC wire)

add_definitions(-D_GNU_SOURCE)
add_executable(asyncio asyncio.c)
target_link_libraries (asyncio LINK_PUBLIC wire)

add_executable(pool pool.c)
target_link_libraries (pool LINK_PUBLIC wire)

add_executable(wait wait.c)
target_link_libraries (wait LINK_PUBLIC wire)

add_executable(log_stdout log_stdout.c)
target_link_libraries (log_stdout LINK_PUBLIC wire)
