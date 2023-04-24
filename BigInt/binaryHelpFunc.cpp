#include "binaryHelpFunc.h"

BigInt::BigInt()
{
    bits = "";
    hexs = "";
}

BigInt::BigInt(const BigInt &a)
{
    bits = a.bits;
    hexs = a.hexs;
}

BigInt::BigInt(string str)
{
    bits = str;
    toHex();
}

string BigInt::getbits()
{
    return bits;
}

string BigInt::gethexs()
{
    return hexs;
}

void BigInt::toHex()
{
    hexs.clear();
    string temp;
    int x;
    int left = 4 - (bits.size() % 4);
    if (left != 4)
    {
        string zeros(left, '0');
        bits = zeros + bits;
    }

    for (int i = 0; i < bits.size(); i += 4)
    {

        temp = bits.substr(i, 4);

        x = 1 * (temp[3] - 48) + 2 * (temp[2] - 48) + 4 * (temp[1] - 48) + 8 * (temp[0] - 48);

        if (x <= 9)
        {
            hexs += to_string(x);
        }
        else if (x == 10)
        {
            hexs += "A";
        }
        else if (x == 11)
        {
            hexs += "B";
        }
        else if (x == 12)
        {
            hexs += "C";
        }
        else if (x == 13)
        {
            hexs += "D";
        }
        else if (x == 14)
        {
            hexs += "E";
        }
        else if (x == 15)
        {
            hexs += "F";
        }
    }
    if (bits.find_first_not_of('0') != std::string::npos)
    {
        bits = bits.substr(bits.find_first_not_of('0'));
    }
}

/**
 * @brief 十六进制转化成二进制
 * 
 */
void BigInt::toBit()
{
    bits.clear();
    string temp;
    for (int i = 0; i < hexs.length(); ++i)
    {
        switch (hexs[i])
        {
        case '0':
            temp += "0000";
            break;
        case '1':
            temp += "0001";
            break;
        case '2':
            temp += "0010";
            break;
        case '3':
            temp += "0011";
            break;
        case '4':
            temp += "0100";
            break;
        case '5':
            temp += "0101";
            break;
        case '6':
            temp += "0110";
            break;
        case '7':
            temp += "0111";
            break;
        case '8':
            temp += "1000";
            break;
        case '9':
            temp += "1001";
            break;
        case 'A':
            temp += "1010";
            break;
        case 'B':
            temp += "1011";
            break;
        case 'C':
            temp += "1100";
            break;
        case 'D':
            temp += "1101";
            break;
        case 'E':
            temp += "1110";
            break;
        case 'F':
            temp += "1111";
            break;
        }
    }
    if (temp.find_first_not_of('0') != std::string::npos)
    {
        temp = temp.substr(temp.find_first_not_of('0'));
    }
    bits = temp;
}

void BigInt::setHexs(string str)
{
    hexs = str;
    toBit();
}

/**
 * @brief 生成对应位数的大数
 *
 * @param size
 */
void BigInt::genBigInt(int size)
{
    bits.clear();
    hexs.clear();
    static int help = 0;
    help++;
    srand(help + (unsigned)time(NULL));
    for (int i = 0; i < size; ++i)
    {
        int temp = rand() % 2;
        if (i == 0)
        {
            temp = 1;
        }
        bits += to_string(temp);
    }
    toHex();
}

void BigInt::print()
{
    cout << "bin: " << bits << endl;
    cout << "hex: " << hexs << endl;
    cout << "size: " << bits.size() << endl;
}

/**
 * @brief 小于运算
 *
 * @param b
 * @return true
 * @return false
 */
bool BigInt::operator<(BigInt &b)
{
    if (this->bits.size() < b.bits.size())
    {
        return true;
    }
    else if (this->bits == b.bits)
    {
        return false;
    }
    else if (this->bits.size() > b.bits.size())
    {
        return false;
    }

    else
    {
        for (int i = 0; i < bits.size(); ++i)
        {
            if (bits[i] < b.bits[i])
            {
                return true;
            }
            if (bits[i] > b.bits[i])
            {
                return false;
            }
        }
    }
    return true;
}

/**
 * @brief 二进制加法
 *
 * @param a
 * @param b.bits
 * @return string
 */
