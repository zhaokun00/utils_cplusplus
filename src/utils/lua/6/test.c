#include <stdio.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <math.h>

/*
lua可以将C函数注册到lua中,C函数必须遵循统一的原型,这个函数原型在lua.h中,
typedef int (*) (lua_State *)

用C函数扩展lua时,一般将所有的C函数编译成一个独立的模块,方便增加新的函数
*/

//定义lua内调用函数的实体
/*
每个被lua调用的C函数都有自己的私有栈,压入参数的索引从1开始递增,结果值也是直接压入
栈中,函数返回时会将压入的参数全部删除,只留下结果值
*/
static int myadd(lua_State *L){
                int a = luaL_checknumber(L, 1);
                int b = luaL_checknumber(L, 2);
                lua_pushnumber(L, a+b);
                return 1;
}

/*
mylib[]声明了模块中所有的C函数列表,每一项映射了C函数在lua的命名,比如代码中myadd函数在lua中用add表示,
列表必须用{NULL,NULL}结束。luaL_newlib在栈中创建一个table,将mylib数组中的C函数注册进这个table中。
luaopen_mylib将这个table的函数加载进lua环境中
*/
static const struct luaL_Reg mylib [] = {
        {"add", myadd}, 
        {NULL, NULL}
};

//lua代码中,reqiure会查找mylib.so，并调用该链接库中的luaopen_mylib,luaopen_的后缀必须与动态库名字一致,这是由require查找函数方式决定的
int luaopen_mylib(lua_State *L) {
  luaL_newlib(L, mylib);
  return 1;
}

/*
编译动态库的方式:gcc -shared -fPIC -o mylib.so mylib.c -llua -lm -ldl
*/


