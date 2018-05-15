#ifndef DEVICE_DEVICE_H_
#define DEVICE_DEVICE_H_

#include <iostream>
#include <string>
#include <memory>
#include "IDeviceController.h"

namespace iot {

    namespace device {

	//引用IDeviceController类
	class IDeviceController;

	class Device {

        public:
            Device() {
		m_mac = "zhaokun";
		std::cout << "Device构造函数" << std::endl;
	    }
            ~Device() {std::cout << "Device析构函数" << std::endl;}
            std::string m_mac;
            IDeviceController * m_controller;
        };
    }
}

#endif


