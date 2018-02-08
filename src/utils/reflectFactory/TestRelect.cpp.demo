#include "ReflectFactory.h"
#include <stdio.h>

class Human {

	public:
		int id;
};

//把类注册进去
REGISTER_CLASS(Human);

//宏定义中##表示连接两个变量
#define f(a,b) a##b

//宏定义中#表示将变量格式化为字符串
#define g(a) #a

#define h(a) g(a)

int main() {

#if 0
	Human *h = (Human *)ReflectFactory::CreateObject("Human");

	h->id = 10;

	printf("id = %d\n",h->id);
	
	delete h;
	h = NULL;
#endif

//	printf("f(a,b) = %s\n",f(1,2)); //%s这种格式化,编译不能通过 
	printf("f(a,b) = %d\n",f(1,2)); //输出:f(a,b) = 12,格式化字符串只能用%d,而不能用%s
//	printf("f(a,b) = %s\n",f('1','4')); //这种形式也不能编译通过

	printf("g(a) = %s\n",g(10)); //输出:g(a) = 10

	printf("g(a) = %s\n",g(A)); //输出:g(a) = A

	printf("g(a) = %s\n",g("1234")); //输出:g(a) = "1234"

	printf("g(a) = %s\n",g(f(1,2))); //输出:g(a) = f(1,2)

	printf("h(a) = %s\n",h(f(1,2))); //输出:h(a) = 12,解释:h里面的f先展开,结果为12,然后再替代g里面的a

	printf("aaaa""bbbb\n"); //输出:aaaabbbb,两个""相连相当于一个""

	return 0;
}
