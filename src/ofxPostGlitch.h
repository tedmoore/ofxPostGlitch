//
//  ofxPostGlitch.h
//
//  Created by maxilla inc. on 2013/02/01.
//
//

#ifndef __ofxPostGlitchExample__ofxPostGlitch__
#define __ofxPostGlitchExample__ofxPostGlitch__

#include "ofMain.h"

#define GLITCH_NUM 17

enum ofxPostGlitchType{
	OFXPOSTGLITCH_CONVERGENCE,
	OFXPOSTGLITCH_GLOW,
	OFXPOSTGLITCH_SHAKER,
	OFXPOSTGLITCH_CUTSLIDER,
	OFXPOSTGLITCH_TWIST,
	OFXPOSTGLITCH_OUTLINE,
	OFXPOSTGLITCH_NOISE,
	OFXPOSTGLITCH_SLITSCAN,
	OFXPOSTGLITCH_SWELL,
	OFXPOSTGLITCH_INVERT,
	OFXPOSTGLITCH_CR_HIGHCONTRAST,
	OFXPOSTGLITCH_CR_BLUERAISE,
	OFXPOSTGLITCH_CR_REDRAISE,
	OFXPOSTGLITCH_CR_GREENRAISE,
	OFXPOSTGLITCH_CR_REDINVERT,
	OFXPOSTGLITCH_CR_BLUEINVERT,
	OFXPOSTGLITCH_CR_GREENINVERT
};

class ofxPostGlitch{
public:

	ofxPostGlitch(){
		targetBuffer = NULL;
		shader[0].load("Shaders/convergence");
		shader[1].load("Shaders/glow");
		shader[2].load("Shaders/shaker");
		shader[3].load("Shaders/cut_slider");
		shader[4].load("Shaders/twist");
		shader[5].load("Shaders/outline");
		shader[6].load("Shaders/noise");
		shader[7].load("Shaders/slitscan");
		shader[8].load("Shaders/swell");
		shader[9].load("Shaders/invert");
		shader[10].load("Shaders/crHighContrast");
		shader[11].load("Shaders/crBlueraise");
		shader[12].load("Shaders/crRedraise");
		shader[13].load("Shaders/crGreenraise");
		shader[14].load("Shaders/crRedinvert");
		shader[15].load("Shaders/crBlueinvert");
		shader[16].load("Shaders/crGreeninvert");
	}

	/* Initialize & set target Fbo */
	void setup(ofFbo* buffer_);

	/* Set target Fbo */
	void setFbo(ofFbo* buffer_);

	/* Switch each effects on/off */
	void setFx(ofxPostGlitchType type_,bool enabled);
    void setFx(int type_, bool enabled);

	/* Toggle each effects on/off */
	void toggleFx(ofxPostGlitchType type_);

	/* Return current effect's enabled info*/
	bool getFx(ofxPostGlitchType type_);
    bool getFx(int type_);

	/* Apply enable effects to target Fbo */
    void generateFx(std::unordered_map<std::string, float>* common_features);
    
    void newParams();
    
    nlohmann::json saveState(){
        nlohmann::json dict;
        
        for(int i = 0; i < 4; i++){
            dict["features_map" + ofToString(i)] = features_map[i];
            dict["invert_param" + ofToString(i)] = invert_param[i];
        }
        
        for(int i = 0; i < GLITCH_NUM; i++){
            dict["bShading" + ofToString(i)] = bShading[i];
        }
        
        return dict;
    };
    
    void loadState(nlohmann::json &dict){
        
        for(int i = 0; i < 4; i++){
            features_map[i] = dict["features_map" + ofToString(i)].get<int>();
            invert_param[i] = dict["invert_param" + ofToString(i)].get<bool>();
        }
        
        for(int i = 0; i < GLITCH_NUM; i++){
            bShading[i] = dict["bShading" + ofToString(i)].get<bool>();
        }
        
    };

protected:
	bool		bShading[GLITCH_NUM];
	ofShader	shader[GLITCH_NUM];
	ofFbo*		targetBuffer;
	ofFbo		ShadingBuffer;
	ofPoint		buffer_size;
	float ShadeVal[4];
    float selected_features[4];
    int features_map[4];
    bool invert_param[4];

};

#endif /* defined(__ofxPostGlitchExample__ofxPostGlitch__) */
