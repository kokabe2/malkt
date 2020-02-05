include_directories(
    ${PROJECT_SOURCE_DIR}/v1
    ${PROJECT_SOURCE_DIR}/external
    ${PROJECT_SOURCE_DIR}/external/bleu/v1
    )

add_library(
    malkt
    ${PROJECT_SOURCE_DIR}/external/bleu/src/heap/gcc/heap.c
    )

add_executable(
    gtests
    main.cpp
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
