��ֲjson��������:
1.json-develop\json-develop\single_include\nlohmann\json.hpp���Ƶ�·����(��json����Դ�����ʽ�ṩ)
2.�����json.hpp������4.9���ϵı��빤����,��g++�汾Ҫ���ڵ���4.9,��Ubuntu 14.04�й������İ汾Ϊ4.8,
  ���Ҫ�ѱ�������������,������������:
  sudo add-apt-repository ppa:ubuntu-toolchain-r/test
  sudo apt-get update
  sudo apt-get install gcc-4.9
  sudo apt-get install g++-4.9
  
  ����Ĭ�ϵİ汾Ϊ4.9
  sudo su
  cd ../../usr/bin
  
  //����������:
  ln -s /usr/bin/g++-4.9 /usr/bin/g++ -f
  ln -s /usr/bin/gcc-4.9 /usr/bin/gcc -f
3.�������������,ϵͳ�н��������������汾,Ĭ��Ϊ֮ǰ��4.8�汾,�����Ƶ���cmake�ļ������ñ������汾
  SET(CMAKE_C_COMPILER "/usr/bin/gcc=4.9")                                                                                                
  SET(CMAKE_CXX_COMPILER "/usr/bin/g++-4.9")