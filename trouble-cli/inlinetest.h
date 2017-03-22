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
    string getString_inline();

    // int   --
    int getInt();
    inline int getInt_inline();

    // vector   --
    vector<int> getVector();
    inline vector<int> getVector_inline();

    // inline   --
    int getFunction_inline();
    int getFunction();

    // complex   --
    complex_d getComplex();
    inline complex_d getComplex_inline();

};

#endif // INLINETEST_H
