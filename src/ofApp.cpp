#include "ofApp.h"

//NOTE: if you are unable to connect to your device on OS X, try unplugging and replugging in the power, while leaving the USB connected.
//ofxKinectV2 will only work if the NUI Sensor shows up in the Superspeed category of the System Profiler in the USB section.

//On OS X if you are not using the example project. Make sure to add OpenCL.framework to the Link Binary With Library Build Phase 
//and change the line in Project.xcconfig to OTHER_LDFLAGS = $(OF_CORE_LIBS) $(OF_CORE_FRAMEWORKS) -framework OpenCL


void ofApp::setup()
{
    // Uncomment for verbose info from libfreenect2
    //ofSetLogLevel(OF_LOG_VERBOSE);

    ofBackground(0);

    //see how many devices we have.
    ofxKinectV2 tmp;
    std::vector <ofxKinectV2::KinectDeviceInfo> deviceList = tmp.getDeviceList();
    
    //allocate for this many devices
    kinects.resize(deviceList.size());
    texDepth.resize(kinects.size());
    texRGB.resize(kinects.size());
    texRGBRegistered.resize(kinects.size());
    texIR.resize(kinects.size());
    

    panel.setup("", "settings.xml", 10, 100);
    
	ofxKinectV2::Settings ksettings;
	ksettings.enableRGB = true;
	ksettings.enableIR = true;
	ksettings.enableDepth = true;
	ksettings.enableRGBRegistration = true;
	ksettings.config.MinDepth = 0.5;
	ksettings.config.MaxDepth = 8.0;
    
    //****************************************//
    // example settings for just getting the depth stream //
//    ksettings.enableRGB = false;
//   ksettings.enableRGBRegistration = false;
//    ksettings.enableIR = false;
//    // use enableDepthRegistration if you would like to use getWorldCoordinateAt
    ksettings.enableDepthRegistration = true;
    //****************************************//
    
    // Note you don't have to use ofxKinectV2 as a shared pointer, but if you
    // want to have it in a vector ( ie: for multuple ) it needs to be.
    for(int d = 0; d < kinects.size(); d++) {
        kinects[d] = std::make_shared<ofxKinectV2>();
        kinects[d]->open(deviceList[d].serial, ksettings);
        panel.add(kinects[d]->params);
        
        
    }
    

    //currentFrame.allocate(1920, 1080);
    //previousFrame.allocate(1920,1080);
    //difference.allocate(1920, 1080);

    

    panel.loadFromFile("settings.xml");

    cam.setDistance(00);

    targetPosition.set(0, 0, 0);
    

    panel.add(posx.setup("posx", 0, -1000, 1000));
    panel.add(posy.setup("posy", 0, -500, 500));
    panel.add(posz.setup("posz", 0, -1500, 1500));
    panel.add(camxx.setup("camx", 0, -5000, 5000));
    panel.add(camyy.setup("camy", 0, -500, 500));
    panel.add(camzz.setup("camz", 0, -2000, 2000));

    font.load("frabk.ttf", 400);
}


void ofApp::update()
{
    int delay = 1000;

    lookPosition.set(posx, posy, posz);
    cam.lookAt(lookPosition);
    float radius = 1000; // Adjust the radius of rotation as needed
    float camX = targetPosition.x + radius * cos(rotationAngle);
    float camY = targetPosition.y ;
    float camZ = targetPosition.z + radius * sin(rotationAngle); // Adjust the camera's height if needed
    
    // Set the camera position
    cam.setPosition(camX, camY, camZ);
    // Increment the rotation angle
    rotationAngle += 0.01;
    ofColor myNewColor;
    for (int d = 0; d < kinects.size(); d++)
    {
        kinects[d]->update();
        
        if (kinects[d]->isFrameNew())
        {
            if (kinects[d]->isRGBEnabled()) texRGB[d].loadData(kinects[d]->getPixels());
            if (kinects[d]->getRegisteredPixels().getWidth() > 10) texRGBRegistered[d].loadData(kinects[d]->getRegisteredPixels());
            if (kinects[d]->isIREnabled()) texIR[d].loadData(kinects[d]->getIRPixels());
            if (kinects[d]->isDepthEnabled()) texDepth[d].loadData(kinects[d]->getDepthPixels());
            //ofPixels rgbPixels;
            //texRGB[d].readToPixels(rgbPixels);
            //rgbPixels = kinects[d]->getRgbPixels();
            //rgbPixels.setNumChannels(3);
            //rgbPixels.swapRgb();
            //testeo.setFromPixels(rgbPixels);
            //currentFrame = testeo;
            //currentFrame.threshold(150); // Adjust the threshold value as needed

            // Compute difference between current and previous frame
            //difference.absDiff(previousFrame, currentFrame);

            // Update previous frame with current frame
            //previousFrame = currentFrame;

            // Find contours in the difference image
            //contourFinder.findContours(difference, 10, (1920 * 1080) / 2, 1, false);

            if (showPointCloud)
            {
                pointCloud.clear();
                
                for (std::size_t x = 0; x < texRGBRegistered[d].getWidth(); x++)
                {
                    for (std::size_t y = 0; y < texRGBRegistered[d].getHeight(); y++)
                    {
                        puntoReal = kinects[d]->getWorldCoordinateAt(x, y);
                        
                        
                        if (puntoReal.z > 0)
                        {
                            if (puntoReal.z < 1.5)
                            {
                                myNewColor = kinects[d]->getRegisteredPixels().getColor(x, y);
                                if (caso == 4)
                                {
                                    myNewColor.r = 0;

                                }
                                else if (caso == 1)
                                {
                                    myNewColor.b = 0;

                                }
                                else if (caso == 2)
                                {
                                    myNewColor.g = 0;

                                }
                                else if (caso == 3)
                                {
                                    myNewColor.g = 0;
                                    myNewColor.b = 0;

                                }
                                else if (caso == 5)
                                {
                                    
                                    
                                    
                                    //if (difference.getPixels()[y*1920+x] > 250);
                                      //  pointCloud.addColor(myNewColor);
                                        //pointCloud.addVertex(puntoReal);

                                }
                                else if (caso == 0)
                                {

                                }
                                pointCloud.addColor(myNewColor);
                                pointCloud.addVertex(puntoReal);
                            }

                        }


                        //pointCloud.addColor(kinects[d]->getRegisteredPixels().getColor(x, y));
                        //pointCloud.addVertex(kinects[d]->getWorldCoordinateAt(x-1, y-1));

                    }
                }
            }
            

        }
        

    }
    if (b_saving) {
        if (ofGetSystemTimeMillis() > counter + delay) {
            if (countDown == 1) {
                // make a unique file name by including the system time in the name we save with
                pointCloud.save("test"  ".ply");
                b_saving = false;
                dibu = 1;
                now = ofGetElapsedTimef();
            }
            else {
                countDown--;
                counter = ofGetSystemTimeMillis();
            }
        }
    }
}




