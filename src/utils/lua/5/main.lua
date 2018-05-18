--[[

Lua数据类型
Lua是动态类型语言,变量不需要类型的定义,只需要为变量赋值。值可以存储在变量中,作为参数传递或结果返回

Lua中有8个基本类型分别为:nil、boolean、number、string、userdata、function、thread和table
--]]

--nil类型
a = nil;--表示一个无效值,在条件表达式中相当于false

--用关键字type来判断数据类型
print("a = ",a,"type = ",type(a))

--number类型
b = 123.8

print("b = ",b)

--string类型
c = "zhaokun"

print("c = ",c)

--table类型
--[[
table是lua的一种数据结构用来帮助我们创建不同的数据类型,如数组、字典等
lua table使用关联数组,可以用任意类型的值作为数组的索引,但这个值不能是nil
lua table是不固定的,可以根据自己的需要进行扩容
table的下标是从1开始的
--]]

d = {"zhao","qian","sun"}
d["key"] = "value"
print("d[0] = ",d[1],"d[key] = ",d["key"])

--将table赋值为nil即销毁该table
--d = nil;
--print("d[0] = ",d[1],"d[key] = ",d["key"])

--定义了function函数
function add(a,b)

  local c = a + b --定义了局部变量c
  d = a -b --定义了全局变量d
  return c	 
end

--调用function函数
print("c = ",add(1,2),"d = ",d)

--for循环语句
--[[
格式:

for var=exp1,exp2,exp3 do  
    <执行体>  
end  

参数说明:
var从exp1变化到exp2，每次变化以exp3为步长递增var，并执行一次"执行体"。exp3是可选的，如果不指定，默认为1。
--]]

--普通for循环语句
for i=0,10,1 do

print("i = ",i);
end

--泛型for循环
--[[
格式:

--打印数组a的所有值  
for i,v in ipairs(a) 
    do print(v) 
end 

i是数组索引值，v是对应索引的数组元素值。ipairs是Lua提供的一个迭代器函数，用来迭代数组

--]]
days = {"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"}

for key,value in ipairs(days) do

 print("key = ",key,"value = ",value)
end

print("************************************************************");
for key,value in pairs(days) do

 print("key = ",key,"value = ",value)
end

local tab= { 
[1] = "a", 
[6] = "b", 
[3] = "c",
[5] = "f",
}

for i,v in pairs(tab) do        -- 输出 "a" ,"b", "c"  ,
    print("key = ",i,"value = ",v) 
end 

--for i,v in ipairs(tab) do    -- 输出 "a" ,k=2时断开 
--    print( tab[i] ) 
--end

