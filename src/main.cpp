#include "ofApp.h"


int main()
{
    ofGLWindowSettings settings;
    settings.setGLVersion(3, 2);
    //settings.windowMode = OF_FULLSCREEN;

    settings.setSize(600, 600);
    shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(settings);

    ofGLWindowSettings settings2;
    settings2.setGLVersion(3, 2);
    settings2.setSize(300, 300);
    // uncomment next line to share main's OpenGL resources with gui
    //settings.shareContextWith = mainWindow;	
    shared_ptr<ofAppBaseWindow> guiWindow = ofCreateWindow(settings2);
    guiWindow->setVerticalSync(false);

    shared_ptr<ofApp> mainApp(new ofApp);
    mainApp->setupGui();
    ofAddListener(guiWindow->events().draw, mainApp.get(), &ofApp::drawGui);

    ofRunApp(mainWindow, mainApp);
    ofRunMainLoop();

    
}
