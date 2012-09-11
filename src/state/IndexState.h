/*
 *  IndexState.h
 *
 *  Copyright (c) 2012, James Kong, http://www.fishkingsin.com
 *  All rights reserved. 
 *  
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions are met: 
 *  
 *  * Redistributions of source code must retain the above copyright notice, 
 *    this list of conditions and the following disclaimer. 
 *  * Redistributions in binary form must reproduce the above copyright 
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the distribution. 
 *  * Neither the name of 16b.it nor the names of its contributors may be used 
 *    to endorse or promote products derived from this software without 
 *    specific prior written permission. 
 *  
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 *  POSSIBILITY OF SUCH DAMAGE. 
 *
 */

#pragma once

#include "ofxState.h"
#include "SharedData.h"


class IndexState : public Apex::ofxState<SharedData>
{
public:
    void setup(){
		ofAddListener(tween.end_E,this,&IndexState::tweenCompleted);
		image.loadImage("images/index.jpg");
	}
	void update(){
        getSharedData().faceTracking.minFaceAreaW = getSharedData().panel.getValueF("minFaceAreaW");
        getSharedData().faceTracking.minFaceAreaH = getSharedData().panel.getValueF("minFaceAreaH");

        getSharedData().faceTracking.detectFace();
        if(getSharedData().faceTracking.facefinder.blobs.size()>=1 && !bExit && tween.isCompleted())
        {
            bExit = true;
            ofLog(OF_LOG_NOTICE,"Detected Faces : "+ofToString(getSharedData().faceTracking.facefinder.blobs.size()));
            ofLog(OF_LOG_NOTICE,"Face Tracked go to SelectPalyerState");
            
            keyPressed(OF_KEY_RETURN);
        }
        if(getSharedData().bParticle)
        {
            for(int i = 0; i < getSharedData().p.size(); i++){
                getSharedData().p[i].update();
            }
        }

    }
	void draw(){

		ofPushStyle();
		ofEnableAlphaBlending();
		ofSetColor(255);
		image.draw(0,0,ofGetWidth(),ofGetHeight());
		ofPopStyle();
        ofPushStyle();
        ofEnableAlphaBlending();
        ofFill();
        ofSetColor(0,0,0,tween.update());
        ofRect(0,0,ofGetWidth(),ofGetHeight());
        ofPopStyle();
        if(ofGetLogLevel()==OF_LOG_VERBOSE)
        {
            getSharedData().faceTracking.draw();
        }
        if(getSharedData().bParticle)
        {        
            for(int i = 0; i < getSharedData().p.size(); i++){
                getSharedData().p[i].draw();
            }
        }
	}
	void mouseMoved(int x, int y) {}
    void mouseDragged(int x, int y, int button) {}
    void mousePressed(int x, int y, int button) {}
    void mouseReleased(int x, int y, int button) {}
    void stateExit(){
        bExit = false;
        getSharedData().bParticle = false;
	}
	void stateEnter()
	{
        getSharedData().bParticle = true;
        bExit = false;
        tween.setParameters(STATE_ENTER,easing,ofxTween::easeIn,255,0,1000,0);
	}
    void tweenCompleted(int &id)
    {
        ofLog(OF_LOG_VERBOSE,getName()+" Tween Complete " + ofToString(id));
        switch(id)
        {
            case STATE_ENTER:
                getSharedData().bParticle = false;
                break;
            case STATE_EXIT:
                changeState("SelectPlayerState");
                break;
        }
    }
    void keyPressed(int key) {
        if(key==OF_KEY_RETURN)
        {
            //bExit = true;
            tween.setParameters(STATE_EXIT,easing,ofxTween::easeOut,0,255,1000,0);
            getSharedData().bParticle = true;
            for(int i = 0; i < getSharedData().p.size(); i++){
                getSharedData().p[i].reset();
            }
        }
    }
    void keyReleased(int key) {}
    
	ofImage image ;
	string getName(){ return "IndexState";}
    ofxTween tween;
    ofxEasingLinear easing;
    bool bExit;
    
};
