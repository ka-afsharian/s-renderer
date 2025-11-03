add_executable(test_ECS "${PROJECT_SOURCE_DIR}/tests/ECS.cpp")
target_link_libraries(test_ECS PUBLIC engproj_core)
set(TEST_DIR "${PROJECT_SOURCE_DIR}/tests")
set_target_properties(test_ECS PROPERTIES
    OUTPUT_NAME "test_ECS" RUNTIME_OUTPUT_DIRECTORY "${TEST_DIR}/$<CONFIGURATION>"
)

add_executable(test_handle_manager "${PROJECT_SOURCE_DIR}/tests/handle_manager_test.cpp")
target_link_libraries(test_handle_manager PUBLIC engproj_core)
set_target_properties(test_handle_manager PROPERTIES
    OUTPUT_NAME "test_handle_manager" RUNTIME_OUTPUT_DIRECTORY "${TEST_DIR}/$<CONFIGURATION>"
)
