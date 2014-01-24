#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
using namespace cocos2d;
using namespace cocos2d::extension;
class HelloWorld : public CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);
    
public:
    HelloWorld(){
        m_controlButton=NULL;
        m_renderTex=NULL;
        
    }
    virtual ~HelloWorld(){
        if(m_controlButton)m_controlButton->release();
        if(m_renderTex)m_renderTex->release();
    }

public:
    
    CCControlButton* m_controlButton;
    void controlButtonCallback(CCObject *senderz, CCControlEvent controlEvent);
    CCRenderTexture* m_renderTex;
};

#endif // __HELLOWORLD_SCENE_H__
