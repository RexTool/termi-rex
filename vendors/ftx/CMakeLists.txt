include(FetchContent)

FetchContent_Declare(
    ftxui
    GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI
    GIT_TAG main  # Replace with a version, tag, or commit hash if need be for testing purpose.
)

FetchContent_MakeAvailable(ftxui)