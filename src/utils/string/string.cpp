#include <stdio.h>
#include <iostream>
#include <thread>
#include <string>
#include <chrono>

//https://blog.csdn.net/tengfei461807914/article/details/52203202

//形参为std::string类型,该方式不会修改字符串的值
void test1_1(std::string s) {

	s = "yangcuiling";

	std::cout << s << std::endl;

}

//形参为std::string类型引用,该方式会修改字符串的值
void test1_2(std::string & s) {

	s = "yangcuiling";

	std::cout << s << std::endl;

}

//返回参数为std::string类型
std::string test1_3() {

	std::string s = "yangcuiling";

	return s;
}
void test1() {

	std::string s = "zhaokun";

//	test1_1(s); //不会修改字符串s的值

//	test1_2(s); //会修改字符串s的值

	s = test1_3(); //会返回字符串sde值

	std::cout << s << std::endl;
}

//测试std::string类型的初始化
void test2() {

	std::string name1 = "zhaokun"; //第一种初始化方式
 
	std::string name2(name1); //第二种初始化方式

	std::string name3 = name1; //第三种初始化方式

	std::cout << name1 << std::endl;
	std::cout << name2 << std::endl;
	std::cout << name3 << std::endl;

	std::string name[2]; //定义字符串数组

	name[0] = "name0";
	name[1] = "name1";

	std::cout << "name[0] = " << name[0] << std::endl;
	std::cout << "name[1] = " << name[1] << std::endl;

	std::string *age = new std::string("20"); //用new方式创建字符串

	std::cout << "age = " << *age << std::endl;

	delete age;
	age = NULL;

	
	std::string *sex = new std::string[2]; //用new方式创建字符串数组

	sex[0] = "sex0";

	sex[1] = "sex1";

	std::cout << "sex[0] = " << sex[0] << std::endl;
	std::cout << "sex[1] = " << sex[1] << std::endl;

	delete [] sex; //释放数组形式
	sex = NULL;
	
}

//测试字符串的追加操作
//append函数可以用来在字符串的末尾追加字符和字符串,由于string重载了运算符,可可以使用+=操作
void test3() {

	std::string s1 = "name:";

	std::string s2 = "zhaokun";

//第一种追加方式
	std::string s3 = s1 + s2; //直接用运算符+=号追加,结果赋值给另外一个string变量

	std::cout << "s3=" << s3 << std::endl;

//第二种追加方式
	//原型:basic_string& append( const CharT* s );
//	s1.append(s2); //直接在s1字符串末尾进行追加,会改变字符串s1的值

//第三种追加方式
	/*原型:basic_string& append( const CharT* s,size_type count );

	  参数:
	  s类型为const CharT *类型
	  count:追加的参数个数

	  s1.append(s2,5); //这种追加方式会报错,由于s2是std::string类型,而非const CharT *类型
	  c_str是将string类型转换为char *类型
	*/

//	s1.append("hello",5); //直接这种追加方式也可以
//	s1.append(s2.c_str(),5); //正确的追加方式
//	s1.append(s2.c_str(),50); //当追加的字节数超过实际字符串长度时,输出的字符串为错误的输出

//	std::cout << "size:" << s1.size() << std::endl; //计算的是字符串的实际长度
//	std::cout << "max_size:" << s1.max_size() << std::endl; //计算字符串的最大长度,max_size:4611686018427387897

//第四种追加方式
	/*
		原型:
		basic_string& append( const basic_string& str,size_type pos,size_type count);

		str:追加的字符串
		pos:开始位置,字符串的位置是从0开始追加
		count:个数
	*/

//	s1.append(s2,1,2);

//第5钟追加方式
	/*
		原型:
		basic_string& append( size_type count, const CharT& ch );

		count:个数
		ch:单个字符
	*/

//	s1.append(10,'a'); //0输出s1=name:aaaaaaaaaa

//第6钟追加方式
	/*
	template< class InputIterator >
	basic_string& append( InputIterator first, InputIterator last );
	*/
	s1.append(s2.begin(),s2.end()); //begin(),end()函数返回的是迭代器

	std::cout << "s1=" << s1 << std::endl;
}

//测试字符串的替换操作
//replace:就是替换的意思,就是先删除后替换,替换还有很多函数,具体可以查看C++参考文档
void test4() {

	std::string s1 = "zhaokun";
	std::string s2 = "abcd";

//第一种替换方式
/*
	原型:basic_string& replace( size_type pos, size_type count,
                       const basic_string& str );

    pos:替换的位置
    count:替换的字节数
    
*/

	s1.replace(2,2,s2);
	std::cout << "s1 = " << s1 << std::endl;
}

int main() {

//	test1();
//	test2();
//	test3();
	test4();
//	test5();
//	test6();
//	test7();
//	test8();
//	test9();
//	test10();
	return 0;
}

