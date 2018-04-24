#include <stdio.h>
#include <iostream>
#include <chrono>
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;


//https://blog.csdn.net/K346K346/article/details/48750417
//https://blog.csdn.net/zhawk/article/details/60880036

//测试:读取xml文件
void test1() {

	XMLDocument doc;  
	if ( doc.LoadFile("../test.xml") ) {
		doc.PrintError();  
		exit( 1 );

	} 
	  
	//获取根元素  
	XMLElement* scene = doc.RootElement();

	//获取根元素上的属性值
	std::cout << "name:" << scene->Attribute("name") << std::endl;
	std::cout << "age:" << scene->Attribute("age") << std::endl;

	// 获取第一个子元素
	XMLElement* surface = scene->FirstChildElement("surface");

	while(surface) {

		//获取
		const XMLAttribute* surfaceAttr = surface->FirstAttribute(); //获取surface元素的第一个属性
		
		while (surfaceAttr) {
			std::cout << surfaceAttr->Name() << ":" << surfaceAttr->Value() << std::endl; //打印属性的名称与属性的值  
			surfaceAttr = surfaceAttr->Next(); //获取下一个属性
		}
	
		//获取surface标签下的第一个子标签
		XMLElement* surfaceChild = surface->FirstChildElement(); //获取surface标签下的第一个元素,没有key值,将从第一个子标签值开始获取
		
	//	XMLElement* surfaceChild = surface->FirstChildElement("front"); //将从第一个标签front获取
	
		while (surfaceChild) { 
			std::cout << surfaceChild->Name() << " = " << surfaceChild->GetText() << std::endl; //获取标签名称与标签值  
			surfaceChild = surfaceChild->NextSiblingElement(); //返回下一个标签	
		}
	
		surface = surface->NextSiblingElement(); //获取surface下一个标签并且赋值给surface
		//surface = surface->NextSiblingElement("person");

	}
	
}


//测试写xml文件
void test2() {

	XMLDocument doc;  
	const char* declaration ="<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>";

	doc.Parse(declaration); //会覆盖xml文件的所有内容

	
	// 创建根元素<China>  
	XMLElement* root = doc.NewElement( "China" );  
	doc.InsertEndChild( root );  
  
	// 创建子元素<City>  
	XMLElement* cityElement = doc.NewElement( "City" );  
	cityElement->SetAttribute( "name", "WuHan" ); // 设置元素属性  
	root->InsertEndChild( cityElement ); //在root标签上添加元素
  
	// 创建子元素<population>	
	XMLElement* populationElement = doc.NewElement( "population" ); 
	populationElement->SetAttribute( "name", "zhaokun" ); // 设置元素属性
	populationElement->SetAttribute( "age", "30" ); // 设置元素属性 
	populationElement->SetText( "8,000,000" ); // 设置元素文本  
	cityElement->InsertEndChild( populationElement );  
  
	// 创建孙元素<area>  
	XMLElement* areaElement = doc.NewElement( "area" );

	//第一种写文本方式
//	XMLText* areaText = doc.NewText( "10000" );  
//	areaElement->InsertEndChild( areaText ); // 设置元素文本

	//第二种写文本方式
	areaElement->SetText( "1000" );
	cityElement->InsertEndChild( areaElement );  
  
	// 输出XML至文件  
	std::cout << "output xml to '1.xml'" << std::endl;  
	doc.SaveFile( "1.xml" );  
   
	XMLPrinter printer;  
	doc.Print( &printer );
	
	std::cout << printer.CStr(); 

}

int main() {

//	test1();
	test2();
	return 0;
}

