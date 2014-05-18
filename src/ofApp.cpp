#include "ofApp.h"

const int totalVids = 8;

void ofApp::exit()
{
    ofRemoveListener(loader.youTubeURLEvent, this, &ofApp::receivedYouTubeURLEvent);
        for(int i =0; i < totalVids;i++) {
            if(ytVideo[i] != NULL) {
                delete ytVideo[i];
            }
        }

}
//------------------------------------------------------------------------------
void ofApp::setup()
{
	ofSetVerticalSync(true);
	ofBackground(0,0,0);

    ofSeedRandom(ofGetSystemTimeMicros());
    //ofSetLogLevel(OF_LOG_VERBOSE);

    currentIndex = 0;
    bVideoPlaying = false;
    bShowInfo = true;


    for(int i =0; i < totalVids;i++) {
		ofVideoPlayer* player = new ofVideoPlayer();
		ytVideo.push_back(player);

        ratio.push_back(0);
        url.push_back("");
        ytVideo[i]->setLoopState(OF_LOOP_NONE);
        getNewVideo(i);
    }

    ofAddListener(loader.youTubeURLEvent, this, &ofApp::receivedYouTubeURLEvent);

}

//------------------------------------------------------------------------------
void ofApp::update()
{

    for(int i =0; i < totalVids;i++) {
        if(ytVideo[i] != NULL && ytVideo[i]->isLoaded()) {
            ytVideo[i]->update();

            if(currentIndex == i)
            {
                if(url[currentIndex]=="") {
                    getNextVideo();
                    return;
                }
                if(ytVideo[currentIndex]->getPosition() > 0.95)
                {
                    getNextVideo();
                    return;
                }

                // get next video if current one is not playable
                float curPosition = ytVideo[currentIndex]->getPosition();
                if((bVideoPlaying) && (curPosition - lastPosition == 0)) {
                    if(!bStartCounter) {
                        bStartCounter = true;
                        startTime = ofGetElapsedTimeMillis();
                    }
                    curTime = ofGetElapsedTimeMillis();
                    if((curTime - startTime) > 500) {
                        bStartCounter = false;
                        startTime = 0;
                        getNextVideo();
                        return;
                    }
                }
                else {
                    bStartCounter = false;
                    startTime = 0;
                }
                lastPosition = curPosition;

                if((ytVideo[i]->getWidth() > 0) && (ratio[i] <= 0)) {
                    ratio[i] = (ytVideo[i]->getHeight() / ytVideo[i]->getWidth());
//                    cout << "ratio =" << 1.0f/ratio[i] << endl;
//                    cout << "video width = " << ytVideo[i]->getWidth() << "  video height=" << ytVideo[i]->getHeight() << endl;
//                     cout << "loaded..." << i << endl;
                }
            }

        }
    }



}

