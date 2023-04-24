#include "rsa.h"

using namespace std;

void RSA::print()
{
    cout << "prime1: " << endl;
    p.print();
    cout << "prime2: " << endl;
    q.print();
    cout << "p * q :" << endl;
    n.print();
    cout << "publicKey_e" << endl;
    publicKey_e.print();
    cout << "secretKey_d" << endl;
    secretKey_d.print();
}

BigInt RSA::getPk()
{
    return publicKey_e;
}

BigInt RSA::getSk()
{
    return secretKey_d;
}

BigInt RSA::getN()
{
    return n;
}

void RSA::setPk(string Pk)
{
    publicKey_e = Pk;
}

void RSA::setSk(string Sk)
{
    secretKey_d = Sk;
}

void RSA::setN(string N)
{
    n = N;
}

/**
 * @brief 生成公私钥对
 * 
 */
void RSA::NewRsaKeyPair(int length)
{
    // 生成两个质数
    p.genPrime(length);
    q.genPrime(length);

    n = p * q;

    BigInt one("1");
    BigInt p_one = p - one;
    BigInt q_one = q - one;

    BigInt euler = p_one * q_one;
    // 生成质数一定与euler互质
    publicKey_e = BigInt("10000000000000001");

    BigInt i("1");

    while (1)
    {
        BigInt temp = (i * euler) + one;
        if ((temp % publicKey_e) == "")
        {
            secretKey_d = temp / publicKey_e;
            break;
        }
        i = i + one;
    }
}

/**
 * @brief 加密函数
 * 
 * @param msg 
 * @return BigInt 
 */
BigInt RSA::encry(string msg)
{
    BigInt M(msg);
    BigInt c;

    c = PowMod(M, secretKey_d, n);
    return c;
}

/**
 * @brief 解密函数
 * 
 * @param msg 
 * @return BigInt 
 */
BigInt RSA::decry(string msg)
{
    BigInt C(msg);
    BigInt c;

    c = PowMod(C, publicKey_e, n);
    return c;
}

// int main()
// {
//     RSA rsa;
//     rsa.NewRsaKeyPair(128);
//     rsa.print();
//     cout << "-------------------------------" << endl;
//     BigInt a = rsa.encry("1111");
//     // BigInt a;
//     // a.genPrime(128);
//     a.print();
//     BigInt b = rsa.decry(a.getbits());
//     // BigInt b;
//     // b.genPrime(128);
//     b.print();
//     return 0;
// }