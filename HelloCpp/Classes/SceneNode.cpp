//
//  SceneNode.cpp
//  HelloWorld
//
//  Created by ???? ?? on 12-4-18.
//  Copyright 2012?? chukong-inc. All rights reserved.
//

#include "SceneNode.h"
#include <iostream>
#include "support/ccUtils.h"

using namespace cocos2d ;
using namespace std ;

SceneNode::SceneNode()
{
	_indexVBO = NULL;
    _ripple = NULL ;
    _texture = NULL ;
}

SceneNode::~SceneNode()
{
	_indexVBO->release();
    _ripple->release() ;
    _texture->release() ;

}


bool SceneNode::initWithTexture(std::string textureName) 
{
	//create texture
    _texture = CCTextureCache::sharedTextureCache()->addImage(textureName.c_str()) ;//��ʱ_texture��retainCountΪ1
	if(_texture==NULL)return false;
    //create ripple model
    _ripple = new RippleModel() ;
	if(_ripple==NULL)return false;
	cout<<"winSize:"<<(int)CCDirector::sharedDirector()->getWinSize().width<<" "<<(int)CCDirector::sharedDirector()->getWinSize().height<<endl;
    _ripple->initWithScreenWidth((int)CCDirector::sharedDirector()->getWinSize().width ,
                                     (int)CCDirector::sharedDirector()->getWinSize().height,
                                     3,//6, //this value need further adjusting
                                     2,
                                     (int)CCDirector::sharedDirector()->getWinSize().width ,
                                     (int)CCDirector::sharedDirector()->getWinSize().height
                                     ) ;
	_ripple->setRippleStrength(false,32);
	//create and init indexVBO
	CindexVBO::enableAttribArrays();
	_indexVBO=new CindexVBO();
	_indexVBO->genBuffers();
	//submit data to vbo
	_indexVBO->submitPos(_ripple->getPosArray(),_ripple->getPosArrayLen(),GL_STATIC_DRAW);
	_indexVBO->submitIndex(_ripple->getIndexArray(),_ripple->getIndexArrayLen(),GL_STATIC_DRAW);
	_indexVBO->submitTexCoord(_ripple->getTexCoordArray(),_ripple->getTexCoordArrayLen(),GL_DYNAMIC_DRAW);
	//gl check
	CHECK_GL_ERROR_DEBUG();

	//release a ripple
	_ripple->initiateRippleAtLocation(ccp(100,100));
	//enable touch
	setTouchEnabled( true );
	//
	setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTexture));
	//start update 
	scheduleUpdate();
    return true ;
}

void SceneNode::update(float t){
	//update ripple model
	_ripple->runSimulation() ;
	//submit texCoord data to vbo
	//(because only texCoord data were modified)
	_indexVBO->submitTexCoord(_ripple->getTexCoordArray(),_ripple->getTexCoordArrayLen(),GL_DYNAMIC_DRAW);
}
void SceneNode::draw()
{
	    
    CC_NODE_DRAW_SETUP();
	//bind texture
	CindexVBO::bindTexture(GL_TEXTURE0,_texture->getName());
	//specify vbo data format
	_indexVBO->setPointers();
	//draw vbo
	_indexVBO->draw(GL_TRIANGLE_STRIP);   
	//gl check
	CHECK_GL_ERROR_DEBUG(); 

}
void SceneNode::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
    //Add a new body/atlas sprite at the touched location
    CCSetIterator it;
    CCTouch* touch;

    for( it = touches->begin(); it != touches->end(); it++)
    {
        touch = (CCTouch*)(*it);
        
        if(!touch)
            break;
        
        CCPoint location = touch->getLocationInView();
        
        location = CCDirector::sharedDirector()->convertToGL(location);
    //    cout<<"mos pos:"<<location.x<<" "<<location.y<<endl;
    }
}
void SceneNode::ccTouchesMoved(cocos2d::CCSet* touches , cocos2d::CCEvent* event)
{
    CCSetIterator it;
    CCTouch* touch;
    for( it = touches->begin(); it != touches->end(); it++)
    {
        touch = (CCTouch*)(*it);
        
        if(!touch)
            break;
        
        CCPoint location = touch->getLocationInView();
        
        location = CCDirector::sharedDirector()->convertToGL(location);
    //    cout<<"mos pos:"<<location.x<<" "<<location.y<<endl;
		_ripple->initiateRippleAtLocation(location);
    }
   
}

void SceneNode::ccTouchesBegan(CCSet* touches, CCEvent* event)
{
    CCSetIterator it;
    CCTouch* touch;
	for( it = touches->begin(); it != touches->end(); it++)
    {
        touch = (CCTouch*)(*it);
        if(!touch)
            break;
        
        CCPoint location = touch->getLocationInView();
        
        location = CCDirector::sharedDirector()->convertToGL(location);
      //  cout<<"mos pos:"<<location.x<<" "<<location.y<<endl;
		_ripple->initiateRippleAtLocation(location);
    }
}
