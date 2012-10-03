/*
 *  SharedData.h
 *
 *  Copyright (c) 2011, James Kong,  http://www.fishkingsin.com
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
#include "FaceTracking.h"
#include "MyBox2D.h"
#include "ofxXmlSettings.h"
#include "FaceMapper.h"
#include "ofxAutoControlPanel.h"
#include "ofxTrueTypeFontUC.h"
#include "ofxTween.h"
#include "Particle.h"
#define STATE_ENTER 0
#define STATE_EXIT 1
class SharedData
{
private:
    int counter;
public:
    bool bDebug;
    int numDigi;
    float wRatio,hRatio;
    string path_to_save;
    vector <string>lastFileNames;
	string lastCode;
    ofxXmlSettings xml;
    ofxXmlSettings filesXml;
	ofImage loadedImage;
    ofxTrueTypeFontUC font;
    ofxAutoControlPanel panel;
    FaceTracking faceTracking;
    int getCounter()
    {
        return counter; 
    }
	void load()
	{
		//counter = 0;
        ofxXmlSettings xml;
        xml.loadFile("preset.xml");
        counter = xml.getValue("COUNT",0);
        
//		std::fstream ifs( ofToDataPath("presets.bin").c_str(), std::ios::in | std::ios::binary );
//		ifs.read( (char*) &counter, sizeof(counter) );
//		ifs.close();
        ofLog(OF_LOG_NOTICE,"Loading Counter : " +ofToString( counter));
	}
    void counterAdd()
    {
        counter+=1; 
    }
	void save()
	{
		ofxXmlSettings xml;
         xml.setValue("COUNT",counter);
        xml.saveFile("preset.xml");
        
//        std::fstream ofs( ofToDataPath("presets.bin").c_str(), std::ios::out | std::ios::binary );
//		ofs.write( (const char*)&counter, sizeof(counter) );
//		ofs.close();
        ofLog(OF_LOG_NOTICE,"Saving Counter : " + ofToString( counter));
	}
//    enum NUM_PALYER
//    {
//        ONE,TWO
//    };
    int numPlayer;
    vector <Particle> p;
    bool bParticle;
};
