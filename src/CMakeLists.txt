set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

add_executable(
    app 
    main.cpp 
    dom/editor_view.cpp
    document/document.cpp
)

target_link_libraries(
    app
    PRIVATE
    ftxui::component
    ftxui::dom
    ftxui::screen
)
