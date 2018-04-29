#include "WrapperXml.h"

int WrapperXml::createXml(std :: string xmlPath, std :: string rootName) {

	std::string declaration = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>";

	XMLDocument doc;

	doc.Parse(declaration.c_str());

	XMLElement * root = doc.NewElement(rootName.c_str());

	doc.InsertEndChild(root);

	return doc.SaveFile(xmlPath.c_str());
}

int WrapperXml::insertXmlNode(std :: string xmlPath, const User & user) {

	XMLDocument doc;

	int ret = doc.LoadFile(xmlPath.c_str());

	if(0 != ret) {
		std::cout << "load xmlPath failed" << std::endl;
	}

	//获取根元素
	XMLElement * root = doc.RootElement();

	XMLElement * userNode = doc.NewElement("User");

	userNode->SetAttribute("Name",user.m_userName.c_str()); //设置属性值
	userNode->SetAttribute("Password",user.m_password.c_str());

	root->InsertEndChild(userNode);

	XMLElement* gender = doc.NewElement("Gender");
    XMLText* genderText=doc.NewText(std::to_string(user.m_gender).c_str());
    gender->InsertEndChild(genderText);
    userNode->InsertEndChild(gender);

    XMLElement* mobile = doc.NewElement("Mobile");
    mobile->InsertEndChild(doc.NewText(user.m_mobile.c_str()));
    userNode->InsertEndChild(mobile);

    XMLElement* email = doc.NewElement("Email");
    email->InsertEndChild(doc.NewText(user.m_email.c_str()));
    userNode->InsertEndChild(email);

    return doc.SaveFile(xmlPath.c_str());
}

XMLElement* WrapperXml::queryUserNodeByName(XMLElement* root,const std::string& userName) {

    XMLElement* userNode = root->FirstChildElement("User");
	
    while(userNode != NULL) {
        if(userNode->Attribute("Name") == userName)
            break;
        userNode=userNode->NextSiblingElement();//下一个兄弟节点
    }
    return userNode;
}

User* WrapperXml::queryUserByName(const std::string xmlPath,const std::string& userName) {

    XMLDocument doc;
	
    if(doc.LoadFile(xmlPath.c_str())!=0) {
        std::cout<<"load xml file failed"<<std::endl;
        return NULL;
    }
	
    XMLElement* root = doc.RootElement();	
    XMLElement* userNode = queryUserNodeByName(root,userName);

    if(NULL != userNode) {
        User* user = new User();
        user->m_userName = userName;
        user->m_password = userNode->Attribute("Password"); //获取属性值
        XMLElement* genderNode = userNode->FirstChildElement("Gender"); //获取某个某个元素
        user->m_gender = std::stoi(genderNode->GetText()); //将字符串转换为整型值
        XMLElement* mobileNode = userNode->FirstChildElement("Mobile");
        user->m_mobile = mobileNode->GetText(); //获取元素中的数据     
        XMLElement* emailNode = userNode->FirstChildElement("Email");
        user->m_email=emailNode->GetText();
		
        return user;
    }

    return NULL;
}

bool WrapperXml::updateUser(const std::string xmlPath,User* user) {

    XMLDocument doc;
    if(doc.LoadFile(xmlPath.c_str())!=0) {
        std::cout<<"load xml file failed"<<std::endl;
        return false;
    }
    XMLElement* root=doc.RootElement();
    XMLElement* userNode=queryUserNodeByName(root,user->m_userName);

    if(NULL != userNode) {
		
        if(user->m_password != userNode->Attribute("Password")) {
            userNode->SetAttribute("Password",user->m_password.c_str());  //修改属性
        }
		
        XMLElement* genderNode = userNode->FirstChildElement("Gender");
        if(user->m_gender != std::stoi(genderNode->GetText())) {
            genderNode->SetText(std::to_string(user->m_gender).c_str());   //修改节点内容
        }
		
        XMLElement* mobileNode = userNode->FirstChildElement("Mobile");
        if(user->m_mobile!=mobileNode->GetText()) {
            mobileNode->SetText(user->m_mobile.c_str());
        }
		
        XMLElement* emailNode = userNode->FirstChildElement("Email");
        if(user->m_email!=emailNode->GetText()) {
            emailNode->SetText(user->m_email.c_str());
        }
		
        if(doc.SaveFile(xmlPath.c_str())==0) {
			return true;
		}           
    }
	
    return false;
}

bool WrapperXml::deleteUserByName(const std::string xmlPath,const std::string& userName) {

    XMLDocument doc;
    if(doc.LoadFile(xmlPath.c_str())!=0) {
        std::cout<<"load xml file failed"<<std::endl;
        return false;
    }
	
    XMLElement* root = doc.RootElement();
    //doc.DeleteNode(root);//删除xml所有节点
    XMLElement* userNode = queryUserNodeByName(root,userName);

    if(userNode != NULL) {
		#if 0
        userNode->DeleteAttribute("Password");//删除属性
        XMLElement* emailNode = userNode->FirstChildElement("Email");
        userNode->DeleteChild(emailNode); //删除指定节点
        //userNode->DeleteChildren();//删除节点的所有孩子节点
		#endif

		root->DeleteChild(userNode);
        if(doc.SaveFile(xmlPath.c_str())==0)
            return true;
    }
    return false;
}


