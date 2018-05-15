#include <string>
#include "IDeviceController.h"

using namespace iot::device;

int main() {

	std::shared_ptr<Device> device(new Device());

	std::shared_ptr<IDeviceController> controller(new IDeviceController());


	controller->control(controller);
	return 0;
}
