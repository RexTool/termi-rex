#ifndef DOCUMENT_HPP
#define DOCUMENT_HPP

#include <filesystem>
#include <string> 
#include <vector>

class NoteFile {
public:
    explicit NoteFile(std::filesystem::path path);

    bool load (std::string& note);
    bool save (const std::string& note);

private:
    std::filesystem::path path_;
    std::vector<bool> crlf_;
};

#endif