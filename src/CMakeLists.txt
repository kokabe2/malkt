include_directories(
    ${PROJECT_SOURCE_DIR}/v1
    ${PROJECT_SOURCE_DIR}/external
    ${PROJECT_SOURCE_DIR}/external/bleu/v1
    )

add_library(
    malkt
    time/utkernel/time_unit.c
    uptime/utkernel/uptime.c
    utkernel/inbox/inbox.c
    utkernel/isr/isr.c
    utkernel/memory_pool/memory_pool.c
    utkernel/task/task.c
    utkernel/timer/interval_timer.c
    utkernel/timer/one_shot_timer.c
    utkernel/timer/timer.c
    ${PROJECT_SOURCE_DIR}/external/bleu/src/heap/gcc/heap.c
    ${PROJECT_SOURCE_DIR}/external/bleu/src/heap/heap_usage.c
    )

add_executable(
    gtests
    main.cpp
    time/utkernel/time_unit_test.cpp
    uptime/utkernel/uptime_test.cpp
    uptime/utkernel/utkernel_tim_fake.c
    utkernel/inbox/inbox_test.cpp
    utkernel/inbox/utkernel_mbx_spy.c
    utkernel/inbox/utkernel_mpl_spy.c
    utkernel/isr/isr_test.cpp
    utkernel/isr/utkernel_int_spy.c
    utkernel/memory_pool/memory_pool_test.cpp
    utkernel/memory_pool/utkernel_mpf_spy.c
    utkernel/task/function_entry_spy.c
    utkernel/task/utkernel_tsk_spy.c
    utkernel/task/task_test.cpp
    utkernel/timer/interval_timer_test.cpp
    utkernel/timer/one_shot_timer_test.cpp
    utkernel/timer/timer_handler_spy.c
    utkernel/timer/utkernel_cyc_spy.c
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
