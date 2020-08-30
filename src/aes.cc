#include <algorithm>

#include "aes.h"

namespace encfs
{
namespace aes
{
using std::array;
using std::vector;

AES::AES(const AES_TEXT &input)
{
    array<uint32_t, 4> key = {0};
    for (decltype(input.size()) i = 0;
         i < std::min(input.size(), static_cast<decltype(input.size())>(16));
         ++i) {
        key[i / AES_BYTES_PER_WORD] |=
            Byte2Word(input[i], i % AES_BYTES_PER_WORD);
    }
    round_ = 44;
    KeyExpansion(key);
}

AES_TEXT AES::Encrypt(const AES_TEXT &data)
{
    int i = 0;
    AES_BLOCK input = {0};
    AES_TEXT output;
    for (decltype(data.size()) i = 0;
         i < std::min(data.size(), static_cast<decltype(data.size())>(
                                       AES_TEXT_WORDS * AES_BYTES_PER_WORD));
         ++i) {
        input[i / AES_BYTES_PER_WORD] |=
            Byte2Word(data[i], i % AES_BYTES_PER_WORD);
    }

    input = TransformBlock(input);
    AddRoundKey(input, 0);
    for (i = 1; i < (round_ / AES_TEXT_WORDS - 1); ++i) {
        Substitute(input);
        ShiftRows(input);
        MixColumn(input);
        AddRoundKey(input, i);
    }
    Substitute(input);
    ShiftRows(input);
    AddRoundKey(input, i);

    input = TransformBlock(input);
    for (decltype(data.size()) i = 0; i < data.size(); ++i) {
        output.push_back(
            Word2Byte(input[i / AES_BYTES_PER_WORD], i % AES_BYTES_PER_WORD));
    }
    return output;
}

AES_BLOCK AES::TransformBlock(const AES_BLOCK &data) const
{
    AES_BLOCK tmp = {0};
    for (int i = 0; i < AES_TEXT_WORDS; ++i) {
        uint32_t shift = AES_BITS_PER_BYTE * (AES_BYTES_PER_WORD - i - 1);
        for (int j = 0; j < AES_BYTES_PER_WORD; ++j) {
            tmp[i] |= ((data[j] >> shift) & 0xFF)
                      << (AES_BITS_PER_BYTE * (AES_BYTES_PER_WORD - j - 1));
        }
    }
    return tmp;
}

void AES::KeyExpansion(const array<uint32_t, 4> &key)
{
    // First round key.
    AES_BLOCK block = {0};
    decltype(round_key_) raw_round_key = {0};
    std::copy(key.begin(), key.end(), raw_round_key.begin());

    for (int i = 4; i < round_; ++i) {
        uint32_t temp = raw_round_key[i - 1];
        if (!(i % 4)) {
            temp = RotWord(temp, AES_BITS_PER_BYTE);
            temp = SubWord(temp);
            temp ^= RCON[(i / 4) - 1];
        }
        raw_round_key[i] = raw_round_key[i - 4] ^ temp;
    }
    for (int i = 0; i < (round_ / AES_TEXT_WORDS); ++i) {
        block = TransformBlock({raw_round_key[i * 4], raw_round_key[i * 4 + 1],
                                raw_round_key[i * 4 + 2],
                                raw_round_key[i * 4 + 3]});
        std::copy(block.begin(), block.end(), round_key_.begin() + (i * 4));
    }
}

vector<uint32_t> AES::round_key() const
{
    vector<uint32_t> v;
    for (int i = 0; i < (round_ / AES_TEXT_WORDS); ++i) {
        AES_BLOCK block =
            TransformBlock({round_key_[i * 4], round_key_[i * 4 + 1],
                            round_key_[i * 4 + 2], round_key_[i * 4 + 3]});
        for (const auto &x : block) {
            v.push_back(x);
        }
    }
    return v;
}

void AES::AddRoundKey(AES_BLOCK &data, const int &r)
{
    for (int i = 0; i < AES_TEXT_WORDS; ++i) {
        data[i] ^= round_key_[r * AES_TEXT_WORDS + i];
    }
}

void AES::Substitute(AES_BLOCK &data)
{
    for (int i = 0; i < AES_TEXT_WORDS; ++i) {
        data[i] = SubWord(data[i]);
    }
}

void AES::ShiftRows(AES_BLOCK &data)
{
    for (int i = 0; i < AES_TEXT_WORDS; ++i) {
        data[i] = RotWord(data[i], i * AES_BITS_PER_BYTE);
    }
}

void AES::MixColumn(array<uint32_t, 4> &data)
{
    AES_BLOCK a = data;
    AES_BLOCK b = {0};

    for (int i = 0; i < AES_TEXT_WORDS; ++i) {
        uint32_t mask = data[i] & 0x80808080;
        mask |= mask >> 1;
        mask |= mask >> 2;
        mask |= mask >> 4;
        b[i] = (data[i] << 1) & 0xFEFEFEFE;
        b[i] ^= 0x1B1B1B1B & mask;
    }
    data[0] = b[0] ^ a[3] ^ a[2] ^ b[1] ^ a[1];
    data[1] = b[1] ^ a[0] ^ a[3] ^ b[2] ^ a[2];
    data[2] = b[2] ^ a[1] ^ a[0] ^ b[3] ^ a[3];
    data[3] = b[3] ^ a[2] ^ a[1] ^ b[0] ^ a[0];
}
#define xtime(x)   ((x << 1) ^ (((x >> 7) & 1) * 0x1b))
#define Multiply(x,y)   ((y      & 0x01) * x) \
                      ^ ((y >> 1 & 0x01) * xtime(x)) \
                      ^ ((y >> 2 & 0x01) * xtime(xtime(x))) \
                      ^ ((y >> 3 & 0x01) * xtime(xtime(xtime(x)))) \
                      ^ ((y >> 4 & 0x01) * xtime(xtime(xtime(xtime(x)))))

void AES::InvMixColumn(array<uint8_t, 4> &data)
{
    array<uint8_t, 4> a = data;
    uint8_t tmp = data[0] ^ data[1] ^ data[2] ^ data[3];
    array<uint8_t, 4> m4 = {0};
    array<uint8_t, 4> m2 = {0};

    tmp ^= xtime(xtime(xtime(tmp)));
    for (int i = 0; i < 4; ++i) {
        m4[i] = xtime(xtime(data[i]));
        m2[i] = xtime(data[i]);
    }

    data[0] = tmp ^ a[0] ^ m4[0] ^ m4[2] ^ m2[0] ^ m2[1];
    data[1] = tmp ^ a[1] ^ m4[1] ^ m4[3] ^ m2[1] ^ m2[2];
    data[2] = tmp ^ a[2] ^ m4[2] ^ m4[0] ^ m2[2] ^ m2[3];
    data[3] = tmp ^ a[3] ^ m4[3] ^ m4[1] ^ m2[3] ^ m2[0];

    /*
    uint8_t d0, d1, d2, d3;
    d0 = data[0];
    d1 = data[1];
    d2 = data[2];
    d3 = data[3];

    data[0] = Multiply(d0, 0x0e) ^ Multiply(d1, 0x0b) ^ 
               Multiply(d2, 0x0d) ^ Multiply(d3, 0x09);
    data[1] = Multiply(d0, 0x09) ^ Multiply(d1, 0x0e) ^ 
               Multiply(d2, 0x0b) ^ Multiply(d3, 0x0d);
    data[2] = Multiply(d0, 0x0d) ^ Multiply(d1, 0x09) ^ 
               Multiply(d2, 0x0e) ^ Multiply(d3, 0x0b);
    data[3] = Multiply(d0, 0x0b) ^ Multiply(d1, 0x0d) ^ 
               Multiply(d2, 0x09) ^ Multiply(d3, 0x0e);
    */
}

const std::array<uint8_t, AES_SBOX_BYTES> AES::S_BOX = {
    // 0    1     2     3     4     5     6     7
    // 8    9     A     B     C     D     E     F
    0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5,
    0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,  // 0
    0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0,
    0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,  // 1
    0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC,
    0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,  // 2
    0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A,
    0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,  // 3
    0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0,
    0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,  // 4
    0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B,
    0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,  // 5
    0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85,
    0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,  // 6
    0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5,
    0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,  // 7
    0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17,
    0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,  // 8
    0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88,
    0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,  // 9
    0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C,
    0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,  // A
    0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9,
    0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,  // B
    0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6,
    0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,  // C
    0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E,
    0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,  // D
    0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94,
    0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,  // E
    0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68,
    0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16  // F
};

const std::array<uint32_t, AES_RCON_WORDS> AES::RCON = {
    0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000,
    0x20000000, 0x40000000, 0x80000000, 0x1B000000, 0x36000000};

}  // namespace aes
}  // namespace encfs
