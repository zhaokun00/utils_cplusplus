#include "Hello.h"

//命名空间函数前面定义
HelloService::Hello::Hello() {

	std::cout << "Hello 构造函数" << std::endl;
}

HelloService::Hello::~Hello() {
	std::cout << "Hello 析构函数" << std::endl;
}

void HelloService::Hello::hello() {
	std::cout << "hello" << std::endl;
}

HelloService::GoodByeService::GoodBye::GoodBye() {
	std::cout << "GoodBye 构造函数" << std::endl;
}

HelloService::GoodByeService::GoodBye::~GoodBye() {
	std::cout << "GoodBye 析构函数" << std::endl;
}

void HelloService::GoodByeService::GoodBye::goodBye() {
	std::cout << "goodBye" << std::endl;
}

//命名空间在最开始进行定义
namespace audioService {

namespace mediaPlayer {

MediaPlayer::MediaPlayer() {

}

MediaPlayer::~MediaPlayer() {

}

void MediaPlayer::play() {

	std::cout << "play" << std::endl;
}

}
}