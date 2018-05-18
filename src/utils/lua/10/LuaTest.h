//
// Created by zjs on 18-5-10.
//

#ifndef ZGATEWAY_LUATEST_H
#define ZGATEWAY_LUATEST_H


#include <string>
#include <sstream>
#include <fstream>
#include <climits>

#include "ScriptEngine.h"

using namespace iot::lua;

class LuaTest {
    static std::string readTextFile(const char *fileName) {
        std::ifstream t(fileName);
        std::stringstream buffer;
        buffer << t.rdbuf();
        std::string contents(buffer.str());
        return contents;
    }

public:
    static void test1() {

	//驱动脚本
        std::string script = readTextFile("../driver.lua");

	    ScriptBuffer msg;

        ScriptEngine engine;

        engine.encode(script, &msg);
    }
};


#endif //ZGATEWAY_LUATEST_H
