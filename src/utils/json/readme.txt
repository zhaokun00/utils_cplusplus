移植json步骤如下:
1.json-develop\json-develop\single_include\nlohmann\json.hpp复制到路径下(该json是以源码的形式提供)
2.编译该json.hpp必须是4.9以上的编译工具栏,即g++版本要大于等于4.9,而Ubuntu 14.04中工具链的版本为4.8,
  因此要把编译器进行升级,升级步骤如下:
  sudo add-apt-repository ppa:ubuntu-toolchain-r/test
  sudo apt-get update
  sudo apt-get install gcc-4.9
  sudo apt-get install g++-4.9
  
  设置默认的版本为4.9
  sudo su
  cd ../../usr/bin
  
  //设置软连接:
  ln -s /usr/bin/g++-4.9 /usr/bin/g++ -f
  ln -s /usr/bin/gcc-4.9 /usr/bin/gcc -f
3.升级完编译器后,系统中将有两个编译器版本,默认为之前的4.8版本,因此设计到在cmake文件中设置编译器版本
  SET(CMAKE_C_COMPILER "/usr/bin/gcc=4.9")                                                                                                
  SET(CMAKE_CXX_COMPILER "/usr/bin/g++-4.9")