//------------------------------------------------------------------------------
void ofApp::draw()
{
    if(!bVideoPlaying) {
        ofDrawBitmapString("LOADING VIDEOS", ofGetWidth()/2 - 20,ofGetHeight()/2);
    }
    else {
        ofSetColor(255,255,255);
        if((ratio[currentIndex] != 0) && (!bStartCounter)) {
            if(ofGetWidth()*ratio[currentIndex] < ofGetHeight()) {
                ytVideo[currentIndex]->draw(0,(ofGetHeight() - ofGetWidth()*ratio[currentIndex]) /2.0f,ofGetWidth(),ofGetWidth()*ratio[currentIndex]);
            }
            else {
                ytVideo[currentIndex]->draw(0,0,ofGetWidth(),ofGetWidth()*ratio[currentIndex]);
            }
        } else {
            ofSetColor(0,0,0);
            ofRectangle(0,0,ofGetWidth(),ofGetHeight());//cout << "RATIO is zero!" << endl;
        }
    }

    if(bShowInfo) {
        ofPushStyle();

        ofDrawBitmapString("currentIndex="+ofToString(currentIndex),0,ofGetHeight()-220);
        for(int i=0; i< totalVids;i++)
        {
            if(currentIndex == i) ofSetColor(255,0,0);
            else ofSetColor(125,125,125);

            string paused;
            if(ytVideo[i]->isPaused()) paused = "yes";
            else paused ="no ";
            string playing;
            if(ytVideo[i]->isPlaying()) playing = "yes";
            else playing = "no ";

            ofDrawBitmapString(" paused="+paused+" playing="+playing+" URL"+ofToString(i)+"="+url[i],0,ofGetHeight()-200+i*20);
        }

        ofSetColor(255,0,0);
        ofDrawBitmapString("currentIndex = "+ofToString(currentIndex)+ " ratio = "+ofToString(ratio[currentIndex]) + " width = "+ofToString(ytVideo[currentIndex]->getWidth()) + " height = "+ofToString(ytVideo[currentIndex]->getHeight()) + " position = "+ofToString(ytVideo[currentIndex]->getPosition()), 20,ofGetHeight()-20);
        ofPopStyle();
    }
}

//--------------------------------------------------------------
void ofApp::receivedYouTubeURLEvent(ofxYouTubeURLEvent & event){
    //cout << "URLEvent: " << event.url << endl;
	string moviePath = event.url;
	int movieId = event.id;

    if(bVideoPlaying == false){
        url[movieId] = moviePath;

        //delete ytVideo[event.id];
        //ytVideo[event.id] = new ofVideoPlayer();
        cout << "loadMovie " << movieId  << endl;
        //ytVideo[movieId]->loadMovie(moviePath);
        if(ytVideo[movieId] != NULL) {
            delete ytVideo[movieId];
        }
        ytVideo[movieId] = event.vid;
        ytVideo[movieId]->setUseTexture(true);
        ytVideo[movieId]->setPaused(true);

        if(movieId >= totalVids - 1) {
			//ytVideo[0]->setPaused(false);
            ytVideo[0]->play();
			bVideoPlaying = true;
			cout << "movies loaded" << endl;
		}

    }
    else {
        url[movieId] = moviePath;
        cout << "loadMovie " << movieId  << endl;
        if(ytVideo[movieId] != NULL) {
            delete ytVideo[movieId];
        }
        ytVideo[movieId] = event.vid;
        ytVideo[movieId]->setUseTexture(true);
        ytVideo[movieId]->setPaused(true);
    }
}

//------------------------------------------------------------------------------
void ofApp::getNewVideo(int id)
{
    loader.loadYouTubeURL("",id); //empty string = random url
}

//--------------------------------------------------------------
void ofApp::getNextVideo()
{
    if(!bVideoPlaying) return;

    int nextIndex = currentIndex + 1;
    if(nextIndex >= totalVids) nextIndex = 0;

    if((ytVideo[nextIndex]== NULL) || (url[nextIndex]=="")) //return if next video is not ready
        return;

    if(ytVideo[currentIndex])
    ratio[currentIndex] = 0;
    url[currentIndex] = "";
    ytVideo[currentIndex]->setPaused(true);
    ytVideo[currentIndex]->close();

    getNewVideo(currentIndex);

    currentIndex = nextIndex;

    //ytVideo[currentIndex]->setUseTexture(true);
    ytVideo[currentIndex]->setPaused(false);
    ytVideo[currentIndex]->play();

}

//--------------------------------------------------------------
void ofApp::keyPressed (int key)
{
    switch(key){
        case ' ':
            getNextVideo();
        break;
        case't':
            cout << "Total Frames=" << ytVideo[currentIndex]->getTotalNumFrames() << endl;
        break;
        case'p':
            bPaused = !bPaused;
            ytVideo[currentIndex]->setPaused(bPaused);
        break;
        case 'f':
            bShowInfo = !bShowInfo;
        break;
    }
}


