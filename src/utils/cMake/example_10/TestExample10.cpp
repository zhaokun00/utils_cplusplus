#include <stdio.h>
#include <memory>

using namespace std;

int main() {

	int a = 10;
    std::shared_ptr<int> ptra = std::make_shared<int>(a);
	printf("Hello CMake!\n");
	
	return 0;
}

