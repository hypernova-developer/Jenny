#include <iostream>
#include <string>
#include <vector>

class ProtocolX {
public:
    std::string encode(std::string t) {
        std::string r = "";
        for (unsigned char c : t) {
            std::string s = std::to_string((int)c);
            while (s.length() < 4) s += 'X';
            r += s;
        }
        return r;
    }

    std::string decode(std::string d) {
        std::string r = "";
        for (size_t i = 0; i < d.length(); i += 4) {
            std::string b = d.substr(i, 4);
            std::string n = "";
            for (char c : b) if (isdigit(c)) n += c;
            if (!n.empty()) r += (char)std::stoi(n);
        }
        return r;
    }
};

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cout << "Usage: protocolx --encode/--decode \"message\"" << std::endl;
        return 1;
    }

    ProtocolX px;
    std::string mode = argv[1];
    std::string input = argv[2];

    if (mode == "--encode") {
        std::cout << "Encoded Output: " << px.encode(input) << std::endl;
    } else if (mode == "--decode") {
        std::cout << "Decoded Output: " << px.decode(input) << std::endl;
    } else {
        std::cerr << "Error: Invalid mode selection." << std::endl;
        return 1;
    }

    return 0;
}
