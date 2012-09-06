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
#include "ofxUI.h"
class IndexState : public Apex::ofxState<SharedData>
{
public:
    void setup(){
		
		image.loadImage("images/index.png");
		//float dim = 16; 
		//float xInit = OFX_UI_GLOBAL_WIDGET_SPACING; 
		//float length = 255-xInit; 
		gui = new ofxUICanvas(ofGetWidth()/2-142, ofGetHeight()/2-61, 284,122);

		button = new ofxUIImageButton(284,122, true, "GUI/images/start.png","IMAGEBTN");
		gui->addWidgetDown(button);
		//button->setVidible(true);
		ofAddListener(gui->newGUIEvent,this,&IndexState::guiEvent);
		gui->setDrawBack(false);
		gui->setVisible(false);
	}
	void guiEvent(ofxUIEventArgs &e)
	{
		string name = e.widget->getName(); 
		int kind = e.widget->getKind(); 
		if(name == "IMAGEBTN")
		{
			ofxUIImageButton *btn = (ofxUIImageButton *) e.widget; 
			//cout << "IMAGEBTN " << btn->getValue() << endl; 
			if(btn->getValue()==1)
			{
				changeState("SelectPlayerState");
				
			}
		}
	}
	void update(){}
	void draw(){
		ofPushStyle();
		ofEnableAlphaBlending();
		ofSetColor(255);
		image.draw(0,0);
		button->drawFillHighlight();
		ofPopStyle();
		gui->draw();
	}
	void mouseMoved(int x, int y) {}
    void mouseDragged(int x, int y, int button) {}
    void mousePressed(int x, int y, int button) {}
    void mouseReleased(int x, int y, int button) {}
    void stateExit(){
		gui->setVisible(false);
	}
	void stateEnter()
	{
		gui->setVisible(true);
	}
    void keyPressed(int key) {}
    void keyReleased(int key) {}
    
	ofImage image ;
	ofxUICanvas *gui; 
    ofxUIImageButton *button ;
	string getName(){ return "IndexState";}
};
