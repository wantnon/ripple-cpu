#include "HelloWorldScene.h"
#include "AppMacros.h"
USING_NS_CC;
#include "SceneNode.h"
CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(HelloWorld::menuCloseCallback));
    
	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
                                origin.y + pCloseItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);

    /////////////////////////////
    // 3. add your codes below...

    CCDirector::sharedDirector()->setDisplayStats(true);//false
    
    CCSize winSize=CCDirector::sharedDirector()->getWinSize();
    
    
	SceneNode*sceneNode=new SceneNode();
	sceneNode->initWithTexture("HelloWorld.png");
	sceneNode->setPosition(ccp(0,0));
	sceneNode->setAnchorPoint(ccp(0,0));
	this->addChild(sceneNode);

	
    //contolButton
    {
        CCScale9Sprite* btnUp=CCScale9Sprite::create("button.png");
        CCScale9Sprite* btnDn=CCScale9Sprite::create("button_dn.png");
        CCLabelTTF*title=CCLabelTTF::create("RTT", "Helvetica", 15);
        CCControlButton* controlButton=cocos2d::extension::CCControlButton::create(title, btnUp);
        controlButton->setBackgroundSpriteForState(btnDn,cocos2d::extension::CCControlStateHighlighted);
        controlButton->setPreferredSize(CCSize(40,40));
        controlButton->setPosition(ccp(300+100,60));
        controlButton->addTargetWithActionForControlEvents(this, (cocos2d::extension::SEL_CCControlHandler)(&HelloWorld::controlButtonCallback), cocos2d::extension::CCControlEventTouchDown);
        addChild(controlButton);
        m_controlButton=controlButton;
    }
    
    //renderTexture
    m_renderTex = CCRenderTexture::create(int(winSize.width), int(winSize.height));
    m_renderTex->setPosition(ccp(origin.x+visibleSize.width/2*0.3,origin.y+visibleSize.height/2*0.3));
    m_renderTex->setScale(0.3);
    m_renderTex->retain();
    
    // add a label shows "Hello World"
    // create and initialize a label
    
    CCLabelTTF* pLabel = CCLabelTTF::create("CPU 2D Ripple Effect", "Arial", 32);
    
    // position the label on the center of the screen
    pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - pLabel->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(pLabel, 1);

    // add "HelloWorld" splash screen"
/*    CCSprite* pSprite = CCSprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    pSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(pSprite, 0);
    */
    return true;
}
void HelloWorld::controlButtonCallback(CCObject *senderz, CCControlEvent controlEvent){
    //CCLOG("HI");
    
    if(m_renderTex->getParent()==this)m_renderTex->removeFromParent();
    
    m_renderTex->begin();
    {
        CCDirector::sharedDirector()->getRunningScene()->visit();
    }
    m_renderTex->end();

    addChild(m_renderTex);


}


void HelloWorld::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}
