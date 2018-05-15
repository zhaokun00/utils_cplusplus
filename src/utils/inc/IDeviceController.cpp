#include "IDeviceController.h"
#include "DeviceManager.h"

namespace iot {

	namespace device {
		
		IDeviceController::IDeviceController() {
			std::cout << "IDeviceController构造函数" << std::endl;
		}

		IDeviceController::~IDeviceController() {
			std::cout << "IDeviceController析构函数" << std::endl;
		}

		void IDeviceController::control(std::shared_ptr<IDeviceController> controller) {
			
			m_controller = controller;			
		}
	}
}
