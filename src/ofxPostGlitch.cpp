//
//  ofxPostGlitch.cpp
//
//  Created by maxilla inc. on 2013/02/01.
//

#include "ofxPostGlitch.h"

void ofxPostGlitch::setup(ofFbo *buffer_){
	targetBuffer = buffer_;
	buffer_size.set(buffer_->getWidth(), buffer_->getHeight());
	ShadingBuffer.allocate(buffer_size.x,buffer_size.y);
    newParams();
}

void ofxPostGlitch::setFbo(ofFbo *buffer_){
	targetBuffer = buffer_;
	buffer_size.set(buffer_->getWidth(), buffer_->getHeight());
	ShadingBuffer.allocate(buffer_size.x,buffer_size.y);
}

void ofxPostGlitch::setFx(ofxPostGlitchType type_, bool enabled){
	bShading[type_] = enabled;
}

void ofxPostGlitch::toggleFx(ofxPostGlitchType type_){
	bShading[type_] ^= true;
}

bool ofxPostGlitch::getFx(ofxPostGlitchType type_){
	return bShading[type_];
}

void ofxPostGlitch::newParams(){
    for(int i = 0; i < 4; i++){
        features_map[i] = ofRandom(4);
        invert_param[i] = ofRandom(1.f) < 0.5;
    }
}

void ofxPostGlitch::generateFx(std::unordered_map<std::string, float>* common_features){
	if (targetBuffer == NULL){
		ofLog(OF_LOG_WARNING, "ofxFboFX --- Fbo is not allocated.");
		return;
	}

	float v[2];
	v[0] = ofRandom(3);
    v[1] = ofRandom(3);
    
    selected_features[0] = common_features->at("loudness") * 100.f;
    selected_features[1] = common_features->at("specFlatness") * 100.f;
    selected_features[2] = common_features->at("pitchConfidence") * 100.f;
    selected_features[3] = common_features->at("specCentroid") * 100.f;
    
    for(int i = 0; i < 4; i++){
        ShadeVal[i] = ((1 - selected_features[features_map[i]]) * invert_param[i]) + (selected_features[features_map[i]] * (1 - invert_param[i]));
    }
            
//	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	ofSetColor(255);
	glClearColor(0, 0, 0, 0.0);

	for (int i = 0;i < GLITCH_NUM;i++){
		if (bShading[i]){
			shader[i].begin();
			shader[i].setUniformTexture	("image"		,*targetBuffer,0);
			shader[i].setUniform1i		("trueWidth"	,buffer_size.x);
			shader[i].setUniform1i		("trueHeight"	,buffer_size.y);
			shader[i].setUniform1f		("rand"			,ofRandom(1));
			shader[i].setUniform1f		("mouseX"		,ofGetMouseX());
			shader[i].setUniform1f		("mouseY"		,ofGetMouseY());
			shader[i].setUniform1f		("val1"			,ShadeVal[0]);
			shader[i].setUniform1f		("val2"			,ShadeVal[1]);
			shader[i].setUniform1f		("val3"			,ShadeVal[2]);
			shader[i].setUniform1f		("val4"			,ShadeVal[3]);
			shader[i].setUniform1f		("timer"		,ofGetElapsedTimef());
			shader[i].setUniform2fv		("blur_vec"		,v);

			ShadingBuffer.begin();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			ofRect(0, 0, buffer_size.x, buffer_size.y);
			ShadingBuffer.end();
			shader[i].end();

			targetBuffer->begin();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			ShadingBuffer.draw(0, 0,buffer_size.x,buffer_size.y);
			targetBuffer->end();
		}
	}
}
