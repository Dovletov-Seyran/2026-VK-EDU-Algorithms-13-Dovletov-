#include "Huffman.h"

#include <queue>
#include <stack>
#include <vector>

namespace {

class BitWriter {
public:
    BitWriter() : bitCount(0) {}

    void WriteBit(unsigned char bit) {
        if (bitCount % 8 == 0)
            buffer.push_back(0);
        if (bit)
            buffer[bitCount / 8] |= 1 << (7 - bitCount % 8);
        bitCount++;
    }

    void WriteByte(unsigned char value) {
        if (bitCount % 8 == 0) {
            buffer.push_back(value);
        } else {
            int offset = bitCount % 8;
            buffer[bitCount / 8] |= value >> offset;
            buffer.push_back(value << (8 - offset));
        }
        bitCount += 8;
    }

    const std::vector<unsigned char>& GetBuffer() const {
        return buffer;
    }
    size_t GetBitCount() const {
        return bitCount;
    }

private:
    std::vector<unsigned char> buffer;
    size_t bitCount;
};

class BitReader {
public:
    BitReader(IInputStream& in) : in(in), curByte(0), bitsLeft(0), eof(false) {
        readNextByte();
    }

    bool ReadBit(unsigned char& bit) {
        if (eof && bitsLeft == 0)
            return false;
        bit = (curByte >> 7) & 1;
        curByte <<= 1;
        bitsLeft--;
        if (bitsLeft == 0)
            readNextByte();
        return true;
    }

    bool ReadByte(unsigned char& value) {
        if (bitsLeft == 0 && eof)
            return false;

        if (bitsLeft == 8) {
            value = curByte;
            bitsLeft = 0;
            readNextByte();
            return true;
        }
        value = 0;
        for (int i = 0; i < 8; ++i) {
            unsigned char b;
            if (!ReadBit(b))
                return false;
            value = (value << 1) | b;
        }
        return true;
    }

private:
    void readNextByte() {
        byte b;
        if (in.Read(b)) {
            curByte = b;
            bitsLeft = 8;
        } else {
            eof = true;
            bitsLeft = 0;
        }
    }

    IInputStream& in;
    unsigned char curByte;
    int bitsLeft;
    bool eof;
};

struct HNode {
    unsigned char ch;
    size_t freq;
    HNode* left;
    HNode* right;

