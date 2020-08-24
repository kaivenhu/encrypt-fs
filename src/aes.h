#ifndef ENCFS_AES_H_
#define ENCFS_AES_H_
#include <array>
#include <cstdint>
#include <vector>

namespace encfs
{
namespace aes
{
class AES
{
    static const std::array<uint8_t, 256> S_BOX;
    static const std::array<uint32_t, 10> RCON;

    int round_;

    std::array<uint32_t, 44> round_key_ = {0};

    uint32_t SubWord(const uint32_t &word)
    {
        uint32_t x = 0;
        for (int i = 3; i >= 0; --i) {
            x |= S_BOX[static_cast<uint8_t>((word >> (8 * i)) & 0xFF)]
                 << (8 * i);
        }
        return x;
    }
    uint32_t RotWord(const uint32_t &word, const uint32_t &shift)
    {
        return (word << shift) | (word >> (-shift & 31));
    }
    void KeyExpansion(const std::array<uint32_t, 4> &key);

public:
    AES(const std::vector<uint8_t> &input);
    ~AES() = default;
    AES(const AES &) = delete;
    AES(AES &&) = delete;
    AES &operator=(AES) = delete;

    std::vector<uint32_t> round_key() const
    {
        return std::vector<uint32_t>(round_key_.begin(), round_key_.end());
    };
};

}  // namespace aes
}  // namespace encfs

#endif /* ENCFS_AES_H_ */
