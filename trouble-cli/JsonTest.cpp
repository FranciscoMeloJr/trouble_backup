#include "JsonTest.h"

void JsonTest::test()
{
    ifstream ifs("/home/frank/Desktop/Research/jsoncpp/test/my_tests/alice.json");
        Json::Reader reader;
        Json::Value obj;
        reader.parse(ifs, obj); // reader can also read strings
        cout << "Book: " << obj["book"].asString() << endl;
        cout << "Year: " << obj["year"].asUInt() << endl;
        const Json::Value& characters = obj["characters"]; // array of characters
        for (int i = 0; i < characters.size(); i++){
            cout << "    name: " << characters[i]["name"].asString();
            cout << " chapter: " << characters[i]["chapter"].asUInt();
            cout << endl;
        }

        return;
}
void JsonTest::setUp()
{
    ifs.open("/home/frank/Desktop/Research/jsoncpp/test/my_tests/alice.json");

}
void JsonTest::read()
{

     reader.parse(ifs, obj); // reader can also read strings

}
void JsonTest::print()
{
    cout << "Book: " << obj["book"].asString() << endl;
    cout << "Year: " << obj["year"].asUInt() << endl;
    const Json::Value& characters = obj["characters"]; // array of characters
    for (int i = 0; i < characters.size(); i++){
         cout << "    name: " << characters[i]["name"].asString();
         cout << " chapter: " << characters[i]["chapter"].asUInt();
         cout << endl;
    }

    return;
}

