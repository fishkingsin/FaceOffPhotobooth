/*
 *  CaptureState.cpp
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

#include "CaptureState.h"
FaceFeature *feature[4];
static int timeremain = 20;
string CaptureState::getName()
{
	return "CaptureState";
}
//--------------------------------------------------------------
void CaptureState::setup(){
    
    getSharedData().faceTracking.setup();
    
    getSharedData().panel.setWhichPanel("General");
    getSharedData().panel.setWhichColumn(0);
    getSharedData().panel.addSlider("videoOffsetX",videoOffset.x,0,ofGetWidth());
    getSharedData().panel.addSlider("videoOffsetY",videoOffset.y,0,ofGetWidth());
    getSharedData().panel.setWhichPanel("FaceTracking");
    getSharedData().panel.setWhichColumn(0);
    getSharedData().panel.addSlider("minFaceAreaW",getSharedData().faceTracking.minFaceAreaW,1,512);
	getSharedData().panel.addSlider("minFaceAreaH",getSharedData().faceTracking.minFaceAreaH,1,512);
    getSharedData().panel.addSlider("faceOffsetX",getSharedData().faceTracking.faceOffset.x,1,512);
    getSharedData().panel.addSlider("faceOffsetY",getSharedData().faceTracking.faceOffset.y,1,512);
    getSharedData().panel.addSlider("faceOffsetW",getSharedData().faceTracking.faceOffset.width,1,512);
    getSharedData().panel.addSlider("faceOffsetH",getSharedData().faceTracking.faceOffset.height,1,512);
	
	
	capturedScreen.allocate(camW,camH);
    
    ratio = (ofGetHeight()*1.0f)/(camH*1.0f);
	screenWidth = camW*ratio;
	screenHeight = ofGetHeight();
	
    for(int j = 0 ;j < MAX_PLAYER ; j++)
    {
		
        getSharedData().panel.setWhichPanel("FaceTracking"+ofToString(j));
        feature[0] = &getSharedData().faceTracking.leftEye[j];
        feature[1] = &getSharedData().faceTracking.rightEye[j];
        feature[2] = &getSharedData().faceTracking.nose[j];
        feature[3] = &getSharedData().faceTracking.mouth[j];
        for(int i = 0 ; i < 4 ; i++)
        {
            getSharedData().panel.setWhichColumn(i) ;
            getSharedData().panel.addSlider(feature[i]->name+"minFeatureArea"+ofToString(j),BUFFER_SIZE*0.5,1,BUFFER_SIZE);
            
            getSharedData().panel.addSlider(feature[i]->name+"ROIL_X"+ofToString(j),0,1,BUFFER_SIZE);
            getSharedData().panel.addSlider(feature[i]->name+"ROIL_W"+ofToString(j),BUFFER_SIZE,1,BUFFER_SIZE);
            getSharedData().panel.addSlider(feature[i]->name+"ROIL_Y"+ofToString(j),0,1,BUFFER_SIZE);
            getSharedData().panel.addSlider(feature[i]->name+"ROIL_H"+ofToString(j),BUFFER_SIZE,1,BUFFER_SIZE);
            getSharedData().panel.addSlider( feature[i]->name+"offset.x"+ofToString(j),0,-BUFFER_SIZE,BUFFER_SIZE);
            getSharedData().panel.addSlider( feature[i]->name+"offset.y"+ofToString(j),0,-BUFFER_SIZE,BUFFER_SIZE);
            getSharedData().panel.addSlider(feature[i]->name+ "offset.width"+ofToString(j),0,-BUFFER_SIZE,BUFFER_SIZE);
            getSharedData().panel.addSlider(feature[i]->name+ "offset.height"+ofToString(j),0,-BUFFER_SIZE,BUFFER_SIZE);
        }
    }
    
    box2d.setup();
    bCapture = false;
    bBox2D = false;
    countDown.setup(getSharedData().xml.getValue("COUNTDOWN",1)*1000);
    ofAddListener(countDown.COUNTER_REACHED,this,&CaptureState::Shot);
    ofAddListener(countDown.COMPLETE,this,&CaptureState::completeShot);
	ofAddListener(tween.end_E,this,&CaptureState::tweenCompleted);
}

//--------------------------------------------------------------
void CaptureState::update(){
    if(getSharedData().bParticle)
    {        
        for(int i = 0; i < getSharedData().p.size(); i++){
            getSharedData().p[i].update();
        }
        return;
    }
    
    if(!bBox2D)
    {
        videoOffset.x = getSharedData().panel.getValueF("videoOffsetX");
        videoOffset.y = getSharedData().panel.getValueF("videoOffsetY");
        getSharedData().faceTracking.minFaceAreaW = getSharedData().panel.getValueF("minFaceAreaW");
        getSharedData().faceTracking.minFaceAreaH = getSharedData().panel.getValueF("minFaceAreaH");
        getSharedData().faceTracking.faceOffset.x = getSharedData().panel.getValueF("faceOffsetX");
        getSharedData().faceTracking.faceOffset.y = getSharedData().panel.getValueF("faceOffsetY");
        getSharedData().faceTracking.faceOffset.width = getSharedData().panel.getValueF("faceOffsetW");
        getSharedData().faceTracking.faceOffset.height = getSharedData().panel.getValueF("faceOffsetH");
        for(int j = 0 ;j < MAX_PLAYER ; j++)
        {
            
            feature[0] = &getSharedData().faceTracking.leftEye[j];
            feature[1] = &getSharedData().faceTracking.rightEye[j];
            feature[2] = &getSharedData().faceTracking.nose[j];
            feature[3] = &getSharedData().faceTracking.mouth[j];
            for(int i = 0 ; i < 4 ; i++)
            {
                feature[i]->minArea = getSharedData().panel.getValueF(feature[i]->name+"minFeatureArea"+ofToString(j));
                feature[i]->ROI.x = getSharedData().panel.getValueF(feature[i]->name+"ROIL_X"+ofToString(j));
                feature[i]->ROI.width = getSharedData().panel.getValueF(feature[i]->name+"ROIL_W"+ofToString(j));
                feature[i]->ROI.y = getSharedData().panel.getValueF(feature[i]->name+"ROIL_Y"+ofToString(j));
                feature[i]->ROI.height = getSharedData().panel.getValueF(feature[i]->name+"ROIL_H"+ofToString(j));
                feature[i]->offset.x = getSharedData().panel.getValueF(feature[i]->name+"offset.x"+ofToString(j));
                feature[i]->offset.y = getSharedData().panel.getValueF(feature[i]->name+"offset.y"+ofToString(j));
                feature[i]->offset.width = getSharedData().panel.getValueF(feature[i]->name+"offset.width"+ofToString(j));
                feature[i]->offset.height = getSharedData().panel.getValueF(feature[i]->name+"offset.height"+ofToString(j));
                
            }
        }
        
        getSharedData().faceTracking.update(bCapture);
    }
    else {
        box2d.update();
    }
    
    if(bCapture)
    {
        lastCapture.setFromPixels(getSharedData().faceTracking.getPixels(),camW,camH,OF_IMAGE_COLOR);
        bCapture = false;
        for(int player = 0 ; player<getSharedData().numPlayer ; player++)
        {
            char imagename[1024];
            stringstream format;
            stringstream format2;
            
            if(player==1)format <<"%0"<<getSharedData().numDigi<<"d2_.jpg";
            else format <<"%0"<<getSharedData().numDigi<<"d.jpg";
            ofLog(OF_LOG_VERBOSE, "format ",format.str().data() );
            sprintf(imagename, format.str().c_str(), getSharedData().counter);
            char code[getSharedData().numDigi];
            
            format2 <<"%0"<<getSharedData().numDigi<<"d";
            sprintf(code, format2.str().c_str(), getSharedData().counter);
            
            getSharedData().lastCode = code;
            ofLog(OF_LOG_NOTICE, "Exported file name = ",imagename );
            ofLog(OF_LOG_NOTICE, "Exported code = "+getSharedData().lastCode);
            getSharedData().faceTracking.savingFace(player,getSharedData().path_to_save+"/"+imagename);
            getSharedData().lastFileNames.push_back(getSharedData().path_to_save+"/"+imagename);
        }
        getSharedData().counter++;  
        if (getSharedData().xml.pushTag("DATA")) {
            getSharedData().xml.setValue("COUNTER", getSharedData().counter);
            getSharedData().xml.popTag();
            
            
        }
        getSharedData().save();	
        
    }
    if(bSaveFace && bBox2D)
    {
        saveFace();
        bSaveFace = false;
    }
    
}


//--------------------------------------------------------------
void CaptureState::draw(){
    capturedScreen.begin();
	ofClear(0);
    if(!bBox2D)
    {
        getSharedData().faceTracking.draw();
    }
    if(lastCapture.isAllocated())
    {
        lastCapture.draw(0,0);
    }
    if(bBox2D)
    {
        getSharedData().faceTracking.drawFeaturesBlur(0);
        getSharedData().faceTracking.drawFeaturesBlur(1);
        box2d.draw();
    }
    
	capturedScreen.end();
    if(bBox2D)
    {
        switch(getSharedData().numPlayer)
        {
            case 1:
            {
                
                int x = SINGLE_X*ofGetWidth();//757;
                int y = SINGLE_Y*ofGetHeight();//178;    
                int w = SINGLE_W*ofGetWidth();//463;
                int h = SINGLE_H*ofGetHeight();//754;
                capturedScreen.getTextureReference().drawSubsection(x+w, y, -w, h, 0, 0, camW*0.5, camH);
                image.draw(0,0,ofGetWidth(),ofGetHeight());
            }
                break;
            case 2:
            {
                
                
                capturedScreen.getTextureReference().drawSubsection(x2+w, y, -w, h, 0, 0, camW*0.5, camH);
                
                capturedScreen.getTextureReference().drawSubsection(x+w, y, -w, h, camW*0.5, 0, camW*0.5, camH);
                image.draw(0,0,ofGetWidth(),ofGetHeight());
            }
                break;
        }
    }
    else
    {
        
        
        
        switch(getSharedData().numPlayer)
        {
            case 1:
                capturedScreen.draw(videoOffset.x+screenWidth+0.5*(ofGetWidth()-screenWidth)-screenWidth*0.25,
                                    videoOffset.y,
                                    -screenWidth,
                                    screenHeight);
                overlayimage.draw(0,0,ofGetWidth(),ofGetHeight());
                break;
            case 2:
                capturedScreen.draw(videoOffset.x+screenWidth+0.5*(ofGetWidth()-screenWidth),
                                    videoOffset.y,
                                    -screenWidth,
                                    screenHeight);
                overlayimage.draw(0,0,ofGetWidth(),ofGetHeight());
                break;
        }
        
    }
    {
        int diff = ofGetElapsedTimef()-timeCount;
        
        if(diff<timeremain)
        {
            if(!bBox2D)
            {
                ofPushStyle();
                ofSetColor(0);
                glPushMatrix();
                {
                    glTranslatef(ofGetWidth()-64,ofGetHeight()-104,0);
                    glPushMatrix();
                    {
                        glScalef(0.5,0.5,1);
                        getSharedData().font.drawString(ofToString(timeremain-(diff),0), - 256,0);
                    }
                    glPopMatrix();
                    
//                    glPushMatrix();
//                    {
//                        glTranslatef(-350,0,0);
//                        glScalef(0.15f,0.15f,1.0f);
//                        getSharedData().font.drawString("Time Remain : ", 0,0);
//                    }
                    
                    glPopMatrix();
                }
                glPopMatrix();
                ofPopStyle();
            }
            
        }
        else if (diff==timeremain)
        {
            overlayimage.clear();
            if(getSharedData().numPlayer==1)
            {
                
                overlayimage.loadImage("images/CaptureStateOverlay_b.png");
            }else
            {
                overlayimage.loadImage("images/CaptureStateOverlay2_b.png");                
            }
            keyPressed('C');
            
        }
    }
    if(showMask)mask.draw(0,0,mask.width*getSharedData().wRatio,mask.height*getSharedData().hRatio);
	countDown.draw(ofGetWidth()/2-70,50, 200,200);
    
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
    if(ofGetLogLevel()==OF_LOG_VERBOSE)
    {
        getSharedData().faceTracking.draw();
    }
}

void CaptureState::stateExit() {
    
	getSharedData().faceTracking.exit();
	image.clear();
    image.clear();
	overlayimage.clear();
    overlayimage.clear();
    timeCount = 0;
    countDown.stop();
    
    showMask = false;
}
void CaptureState::stateEnter()
{
    showMask =  false;
    x = DOU1_X*ofGetWidth();
    y = SINGLE_Y*ofGetHeight();
    w = SINGLE_W*ofGetWidth();
    h = SINGLE_H*ofGetHeight();
    x2 = DOU2_X*ofGetWidth();
    bSaveFace = false;
    ratio = (ofGetHeight()*1.0f)/(camH*1.0f);
	screenWidth = camW*ratio;
	screenHeight = ofGetHeight();
    
    timeCount = ofGetElapsedTimef();
    //    keyPressed(OF_KEY_BACKSPACE);
    lastCapture.clear();
    bBox2D = false;
    mask.loadImage("images/captureStateMask.jpg");
    if(getSharedData().numPlayer==1)
    {
        image.loadImage("images/CaptureState.png");
        overlayimage.loadImage("images/CaptureStateOverlay.png");
    }
    else
    {
        image.loadImage("images/CaptureState2.png");
        overlayimage.loadImage("images/CaptureStateOverlay2.png");
    }
    getSharedData().bParticle = true;
    tween.setParameters(STATE_ENTER,easing,ofxTween::easeIn,255,0,1000,0);
    capturedScreen.begin();
    ofClear(0);
    capturedScreen.end();
}
void CaptureState::tweenCompleted(int &id)
{
    ofLog(OF_LOG_VERBOSE,getName()+" Tween Complete " + ofToString(id));
    switch(id)
    {
        case STATE_ENTER:
            getSharedData().bParticle = false;
            break;
        case STATE_EXIT:
            changeState("EndState");
            break;
    }
}

//--------------------------------------------------------------
void CaptureState::keyPressed(int key){
    switch(key)
    {
        case 'c':
            bCapture = true;
            break;
        case 'b':
        {
            
            
            if(bBox2D)
            {
                showMask = false;
                //TO-DO add clear box2d body and patricle when add new set of face feature
                //****************************************
				box2d.clear();
                //****************************************
				
				for(int i = 0 ; i < getSharedData().faceTracking.facefinder.blobs.size() ;i++)
				{
					if(i<= MAX_PLAYER)
					{
                        feature[0] = &getSharedData().faceTracking.leftEye[i];
                        feature[1] = &getSharedData().faceTracking.rightEye[i];
                        feature[2] = &getSharedData().faceTracking.nose[i];
                        feature[3] = &getSharedData().faceTracking.mouth[i];
                        ofRectangle rect =  getSharedData().faceTracking.faceRect[i];
                        for(int j = 0 ; j < 4 ; j++)
                        {
                            CustomParticle p;
                            //float density, float bounce, float friction
                            p.setPhysics(0.3, 0.7, 0.7);
                            
                            float scaleX = rect.width/BUFFER_SIZE*1.0f;
                            float scaleY = rect.height/BUFFER_SIZE*1.0f;
                            float x = rect.x+(feature[j]->rect.x*scaleX);
                            float y = rect.y+(feature[j]->rect.y*scaleY);
                            float w = feature[j]->rect.width*scaleX;
                            float h = feature[j]->rect.height*scaleY;
                            if(x<camW*0.5)
                            {
                                p.setup(box2d.box2d[0].getWorld(),x+(w*0.5),y+(h*0.5),w*0.5*0.5);
                            }
                            else
                            {
                                p.setup(box2d.box2d[1].getWorld(),x+(w*0.5),y+(h*0.5),w*0.5*0.5); 
                            }
                            p.setupTheCustomData(feature[j],&getSharedData().faceTracking.alphaMaskShader,scaleX,scaleY);
                            box2d.addParticle( p);
                        }
						
                    }
                    
                }
                getSharedData().faceTracking.clear();
            }
            
        }
            break;
        case 'C':
            //bBox2D = !bBox2D;
            if(!bBox2D){
                
                countDown.start();
                showMask = true;
            }
			else {
                
                getSharedData().bParticle = true;
                for(int i = 0; i < getSharedData().p.size(); i++){
                    getSharedData().p[i].reset();
                }
                tween.setParameters(STATE_EXIT,easing,ofxTween::easeOut,0,255,1000,0);
                
            }
            break;
        case OF_KEY_RETURN:
            tween.setParameters(STATE_EXIT,easing,ofxTween::easeOut,0,255,1000,0);
            break;
        case OF_KEY_BACKSPACE:
            changeState("SelectPlayerState");
            
            break;
    }
}

//--------------------------------------------------------------
void CaptureState::keyReleased(int key){
    
}

//--------------------------------------------------------------
void CaptureState::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void CaptureState::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void CaptureState::mousePressed(int x, int y, int button){
    
    
}

//--------------------------------------------------------------
void CaptureState::mouseReleased(int x, int y, int button){
    
}

void CaptureState::Shot(ofEventArgs &arg)
{
    ofLog(OF_LOG_VERBOSE,"Shot");
    bCapture = true;
    
}
void CaptureState::completeShot(ofEventArgs &arg)
{
    ofLog(OF_LOG_VERBOSE,"completeShot");
    bBox2D = true;
    bSaveFace = true;
    keyPressed('b');
    
}
void CaptureState::saveFace()
{
    if(getSharedData().numPlayer==1)
    {
        ofLog(OF_LOG_VERBOSE,"Start Saving one face\n-----------------------------------------");
        string file_name = getSharedData().lastFileNames.back();
        FaceData faceData;
        faceData.setup(file_name,"face_profile/settings_a.xml");
        getSharedData().filesXml.addValue("FILE",faceData.save());
        faceData.setup(file_name,"face_profile/settings_b.xml");
        getSharedData().filesXml.addValue("FILE",faceData.save());
        faceData.setup(file_name,"face_profile/settings_c.xml");
        getSharedData().filesXml.addValue("FILE",faceData.save());
        faceData.setup(file_name,"face_profile/settings_d.xml");
        getSharedData().filesXml.addValue("FILE",faceData.save());
        faceData.setup(file_name,"face_profile/settings_e.xml");
        getSharedData().filesXml.addValue("FILE",faceData.save());
        getSharedData().filesXml.saveFile(getSharedData().path_to_save+"/files.xml");
        //TO_DO load image and map the face on shoes;
        getSharedData().lastFileNames.pop_back();
        ofLog(OF_LOG_VERBOSE,"End Saving one face\n-----------------------------------------");
    }
    else
    {
        ofLog(OF_LOG_VERBOSE,"Start Saving two face\n-----------------------------------------");
        string file_name2 = getSharedData().lastFileNames.back();
        getSharedData().lastFileNames.pop_back();
        string file_name = getSharedData().lastFileNames.back();
        getSharedData().lastFileNames.pop_back();
        DoubleFaceData faceData;
        faceData.setup(file_name, file_name2, "face_profile/settings_a.xml");
        faceData.save();
        faceData.setup(file_name, file_name2, "face_profile/settings_b.xml");
        faceData.save();
        faceData.setup(file_name, file_name2, "face_profile/settings_c.xml");
        faceData.save();
        faceData.setup(file_name, file_name2, "face_profile/settings_d.xml");
        faceData.save();
        faceData.setup(file_name, file_name2, "face_profile/settings_e.xml");
        faceData.save();
        ofLog(OF_LOG_VERBOSE,"End Saving two face\n-----------------------------------------");
        //TO_DO load image and map the face on shoes;
        //getSharedData().lastFileNames.pop_back();
    }
    timeCount = ofGetElapsedTimef();
}
