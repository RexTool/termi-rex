#include "document.hpp"

#include <fstream>
#include <random>
#include <system_error>
#include <utility>

namespace fs = std::filesystem;

NoteFile::NoteFile(fs::path path) : path_(std::move(path)) {}

bool NoteFile::load(std::string &note){
    std::error_code error;
    const auto entry = fs::symlink_status(path_, error);

    if(error && error != std::errc::no_such_file_or_directory) return false;

    if(entry.type() == fs::file_type::not_found) {
        note.clear();
        crlf_.clear();
        return true;
    }
    
    if(!fs::is_regular_file(path_, error)) return false;

    std::ifstream input(path_, std::ios::binary);
    if(!input) return false;

    std::string contents;
    std::string line;
    std::vector<bool> endings;

    while(std::getline(input, line)) {
        if(line.contains('\0')) return false;

        if(!input.eof()) {
            endings.push_back(line.ends_with('\r'));
            if(endings.back()) line.pop_back();
            line.push_back('\n');
        }
        contents+=line;
    }
    if(!input.eof() || input.bad()) return false;

    note = std::move(contents);
    crlf_ = std::move(endings);
    return true;
}