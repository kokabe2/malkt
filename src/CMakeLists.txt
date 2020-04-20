include_directories(
    ${PROJECT_SOURCE_DIR}/v1
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/external
    ${PROJECT_SOURCE_DIR}/external/bleu/v1
    ${PROJECT_SOURCE_DIR}/external/bleu/src
    )

add_library(
    malkt
    inbox/utkernel/simple_inbox.c
    isr/utkernel/basic_isr.c
    memory_pool/utkernel/basic_memory_pool.c
    task/utkernel/objectizable_task.c
    task/utkernel/simple_task.c
    time/utkernel/time_unit.c
    timer/utkernel/interval_timer.c
    timer/utkernel/one_shot_timer.c
    uptime/utkernel/uptime.c
    ${PROJECT_SOURCE_DIR}/external/bleu/src/heap/gcc/heap.c
    ${PROJECT_SOURCE_DIR}/external/bleu/src/heap/heap_usage.c
    )

add_executable(
    gtests
    main.cpp
    inbox/utkernel/simple_inbox_test.cpp
    inbox/utkernel/utkernel_mbx_spy.c
    inbox/utkernel/utkernel_mpl_spy.c
    isr/utkernel/basic_isr_test.cpp
    isr/utkernel/utkernel_int_spy.c
    memory_pool/utkernel/basic_memory_pool_test.cpp
    memory_pool/utkernel/utkernel_mpf_spy.c
    task/utkernel/objectizable_task_test.cpp
    task/utkernel/simple_task_test.cpp
    task/utkernel/utkernel_tsk_spy.c
    time/utkernel/time_unit_test.cpp
    timer/utkernel/interval_timer_test.cpp
    timer/utkernel/one_shot_timer_test.cpp
    timer/utkernel/utkernel_cyc_spy.c
    uptime/utkernel/uptime_test.cpp
    uptime/utkernel/utkernel_tim_fake.c
    util/system_call_logger.c
    util/system_call_logger_test.cpp
    util/system_call_template.c
    util/system_call_template_test.cpp
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
