#include "Particle.h"

//------------------------------------------------------------------
Particle::Particle(){
    image = NULL;
}

//------------------------------------------------------------------
void Particle::setMode(particleMode newMode){
	mode = newMode;
}

//------------------------------------------------------------------
void Particle::reset(){
	//the unique val allows us to set properties slightly differently for each particle
	uniqueVal = ofRandom(-10000, 10000);
	
	pos.x = -ofRandomWidth();
	pos.y = ofRandomHeight();
	
	vel.x = ofRandom(-3.9, 3.9);
	vel.y = ofRandom(-3.9, 3.9);
	
	frc   = ofPoint(0,0,0);
	
	scale = ofRandom(0.5,1);
	
	if( mode == PARTICLE_MODE_NOISE ){
		drag  = ofRandom(0.97, 0.99);
		vel.x = fabs(vel.x) * 10.0; //make the particles all be going down
	}else{
		drag  = ofRandom(0.95, 0.998);	
	}
    float rand = ofRandom(2,3);
    float randW = ofRandomWidth();
    tween.setParameters(9,easing,ofxTween::easeOut,-ofRandomWidth()-10,randW,rand*400,0);
    tween2.setParameters(9,easing,ofxTween::easeOut,0,ofGetWidth()+ofRandomWidth()+10,1000,500+rand*400);
}

//------------------------------------------------------------------
void Particle::update(){

	//1 - APPLY THE FORCES BASED ON WHICH MODE WE ARE IN 
	if( mode == PARTICLE_MODE_ATTRACT)
    {
//        if(tweenquart.isRunning() || tweenquart2.isRunning())
//        if(!tweenquart.isCompleted())
            pos.x = tween.update()+tween2.update();
//        else
//        {
//        tweenquart2.update();
//        }
    }
	else if( mode == PARTICLE_MODE_NOISE )
    {
		//lets simulate falling snow 
		//the fake wind is meant to add a shift to the particles based on where in x they are
		//we add pos.y as an arg so to prevent obvious vertical banding around x values - try removing the pos.y * 0.006 to see the banding
		float fakeWindX = ofSignedNoise(pos.x * 0.003, pos.y * 0.006, ofGetElapsedTimef() * 0.6);
		
		frc.x = ofSignedNoise(uniqueVal, pos.y * 0.006, ofGetElapsedTimef()*0.2) * 0.09 + 0.18;
		frc.y = fakeWindX * 0.25 + ofSignedNoise(uniqueVal, pos.x * 0.04) * 0.6;

		vel *= drag; 
		vel += frc * 0.4;
		
		//we do this so as to skip the bounds check for the bottom and make the particles go back to the top of the screen
//		if( pos.y + vel.y*2 > ofGetHeight() ){
//			pos.y -= ofGetHeight();
//		}
//        if( pos.x + vel.x*2 > ofGetWidth() ){
//			pos.x -= ofGetWidth();
//		}
        pos += vel; 
	}

	
	
	//2 - UPDATE OUR POSITION
	
	
	
	
	//3 - (optional) LIMIT THE PARTICLES TO STAY ON SCREEN 
	//we could also pass in bounds to check - or alternatively do this at the testApp level
//	if( pos.x > ofGetWidth() ){
//		pos.x = ofGetWidth();
//		vel.x *= -1.0;
//	}else if( pos.x < 0 ){
//		pos.x = 0;
//		vel.x *= -1.0;
//	}
//	if( pos.y > ofGetHeight() ){
//		pos.y = ofGetHeight();
//		vel.y *= -1.0;
//	}
//	else if( pos.y < 0 ){
//		pos.y = 0;
//		vel.y *= -1.0;
//	}	
		
}

//------------------------------------------------------------------
void Particle::draw(){
    ofPushStyle();
    ofFill();
	if( mode == PARTICLE_MODE_ATTRACT ){
		ofSetColor(255, 63, 180);
	}
	else if( mode == PARTICLE_MODE_NOISE )
    {
		ofSetColor(99, 63, 255);
	}
    if(image!=NULL)
    {
        image->draw(pos.x, pos.y);
    }
    else
    {
        ofCircle(pos.x, pos.y, scale * 10.0);
    }
    ofPopStyle();
}

