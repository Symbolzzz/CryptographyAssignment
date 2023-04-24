#include "BigInt/binaryHelpFunc.h"

using namespace std;

class RSA
{
private:
    BigInt p;
    BigInt q;
    BigInt publicKey_e;
    BigInt secretKey_d;
    BigInt n;
public:
    void NewRsaKeyPair(int length);
    BigInt encry(string msg);
    BigInt decry(string msg);
    BigInt getPk();
    BigInt getSk();
    BigInt getN();
    void setPk(string Pk);
    void setSk(string Sk);
    void setN(string N);
    void print();
};

