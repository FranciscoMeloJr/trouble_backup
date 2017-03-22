#ifndef JSONTEST_H
#define JSONTEST_H

#include <iostream>
#include <fstream>
#include <jsoncpp/json/json.h>

using namespace std;

class JsonTest
{
public:
    ifstream ifs;

    Json::Reader reader;
    Json::Value obj;

    void test();

    void setUp();

    void read();

    void print();

};

#endif // JSONTEST_H
