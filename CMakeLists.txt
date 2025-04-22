cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

project(mp_os LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Установка путей vcpkg
set(VCPKG_ROOT "C:/Users/u/.vcpkg-clion/vcpkg")
set(VCPKG_INCLUDE_DIR "${VCPKG_ROOT}/installed/x64-windows/include")
set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")

message(STATUS "=== Начало конфигурации ===")
message(STATUS "VCPKG root: ${VCPKG_ROOT}")
message(STATUS "VCPKG include dir: ${VCPKG_INCLUDE_DIR}")

# Поиск зависимостей
find_package(Boost CONFIG REQUIRED COMPONENTS system container)
find_package(OpenSSL CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)
find_package(Asio CONFIG REQUIRED)
find_package(Crow CONFIG REQUIRED)
find_package(httplib CONFIG REQUIRED)  # Добавляем httplib

message(STATUS "Boost: ${Boost_FOUND} (${Boost_VERSION})")
message(STATUS "OpenSSL: ${OPENSSL_FOUND} (${OPENSSL_VERSION})")
message(STATUS "nlohmann_json: ${nlohmann_json_FOUND}")
message(STATUS "GTest: ${GTest_FOUND}")
message(STATUS "Asio: ${Asio_FOUND}")
message(STATUS "Crow: ${Crow_FOUND}")
message(STATUS "httplib: ${httplib_FOUND}")  # Проверяем httplib

# Добавление подпроектов
message(STATUS "=== Добавление подпроектов ===")
add_subdirectory(allocator)
add_subdirectory(arithmetic)
add_subdirectory(associative_container)
add_subdirectory(common)
add_subdirectory(logger)

message(STATUS "=== Конфигурация завершена ===")
