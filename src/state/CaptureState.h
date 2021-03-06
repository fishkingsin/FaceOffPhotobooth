/*
 *  CaptureState.h
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
#include "ofxTimer.h"
#include "CountDown.h"
#define SINGLE_X 0.401042f
#define SINGLE_Y 0.170370f
#define SINGLE_W 0.204166f
#define SINGLE_H 0.444444f

#define DOU1_X 0.246875f

#define DOU2_X 0.557291f

class CaptureState : public Apex::ofxState<SharedData>
{
public:
    void setup();
	void update();
	void draw();
	void mouseMoved(int x, int y) ;
    void mouseDragged(int x, int y, int button) ;
    void mousePressed(int x, int y, int button) ;
    void mouseReleased(int x, int y, int button) ;
    void tweenCompleted(int &id);
    void stateExit();
    void stateEnter();
    void keyPressed(int key) ;
    void keyReleased(int key) ;
    void Shot(ofEventArgs &arg);
    void completeShot(ofEventArgs &arg);
    void saveFace();
	string getName();
    MyBox2D box2d;

    bool bBox2D;
    bool bCapture;
    bool bSaveFace;
    bool showMask;
    CountDown countDown;
    ofImage lastCapture;
	
	ofFbo capturedScreen;
	float ratio;
	float screenWidth;
		float screenHeight;
	ofImage image,overlayimage;//,mask;
    int timeCount;

    ofxTween tween;
    ofxEasingLinear easing;
    bool bExit;
    ofPoint videoOffset;
    int x,y,w,h,x2;

};