BigInt BigInt::operator+(BigInt &b)
{
    BigInt c;
    BigInt copy_this = *this;
    BigInt copy_b = b;

    string temp(max(bits.size(), copy_b.bits.size()) + 1, '0');
    c.bits = temp;

    reverse(copy_this.bits.begin(), copy_this.bits.end());
    reverse(copy_b.bits.begin(), copy_b.bits.end());
    if (copy_this.bits.size() < copy_b.bits.size()) //补零，使两串长度相等
    {
        copy_this.bits.insert(copy_this.bits.end(), copy_b.bits.size() - copy_this.bits.size(), '0');
    }
    else if (copy_b.bits.size() < copy_this.bits.size())
    {
        copy_b.bits.insert(copy_b.bits.end(), copy_this.bits.size() - copy_b.bits.size(), '0');
    }

    // 进位
    bool carry = 0;
    for (int i = 0; i < copy_this.bits.size(); ++i)
    {
        if (copy_this.bits[i] != copy_b.bits[i] && carry == 0)
        {
            c.bits[i] = '1';
            carry = 0;
        }
        else if (copy_this.bits[i] != copy_b.bits[i] && carry == 1)
        {
            c.bits[i] = '0';
            carry = 1;
        }
        else if (copy_this.bits[i] == copy_b.bits[i] && copy_this.bits[i] == '0' && carry == 0)
        {
            c.bits[i] = '0';
            carry = 0;
        }
        else if (copy_this.bits[i] == copy_b.bits[i] && copy_this.bits[i] == '0' && carry == 1)
        {
            c.bits[i] = '1';
            carry = 0;
        }
        else if (copy_this.bits[i] == copy_b.bits[i] && copy_this.bits[i] == '1' && carry == 0)
        {
            c.bits[i] = '0';
            carry = 1;
        }
        else if (copy_this.bits[i] == copy_b.bits[i] && copy_this.bits[i] == '1' && carry == 1)
        {
            c.bits[i] = '1';
            carry = 1;
        }
    }
    if (carry == 1)
    {
        c.bits[copy_this.bits.size()] = '1';
    }
    reverse(c.bits.begin(), c.bits.end());
    if (c.bits.find_first_not_of('0') != std::string::npos)
    {
        c.bits = c.bits.substr(c.bits.find_first_not_of('0'));
    }
    c.toHex();

    return c;
}

/**
 * @brief 二进制减法
 *
 * @param b
 * @return BigInt
 */
BigInt BigInt::operator-(BigInt &b)
{
    BigInt copy_this = *this;
    BigInt copy_b = b;
    BigInt c;

    // 仅实现大-小
    if (b < *this)
    {
        string temp(bits.size(), '0');
        c.bits = temp;

        reverse(copy_this.bits.begin(), copy_this.bits.end());
        reverse(copy_b.bits.begin(), copy_b.bits.end());

        copy_b.bits.insert(copy_b.bits.end(), copy_this.bits.size() - copy_b.bits.size(), '0');
        bool rent = 0;
        for (int i = 0; i < copy_this.bits.size(); ++i)
        {
            // cout << copy_this.bits[i] << " " << copy_b.bits[i] << " " << rent << endl;
            if (copy_this.bits[i] == '0' && copy_b.bits[i] == '0' && rent == 0)
            {
                c.bits[i] = '0';
                rent = 0;
            }
            else if (copy_this.bits[i] == '0' && copy_b.bits[i] == '0' && rent == 1)
            {
                c.bits[i] = '1';
                rent = 1;
            }
            else if (copy_this.bits[i] == '1' && copy_b.bits[i] == '0' && rent == 0)
            {
                c.bits[i] = '1';
                rent = 0;
            }
            else if (copy_this.bits[i] == '1' && copy_b.bits[i] == '0' && rent == 1)
            {
                c.bits[i] = '0';
                rent = 0;
            }
            else if (copy_this.bits[i] == '0' && copy_b.bits[i] == '1' && rent == 0)
            {
                c.bits[i] = '1';
                rent = 1;
            }
            else if (copy_this.bits[i] == '0' && copy_b.bits[i] == '1' && rent == 1)
            {
                c.bits[i] = '0';
                rent = 1;
            }
            else if (copy_this.bits[i] == '1' && copy_b.bits[i] == '1' && rent == 0)
            {
                c.bits[i] = '0';
                rent = 0;
            }
            else if (copy_this.bits[i] == '1' && copy_b.bits[i] == '1' && rent == 1)
            {
                c.bits[i] = '1';
                rent = 1;
            }
        }

        reverse(c.bits.begin(), c.bits.end());

        if (c.bits.find_first_not_of('0') != std::string::npos)
        {
            c.bits = c.bits.substr(c.bits.find_first_not_of('0'));
        }
        c.toHex();
    }
    // else
    // {
    //         c.bits = "";
    //         c.hexs = "";

    // }
    return c;
}

/**
 * @brief 二进制乘法
 *
 * @param b
 * @return BigInt
 */
BigInt BigInt::operator*(BigInt &b)
{
    BigInt c;
    BigInt x = b;

    reverse(bits.begin(), bits.end());
    reverse(b.bits.begin(), b.bits.end());

    for (int i = 0; i < bits.size(); ++i)
    {
        if (bits[i] == '1')
        {
            string zeros(i, '0');
            // x.bits.insert(b.bits.end(), i,'0');
            string y = zeros + b.bits;
            reverse(y.begin(), y.end());
            x.bits = y;
            c = c + x;
        }
        else
        {
            continue;
        }
    }

    if (c.bits.find_first_not_of('0') != std::string::npos)
    {
        c.bits = c.bits.substr(c.bits.find_first_not_of('0'));
    }

    reverse(bits.begin(), bits.end());
    reverse(b.bits.begin(), b.bits.end());
    c.toHex();
    return c;
}

/**
 * @brief 大数除法
 *
 * @param b
 * @return BigInt
 */
