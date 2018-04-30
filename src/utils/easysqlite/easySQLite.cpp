// easySQLite.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
//http://www.codeweblog.com/easysqlite%E4%BD%BF%E7%94%A8%E5%B8%AE%E5%8A%A9/
//https://blog.csdn.net/e5Max/article/details/42917467
//https://blog.csdn.net/wangshubo1989/article/details/52576556
#include <iostream>
#include "easySQLite/SqlCommon.h"
#include "easySQLite/SqlField.h"
#include "easySQLite/SqlDatabase.h"
#include "easySQLite/SqlTable.h"
//#include "UnitTest++/src/UnitTest++.h"

using namespace sql;

void example1()
{
	//define table structure
	Field definition_tbPerson[] = 
	{
		Field(FIELD_KEY),
		Field("fname", type_text, flag_not_null),
		Field("lname", type_text, flag_not_null),
		Field("birthdate", type_time),
		Field(DEFINITION_END),
	};

	//define database object
	sql::Database db;

	try
	{
		//open database file
		db.open("test.db");

		//define table object
		Table tbPerson(db.getHandle(), "person", definition_tbPerson);

		//remove table from database if exists
		if (tbPerson.exists())
			tbPerson.remove();

		//create new table
		tbPerson.create();

		//define new record
		Record record(tbPerson.fields());

		//set record data
		record.setString("fname", "Jan");
		record.setString("lname", "Kowalski");
		record.setTime("birthdate", time::now());

		//add 10 records
		for (int index = 0; index < 10; index++)
			tbPerson.addRecord(&record);

		//select record to update
		if (Record* record = tbPerson.getRecordByKeyId(7))
		{
			record->setString("fname", "Frank");
			record->setString("lname", "Sinatra");
			record->setNull("birthdate");

			tbPerson.updateRecord(record);
		}

		//load all records
		tbPerson.open();

		//list loaded records
		for (int index = 0; index < tbPerson.recordCount(); index++)
			if (Record* record = tbPerson.getRecord(index))
				sql::log(record->toString());

		sql::log("");
		sql::log("ALL OK");

	} catch (Exception e) {
		printf("ERROR: %s\r\n", e.msg().c_str());
	}
}

void example2()
{
	Field definition_tbTest[] = 
	{
		Field(FIELD_KEY),
		Field("name", type_text, flag_not_null),
		Field("valueInt", type_int),
		Field("valueDbl", type_float),
		Field("valueTxt", type_text),
		Field("valueBol", type_bool, flag_not_null),
		Field("valueTme", type_time),
		Field(DEFINITION_END),
	};

	Database db;

	try
	{
		db.open("test.db");

		Table source(db.getHandle(), "tbTest", definition_tbTest);

		if (source.exists())
			source.remove();

		source.create();

		for (int index = 0; index < 5; index++)
		{
			Record record(source.fields());

			record.setString("name", "test_name_1");
			record.setInteger("valueInt", 1234567890);
			record.setDouble("valueDbl", 12345.67890);
			record.setString("valueTxt", "test'value'1");
			record.setBool("valueBol", true);
			record.setTime("valueTme", time::now());

			source.addRecord(&record);
		}

		if (Table* target = Table::createFromDefinition(db.getHandle(), "_backup_" + source.name(), source.fields()->getDefinition()))
		{
			if (target->backup(source))
			{
				sql::log("");
				sql::log("ALL OK");
			} else {
				sql::log(source.errMsg());
				sql::log(target->errMsg());
			}

			delete target;
		}
	} catch (Exception e) {
		printf("ERROR: %s\r\n", e.msg().c_str());
	}
}

/*
	1.easySqlite是一个sqlite的C++封装库,非常简洁,适合于小型项目.
	2.easySqlite是一个sqliteC数据引擎接口的C++封装库,它的结构非常简单,可以很轻松的通过它来操作本地数据库文件
*/

//测试打开数据库,关闭数据库
void test1() {

	sql::Database db;

	//打开数据库
	bool ret = db.open("../test.db");

	std::cout << "ret = " << ret << std::endl;

	//关闭数据库
	db.close();
}

