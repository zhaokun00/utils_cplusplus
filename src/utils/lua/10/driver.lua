
local Message = { name} --声明类名和类成员变量

function Message : new (n) --声明新建实例的New方法

    local o = {
    --设定各个项的值
     name = n
    }

    setmetatable(o,{__index = self} )--将自身的表映射到新new出来的表中
return o

end

function ziroom_encode(source)

    --print(source.type) --默认会调用属性的get方法
    --source.type = "123" --默认会调用属性的set方法

    --source:play(12);

    --type = source:getType()

    --print("type = ",type)

    --source:setType("ycl")

    --print(source.msg.name)

    --print(source:findMsg().name)

    --a = Message:new("zhaokun")

    --source:sendMsg(a)

    --local a = zm:Message()

    --a.name = "ycl"

    --source:sendMsg(a)

    local list = source:findList()

    print(list[1])
end