BigInt BigInt::operator/(BigInt &b)
{
    BigInt copy_this = *this;
    BigInt copy_b = b;
    BigInt c;

    // if (bits == b.bits)
    // {
    //     c.bits = "1";
    //     c.toHex();
    //     return c;
    // }

    if (bits.size() > b.bits.size())
    {
        int length_diff = bits.size() - b.bits.size();
        // 补n个零操作相当于乘 b10...
        string zeros(length_diff, '0');
        copy_b.bits += zeros;
    }
    while (copy_b.bits.size() >= b.bits.size())
    {
        if (copy_b < copy_this || copy_b.bits == copy_this.bits)
        {
            copy_this = copy_this - copy_b;
            c.bits += '1';
        }
        else
        {
            c.bits += '0';
        }
        copy_b.bits.erase(copy_b.bits.end() - 1);
    }

    if (c.bits.find_first_not_of('0') != std::string::npos)
    {
        c.bits = c.bits.substr(c.bits.find_first_not_of('0'));
    }
    c.toHex();
    return c;
}

bool BigInt::operator==(BigInt &b)
{
    return bits == b.bits;
}

bool BigInt::operator==(string str)
{
    return bits == str;
}

/**
 * @brief 二进制取模
 *
 * @param b
 * @return BigInt
 */
BigInt BigInt::operator%(BigInt &b)
{
    BigInt copy_this = *this;
    BigInt copy_b = b;

    if (bits.size() > b.bits.size())
    {
        int length_diff = bits.size() - b.bits.size();
        // 补n个零操作相当于乘 b10...
        string zeros(length_diff, '0');
        copy_b.bits += zeros;
    }

    while (1)
    {
        if (copy_b < copy_this)
        {
            copy_this = copy_this - copy_b;
        }
        if (copy_b.bits.size() == b.bits.size())
        {
            break;
        }
        else
        {
            copy_b.bits.erase(copy_b.bits.end() - 1);
        }
    }

    if (b.bits == copy_this.bits)
    {
        copy_this.bits = "";
        copy_this.hexs = "";
    }

    return copy_this;
}

/**
 * @brief 大数取模int型
 *
 * @param b
 * @return BigInt
 */
BigInt BigInt::operator%(int &b)
{
    string str;
    _itoa(b, (char *)str.c_str(), 2);
    BigInt a(str);
    return (*this % a);
}

BigInt BigInt::operator++()
{
    BigInt one("1");
    *this = *this + one;
    return *this;
}

/**
 * @brief 计算(a * b) % c
 *
 * @param a
 * @param b
 * @param c
 * @return BigInt
 */
BigInt MulMod(BigInt a, BigInt b, BigInt c)
{
    BigInt d = a % c;
    BigInt e = b % c;
    BigInt f = d * e;
    BigInt g = f % c;
    return g;
}

/**
 * @brief 计算(a ^ b) % c
 *
 * @param a
 * @param b
 * @param c
 * @return BigInt
 */
BigInt PowMod(BigInt base, BigInt pow, BigInt n)
{
    BigInt a = base, b = pow, c("1"), one("1");
    while (b.bits.size() > 0)
    {
        while (!(b.bits[b.bits.size() - 1] - 48))
        {
            a = MulMod(a, a, n);
            b.bits.erase(b.bits.end() - 1);
        }
        b = b - one;
        c = MulMod(a, c, n);
    }
    return c;
}

/**
 * @brief 判断是否是质数
 *
 * @param b
 * @return true
 * @return false
 */
bool RabinMillerKnl(BigInt b)
{
    BigInt one("1"), two("10");
    BigInt q = b - one;
    BigInt b_one = q;
    int k = 0;
    while (!(q.bits[q.bits.size() - 1] - 48))
    {
        ++k;
        q.bits.erase(q.bits.end() - 1);
    }

    BigInt a;
    int help = 2 + rand() % b.bits.size();
    a.genBigInt(help);

    if (PowMod(a, q, b) == "1")
    {
        return true;
    }
    for (int j = 0; j < k; j++)
    {
        BigInt z("1");
        for (int w = 0; w < j; ++w)
        {
            z = z * two;
        }
        if (PowMod(a, z * q, b) == b_one)
        {
            return true;
        }
    }
    return false;
}

/**
 * @brief 生成2000以内的素数
 *
 */
void genSamllPrime()
{
}

void BigInt::genPrime(int size)
{
    do
    {
        // isprime = 1;
        genBigInt(size);
        if (bits[bits.size() - 1] == '1')
        {
            bits[bits.size() - 1] = '1';
        }
        // 用2000以内的素数判断
        // for (int i = 1; i < 303; ++i)
        // {
        //     if ((*this % primes[i]).bits.size() == 0)
        //     {
        //         isprime = 0;
        //         break;
        //     }
        // }
        // if (isprime == 0)
        // {
        //     continue;
        // }
        if (RabinMillerKnl(*this))
        {
            break;
        }
    } while (1);
}

// int main()
// {
//     BigInt a, b, c;
//     a.genBigInt(4);
//     b.genBigInt(4);
//     // c.genBigInt(12);

//     a.print();
//     b.print();
//     c = a / b;
//     c.print();

//     // BigInt a;
//     // a.genPrime(512);
//     // a.print();

//     return 0;
// }
