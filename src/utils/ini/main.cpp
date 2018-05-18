#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <map>
#include "INIParser.h"
using namespace std;

int main()
{
    INIParser ini_parser;

//读操作
#if 0
    ini_parser.ReadINI("../conf.ini");

    cout << ini_parser.GetValue("default", "server_port") << endl;
    ini_parser.Clear();
#endif

//写操作
#if 1	
//    ini_parser.SetValue("class1","name1","Tom");
	
//    ini_parser.SetValue("class2","name2","Lucy");

    ini_parser.SetValue("class3","name3","Tom");
	
    ini_parser.WriteINI("test.ini");
#endif
    return 0;
}
