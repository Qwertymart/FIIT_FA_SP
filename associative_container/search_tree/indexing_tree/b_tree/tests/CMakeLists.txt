add_executable(
        mp_os_assctv_cntnr_srch_tr_indxng_tr_b_tr_tests
        b_tree_tests.cpp)

target_link_libraries(
        mp_os_assctv_cntnr_srch_tr_indxng_tr_b_tr_tests
        PRIVATE
        GTest::gtest_main
        GTest::gtest)
target_link_libraries(
        mp_os_assctv_cntnr_srch_tr_indxng_tr_b_tr_tests
        PRIVATE
        mp_os_lggr_clnt_lggr)
target_link_libraries(
        mp_os_assctv_cntnr_srch_tr_indxng_tr_b_tr_tests
        PRIVATE
        mp_os_assctv_cntnr_srch_tr_indxng_tr_b_tr)