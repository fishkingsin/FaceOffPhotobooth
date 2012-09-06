#include "ofAppGlutWindow.h"
#include "testApp.h"

int main() {
    ofAppGlutWindow window;
#ifdef TARGET_OSX
    window.setGlutDisplayString( "rgb double depth alpha samples>=4");
#endif
	ofSetupOpenGL(&window, 1280,720, OF_WINDOW);
	ofRunApp(new testApp());
}
