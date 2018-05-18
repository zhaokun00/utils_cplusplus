#ifndef ZJS_SCRIPTENGINE_H
#define ZJS_SCRIPTENGINE_H

#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
#include <memory>
#include <list>
#include <lua.hpp>

#include "LuaBridge.h"

namespace iot {
    namespace lua {

        using namespace luabridge;

        class Message {

        public:
            Message() {
                name = "message22";
            }

            ~Message() {}

            std::string name;

            std::string getName() const {

                std::cout << "getName" << std::endl;
                return name;
            }

            void setName(std::string t) {
                std::cout << "setName" << std::endl;
                name = t;
                std::cout << name << std::endl;
            }
        };

        class ScriptBuffer {

        public:
            ScriptBuffer() {

                type = "zhaokun";

                msg = new Message();

                list.push_back(msg);
            }

            ~ScriptBuffer() {}

            void play(int data) {
                std::cout << "data = " << data << std::endl;

                std::cout << type << std::endl;
            }

            Message * findMsg() {

                return msg;
            }

            void sendMsg(Message * msg) {
                std::cout << "send Msg" << std::endl;

                std::cout << msg->name << std::endl;
            }

            std::list<Message *> findList() {

                std::cout << "findList" << std::endl;

                std::cout << list.front()->name << std::endl;

                //std::cout << list << std::endl;

                printf("%p\n",list.front());
                return list;
            }
            std::list<Message *> list;

            std::string type;

            Message *msg;

            std::string getType() const {

                std::cout << "getType" << std::endl;
                return type;
            }

            void setType(std::string t) {

                std::cout << "setType" << std::endl;
                type = t;
                std::cout << type << std::endl;
            }
        };

        class ScriptEngine {
        private:
            lua_State *L;
        public:

            /*
             * 注册ScriptBuffer class for lua to use
             */
            void registerClass() {

                getGlobalNamespace(L)

                        .beginNamespace("zm")

                        .beginClass<ScriptBuffer>("ScriptBuffer")
                        .addProperty<std::string>("type", &ScriptBuffer::getType, &ScriptBuffer::setType)
                        .addFunction("play", &ScriptBuffer::play)
                        .addFunction("getType", &ScriptBuffer::getType)
                        .addFunction("setType", &ScriptBuffer::setType)
                        .addFunction("findMsg", &ScriptBuffer::findMsg)
                        .addFunction("sendMsg", &ScriptBuffer::sendMsg)
                        .addFunction("findList", &ScriptBuffer::findList)
                        .addData("msg",&ScriptBuffer::msg)
                        .endClass()

                        .beginClass<Message>("Message")
                        .addConstructor <void (*) (void)> ()
                        .addProperty<std::string>("name", &Message::getName, &Message::setName)
                        .endClass()

                        .endNamespace();
            }

            ScriptEngine() {
                L = luaL_newstate();
                luaL_openlibs(L);
                registerClass();
            }

            ~ScriptEngine() {
                if (L != NULL) {
                    lua_close(L);
                }
            }

            void encode(std::string script, ScriptBuffer *pSource) {
                int error = luaL_loadbuffer(L, script.c_str(), script.length(), "endecode") || lua_pcall(L, 0, 0, 0);

                if (error) {
                    fprintf(stderr, "%s", lua_tostring(L, -1));    // 从栈顶取出错误信息打印。
                }
                luabridge::LuaRef encoder = luabridge::getGlobal(L, "ziroom_encode");
                encoder(pSource);

            }

        };
    }
}


#endif //ZJS_SCRIPTENGINE_H
