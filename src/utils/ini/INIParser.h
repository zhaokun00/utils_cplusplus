#ifndef INI_PARSER_H
#define INI_PARSER_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <map>

/*
定义ini文件的node节点,格式为
[root]
key = value

例如下面的形式:

[student]
name = zhaokun
sex = 男

*/
class ININode
{
public:
    ININode(std::string root, std::string key, std::string value)
    {
        this->root = root;
        this->key = key;
        this->value = value;
    }
	
    std::string root; 			//root元素
    std::string key;  			//标签key值
    std::string value;			//标签value值
};

//定义root节点下的子节点
class SubNode
{
public:
    void InsertElement(std::string key, std::string value)
    {
        sub_node.insert(std::pair<std::string, std::string>(key, value));
    }

    std::map<std::string, std::string> sub_node; //存放子节点的map链表
};

class INIParser
{
public:
    int ReadINI(std::string path);    //读取配置文件
    int WriteINI(std::string path);   //写配置文件
    std::string GetValue(std::string root, std::string key); //获取元素的值
    void SetValue(std::string root, std::string key, std::string value); //写节点
	std::vector<ININode>::size_type GetSize(){return map_ini.size();} //获取节点的数量
    void Clear(){map_ini.clear();} //清空节点

private:
    std::string &TrimString(std::string &str);
    std::map<std::string, SubNode> map_ini; //存放节点的map

};

#endif // INI_PARSER_H
