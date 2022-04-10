set(DynamoRIO_DIR ${CMAKE_SOURCE_DIR}/DynamoRIO/cmake)
find_package(DynamoRIO)
if (NOT DynamoRIO_FOUND)
    message(FATAL_ERROR "DynamoRIO package required to build")
endif()

add_library(tracer SHARED tracer.c wrappers.c sockinfo.c)
configure_DynamoRIO_client(tracer)
use_DynamoRIO_extension(tracer drwrap drmgr drcontainers)

add_custom_target(run
                  COMMAND ${CMAKE_SOURCE_DIR}/DynamoRIO/bin64/drrun 
                          -c ${CMAKE_SOURCE_DIR}/build/libtracer.so
                          -- curl mipt.ru
                  DEPENDS tracer)
