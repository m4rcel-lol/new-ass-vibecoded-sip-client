# Basic unit tests
add_executable(test_sipengine test_sipengine.cpp)
target_link_libraries(test_sipengine
    Qt6::Test
    Qt6::Core
)
add_test(NAME test_sipengine COMMAND test_sipengine)

add_executable(test_audioengine test_audioengine.cpp)
target_link_libraries(test_audioengine
    Qt6::Test
    Qt6::Core
)
add_test(NAME test_audioengine COMMAND test_audioengine)
