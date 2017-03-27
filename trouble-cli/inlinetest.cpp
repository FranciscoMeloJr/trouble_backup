#include "inlinetest.h"

using namespace std;

InlineTest::InlineTest()
{

}
//Int
int InlineTest::getInt(){
   return 27;
}

//int inline InlineTest::getInt_inline(){
//   return 27;
//}

//String --


string InlineTest::getString(){

   string s ="1234567890";

   return s;
}

//inline string InlineTest::getString_inline(){

//   string s ="1234567890";

//   return s;
//}

//Vector --
inline vector<int> InlineTest::getVector_inline(){
   vector<int> v = { 16, 2, 77, 40, 12071 };
   return v;
}


vector<int> InlineTest::getVector(){
   vector<int> v = { 16, 2, 77, 40, 12071 };
   return v;

}
//Functions --
int inline InlineTest::getFunction_inline(){

   return square(3);
}


int InlineTest::getFunction(){

   return square(3);

}

// complex   --
complex_d InlineTest::getComplex()
{
   complex<double> d(2.0,3.0);
   return d;

}

inline complex_d InlineTest::getComplex_inline()
{
   complex<double> d(2.0,3.0);
   return d;

}

