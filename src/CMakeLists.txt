include_directories(
    ${PROJECT_SOURCE_DIR}/v1
    ${PROJECT_SOURCE_DIR}/external
    ${PROJECT_SOURCE_DIR}/external/bleu/v1
    )

add_library(
    malkt
    utkernel/inbox/inbox.c
    utkernel/memory_pool/memory_pool.c
    utkernel/task/task.c
    ${PROJECT_SOURCE_DIR}/external/bleu/src/heap/gcc/heap.c
    )

add_executable(
    gtests
    main.cpp
    utkernel/inbox/inbox_test.cpp
    utkernel/inbox/utkernel_mbx_spy.c
    utkernel/inbox/utkernel_mpl_spy.c
    utkernel/memory_pool/memory_pool_test.cpp
    utkernel/memory_pool/utkernel_mpf_spy.c
    utkernel/task/function_entry_spy.c
    utkernel/task/utkernel_tsk_spy.c
    utkernel/task/task_test.cpp
    utkernel/util/system_call_logger.c
    utkernel/util/system_call_logger_test.cpp
    utkernel/util/system_call_template.c
    utkernel/util/system_call_template_test.cpp
    )

add_dependencies(gtests googletest)

target_link_libraries(
    gtests
    googletest
    malkt
    pthread
    )

include(CTest)

add_test(malkt_test ${PROJECT_BINARY_DIR}/gtests)
