#include "cryptyScanner.hpp"

using namespace std;

//Load the signature file into a byte vector
vector<uint8_t> load_signature(const string& sig_path) {
    ifstream sig_file(sig_path, ios::binary);
    if (!sig_file) throw runtime_error("Failed to open signature file");

    vector<uint8_t> sig;
    char ch;
    while (sig_file.get(ch)) {
        sig.push_back(static_cast<uint8_t>(ch));
    }
    return sig;
}

//file format structure
struct Format {
    MagicIdentifier id;
    string  name;
};

//Main function to scan files in a directory
int run(const string& root_dir, const string& sig_path) {
    namespace fs = filesystem;

     if (int ret = invalidInput(root_dir, sig_path)) {
       return ret;
    }

    vector<uint8_t> signature;
    try {
        signature = load_signature(sig_path);
    } catch (const exception& e) {
        cerr << e.what() << "\n";
        return 1;
    }

    SignatureScanner scanner;
    scanner.set_signature(signature);

    vector<Format> formats = {
        { MagicIdentifier({0x7F, 'E', 'L', 'F'}), "ELF" },
    };

    cout << "Scanning started...\n";

    for (const auto& entry : fs::recursive_directory_iterator(root_dir)) {
        if (!entry.is_regular_file()) continue;
        const auto path = entry.path().string();

        for (const auto& fmt : formats) {
            if (!fmt.id.matches(path))
                continue;

            if (scanner.contains_signature(path)) {
                cout << fmt.name
                    << " File "
                    << path
                    << " is infected!\n";
            }
            break;
        }
    }

    return 0;
}

//Check if the input is valid
int invalidInput(const string& root_dir, const string& sig_path) {
    namespace fs = filesystem;
   if (!fs::exists(root_dir)) {
        cerr << "Provided path does not exist.\n";
        return 1;
    }
    if (!fs::is_regular_file(sig_path)) {
        cerr << "Provided signature file path is not a file.\n";
        return 1;
    }
    if (fs::is_empty(sig_path)) {
        cerr << "Provided signature file is empty.\n";
        return 1;
    }
    if (fs::is_empty(root_dir)) {
        cerr << "Provided directory is empty.\n";
        return 1;
    }
    if (!fs::is_directory(root_dir)) {
        cerr << "Provided path is not a directory.\n";
        return 1;
    }
    return 0;

}