add_executable(
        mp_os_lggr_clnt_lggr_tests
        client_logger_tests.cpp
)

# Подключаем GTest
target_link_libraries(
        mp_os_lggr_clnt_lggr_tests
        PRIVATE
        GTest::gtest
        GTest::gtest_main
)


# Подключаем клиентский логгер
target_link_libraries(
        mp_os_lggr_clnt_lggr_tests
        PUBLIC
        mp_os_lggr_clnt_lggr
)

# Устанавливаем директорию для тестов
target_include_directories(
        mp_os_lggr_clnt_lggr_tests
        PRIVATE
        ${GTest_INCLUDE_DIRS}
)
