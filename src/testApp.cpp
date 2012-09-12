#include "testApp.h"
#include "IndexState.h"
#include "SelectPlayerState.h"
#include "EditState.h"
#include "CaptureState.h"
#include "EndState.h"
#include "LicenseState.h"
const static string settingFileName = "config.xml";
//--------------------------------------------------------------
void testApp::setup(){
    ofEnableSmoothing();
	ofEnableAlphaBlending();
    ofSetVerticalSync(true);
    glDisable(GL_DEPTH_TEST);
    stateMachine.getSharedData().counter = 0;
	stateMachine.getSharedData().load();
    ofxXmlSettings xml  = stateMachine.getSharedData().xml;
    if(xml.loadFile(settingFileName))
	{
		if(xml.pushTag("DATA"))
		{
            int width = xml.getValue("WIDTH", 1280);
            int height = xml.getValue("HEIGHT", 720);
            ofSetFullscreen(xml.getValue("FULLSCREEN", 0));
            ofSetWindowShape(width, height);
			stateMachine.getSharedData().path_to_save = xml.getValue("CAPTURE_PATH", "./captures");
            stateMachine.getSharedData().filesXml.loadFile(stateMachine.getSharedData().path_to_save+"/files.xml");
            if(!stateMachine.getSharedData().filesXml.pushTag("XML"))
            {
                stateMachine.getSharedData().filesXml.addTag("XML");
                stateMachine.getSharedData().filesXml.pushTag("XML");
            }
			stateMachine.getSharedData().numDigi = xml.getValue("DIGI", 5);
            stateMachine.getSharedData().font.loadFont(xml.getValue("FONT_PATH", "fonts/LunacyMore.ttf"),xml.getValue("FONT_SIZE", 128));
			ofDirectory dir;
			if(dir.listDir(stateMachine.getSharedData().path_to_save)<1)
			{
				dir.createDirectory(stateMachine.getSharedData().path_to_save);
			}
			
			xml.popTag();
		}
	}
	else
	{
		ofLog(OF_LOG_ERROR,"Faile to load "+ settingFileName);
	}
    
	//    // setup shared data
    ofxControlPanel::setBackgroundColor(simpleColor(30, 30, 60, 100));
	ofxControlPanel::setTextColor(simpleColor(240, 50, 50, 255));
    stateMachine.getSharedData().panel.setup(ofGetWidth(),ofGetHeight());
	stateMachine.getSharedData().panel.loadFont("MONACO.TTF", 8);		
	stateMachine.getSharedData().panel.addPanel("General", 4,false);
    
	ofxControlPanel::setBackgroundColor(simpleColor(60, 30, 30, 100));	
	stateMachine.getSharedData().panel.addPanel("FaceTracking", 5, false);
    ofxControlPanel::setBackgroundColor(simpleColor(60, 30, 30, 100));	
	stateMachine.getSharedData().panel.addPanel("FaceTracking0", 4, false);
    ofxControlPanel::setBackgroundColor(simpleColor(60, 30, 30, 100));	
	stateMachine.getSharedData().panel.addPanel("FaceTracking1", 4, false);
    
	ofxControlPanel::setBackgroundColor(simpleColor(70, 70, 30, 100));	
	stateMachine.getSharedData().panel.addPanel("FaceMapEdit", 4, false);
    
	ofxControlPanel::setBackgroundColor(simpleColor(30, 30, 30, 100));	
    //some dummy vars we will update to show the variable lister object
	appFrameRate	= ofGetFrameRate();
    stateMachine.getSharedData().panel.setWhichPanel("General");
    stateMachine.getSharedData().panel.setWhichColumn(0);
	stateMachine.getSharedData().panel.addChartPlotter("some chart", guiStatVarPointer("app fps", &appFrameRate, GUI_VAR_FLOAT, true, 2), 200, 50, 200, 5, 80);
    vector<string> loglevel;
    loglevel.push_back("OF_LOG_VERBOSE");
    loglevel.push_back("OF_LOG_NOTICE");
    loglevel.push_back("OF_LOG_WARNING");
    loglevel.push_back("OF_LOG_ERROR");
    loglevel.push_back("OF_LOG_FATAL_ERROR");
    loglevel.push_back("OF_LOG_SILENT");
    stateMachine.getSharedData().panel.addTextDropDown("LogLevel","LogLevel", 0, loglevel);
	
	// initialise state machine
	stateMachine.addState(new IndexState());
	stateMachine.addState(new SelectPlayerState());
	stateMachine.addState(new CaptureState());
	stateMachine.addState(new EditState());
	stateMachine.addState(new EndState());
	stateMachine.addState(new LicenseState());
	stateMachine.changeState(xml.getValue("DATA:INIT_STATE", "IndexState"));
    stateMachine.getSharedData().panel.loadSettings("settings.xml");
    stateMachine.getSharedData().panel.hide();
    stateMachine.getSharedData().numPlayer = 1;
    for(int i = 0 ; i < NUM_SEQ ;i++)
    {
        image[i].loadSequence("images/bat/bat_", "png", 0, 154, 5);
    }
    
    int num = 3000;
    
	stateMachine.getSharedData().p.assign(num, Particle());
    resetParticles();
    
}

//--------------------------------------------------------------
void testApp::update(){
    
	appFrameRate	= ofGetFrameRate();
    int loglevel = stateMachine.getSharedData().panel.getValueI("LogLevel");
    if(ofGetLogLevel()!=  loglevel  )
    {
        ofSetLogLevel((ofLogLevel)loglevel);
    }
	
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackgroundGradient(ofColor(50,50,50) , ofColor::black);
}
void testApp::exit()
{
	stateMachine.getSharedData().save();
}
//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch(key)
    {
        case OF_KEY_F1:
            stateMachine.changeState("CaptureState");
            break;
        case OF_KEY_F2:
            stateMachine.changeState("EditState");
            break;
		case OF_KEY_F3:
            stateMachine.changeState("IndexState");
            break;
        case OF_KEY_F4:
            stateMachine.changeState("SelectPlayerState");
            break;
		case OF_KEY_F5:
            stateMachine.changeState("EndState");
            break;
        case OF_KEY_F12:
            stateMachine.changeState("LicenseState");
            break;
		case 's':
            stateMachine.getSharedData().panel.saveSettings();
            break;
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    ofLog(OF_LOG_VERBOSE,"keyReleased :" + ofToString(key));
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 
    
}
void testApp::resetParticles(){
    for(int i = 0; i < stateMachine.getSharedData().p.size(); i++){
		stateMachine.getSharedData().p[i].setMode(PARTICLE_MODE_ATTRACT);		
    
		stateMachine.getSharedData().p[i].reset();
        stateMachine.getSharedData().p[i].image = &image[i%NUM_SEQ];
        stateMachine.getSharedData().p[i].id = i;
	}	
}
