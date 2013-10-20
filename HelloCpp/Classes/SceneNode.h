
#ifndef HelloWorld_SceneNode_h
#define HelloWorld_SceneNode_h
#include "cocos2d.h"

#include "RippleModel.h"
#include "indexVBO.h"
using namespace cocos2d;

class SceneNode : public CCLayer{
public:
	SceneNode() ;
	virtual ~SceneNode() ;
	bool initWithTexture(std::string textureName) ;
	void draw() ;
	void update(float t);
	//touch
	virtual void ccTouchesBegan(cocos2d::CCSet* touches , cocos2d::CCEvent* event);
	virtual void ccTouchesMoved(cocos2d::CCSet* touches , cocos2d::CCEvent* event);
	virtual void ccTouchesEnded(cocos2d::CCSet* touches , cocos2d::CCEvent* event);

private:
	CindexVBO *_indexVBO;
	RippleModel *_ripple ;
	CCTexture2D *_texture ;

};
#endif
