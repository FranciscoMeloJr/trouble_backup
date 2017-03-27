#ifndef INLINETEST_H
#define INLINETEST_H

#include <complex>
#include <vector>
#include <string>
#include <iostream>

using namespace std;
typedef complex<double> complex_d;
auto square = [&](int x) { return x*x; };


class InlineTest
{
public:
    InlineTest();

    // string   --
    string getString();
    //inline string getString_inline();
    inline string getString_inline(){ string s = "1234567890"; return s; }

    // int   --
    int getInt();
    inline int getInt_inline() {int i = 27; return i; }

    // vector   --
    vector<int> getVector();
    vector<int> inline getVector_inline();

    // inline   --
    int getFunction();
    int inline getFunction_inline();

    // complex   --
    complex_d getComplex();
    complex_d inline getComplex_inline();

};

#endif // INLINETEST_H
