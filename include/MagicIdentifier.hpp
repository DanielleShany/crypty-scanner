#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include <fstream>
#include <algorithm>

class MagicIdentifier {
    std::vector<uint8_t> magic;
    size_t offset;
public:
    // magic: sequence of bytes to match at offset
    // offset: where to look (default=0)
    MagicIdentifier(const std::vector<uint8_t>& _magic, size_t _offset = 0)
      : magic(_magic), offset(_offset) {}

    // checks if the file at filepath matches the magic number
    bool matches(const std::string& filepath) const {
        std::ifstream f(filepath, std::ios::binary);
        if (!f || magic.empty()) return false;

        f.seekg(offset, std::ios::beg);
        std::vector<uint8_t> buf(magic.size());
        f.read(reinterpret_cast<char*>(buf.data()), buf.size());
        if (f.gcount() != static_cast<std::streamsize>(buf.size())) return false;

        return std::equal(buf.begin(), buf.end(), magic.begin());
    }
};
