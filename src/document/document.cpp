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

bool NoteFile::save(const std::string &note) {
    std::error_code error;
    const auto entry = fs::symlink_status(path_, error);
    if(error && error != std::errc::no_such_file_or_directory) return false;

    auto target = path_;
    if(fs::is_symlink(entry)) {
        target = fs::canonical(path_, error);
        if(error) return false;
    }

    const auto status = fs::status(target, error);
    if(error && error != std::errc::no_such_file_or_directory) return false;

    if(fs::exists(status)) {
        if(!fs::is_regular_file(status)) return false;

        std::fstream writable(target, std::ios::in | std::ios::out | std::ios::binary);
        if(!writable) return false;
    }

    const auto temporary = target.parent_path() / (".termi-rex-" + std::to_string(std::random_device{}()) + ".tmp");
    std::ofstream output(temporary, std::ios::binary | std::ios::noreplace);
    if(!output) return false;

    error.clear();
    if(fs::exists(status)){
        fs::permissions(temporary, status.permissions(), error);
    }
    std::vector<bool> writtenEndings;
    
    if(!error) {
        for (char ch : note) {
            if( ch == '\n') {
                const auto index = writtenEndings.size();
                const bool useCRLF = index < crlf_.size()
                    ? crlf_[index] : !crlf_.empty() && crlf_.back();

                writtenEndings.push_back(useCRLF);
                if (useCRLF) output.put('\r');
            }
            output.put(ch);
        }
    }
    output.close();

    if(output && !error) {
        fs::rename(temporary, target, error);
        if(!error) {
            crlf_ = std::move(writtenEndings);
            return true;
        }
    }

    fs::remove(temporary, error);
    return false;
}