/*
 *  EndState.h
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
#include "ofxUI.h"
//#include "ofxTimer.h"
class EndState : public Apex::ofxState<SharedData>
{
public:
    void setup(){
		
//		timer.setup(8000,false);
//		ofAddListener(timer.TIMER_REACHED,this,&EndState::timesUp);
		ofAddListener(tween.end_E,this,&EndState::tweenCompleted);
//		timer.stopTimer();
	}
	
	void update(){
        if(getSharedData().bParticle)
        {        
            for(int i = 0; i < getSharedData().p.size(); i++){
                getSharedData().p[i].update();
            }
        }
        if(ofGetElapsedTimef() - timeCount >=8  && !bExit)
        {
            keyPressed(OF_KEY_RETURN);
        }
    }
	void draw(){
		ofPushStyle();
		ofEnableAlphaBlending();
        ofSetColor(255);
		image.draw(0,0,ofGetWidth(),ofGetHeight());
        ofPopStyle();

//        ofPushStyle();
//        ofSetColor(0);
//        glPushMatrix();
//        glTranslatef(ofGetWidth()-64,ofGetHeight()-64,0);
//        glPushMatrix();
//        glScalef(0.5,0.5,1);
//        getSharedData().font.drawString(ofToString(9-(ofGetElapsedTimef()-timeCount),0), - 128,0);
//        glPopMatrix();
//        glPopMatrix();
//        ofPopStyle();
//        ofSetColor(253,127,2);
//        glPushMatrix();
//        glScalef(0.5,0.5,1);
//        getSharedData().font.drawString(ofToString(9-(ofGetElapsedTimef()-timeCount),0),ofGetWidth() - 64,128);
//        glPopMatrix();
        ofPushStyle();
        ofSetColor(0);
		getSharedData().font.drawString(getSharedData().lastCode,strPos.x,strPos.y);
		ofPopStyle();
		
		
		ofPushStyle();
        ofEnableAlphaBlending();
        ofFill();
        ofSetColor(0,0,0,tween.update());
        ofRect(0,0,ofGetWidth(),ofGetHeight());
        ofPopStyle();
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
//		timer.reset();
//		timer.stopTimer();
		image.clear();
        timeCount = 0;
        bExit = false;
	}
	void stateEnter()
	{
        bExit = false;
        timeCount = ofGetElapsedTimef();
        image.loadImage("images/end.jpg");
		if(getSharedData().lastCode=="")
		{
			getSharedData().lastCode="XXXXXX";
		}
//		timer.startTimer();
		strPos.x = ofGetWidth()*0.5f-getSharedData().font.stringWidth(getSharedData().lastCode)*0.5f;
		strPos.y = ofGetHeight()*0.5f;
        tween.setParameters(STATE_ENTER,easing,ofxTween::easeIn,255,0,1000,0);
	}
    void keyPressed(int key) {
        if(key==OF_KEY_RETURN)
        {
            bExit = true;
            tween.setParameters(STATE_EXIT,easing,ofxTween::easeOut,0,255,1000,0);
        }

        else if(key==OF_KEY_BACKSPACE)
        {
            changeState("CaptureState");
        }

    }
    void keyReleased(int key) {}
//	void timesUp(ofEventArgs &args)
//	{
//        keyPressed(OF_KEY_RETURN);
//	}
    void tweenCompleted(int &id)
    {
        ofLog(OF_LOG_VERBOSE,getName()+" Tween Complete " + ofToString(id));
        switch(id)
        {
            case STATE_ENTER:
                getSharedData().bParticle = false;
                break;
            case STATE_EXIT:
                changeState("IndexState");
                break;
        }
    }
	ofImage image ;
	ofxUICanvas *gui; 
    ofxUIImageButton *button ;
//	ofxTimer timer;

	
	ofPoint strPos;
	string getName(){ return "EndState";}
    int timeCount;
    ofxTween tween;
    ofxEasingLinear easing;
    bool bExit;
    
};
