#ifndef DOCUMENT
#define DOCUMENT

#include <cstddef> 
#include <string>
#include <vector>
#include <optional>
#include <filesystem>


struct Line {
    std::string text;
    std::string ending;
};

struct Document {
    std::vector<Line> lines{{}};
    std::filesystem::path path;

    std::string Open(const std::filesystem::path &filename);
    std::string Save(const std::filesystem::path &filename);

    void Split(std::size_t row, std::size_t column);
    void Join(std::size_t row);
    void DeleteLine(std::size_t row);

    private:
        std::string newline_ = "\n";
        std::optional<stdd::filesystem::file_time_type> last_write_;
};

#endif