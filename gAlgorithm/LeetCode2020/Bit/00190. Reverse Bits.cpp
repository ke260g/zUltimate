// Reverse bits of a given 32 bits unsigned integer.

// 1. 16bit 16bit 地翻转 ffff 0000 <> 0x0000 ffff
// 2. 8bit   8bit 地翻转 ff00 <> 00ff
// 3. 4bit   4bit 地翻转 f0 <> 0f
// 4. 2bit   2bit 地翻转 1100 <> 0011
// 5. 1bit   1bit 地翻转 1010 <> 0101
class Solution {
public:
    uint32_t reverseBits(uint32_t n) {
        n = ((n & 0x0000ffff) << 16) | ((n & 0xffff0000) >> 16);
        n = ((n & 0x00ff00ff) <<  8) | ((n & 0xff00ff00) >>  8);
        n = ((n & 0x0f0f0f0f) <<  4) | ((n & 0xf0f0f0f0) >>  4);
        n = ((n & 0x33333333) <<  2) | ((n & 0xcccccccc) >>  2);
        n = ((n & 0x55555555) <<  1) | ((n & 0xaaaaaaaa) >>  1);
        return n;
    }
};