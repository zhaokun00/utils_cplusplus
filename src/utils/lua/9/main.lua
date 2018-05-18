--创建一个类，表示四边形
local RectAngle = { length, width, area} --声明类名和类成员变量

     function RectAngle: new (len,wid) --声明新建实例的New方法
        local o = {
        --设定各个项的值
         length = len or 0,
         width = wid or 0,
         area =len*wid
        }
        setmetatable(o,{__index = self} )--将自身的表映射到新new出来的表中
        return o
    end
    
    function RectAngle:getInfo()--获取表内信息的方法
        return self.length,self.width,self.area
    end


a = RectAngle:new(10,20)
print(a:getInfo())      -- 输出：10    20    200
b = RectAngle:new(10,10)
print(b:getInfo())      -- 输出：10    10    100
print(a:getInfo())      -- 输出：10    20    200
