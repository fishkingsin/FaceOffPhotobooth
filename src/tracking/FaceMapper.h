/*
 *  FaceMapper.h
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

#ifndef FaceFeatureDetect_FaceMapper_h
#define FaceFeatureDetect_FaceMapper_h
#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ofxGLWarper.h"
#include "ofxPSBlend.h"
#define gridRes 8
#define MULTIPY 1
#define POS_X 43
#define POS_Y 53
#define POS_W 386
#define POS_H 501
//#define USE_PSBLEND
class FaceData
{
public:
    ofImage backgroundImage;
    ofImage overlayImage;
    ofImage faceImage;
    ofImage frame;
    string imageFile;
    string prefix;
    ofxGLWarper warpper;
#ifdef    USE_PSBLEND
	ofxPSBlend psBlend;
#endif
	bool isSetup;
    FaceData()
    {
        isSetup = false;
    }
	~FaceData()
    {
		backgroundImage.clear();
		overlayImage.clear();
		faceImage.clear();
    }
    void setup(string img_fn, string setting_fn)
    {
		ofEnableAlphaBlending();
        backgroundImage.clear();
        overlayImage.clear();
        faceImage.clear();
		if(!isSetup)
		{
#ifdef    USE_PSBLEND
			psBlend.setup(overlayImage.getWidth(), overlayImage.getHeight());
#endif
			isSetup = true;
        }
		imageFile = img_fn;
        ofxXmlSettings xml;
        
        if(!xml.loadFile(setting_fn))
        {
            ofLog(OF_LOG_VERBOSE,"Faile to load "+setting_fn);
            xml.addTag("DATA");
            if(xml.pushTag("DATA"))
            {
                xml.setValue("PREFIX","a");
                xml.setValue("BACKGROUND","background.png");
                xml.setValue("OVERLAY","overlay.png");
				xml.popTag();
            }
            xml.saveFile(setting_fn);
        }
        else
        {
            faceImage.loadImage(imageFile);
            if(xml.pushTag("DATA"))
            {
                prefix = xml.getValue("PREFIX","a");

                backgroundImage.loadImage(xml.getValue("BACKGROUND","background.png"));
                overlayImage.loadImage(xml.getValue("OVERLAY","overlay.png"));
                frame.loadImage(xml.getValue("FRAME","frame.png"));
                warpper.setup(0,0,faceImage.width,faceImage.height);
                warpper.load("face_profile/warpper_"+prefix+".xml");

                xml.popTag();
            }
            //save();
        }

    }
    virtual void draw()
    {
		ofSetColor(255);
        ofEnableAlphaBlending();
#ifdef    USE_PSBLEND
		psBlend.begin();
#endif
		backgroundImage.draw(POS_X,POS_Y,POS_W,POS_H);
		ofPushStyle();
		ofPushMatrix();
        warpper.begin();
        faceImage.draw(0,0);
        warpper.end();
		
		ofPopMatrix();
		ofPopStyle();
#ifdef    USE_PSBLEND
		psBlend.end();
		psBlend.draw(backgroundImage.getTextureReference(), MULTIPY,POS_X,POS_Y,POS_W,POS_H);
#endif
        warpper.draw();
		overlayImage.draw(POS_X,POS_Y,POS_W,POS_H);
        frame.draw(0,0);
		if(ofGetLogLevel()==OF_LOG_VERBOSE)
		{
			warpper.begin();
			ofPushStyle();
			ofNoFill();
			ofSetColor(255,0,255);
			ofRect(0,0,faceImage.width,faceImage.height);
			ofPopStyle();
			warpper.end();
		}
    }
    string save()
    {
        ofFbo fbo;
        fbo.allocate(frame.width,frame.height);
        fbo.begin();
        ofClear(0);
        draw();
        fbo.end();
        ofPixels pixels;		
        pixels.allocate(overlayImage.width,overlayImage.height,OF_IMAGE_COLOR_ALPHA);
        fbo.readToPixels(pixels);
        string subImageName = imageFile.substr(0,imageFile.length()-4);
        string fileName = subImageName+prefix+".png";
        ofSaveImage(pixels,fileName ,OF_IMAGE_QUALITY_BEST);
		return fileName.substr(fileName.length()-10,fileName.length());
        
    }
    void saveSetting()
    {
        warpper.save("face_profile/warpper_"+prefix+".xml");
    }
};
class DoubleFaceData : public FaceData
{
public:
    ofxGLWarper warpper2;
    ofImage faceImage2;
	~DoubleFaceData()
    {
		faceImage2.clear();
    }
    void setup(string img_fn,string img_fn2, string setting_fn)
    {
		ofEnableAlphaBlending();
        faceImage2.clear();
        FaceData::setup(img_fn, setting_fn);
        faceImage2.loadImage(img_fn2);
        warpper2.setup(0,0,faceImage2.width,faceImage2.height);
        warpper2.load("face_profile/warpper2_"+prefix+".xml");
        
    }
    void draw()
    {
        ofEnableAlphaBlending();
		ofSetColor(255);
		
        backgroundImage.draw(POS_X,POS_Y,POS_W,POS_H);
		#ifdef    USE_PSBLEND
		psBlend.begin();
#endif
		ofPushStyle();
		ofPushMatrix();
		{
			warpper.begin();
			faceImage.draw(0,0);
			warpper.end();
		}
		ofPopMatrix();
		ofPopStyle();

		ofPushStyle();
		ofPushMatrix();
		{
			warpper2.begin();
			faceImage2.draw(0,0);
			warpper2.end();
			
		}
        ofPopMatrix();
		ofPopStyle();
        
#ifdef    USE_PSBLEND
		psBlend.end();
		psBlend.draw(backgroundImage.getTextureReference(), MULTIPY,POS_X,POS_Y,POS_W,POS_H);
#endif
		warpper.draw();
		warpper2.draw();
        
		overlayImage.draw(POS_X,POS_Y,POS_W,POS_H);
        frame.draw(0,0);
		
		if(ofGetLogLevel()==OF_LOG_VERBOSE)
		{
			warpper.begin();
			ofPushStyle();
			ofNoFill();
			ofSetColor(255,0,255);
			ofRect(0,0,faceImage.width,faceImage.height);
			ofPopStyle();
			warpper.end();
			
			warpper2.begin();
			ofPushStyle();
			ofNoFill();
			ofSetColor(255,0,255);
			ofRect(0,0,faceImage2.width,faceImage2.height);
			ofPopStyle();
			warpper2.end();
			
		}
    }
    void saveSetting()
    {
        FaceData::saveSetting();
        warpper2.save("face_profile/warpper2_"+prefix+".xml");
    }
};
class FaceMapper
{
public:
    void setup()
    {
        
    }
    void update()
    {
        
    }
};


#endif