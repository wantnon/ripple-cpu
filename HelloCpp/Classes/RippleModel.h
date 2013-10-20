//
//  RippleModel.h
//  HelloWorld
//
//  Created by ?? ???? on 12-4-7.
//  Copyright (c) 2012?? chukong-inc. All rights reserved.
//

#ifndef HelloWorld_RippleModel_h
#define HelloWorld_RippleModel_h

#include "cocos2d.h"
class RippleModel : public cocos2d::CCObject{
    
public:
    RippleModel() ;
    virtual ~RippleModel() ;
    void initRippleCoeff() ;
    void initMesh() ;
    bool initWithScreenWidth( int width, 
                              int height,
                              int factor, 
                              int radius,
                              int texWidth,
                              int texHeight) ;
    void runSimulation() ;
    void initiateRippleAtLocation(const cocos2d::CCPoint& location) ;
    void setRippleStrength(bool repeat, float strength) ;

    float* getPosArray(){return rippleVertices;} ;
	float* getTexCoordArray(){return rippleTexCoords;} ;
	int* getIndexArray() {return rippleIndicies;};
	int getPosArrayLen() {return posArrayLen;};
	int getTexCoordArrayLen() {return texCoordArrayLen;};
	int getIndexArrayLen() {return indexArrayLen;};

   
    
private:
	bool isNeedSimulation;
    int _screenWidth ;
    int _screenHeight ;
    int poolWidth ;
    int poolHeight ;
    int touchRadius ;
    int textureWidth ;
    int textureHeight ;
    int meshFactor ;

	int posArrayLen;
	int texCoordArrayLen;
	int indexArrayLen;
    
    float *rippleCoeff ;
    float *rippleSource ;
    float *rippleDest ;
    
	float *s_tcBuff;
	float *t_tcBuff;
    
    bool    isRepeat ;
    float   rippleStrength ;
    
    float  *rippleVertices ;
    float  *rippleTexCoords ;
    int *rippleIndicies ;
};

#endif
