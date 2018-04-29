#include "WrapperXml.h"

//https://blog.csdn.net/K346K346/article/details/48750417
//https://blog.csdn.net/zhawk/article/details/60880036
//https://blog.csdn.net/u010510020/article/details/73799996 字符串与整形互相转换
//https://blog.csdn.net/u012851419/article/details/26864415 给string赋值出现错误
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

//测试插入节点元素
void test3() {

	WrapperXml::createXml("test.xml", "msg");

	User user1("zhaokun","00521",0,"15301070551","825537475@qq.com");
	WrapperXml::insertXmlNode("test.xml", user1);

	User user2("ycl","00521",0,"15301070551","825537475@qq.com");
	WrapperXml::insertXmlNode("test.xml", user2);

}

//测试查询节点
void test4() {

	User * u = WrapperXml::queryUserByName("test.xml","ycl");

	if(NULL != u) {
		std::cout << "name = " << u->m_userName << std::endl;
		std::cout << "password = " << u->m_password << std::endl;
		std::cout << "mobile = " << u->m_mobile << std::endl;
		std::cout << "email = " << u->m_email << std::endl;
	}
}

void test5_1(int num) {

	std::cout << "NULL" << std::endl;
}

void test5_1(int *p) {
	std::cout << "nullptr" << std::endl;
}

//测试NULL与nullptr:空指针
void test5() {

//	std::cout << NULL << std::endl;

	if(0 == NULL) {
		std::cout << "NULL is 0" << std::endl;
	}
	else {
		std::cout << "NULL is not 0" << std::endl;
	}

	if(nullptr == 0) {
		std::cout << "nullptr is 0" << std::endl;
	}
	else {
		std::cout << "nullptr is not 0" << std::endl;
	}

	if(NULL == nullptr) {
		std::cout << "nullptr is NULL" << std::endl;
	}
	else {
		std::cout << "nullptr is not NULL" << std::endl;
	}

	test5_1(0);
	//这样调用编译不过
//	test5_1(NULL);
	test5_1(nullptr);
	
}

//测试更新节点
void test6() {

	User user("ycl","00522",1,"15301070551","825537475@qq.com");

	WrapperXml::updateUser("test.xml", &user);

}

void test7() {

	WrapperXml::deleteUserByName("test.xml", "ycl");
}
int main() {

//	test1();
//	test2();
	test3();
//	test4();
//	test5();
//	test6();
	test7();
	return 0;
}

