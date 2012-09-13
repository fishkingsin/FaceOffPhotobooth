#pragma once
#include "ofMain.h"
#include "ofxTween.h"
#include "ofxImageSequence.h"
enum particleMode{
	PARTICLE_MODE_ATTRACT = 0,
	PARTICLE_MODE_REPEL,
	PARTICLE_MODE_NEAREST_POINTS,
	PARTICLE_MODE_NOISE
};

class Particle{

	public:
		Particle();
		
		void setMode(particleMode newMode);	


		void reset();
		void update();
		void draw();		
		
		ofPoint pos;
		ofPoint vel;
		ofPoint frc;
		
		float drag; 
		float uniqueVal;
		float scale;
		
		particleMode mode;
    float initTime;
    ofxTween tween;
    ofxTween tween2;
    ofxEasingCubic 	easing;
    ofxImageSequence *image;
    int id;
};