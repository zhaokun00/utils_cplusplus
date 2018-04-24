#include <stdio.h>
#include <iostream>
#include <chrono>
#include <vector>
#include <map>
#include"json.hpp"
//using namespace std;
//using json=nlohmann::json;

//https://blog.csdn.net/forrid/article/details/78867679
//https://blog.csdn.net/fengxinlinux/article/details/71037244

//测试:std::cin函数,读取终端输入的字符串
void test1() {

	std::string str;
#if 1
	std::cout << "输入字符串:";

	//将输入的字符串赋值给str变量
	std::cin >> str;

	std::cout << str << std::endl;
#endif

	std::string s = "q";

	if(s == str) {
		std::cout << "两个字符串相等" << std::endl;
	}
	else {
		std::cout << "两个字符串不相等" << std::endl;
	}
}

//测试nlohmann::json的基本用法

/*
	1.拼接json串,将json串转换为字符串类型
	2.将字符串类型转化为json串,从json串中获取想要的数据
*/

void test2() {

	nlohmann::json j; //声明一个nlohmann::json类型对象

	j["pi"] = 3.14; //添加整型元素
	j["happy"] = true; //添加bool类型元素
	j["answer"]["today"] = 1; //在对象中添加对象
	j["answer"]["tomorrow"] = "monday";
	j["answer"]["day"]["sex"] = 1;
	j["answer"]["day"]["name"] = "zhaokun";
//	j["nothing"] = NULL; //这种形式输出为"nothing":0
	j["nothing"] = nullptr; //这种形式处处为"nothing":null

	j["list"] = {1,2,3};

	j["list"][3] = 4;
	//普通打印字符串的形式
//	std::cout << j.dump() << std::endl;

	//格式化输出字符串
//	std::cout << j.dump(4) << std::endl;

//测试:不能将nlohmann::json类型对象直接转换为std::string类型对象
#if 0
	std::string str = j;

	std::cout << str << std::endl;
#endif

//测试:用dump函数将nlohmann::json类型转换为std::string类型
#if 0
	std::string str = j.dump();
	std::cout << str << std::endl;
#endif

//测试:从nlohmann::json类型中获取值
#if 0
	//get函数后面要添加获取的类型
 	int pi = j["pi"].get<std::int>();

	//错误的形式
//	std::string str = j["pi"].get();

	std::cout << pi << std::endl;
#endif

//测试将字符串转化为nlohmann::json类型
#if 1
	std::string str = j.dump();
	nlohmann::json jj = nlohmann::json::parse(str);

//	std::cout << jj << std::endl;

/*
	double pi = j["pi"].get<double>();

	std::cout << "pi = " << pi << std::endl;

	std::cout << j["happy"].get<bool>() << std::endl;
	std::cout << j["answer"]["today"].get<int>() << std::endl;
	std::cout << j["answer"]["tomorrow"].get<std::string>() << std::endl;
	std::cout << j["answer"]["day"]["sex"].get<int>() << std::endl;
*/
//	std::cout << j["list"][0] << std::endl;

	//获取数组
	nlohmann::json i = j["list"];

//	std::cout << i.dump(4);
	std::cout << "size = " << i.size() << std::endl;
//    std::cout << "type = " << i.type() << std::endl;
	
	for (nlohmann::json::iterator it = i.begin(); it != i.end(); ++it) { //获取数组的值
	  std::cout << *it << '\n';
	}
	
#endif

//测试获取对象中的对象
#if 0
	std::cout << j["happy"].get<bool>() << std::endl;
	std::cout << j["answer"]["everything"].get<int>() << std::endl;
#endif

}

//从字符串转换为json类型对象
void test3() {

	//第一种形式:后面必须加__json,没有附加__json后缀,传递的字符串文字不会解析,而只是用作JSON字符串,也就是说
	//nlohmann::json j = "{ \"happy\": true, \"pi\": 3.141 }"只存储字符串"{ "happy": true, "pi": 3.141 }",而不是解析实际对象
//	nlohmann::json j = "{\"happy\": true, \"pi\": 3.141}"_json;
	
//	json j = "{\"happy\": true, \"pi\": 3.141}"; //这种方式获取key值不能获取到

	//第二种方式:使用nlohmann::json::parse函数将字符串转换为json类型对象
	nlohmann::json j = nlohmann::json::parse("{\"happy\": true, \"pi\": 3.141}");
	
	std::cout << j["happy"].get<bool>() << std::endl;

}

