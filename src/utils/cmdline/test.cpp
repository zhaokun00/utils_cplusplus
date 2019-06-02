
/*
	cmdline是一个非常好用的C++命令行解析库,其基于模板,由于只包含一个头文件,所以跟容易集成到自己的项目中
	//参考文档:https://blog.csdn.net/xiaohui_hubei/article/details/40479811
	
*/

#include "cmdline.h"
#include <iostream>

//用于简单的测试,长命令输入:./test --name=zhaokun(长命令用=),短命令输入:./test -n zhaokun(短命令用空格)
void test1(int argc, char *argv[]) {

//1.创建一个命令行解析器	
	cmdline::parser parser;

/*
	2.添加指定类型的输入参数-添加时指定数据类型
	 p1:长名称
	 p2:短名称('\0'表示没有短名称)
	 p3:参数描述
	 p4:bool值,表示该参数是否必须存在(可选,默认值为true),当为true时命令行中必须输入输入该参数,当false时命令行中可以不输入也可以输入
	 p5:参数的默认值(可选,当第4个参数为false时该参数有效)
	 
*/
	parser.add<std::string>("name", 'n', "clinet's name", false, "zhaokun");
	
	//必须加上该语句,该语句不仅仅是检查参数,更重要的是把输入的参数放到缓存中。当输入参数不符合要求时会打印语法
	parser.parse_check(argc, argv);

	//exist函数用于判断命令行中是否输入了该参数,如果输入了该参数返回值为true,否则为false
	if(parser.exist("name")) {
		std::string name = parser.get<std::string>("name");
		std::cout << name << std::endl;
	}
	else {
		std::cout << "从命令行中没有输入name参数,因此采用默认的参数:" << parser.get<std::string>("name") << std::endl;
	}

}

//cmdline::range函数使用
void test2(int argc, char *argv[]) {

//1.创建一个命令行解析器	
	cmdline::parser parser;

/*
	2.添加指定类型的输入参数-添加时指定数据类型
	 p1:长名称
	 p2:短名称('\0'表示没有短名称)
	 p3:参数描述
	 p4:bool值,表示该参数是否必须存在(可选,默认值为true),当为true时命令行中必须输入输入该参数,当false时命令行中可以不输入也可以输入
	 p5:参数的默认值(可选,当第4个参数为false时该参数有效)
	 p6:永凯对参数假如额外的限制,通过cmdline::range进行限制,对连续型数据进行限制
	
*/
	parser.add<int>("age", 'a', "clinet's age", true, 0,cmdline::range(0,100));
	
	//必须加上该语句,该语句不仅仅是检查参数,更重要的是把输入的参数放到缓存中。当输入参数不符合要求时会打印语法
	parser.parse_check(argc, argv);

	//exist函数用于判断命令行中是否输入了该参数,如果输入了该参数返回值为true,否则为false
	if(parser.exist("age")) {
		int age = parser.get<int>("age");
		std::cout << "age:" << age << std::endl;
	}

}

//cmdline::oneof函数使用
void test3(int argc, char *argv[]) {

//1.创建一个命令行解析器	
	cmdline::parser parser;

/*
	2.添加指定类型的输入参数-添加时指定数据类型
	 p1:长名称
	 p2:短名称('\0'表示没有短名称)
	 p3:参数描述
	 p4:bool值,表示该参数是否必须存在(可选,默认值为true),当为true时命令行中必须输入输入该参数,当false时命令行中可以不输入也可以输入
	 p5:参数的默认值(可选,当第4个参数为false时该参数有效)
	 p6:永凯对参数假如额外的限制,通过cmdline::range进行限制,这个函数前加上参数类型,对离散型数据进行限制
	
*/
	parser.add<std::string>("sex", 's', "clinet's sex", true, "",cmdline::oneof<std::string>("male","female"));
	
	//必须加上该语句,该语句不仅仅是检查参数,更重要的是把输入的参数放到缓存中。当输入参数不符合要求时会打印语法
	parser.parse_check(argc, argv);

	//exist函数用于判断命令行中是否输入了该参数,如果输入了该参数返回值为true,否则为false
	if(parser.exist("sex")) {
		std::string sex = parser.get<std::string>("sex");
		std::cout << "sex:" << sex << std::endl;
	}

}


//cmdline::oneof函数使用
void test4(int argc, char *argv[]) {

//1.创建一个命令行解析器	
	cmdline::parser parser;

/*
	2.添加指定类型的输入参数-添加时指定数据类型
	 p1:长名称
	 p2:短名称('\0'表示没有短名称)
	 p3:参数描述
	 p4:bool值,表示该参数是否必须存在(可选,默认值为true),当为true时命令行中必须输入输入该参数,当false时命令行中可以不输入也可以输入
	 p5:参数的默认值(可选,当第4个参数为false时该参数有效)
	 p6:永凯对参数假如额外的限制,通过cmdline::range进行限制,这个函数前加上参数类型,对离散型数据进行限制
	
*/
	parser.add<std::string>("sex", 's', "clinet's sex", false, "male",cmdline::oneof<std::string>("male","female"));
	
	//必须加上该语句,该语句不仅仅是检查参数,更重要的是把输入的参数放到缓存中。当输入参数不符合要求时会打印语法
	parser.parse_check(argc, argv);

	parser.footer("自定义脚本内容");

}


int main(int argc, char *argv[]) {

//	test1(argc,argv);

//	test2(argc,argv);
//	test3(argc,argv);

	test4(argc,argv);
#if 0

	if (a.exist("host")) {
	  cout << "aaa" <<endl;
	}
	else {
	   cout << "bbb" <<endl;	
	}
#endif
	//  cout << a.get<bool>("host") << std::endl;


	//  cout << a.get<string>("type") << "://"
	//       << a.get<string>("host") << ":"
	//       << a.get<int>("port") << endl;

	//  if (a.exist("gzip")) cout << "gzip" << endl;

	return 0;
}
