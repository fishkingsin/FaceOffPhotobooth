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
#include "ofxTimer.h"
class EndState : public Apex::ofxState<SharedData>
{
public:
    void setup(){
		
		timer.setup(8000,false);
		ofAddListener(timer.TIMER_REACHED,this,&EndState::timesUp);
		timer.stopTimer();
	}
	
	void update(){}
	void draw(){
		ofPushStyle();
		ofEnableAlphaBlending();
        ofSetColor(255);
		image.draw(0,0,ofGetWidth(),ofGetHeight());
        ofPushStyle();
        ofSetColor(253,127,2);
        glPushMatrix();
        glScalef(0.5,0.5,1);
        getSharedData().font.drawString(ofToString(9-(ofGetElapsedTimef()-timeCount),0),ofGetWidth() - 64,128);
        glPopMatrix();
		getSharedData().font.drawString(getSharedData().lastCode,strPos.x,strPos.y);
        ofPopStyle();
		ofPopStyle();
		
		
		
	}
	void mouseMoved(int x, int y) {}
    void mouseDragged(int x, int y, int button) {}
    void mousePressed(int x, int y, int button) {}
    void mouseReleased(int x, int y, int button) {}
    void stateExit(){
		timer.reset();
		timer.stopTimer();
		image.clear();
        timeCount = 0;
	}
	void stateEnter()
	{
        timeCount = ofGetElapsedTimef();
        image.loadImage("images/end.jpg");
		if(getSharedData().lastCode=="")
		{
			getSharedData().lastCode="XXXXXX";
		}
		timer.startTimer();
		strPos.x = ofGetWidth()*0.5f-getSharedData().font.stringWidth(getSharedData().lastCode)*0.5f;
		strPos.y = ofGetHeight()*0.5f;//-font.stringHeight(getSharedData().lastCode);
	}
    void keyPressed(int key) {}
    void keyReleased(int key) {}
	void timesUp(ofEventArgs &args)
	{
	  changeState("IndexState");
	}
    
	ofImage image ;
	ofxUICanvas *gui; 
    ofxUIImageButton *button ;
	ofxTimer timer;

	
	ofPoint strPos;
	string getName(){ return "EndState";}
    int timeCount;
    
};
