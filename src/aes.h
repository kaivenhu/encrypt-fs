#ifndef ENCFS_AES_H_
#define ENCFS_AES_H_
#include <array>
#include <cstdint>
#include <vector>

namespace encfs
{
namespace aes
{
#define AES_TEXT_BYTES 16
#define AES_BYTES_PER_WORD 4
#define AES_BITS_PER_BYTE 8
#define AES_TEXT_WORDS (AES_TEXT_BYTES / AES_BYTES_PER_WORD)
#define AES_SBOX_BYTES 256
#define AES_RCON_WORDS 10
#define AES_SHIFT_BITS(i) (AES_BITS_PER_BYTE * (AES_BYTES_PER_WORD - 1 - i))

typedef std::array<uint32_t, AES_TEXT_WORDS> AES_BLOCK;
typedef std::vector<uint8_t> AES_TEXT;

class AES
{
    static const std::array<uint8_t, AES_SBOX_BYTES> S_BOX;
    static const std::array<uint8_t, AES_SBOX_BYTES> INV_S_BOX;
    static const std::array<uint32_t, AES_RCON_WORDS> RCON;

    int round_;

    std::array<uint32_t, 44> round_key_ = {0};

    uint8_t Word2Byte(const uint32_t &word, int idx)
    {
        return (word >> AES_SHIFT_BITS(idx)) & 0xFF;
    }

    uint32_t Byte2Word(const uint8_t &word, int idx)
    {
        return static_cast<uint32_t>(word) << AES_SHIFT_BITS(idx);
    }
    uint32_t SubWord(const uint32_t &word,
                     const std::array<uint8_t, AES_SBOX_BYTES> &box)
    {
        uint32_t x = 0;
        for (int i = 3; i >= 0; --i) {
            x |= box[static_cast<uint8_t>((word >> (8 * i)) & 0xFF)] << (8 * i);
        }
        return x;
    }
    uint32_t RotWord(const uint32_t &word, const uint32_t &shift)
    {
        return (word << shift) | (word >> (-shift & 31));
    }
    uint32_t InvRotWord(const uint32_t &word, const uint32_t &shift)
    {
        return (word >> shift) | (word << (-shift & 31));
    }
    uint32_t XTimes(const uint32_t &data)
    {
        uint32_t mask = data & 0x80808080;
        mask |= mask >> 1;
        mask |= mask >> 2;
        mask |= mask >> 4;
        return ((data << 1) & 0xFEFEFEFE) ^ (0x1B1B1B1B & mask);
    }
    AES_BLOCK TransformBlock(const AES_BLOCK &data) const;
    void KeyExpansion(const std::array<uint32_t, 4> &key);
    void AddRoundKey(AES_BLOCK &data, const int &r);
    void Substitute(AES_BLOCK &data);
    void InvSubstitute(AES_BLOCK &data);
    void ShiftRows(AES_BLOCK &data);
    void InvShiftRows(AES_BLOCK &data);
    void MixColumn(AES_BLOCK &data);
    void InvMixColumn(AES_BLOCK &data);

public:
    AES(const AES_TEXT &input);
    ~AES() = default;
    AES(const AES &) = delete;
    AES(AES &&) = delete;
    AES &operator=(AES) = delete;

    AES_TEXT Encrypt(const AES_TEXT &data);
    AES_TEXT Decrypt(const AES_TEXT &data);

    std::vector<uint32_t> round_key() const;
};

}  // namespace aes
}  // namespace encfs

#endif /* ENCFS_AES_H_ */
