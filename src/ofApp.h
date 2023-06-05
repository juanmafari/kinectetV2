#pragma once


#include "ofMain.h"
#include "ofxKinectV2.h"
#include "ofxGui.h"
#include "ofxOpenCv.h"


class ofApp: public ofBaseApp
{
public:
    void setup() override;
    void update() override;
    void draw() override;

    void keyPressed(int key) override;
    
    void drawTextureAtRowAndColumn(const std::string& title,
                                   const ofTexture& tex,
                                   int row,
                                   int column);

    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    
    ofxPanel panel;

    std::vector<std::shared_ptr<ofxKinectV2>> kinects;

    std::vector<ofTexture> texRGB;
    std::vector<ofTexture> texRGBRegistered;
    std::vector<ofTexture> texIR;
    std::vector<ofTexture> texDepth;
    ofDefaultVertexType puntoReal;
    
    std::size_t currentKinect = 0;
    
    int numRows = 2;
    int numColumns = 2;
    int caso = 0;
    int i = 0;

    ofxCvGrayscaleImage currentFrame;
    ofxCvGrayscaleImage previousFrame;
    ofxCvGrayscaleImage difference;
    ofxCvContourFinder contourFinder;
    ofxCvColorImage testeo;
    ofEasyCam cam;
    ofMesh pointCloud;

    ofVec3f targetPosition;
    ofVec3f lookPosition;
    float rotationAngle = 0.01;

    ofxFloatSlider posx, posy, posz, camxx,camyy,camzz;

    
    
    bool showPointCloud = true;

    //photobooth shi
    int countDown;
    float counter;
    bool b_saving;
    ofTrueTypeFont font;
    ofMesh mesh2;
    int dibu = 0;

    float nextEventSeconds = 0;
    float now;
};
