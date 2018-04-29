#ifndef WrapperXml_H__
#define WrapperXml_H__

	#include <stdio.h>
	#include <stdlib.h>
	#include <iostream>
	#include <chrono>
	#include <string>
	#include "tinyxml2.h"
	
	using namespace tinyxml2;

	class User {
		public:
			User() :
				m_userName(""),
				m_password(""),
				m_gender(0),
				m_mobile(""),
				m_email("") {}

		   User(const std::string& userName, const std::string& password, int gender, const std::string& mobile, const std::string& email) {
				m_userName = userName;
				m_password = password;
				m_gender = gender;
				m_mobile = mobile;
				m_email = email;
		   }
		   
			~User() {}
			
			std::string m_userName;
			std::string m_password;
			int m_gender;
			std::string m_mobile;
			std::string m_email;
	};

	class WrapperXml {

		public:
			static int createXml(std::string xmlPath,std::string rootName);
			static int insertXmlNode(std::string xmlPath,const User& user);
			static XMLElement* queryUserNodeByName(XMLElement* root,const std::string& userName);
			static User* queryUserByName(const std::string xmlPath,const std::string& userName);
			static bool updateUser(const std::string stxmlPath,User* user);
			static bool deleteUserByName(const std::string xmlPath,const std::string& userName);

	};
#endif

