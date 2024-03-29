#ifndef HELLO_H___

	#include <iostream>
	#define HELLO_H___

namespace HelloService {

	class Hello {
	
		public:
			Hello();
			~Hello();
	
			void hello();
	};

	namespace GoodByeService {

		class GoodBye {

			public:
				GoodBye();
				~GoodBye();

				void goodBye();
		};
	}

}

namespace audioService {

	namespace mediaPlayer {

		class MediaPlayer {
			public:
				MediaPlayer();
				~MediaPlayer();
				void play();
		};
	}
}
#endif
