/*
 *  SelectPlayerState.h
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

class SelectPlayerState : public Apex::ofxState<SharedData>
{
public:
public:
    void setup(){
		
		
        
		//float dim = 16; 
		//float xInit = OFX_UI_GLOBAL_WIDGET_SPACING; 
		//float length = 255-xInit; 
		gui = new ofxUICanvas(0,0,ofGetWidth(), ofGetHeight());//ofGetWidth()/2, ofGetHeight()/2-61, 284*2,122);
		
		button1 = (ofxUIButton*)gui->addWidgetDown(new ofxUIImageButton(ofGetWidth()*0.5,ofGetHeight(), true, "GUI/images/1player.png","ONE_PLAYER"));
		button2 = (ofxUIButton*)gui->addWidgetRight(new ofxUIImageButton(ofGetWidth()*0.5,ofGetHeight(), true, "GUI/images/2player.png","TWO_PLAYER"));
		//button->setVidible(true);
		ofAddListener(gui->newGUIEvent,this,&SelectPlayerState::guiEvent);
		gui->setDrawBack(false);
		gui->setVisible(false);
        ofAddListener(tween.end_E,this,&SelectPlayerState::tweenCompleted);
	}
	void guiEvent(ofxUIEventArgs &e)
	{
		string name = e.widget->getName(); 
		int kind = e.widget->getKind(); 
		
		if(name == "ONE_PLAYER")
		{
			ofxUIImageButton *btn = (ofxUIImageButton *) e.widget; 
			cout << "ONE_PLAYER " << btn->getValue() << endl; 
			if(btn->getValue()==1)
			{
				keyPressed('1');
				
			}
		}
		else if(name == "TWO_PLAYER")
		{
			ofxUIImageButton *btn = (ofxUIImageButton *) e.widget; 
			cout << "TWO_PLAYER " << btn->getValue() << endl; 
			if(btn->getValue()==1)
			{
				keyPressed('2');				
			}
		}
	}
	void update(){
        //        if(ofGetElapsedTimef()-timeCount>30)
        //        {
        ////            changeState("IndexState");
        //            tween.setParameters(OF_KEY_BACKSPACE,easing,ofxTween::easeOut,0,255,1000,0);
        //        }
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
		gui->draw();
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
		gui->setVisible(false);
        image.clear();
        
	}
	void stateEnter()
	{
        timeCount = ofGetElapsedTimef();
        image.loadImage("images/selectplayer.jpg");
        button1->drawFillHighlight();
        button2->drawFillHighlight();
		gui->setVisible(true);
        bExit = false;
        tween.setParameters(STATE_ENTER,easing,ofxTween::easeIn,255,0,1000,0);
        getSharedData().bParticle = true;
	}
    void keyPressed(int key) {
        tween.setParameters(key,easing,ofxTween::easeOut,0,255,1000,0);
        switch(key)
        {
            case '1':
            case '2':
            case OF_KEY_RETURN:
            {
                
                getSharedData().bParticle = true;
                for(int i = 0; i < getSharedData().p.size(); i++){
                    getSharedData().p[i].reset();
                }
            }
                break;
        }
    }
    void tweenCompleted(int &id)
    {
        ofLog(OF_LOG_VERBOSE,getName()+" Tween Complete " + ofToString(id));
        switch(id)
        {
            case STATE_ENTER:
                getSharedData().bParticle = false;
                break;
                //            case STATE_EXIT:
                //                changeState("SelectPlayerState");
                //                break;
            case '1':
            {
                getSharedData().numPlayer = 1;
                changeState("CaptureState");
            }
                break;
            case '2':
            {
                getSharedData().numPlayer = 2;
                changeState("CaptureState");
                
            }
                break;
            case OF_KEY_RETURN:
            {
                getSharedData().numPlayer = 1;
                changeState("CaptureState");
            }
                break;
            case OF_KEY_BACKSPACE:
            {
                changeState("IndexState");
            }
                break;
        }
        
    }
    void keyReleased(int key) {}
    
	ofImage image ;
	ofxUICanvas *gui; 
    string getName(){ return "SelectPlayerState";}
    
	ofxUIButton  *button1;
	ofxUIButton  *button2;
    int timeCount;
    
    ofxTween tween;
    ofxEasingLinear easing;
    bool bExit;
    
};
