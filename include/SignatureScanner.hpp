#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include <cstdint>
using namespace std;

static constexpr size_t BUFFER_SIZE = 4096;
class SignatureScanner {
    vector<uint8_t> signature;
    vector<int> lps;


public:
    void set_signature(const vector<uint8_t>& sig) {
        signature = sig;
        lps = build_lps();
    }

    bool contains_signature(const string& filepath) const {
        if (signature.empty()) return false;
        ifstream file(filepath, ios::binary);
        if (!file) return false;

        vector<uint8_t> buffer(BUFFER_SIZE);
        size_t j = 0;

        while (file) {
            file.read(reinterpret_cast<char*>(buffer.data()), BUFFER_SIZE);
            streamsize bytes_read = file.gcount();
            if (bytes_read <= 0) 
                break;

            for (int i = 0; i < bytes_read; ++i) {
                while (j > 0 && buffer[i] != signature[j]) {
                    j = lps[j - 1];
                }
                if (buffer[i] == signature[j]) {
                    ++j;
                    if (j == signature.size()) {
                        return true;
                    }
                }
            }
        }
        return false;
    }

private:
    vector<int> build_lps() {
        int sig_size = signature.size();
        vector<int> lps(sig_size, 0);
        int len = 0;
        int i = 1;
        while (i < sig_size) {
            if (signature[i] == signature[len]) {
                lps[i++] = ++len;
            } else if (len > 0) {
                len = lps[len - 1];
            } else {
                lps[i++] = 0;
            }
        }
        return lps;
    }
};
