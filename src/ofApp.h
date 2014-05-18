#pragma once


#include "ofMain.h"
#include "ofxJSONElement.h"
#include "ofxThreadedYouTubeVideo.h"

//class YouTubePlayer {
//public
//
//    string url;
//    float ratio;
//    bool bLoaded;
//
//};

class ofApp : public ofBaseApp {
public:
    void setup();
    void draw();
    void update();
    void exit();
    void keyPressed (int key);

    void getNewVideo(int id);
    void getNextVideo();


    void receivedYouTubeURLEvent(ofxYouTubeURLEvent & event);


    ofxThreadedYouTubeVideo loader;
	vector<ofVideoPlayer*> ytVideo;
    vector <string> url;
    vector <float> ratio;

    int currentIndex;

    bool bPaused;
    bool bShowInfo;
    bool bVideoPlaying;

    // Hacks to avoid dud videos
    float lastPosition;
    bool bStartCounter;
    float startTime;
    float curTime;
};