    HNode(unsigned char c, size_t f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
    HNode(HNode* l, HNode* r) : ch(0), freq(l->freq + r->freq), left(l), right(r) {}

    bool isLeaf() const {
        return !left && !right;
    }
};

void destroyTree(HNode* node) {
    if (!node)
        return;
    destroyTree(node->left);
    destroyTree(node->right);
    delete node;
}

struct HNodeCmp {
    bool operator()(HNode* a, HNode* b) const {
        return a->freq > b->freq;
    }
};

HNode* buildTree(const std::vector<size_t>& freq) {
    std::priority_queue<HNode*, std::vector<HNode*>, HNodeCmp> heap;
    for (int c = 0; c < 256; ++c) {
        if (freq[c] > 0)
            heap.push(new HNode((unsigned char)c, freq[c]));
    }

    if (heap.size() == 1) {
        HNode* only = heap.top();
        heap.pop();
        return new HNode(only, new HNode(only->ch, 0));
    }

    while (heap.size() > 1) {
        HNode* a = heap.top();
        heap.pop();
        HNode* b = heap.top();
        heap.pop();
        heap.push(new HNode(a, b));
    }

    return heap.top();
}

void buildCodes(HNode* node,
                std::vector<unsigned char>& path,
                std::vector<std::vector<unsigned char>>& codes) {
    if (!node)
        return;
    if (node->isLeaf()) {
        if (path.empty())
            codes[node->ch].push_back(0);
        else
            codes[node->ch] = path;
        return;
    }
    path.push_back(0);
    buildCodes(node->left, path, codes);
    path.back() = 1;
    buildCodes(node->right, path, codes);
    path.pop_back();
}

void writeTree(HNode* node, BitWriter& bw) {
    if (node->isLeaf()) {
        bw.WriteBit(1);
        bw.WriteByte(node->ch);
    } else {
        writeTree(node->left, bw);
        writeTree(node->right, bw);
        bw.WriteBit(0);
    }
}

HNode* readTree(BitReader& br, size_t uniqueSymbols) {
    std::stack<HNode*> stack;
    size_t leavesRead = 0;

    while (true) {
        if (leavesRead >= uniqueSymbols && stack.size() == 1)
            break;

        unsigned char bit;
        if (!br.ReadBit(bit))
            break;

        if (bit == 1) {
            unsigned char ch;
            br.ReadByte(ch);
            stack.push(new HNode(ch, 0));
            leavesRead++;
        } else {
            HNode* right = stack.top();
            stack.pop();
            HNode* left = stack.top();
            stack.pop();
            stack.push(new HNode(left, right));
        }
    }

    return stack.top();
}

}  // namespace

void Encode(IInputStream& original, IOutputStream& compressed) {
    std::vector<unsigned char> data;
    std::vector<size_t> freq(256, 0);
    {
        byte b;
        while (original.Read(b)) {
            data.push_back(b);
            freq[b]++;
        }
    }

    if (data.empty())
        return;

    HNode* root = buildTree(freq);
    std::vector<std::vector<unsigned char>> codes(256);
    {
        std::vector<unsigned char> path;
        buildCodes(root, path, codes);
    }

    size_t uniqueSymbols = 0;
    for (int c = 0; c < 256; ++c)
        if (freq[c] > 0)
            uniqueSymbols++;

    BitWriter bw;
    bw.WriteByte((unsigned char)(uniqueSymbols & 0xFF));
    writeTree(root, bw);

    for (unsigned char c : data) {
        const auto& code = codes[c];
        for (unsigned char bit : code)
            bw.WriteBit(bit);
    }

    size_t tailBits = bw.GetBitCount() % 8;
    if (tailBits == 0)
        tailBits = 8;

    while (bw.GetBitCount() % 8 != 0)
        bw.WriteBit(0);

    bw.WriteByte((unsigned char)tailBits);

    for (unsigned char b : bw.GetBuffer())
        compressed.Write(b);

    destroyTree(root);
}

void Decode(IInputStream& compressed, IOutputStream& original) {
    std::vector<unsigned char> archive;
    {
        byte b;
        while (compressed.Read(b))
            archive.push_back(b);
    }

    if (archive.empty())
        return;

    unsigned char tailBits = archive.back();
    archive.pop_back();

    if (archive.empty())
        return;

    struct VecInput : IInputStream {
        const std::vector<unsigned char>& v;
        size_t pos;
        VecInput(const std::vector<unsigned char>& v) : v(v), pos(0) {}
        bool Read(byte& value) override {
            if (pos >= v.size())
                return false;
            value = v[pos++];
            return true;
        }
    };

    VecInput vin(archive);
    BitReader br(vin);

    unsigned char uniqByte;
    br.ReadByte(uniqByte);
    size_t uniqueSymbols = uniqByte == 0 ? 256 : uniqByte;

    HNode* root = readTree(br, uniqueSymbols);

    size_t totalBits = (archive.size() - 1) * 8 + tailBits;
    size_t treeBits = uniqueSymbols == 1 ? 9 : uniqueSymbols * 9 + (uniqueSymbols - 1);
    size_t headerBits = 8 + treeBits;
    size_t dataBits = totalBits - headerBits;

    HNode* cur = root;
    size_t bitsRead = 0;
    while (bitsRead < dataBits) {
        unsigned char bit;
        if (!br.ReadBit(bit))
            break;
        bitsRead++;

        cur = (bit == 0) ? cur->left : cur->right;
        if (cur && cur->isLeaf()) {
            original.Write(cur->ch);
            cur = root;
        }
    }

    destroyTree(root);
}