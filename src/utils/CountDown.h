#ifndef COUNTDOWN_H
#define COUNTDOWN_H
#include "ofMain.h"
#include "ofxTimer.h"
#include "ofxTween.h"
class CountDown 
{
public:
    ofEvent <ofEventArgs> COUNTER_REACHED;
    ofEvent <ofEventArgs> COMPLETE;
    CountDown();
    virtual ~CountDown();
    void setup(int _second);
    void update(ofEventArgs &args);
    void draw(float x,float y){
        draw(x,y,ofGetWidth(),ofGetHeight());
    };
    void draw(float x ,float y, float w, float h);//bool blips);
	
	
	float getHeight(){return 0;}
	float getWidth(){return 0;}
	
	void setAnchorPercent(float xPct, float yPct){};
    void setAnchorPoint(float x, float y){};
	void resetAnchor(){};
    void start();
    void stop();
	
	
	
    ofxTimer mTimer;
    int mCount;
    ofImage *countImage;
    int num ;
    bool bShow;
    float second;
    ofSoundPlayer  shutter;
    ofSoundPlayer  button;
    float scale;
    bool bFlash;
    ofxTween tween;
    ofxEasingLinear linearEase;
    bool fireComplete;
	
	
};

#endif // COUNTDOWN_H
