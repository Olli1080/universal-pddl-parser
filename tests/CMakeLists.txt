include(CTest)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/52eb8108c5bdec04579160ae17225d66034bd723.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

file(COPY expected DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY "../domains" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)

add_executable(DomainTests DomainTests.cpp)
target_link_libraries(DomainTests
    PUBLIC
        parser
        GTest::gtest_main
)

target_compile_features(DomainTests PUBLIC cxx_std_20)
gtest_discover_tests(DomainTests)
#add_test(NAME DomainTests
#         COMMAND DomainTests)