//测试创建一个表

void test2() {

	sql::Database db;

	/*
		表的结构
		Field构造函数默认需要如下参数
		name:字段名
		type:字段类型
		flags:是否为空

		表定义必须以Field(FIELD_KEY)开头,以Field(DEFINITON_END)结尾
		Field(FIELD_KEY)可以看做Field("_ID",type_int,flag_primary_key)
	*/
	Field definition_tbPerson[] = {
		Field(FIELD_KEY),
		Field("id",type_int, flag_not_null),
		Field("name", type_text, flag_not_null),
		Field("score",type_text, flag_not_null),
		Field("birthdate", type_time),
		Field(DEFINITION_END),
	};
		

	//打开数据库
	bool ret = db.open("../test.db");

	Table tbPerson(db.getHandle(), "person", definition_tbPerson); //创建一个表

	tbPerson.create(); //创建一个表
	
	db.close();
}

//测试插入数据
void test3() {

	sql::Database db;

	/*
		表的结构
		Field构造函数默认需要如下参数
		name:字段名
		type:字段类型
		flags:是否为空

		表定义必须以Field(FIELD_KEY)开头,以Field(DEFINITON_END)结尾
		Field(FIELD_KEY)可以看做Field("_ID",type_int,flag_primary_key)
	*/
	Field definition_tbPerson[] = {
		Field(FIELD_KEY),
		Field("id",type_int, flag_not_null),
		Field("name", type_text, flag_not_null),
		Field("score",type_float, flag_not_null),
		Field("birthdate", type_time),
		Field(DEFINITION_END),
	};
		

	//打开数据库
	bool ret = db.open("../test.db");

	Table tbPerson(db.getHandle(), "person", definition_tbPerson); //定义一个表结构

/*
	if(true == tbPerson.exists()) { //判断表是否存在,如果存在直接打开表,如果不存在则创建表	
		tbPerson.open(); //加载
	}
	else {
		tbPerson.create(); //创建表
	}

	tbPerson.create(); //创建一个表
*/	
	Record record(tbPerson.fields()); //定义插入数据的结构
	std::string name[] = {"zhao","qian","sun","li","zhou"};
	
	for(int i=1; i<=5; i++) {
		//设置数据
		record.setInteger("id", i);
		record.setString("name", name[i-1]);
		record.setDouble("score", i + 1.0);
		record.setTime("birthdate", time::now());

		tbPerson.addRecord(&record); //添加数据
	}

	db.close();

}

//测试查询所有数据
void test4() {

	sql::Database db;

	/*
		表的结构
		Field构造函数默认需要如下参数
		name:字段名
		type:字段类型
		flags:是否为空

		表定义必须以Field(FIELD_KEY)开头,以Field(DEFINITON_END)结尾
		Field(FIELD_KEY)可以看做Field("_ID",type_int,flag_primary_key)
	*/
	Field definition_tbPerson[] = {
		Field(FIELD_KEY),
		Field("id",type_int, flag_not_null),
		Field("name", type_text, flag_not_null),
		Field("score",type_float, flag_not_null),
		Field("birthdate", type_time),
		Field(DEFINITION_END),
	};
		

	//打开数据库
	bool ret = db.open("../test.db");

	Table tbPerson(db.getHandle(), "person", definition_tbPerson); //定义一个表结构

	int count = tbPerson.recordCount();

	std::cout << "count = " << count << std::endl; //在没有打开表是,数据记录为0条

	tbPerson.open(); //在pen时会遍历所有的数据记录
	count = tbPerson.recordCount();

	std::cout << "count = " << count << std::endl;

	for(int i=0; i<count; i++) {
		Record * record = tbPerson.getRecord(i);

		//获取记录中的字段值
		std::cout << "id = " << record->getValue("id")->asInteger() << std::endl;
		std::cout << "name = " << record->getValue("name")->asString() << std::endl;
		std::cout << "score = " << record->getValue("score")->asDouble() << std::endl;
		std::cout << "birthdate = " << record->getValue("birthdate")->asTime().asString() << std::endl;
	}	
}