//直接把vector中数据放入json类型对象中
void test4() {

	std::vector<int> v;

	v.push_back(1);
	v.push_back(2);

	nlohmann::json json(v);

	for (nlohmann::json::iterator it = json.begin(); it != json.end(); ++it) { //获取数组的值
	  std::cout << *it << '\n';
	}

}

//直接把map类型对象放入到json类型对象中
void test5() {

	std::map<std::string,int> m;

	m.insert(std::make_pair("num1",1));
	m.insert(std::make_pair("num2",2));

	nlohmann::json json(m);

	std::cout << json["num1"].get<int>() << std::endl;
	std::cout << json["num2"].get<int>() << std::endl;

	for(nlohmann::json::iterator it = json.begin();it != json.end();++it) {

		std::cout << "key =" << it.key() << std::endl;
		std::cout << "value =" << it.value() << std::endl;
	}
}

namespace ns {
    // 一个简单的结构，为 person 建模
    struct person {
        std::string name;
        std::string address;
        int age;
    };

	void to_json(nlohmann::json &j, const person& p) {
        //j = json{{"name", p.name}, {"address", p.address}, {"age", p.age}};

		j["name"] = p.name;
		j["address"] = p.address;
		j["age"] = p.age;
    }

    void from_json(const nlohmann::json &j, person& p) {
        p.name = j.at("name").get<std::string>();
        p.name = j.at("address").get<std::string>();
        p.name = j.at("age").get<int>();
    }
}

class Human {

	public:
		std::string name;
        std::string address;
        int age;

		Human() {}
		~Human() {}

		void to_json(nlohmann::json &j) {
			j["name"] = this->name;
			j["address"] = this->address;
			j["age"] = this->age;
   		}
	    void from_json(const nlohmann::json &j) {
	        this->name = j.at("name").get<std::string>();
	        this->address = j.at("address").get<std::string>();
	        this->age = j.at("age").get<int>();
	    }
		
};


void test6() {

//测试使用结构体形式
	ns::person p = {"Ned Flanders", "744 Evergreen Terrace", 60};

	// 转换至 JSON：将每个值拷贝至 JSON 对象
	nlohmann::json j;
	j["name"] = p.name;
	j["address"] = p.address;
	j["age"] = p.age;

//	std::cout << j.dump(4) << std::endl;

//测试使用类的形式
	Human h;
	nlohmann::json json;
	h.name = "zhaokun";
	h.address = "hebei";
	h.age = 30;

	json["name"] = h.name;
	json["address"] = h.address;
	json["age"] = h.age;

	std::cout << json.dump(4) << std::endl;

/*
	// 转换自 JSON：从 JSON 对象中拷贝每个值
	ns::person p {
	    j["name"].get<std::string>(),
	    j["address"].get<std::string>(),
	    j["age"].get<int>()
	};
*/
}

void test7() {

	ns::person p = {"Ned Flanders", "744 Evergreen Terrace", 70};
	nlohmann::json j = p;

//	std::cout << j.dump(4) << std::endl;

	Human h;
	nlohmann::json json;
	h.name = "zhaokun";
	h.address = "hebei";
	h.age = 30;

	h.to_json(json);

	std::cout << json.dump(4) << std::endl;

	Human h1;

	h1.from_json(json);

	std::cout << "name = " << h1.name << std::endl;
	std::cout << "address = " << h1.address << std::endl;
	std::cout << "age = " << h1.age << std::endl;
}

//nlohmann::json变量中嵌套nlohmann::json类型变量
void test8() {

	nlohmann::json j; //声明一个nlohmann::json类型对象
	nlohmann::json jj;

	j["pi"] = 3.14; //添加整型元素

	jj["name"] = "zhaokun";

	j["person"] = jj;

	std::cout << j.dump(4) << std::endl;
}
int main() {

//	test1();
//	test2();
//	test3();
//	test4();
//	test5();
//	test6();
//	test7();
	test8();
	return 0;
}

