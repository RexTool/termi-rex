#include "document.hpp"

#include <fstream>
#include <iostream>
#include <string>

namespace fs = std::filesystem;

std::string Document::Open(const fs::path &filename) {
    Document next;
    next.path = filename;
    std::error_code error;

    auto entry = fs::symlink_status(filename, error);
    if(fs::exists(entry)) {
        if(!fs::is_regular_file(filename, error)){
            return "Expected a text file: " + filename.string();
        }

        std::ifstream file(filename, std:: ios::binary);

        next.last_write_ = fs::last_write_time(filename, error);
        if(!file || error) {
            return "Could not Open " + filename.string();
        }
        next.lines.clear();
        std::string next;

        while (std::getline(file, text)) {
            if(text.find('\0') != std::string::npos){
                return "Expected a text File: " + filename.string();
            }
            std::string ending;
            if(!file.eof()){
                ending = "\n";
                if(!text.empty() && text.back() == '\r') {
                    text.pop_back();
                    ending = "\r\n";
                }
                if(next.lines.empty()){
                    next.newline_ = ending;
                }
            }
            next.lines.push_back({
                std::move(text), std::move(ending)
            });
        }
        if(file.bad() || !file.eof()) {
            return "Could not read " + filename.string();
        }
        if(next.lines.empty()) {
            next.lines.emplace_back();
        }
    }else if (error && entry.type() != fs::file_type::not_found) {
        return "Could not open " + filename.string() + ": " + error.message();
    }
    *this = std::move(next);
    return {};
}