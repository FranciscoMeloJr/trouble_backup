#ifndef JSONTEST_H
#define JSONTEST_H

#include <iostream>
#include <fstream>
#include <jsoncpp/json/json.h>

using namespace std;

class JsonTest
{
public:
    //Data:
    ifstream ifs;
    Json::Reader reader;
    Json::Value obj;

    //Methods related with JsonCPP
    void test();
    void setUp();
    //Tested function:
    void read();
    void print();
    ~JsonTest();
};

#endif // JSONTEST_H
