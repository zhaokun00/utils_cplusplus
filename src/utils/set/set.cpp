#include <stdio.h>
#include <iostream>
#include <set>
#include <chrono>
/*
	set的特性是,所有元素都会根据元素的键值自动排序,set的元素不像map那样可以同时拥有实值(value)和
	键值(key),set元素的键值就是实值,实值就是键值,set不允许两个元素有相同的键值

	set的各成员函数列表如下:
	1. begin()--返回指向第一个元素的迭代器

	2. clear()--清除所有元素

	3. count()--返回某个值元素的个数

	4. empty()--如果集合为空，返回true

	5. end()--返回指向最后一个元素的迭代器

	6. equal_range()--返回集合中与给定值相等的上下限的两个迭代器

	7. erase()--删除集合中的元素

	8. find()--返回一个指向被查找到元素的迭代器

	9. get_allocator()--返回集合的分配器

	10. insert()--在集合中插入元素

	11. lower_bound()--返回指向大于（或等于）某值的第一个元素的迭代器

	12. key_comp()--返回一个用于元素间值比较的函数

	13. max_size()--返回集合能容纳的元素的最大限值

	14. rbegin()--返回指向集合中最后一个元素的反向迭代器

	15. rend()--返回指向集合中第一个元素的反向迭代器

	16. size()--集合中元素的数目

	17. swap()--交换两个集合变量

	18. upper_bound()--返回大于某个值元素的迭代器

	19. value_comp()--返回一个用于比较元素间的值的函数
*/
//测试set中的基本类型
void test1() {

	std::set<std::string> s1;

	std::pair<std::set<std::string>::iterator,bool> it = s1.insert("zhao"); //插入元素

	if(true == it.second) {
		std::cout << "第一次插入数据成功" << std::endl;
		std::cout << "插入的数据 = " << *it.first << std::endl;
	}

	it = s1.insert("zhao");

	if(true == it.second) {
		std::cout << "第二次插入数据成功" << std::endl;
	}
	else {
		std::cout << "第二次插入数据失败" << std::endl;

	}
	
	auto itt = s1.insert("qian");

	for(auto r = s1.begin();r != s1.end();r++) { //遍历所有元素
		std::cout << "value = " << *r << std::endl;
	}

	auto value = s1.find("zhaokun"); //查找元素

	if(value != s1.end()) {
		std::cout << "找到了该元素" << std::endl;
	}
	else {
		std::cout << "没有找到该元素" << std::endl;
	}
}
int main() {

	test1();
//	test2();
//	test3();
//	test4();
//	test5();
//	test6();

	return 0;
}

