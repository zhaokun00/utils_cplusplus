#include <stdio.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

//编译命令:gcc test.c -o test -llua -lm -ldl
void test1() {

  lua_State *L = luaL_newstate();

  luaL_openlibs(L);

  if(luaL_loadfile(L, "test.lua") || lua_pcall(L, 0,0,0)) {
    printf("error %s\n", lua_tostring(L,-1));
    return;
  }

  lua_getglobal(L,"add");

  lua_pushnumber(L,10);
  lua_pushnumber(L,20);


  if(lua_pcall(L,2,1,0) != 0) { 
    printf("error %s\n", lua_tostring(L,-1));
  }

  double result = lua_tonumber(L,-1);

  printf("result = %lf\n",result); //打印30

  lua_getglobal(L,"sub");

  lua_pushnumber(L,10);
  lua_pushnumber(L,20);


  if(lua_pcall(L,2,1,0) != 0) { 
    printf("error %s\n", lua_tostring(L,-1));
  }

  double result1 = lua_tonumber(L,-1);

  printf("result1 = %lf\n",result1); //打印-10


  double result2 = lua_tonumber(L,-2);

  //原因在于:之前栈中的元素没有被弹出
  printf("result2 = %lf\n",result2); //打印30
  lua_close(L);
}


int main() {

  test1();

  return 0;
}
