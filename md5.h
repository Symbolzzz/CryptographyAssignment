#include <iostream>
#include <string.h>
#include <cmath>
#include <vector>
#include <unordered_map>

using namespace std;

#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))                // F函数
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))                // G函数
#define H(x, y, z) ((x) ^ (y) ^ (z))                           // H函数
#define I(x, y, z) ((y) ^ ((x) | (~z)))                        // I函数
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32 - (n)))) // 32位数字x的循环左移n位操作

#define FF(a, b, c, d, x, s, ac)            \
    {                                       \
        (a) += F((b), (c), (d)) + (x) + ac; \
        (a) = ROTATE_LEFT((a), (s));        \
        (a) += (b);                         \
    }
#define GG(a, b, c, d, x, s, ac)            \
    {                                       \
        (a) += G((b), (c), (d)) + (x) + ac; \
        (a) = ROTATE_LEFT((a), (s));        \
        (a) += (b);                         \
    }
#define HH(a, b, c, d, x, s, ac)            \
    {                                       \
        (a) += H((b), (c), (d)) + (x) + ac; \
        (a) = ROTATE_LEFT((a), (s));        \
        (a) += (b);                         \
    }
#define II(a, b, c, d, x, s, ac)            \
    {                                       \
        (a) += I((b), (c), (d)) + (x) + ac; \
        (a) = ROTATE_LEFT((a), (s));        \
        (a) += (b);                         \
    }

#define T(i) 4294967296 * abs(sin(i))

/**
 * @brief MD5类
 *
 */
class MD5
{
public:
    void Update(const string &str); // 对给定长度的字符串进行MD5运算
    string Tostring();

private:
    void Reset(); // 重置初始变量
    void Update(vector<uint8_t> input);
    void Transform(const vector<uint8_t> block);
    // 将64byte的数据块划分为16个32bit大小的子分组
    vector<uint32_t> Decode(const vector<uint8_t> input); 
    // 32位十进制转成16进制，用8个字母的string表示
    string from10To16(uint32_t decimal);
    // 将1个64位int转成vector<uint8_t>
    vector<uint8_t> fromInt64ToInt8Vec(uint64_t num);     
    uint32_t swap_endian(uint32_t val);

private:
    uint32_t state[4]; // 用于表示4个初始向量
};