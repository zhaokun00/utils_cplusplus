
project(TestFun)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/fun
)

message(WARNING ${CMAKE_CURRENT_SOURCE_DIR})
message(WARNING ${CMAKE_SOURCE_DIR})

set(SRC_LIST
    ./fun.cpp
	)
ADD_LIBRARY(fun SHARED ${SRC_LIST})

