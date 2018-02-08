#include "ReflectFactory.h"

std::map<std::string, NewInstancePt> ReflectFactory::dynCreateMap;

void* ReflectFactory::CreateObject(const char * className) {
	
	std::map<std::string, NewInstancePt>::const_iterator it;
	it = dynCreateMap.find(className);
	if(it == dynCreateMap.end()) {
		return NULL;
	}		
	else {
		NewInstancePt np = it->second;
		return np();
	}

}

void ReflectFactory::RegisterClass(const char * className, NewInstancePt np) {

	dynCreateMap[className] = np;
}

Register::Register(const char * className, NewInstancePt np) {

	ReflectFactory::RegisterClass(className, np);

}

