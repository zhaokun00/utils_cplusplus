#include <stdio.h>
#include <iostream>
#include <chrono>
#include "tinyxml2.h"

using namespace std;  
using namespace tinyxml2;

void test1() {
	XMLDocument doc;  
		if ( doc.LoadFile("../test.xml") )  
		{  
			doc.PrintError();  
			exit( 1 );	
		}  
	  
		// 根元素  
		XMLElement* scene = doc.RootElement();	
		cout << "name:" << scene->Attribute( "name" ) << endl << endl;	
	  
		// 遍历<surface>元素  
		XMLElement* surface = scene->FirstChildElement( "surface" );  
		while (surface)  
		{  
			// 遍历属性列表  
			const XMLAttribute* surfaceAttr = surface->FirstAttribute();  
			while ( surfaceAttr )  
			{  
				cout << surfaceAttr->Name() << ":" << surfaceAttr->Value() << "  ";  
				surfaceAttr = surfaceAttr->Next();	
			}  
			cout << endl;  
	  
			// 遍历子元素	
			XMLElement* surfaceChild = surface->FirstChildElement();  
			while (surfaceChild)  
			{  
				cout << surfaceChild->Name() << " = " << surfaceChild->GetText() << endl;  
				surfaceChild = surfaceChild->NextSiblingElement();	
			}  
			cout << endl;  
	  
			surface = surface->NextSiblingElement( "surface" );  
		}  

}

void test2() {
	XMLDocument doc;  
	  
		// 创建根元素<China>  
		XMLElement* root = doc.NewElement( "China" );  
		doc.InsertEndChild( root );  
	  
		// 创建子元素<City>  
		XMLElement* cityElement = doc.NewElement( "City" );  
		cityElement->SetAttribute( "name", "WuHan" ); // 设置元素属性  
		root->InsertEndChild( cityElement );  
	  
		// 创建孙元素<population>	
		XMLElement* populationElement = doc.NewElement( "population" );  
		populationElement->SetText( "8,000,000" ); // 设置元素文本  
		cityElement->InsertEndChild( populationElement );  
	  
		// 创建孙元素<area>  
		XMLElement* areaElement = doc.NewElement( "area" );  
		XMLText* areaText = doc.NewText( "10000" );  
		areaElement->InsertEndChild( areaText ); // 设置元素文本	
		cityElement->InsertEndChild( areaElement );  
	  
		// 输出XML至文件  
		cout << "output xml to '1.xml'" << endl << endl;  
		doc.SaveFile( "1.xml" );  
	  
		// 输出XML至内存  
		cout << "output xml to memory" << endl	
			 << "--------------------" << endl;  
		XMLPrinter printer;  
		doc.Print( &printer );	
		cout << printer.CStr(); 

}

int main() {

	test1();
	test2();
//	test3();
//	test4();
//	test5();
//	test6();
//	test7();
//	test8();
//	test9();
//	test10();
	return 0;
}

