add_executable(
        lemito
        meow_test_fraction.cpp
)

target_link_libraries(
        lemito
        PRIVATE
        GTest::gtest
        GTest::gtest_main)
target_link_libraries(
        lemito
        PRIVATE
        mp_os_lggr_clnt_lggr)
target_link_libraries(
        lemito
        PRIVATE
        mp_os_arthmtc_frctn)