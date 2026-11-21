set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

add_executable(app main.cpp)

target_link_libraries(
    app
    PRIVATE
    ftxui::component
    ftxui::dom
    ftxui::screen
)
