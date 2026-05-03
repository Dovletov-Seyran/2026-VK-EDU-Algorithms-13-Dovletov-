#include <fstream>
#include <vector>

#include "Huffman.h"

class FileInput : public IInputStream {
    std::ifstream f;

public:
    FileInput(const char* p) : f(p, std::ios::binary) {}
    bool Read(byte& v) override {
        char c;
        if (!f.get(c))
            return false;
        v = (byte)c;
        return true;
    }
};

class FileOutput : public IOutputStream {
    std::ofstream f;

public:
    FileOutput(const char* p) : f(p, std::ios::binary) {}
    void Write(byte v) override {
        f.put((char)v);
    }
};

int main() {
    {
        FileInput in("input.txt");
        FileOutput out("compressed.bin");
        Encode(in, out);
    }
    {
        FileInput in("compressed.bin");
        FileOutput out("decoded.txt");
        Decode(in, out);
    }
    return 0;
}