void ofApp::draw()
{
    if (!showPointCloud && currentKinect < texRGB.size())
    {
        ofBackground(0);
        
        drawTextureAtRowAndColumn("RGB Pixels",
                                  texRGB[currentKinect],
                                  0, 0);
        
        drawTextureAtRowAndColumn("RGB Pixels, Registered",
                                  texRGBRegistered[currentKinect],
                                  1, 0);

        drawTextureAtRowAndColumn("Depth Pixels, Mapped",
                                  texDepth[currentKinect],
                                  1, 1);

        drawTextureAtRowAndColumn("IR Pixels, Mapped",
                                  texIR[currentKinect],
                                  0, 1);
                                  
        

        ofSetColor(255);
        ofScale(1, 1);
        // Draw the current frame
        //currentFrame.draw(0, 0,1280,768);

        // Draw the difference image
        //difference.draw(0, 0,1280,768);
        //testeo.draw(0, 0, 1280, 768);
        // Draw the contours
        //contourFinder.draw(0, 0);
        //ofPopMatrix();
    }
    else
    {
        cam.begin();
        pointCloud.setMode(OF_PRIMITIVE_POINTS);
        mesh2.setMode(OF_PRIMITIVE_POINTS);
        glPointSize(2);
        // Set the initial target position
        ofPushMatrix();
        ofScale(1500, -1500, -1500);
        pointCloud.draw();
        
        if (dibu == 1)
        {
            
            if (now > nextEventSeconds) {
                mesh2.load("test.ply");
                
                dibu = 0;
            }
            
        }
        mesh2.draw();
        ofPopMatrix();
        cam.end();

    }
    
    if( kinects.size() < 1 ) {
        ofDrawBitmapStringHighlight( "No Kinects Detected", 40, 40 );
    }
    if (b_saving) {
        // if we are saving a file draw the countdown number to screen
        font.drawString(ofToString(countDown), ofGetWidth() / 2 - font.stringWidth(ofToString(countDown)) / 2, ofGetHeight() / 2);
        ofDrawBitmapString(countDown, ofGetWidth()/2, ofGetHeight()/2);
    }

    
        
    panel.draw();
}


void ofApp::keyPressed(int key)
{
    switch (key) {
    case 's':
        b_saving = true;
        counter = ofGetSystemTimeMillis();
        countDown = 3; // start our count down from 3
        break;

    case ' ':
        currentKinect = (currentKinect + 1) % kinects.size();
        break;

    case 'p':
        showPointCloud = !showPointCloud;
        break;

    case '0':
        caso = 0;
        break;

    case '1':
        caso = 1;
        break;

    case '2':
        caso = 2;
        break;

    case '3':
        caso = 3;
        break;

    case '4':
        caso = 4;
        break;

    case '5':
        caso = 5;
        break;

    }
}


void ofApp::drawTextureAtRowAndColumn(const std::string& title,
                                      const ofTexture& tex,
                                      int row,
                                      int column)
{
    float displayWidth = ofGetWidth() / numColumns;
    float displayHeight = ofGetHeight() / numRows;
    
    ofRectangle targetRectangle(row * displayWidth,
                                column * displayHeight,
                                displayWidth,
                                displayHeight);
    
    ofNoFill();
    ofSetColor(ofColor::gray);
    ofDrawRectangle(targetRectangle);
    
    ofFill();
    ofSetColor(255);
    if (tex.isAllocated())
    {
        ofRectangle textureRectangle(0, 0, tex.getWidth(), tex.getHeight());
        
        // Scale the texture rectangle to its new location and size.
        textureRectangle.scaleTo(targetRectangle);
        tex.draw(textureRectangle);
    }
    else
    {
        ofDrawBitmapStringHighlight("...",
                                    targetRectangle.getCenter().x,
                                    targetRectangle.getCenter().y);
    }
    
    ofDrawBitmapStringHighlight(title,
                                targetRectangle.getPosition() + glm::vec3(14, 20, 0));
}
//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{

}