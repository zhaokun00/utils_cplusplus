#ifndef DEVICE_DEVICEMANAGER_H_
#define DEVICE_DEVICEMANAGER_H_

#include <map>
#include "IDeviceController.h"

namespace iot {

    namespace device {

        class DeviceManager {

	public:
	    DeviceManager() {std::cout << "DeviceManager构造函数" << std::endl;}
            ~DeviceManager() {std::cout << "DeviceManager析构函数" << std::endl;}
           std::shared_ptr<Device> m_deviceList; //设备链表:设备mic地址、设备信息

        };
    }
}
#endif