//测试根据主键查询
void test5() {

	sql::Database db;

	Field definition_tbPerson[] = {
		Field(FIELD_KEY),
		Field("id",type_int, flag_not_null),
		Field("name", type_text, flag_not_null),
		Field("score",type_float, flag_not_null),
		Field("birthdate", type_time),
		Field(DEFINITION_END),
	};
		

	//打开数据库
	bool ret = db.open("../test.db");

	Table tbPerson(db.getHandle(), "person", definition_tbPerson); //定义一个表结构

	tbPerson.open();

	//根据主键查询数据记录
	Record* record = tbPerson.getRecordByKeyId(1);

	//获取记录中的字段值
	std::cout << "id = " << record->getValue("id")->asInteger() << std::endl;
	std::cout << "name = " << record->getValue("name")->asString() << std::endl;
	std::cout << "score = " << record->getValue("score")->asDouble() << std::endl;
	std::cout << "birthdate = " << record->getValue("birthdate")->asTime().asString() << std::endl;

	db.close();
}

//根据sql语句查询
void test6() {

sql::Database db;

	Field definition_tbPerson[] = {
		Field(FIELD_KEY),
		Field("id",type_int, flag_not_null),
		Field("name", type_text, flag_not_null),
		Field("score",type_float, flag_not_null),
		Field("birthdate", type_time),
		Field(DEFINITION_END),
	};
		

	//打开数据库
	bool ret = db.open("../test.db");

	Table tbPerson(db.getHandle(), "person", definition_tbPerson); //定义一个表结构

	std::string queryStr = "select * from person where id = 2" ;

	//穿入sql语句进行查询
	tbPerson.query(queryStr);

	std::cout << "count = " << tbPerson.recordCount() << std::endl;

	for(int i=0; i<tbPerson.recordCount(); i++) {
		Record * record = tbPerson.getRecord(i);

		//获取记录中的字段值
		std::cout << "id = " << record->getValue("id")->asInteger() << std::endl;
		std::cout << "name = " << record->getValue("name")->asString() << std::endl;
		std::cout << "score = " << record->getValue("score")->asDouble() << std::endl;
		std::cout << "birthdate = " << record->getValue("birthdate")->asTime().asString() << std::endl;
	}
}

//测试修改数据
void test7() {

	sql::Database db;

	Field definition_tbPerson[] = {
		Field(FIELD_KEY),
		Field("id",type_int, flag_not_null),
		Field("name", type_text, flag_not_null),
		Field("score",type_float, flag_not_null),
		Field("birthdate", type_time),
		Field(DEFINITION_END),
	};
		

	//打开数据库
	bool ret = db.open("../test.db");

	Table tbPerson(db.getHandle(), "person", definition_tbPerson); //定义一个表结构

	tbPerson.open();

	//根据主键查询数据记录
	Record* record = tbPerson.getRecordByKeyId(1);

	record->setInteger("id", 100);
	record->setDouble("score", 12.3);

	//更新数据
	tbPerson.updateRecord(record);
	
	db.close();

}

//删除数据
void test8() {

	sql::Database db;

	Field definition_tbPerson[] = {
		Field(FIELD_KEY),
		Field("id",type_int, flag_not_null),
		Field("name", type_text, flag_not_null),
		Field("score",type_float, flag_not_null),
		Field("birthdate", type_time),
		Field(DEFINITION_END),
	};
		

	//打开数据库
	bool ret = db.open("../test.db");

	Table tbPerson(db.getHandle(), "person", definition_tbPerson); //定义一个表结构

//	tbPerson.open();

	std::string str = "id=100";
	if(!tbPerson.deleteRecords(str)) { //删除记录
		std::cout << "msg:=" << db.errMsg() << std::endl;
	}
	
	db.close();
}

//测试删除表格
int main() {

//	test1();
//	test2();
//	test3();
//	test4();
//	test5();
//	test6();
//	test7();
//	test8();

	return 0;
}
