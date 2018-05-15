#ifndef DEVICE_IDEVICECONTROLLER_H_
#define DEVICE_IDEVICECONTROLLER_H_

#include <iostream>
#include <string>
#include <memory>
#include "Device.h"

namespace iot {

    namespace device {

	//引用Device类
	class Device;

        class IDeviceController {
        public:
	    IDeviceController();
            virtual ~IDeviceController();		
            void control(std::shared_ptr<IDeviceController> controller);

	    Device * m_device;
	    //不能是std::shared_ptr形式
	    std::weak_ptr<IDeviceController> m_controller;
        };
    }
}

#endif


