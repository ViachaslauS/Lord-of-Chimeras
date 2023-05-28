/**************************************\
 *
 *  Created by Vyacheslav Stankevich
 *  silentiumslava@gmail.com
 *
\**************************************/

#include "AppDelegate.h"

#include "Menu/MenuScene.h"
// #include "GameScene.h"

#include "Unit/UnitLoader.h"
#include "Profile.h"

// #define USE_AUDIO_ENGINE 1

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
#endif

// GUID: {663D1284-3ACF-4C0B-AABD-45028AD4EDFF}

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(800, 600);
static cocos2d::Size smallResolutionSize = cocos2d::Size(800, 600);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(800, 600);
static cocos2d::Size largeResolutionSize = cocos2d::Size(800, 600);

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate() 
{
    unit_loader::saveToFile(m_units, unit_loader::defaultInventoryFile());
#if USE_AUDIO_ENGINE
    AudioEngine::end();
#endif
}

void AppDelegate::initGLContextAttrs()
{
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil,multisamplesCount
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8, 0};

    GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages,  
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("LordOfChimeras", cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height), 1.0f, true);
#else
        glview = GLViewImpl::create("LordOfChimeras");
#endif
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60);

    // Set the design resolution
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::SHOW_ALL);
    auto frameSize = glview->getFrameSize();
    // if the frame's height is larger than the height of medium size.
    if (frameSize.height > mediumResolutionSize.height)
    {        
        director->setContentScaleFactor(MIN(largeResolutionSize.height/designResolutionSize.height, largeResolutionSize.width/designResolutionSize.width));
    }
    // if the frame's height is larger than the height of small size.
    else if (frameSize.height > smallResolutionSize.height)
    {        
        director->setContentScaleFactor(MIN(mediumResolutionSize.height/designResolutionSize.height, mediumResolutionSize.width/designResolutionSize.width));
    }
    // if the frame's height is smaller than the height of medium size.
    else
    {        
        director->setContentScaleFactor(MIN(smallResolutionSize.height/designResolutionSize.height, smallResolutionSize.width/designResolutionSize.width));
    }

    register_all_packages();

    initSpriteCaches();
    initInventory();

    // create a scene. it's an autorelease object
    auto scene = MenuScene::create(m_units);

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::pauseAll();
#endif
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::resumeAll();
#endif
}

void AppDelegate::initInventory()
{
    cocos2d::UserDefault* userdef = cocos2d::UserDefault::getInstance();

    // userdef->setBoolForKey("first_launch", false);

    if (userdef->getBoolForKey("first_launch") == false)
    {
        userdef->setBoolForKey("first_launch", true);
        userdef->flush();

        unit_loader::saveToFile({}, unit_loader::defaultInventoryFile());
    }
    else
    {
        m_units = unit_loader::loadFromFile(unit_loader::defaultInventoryFile());
    }
}

void AppDelegate::initSpriteCaches()
{
    SpriteFrameCache* spriteCache = SpriteFrameCache::getInstance();
    spriteCache->addSpriteFramesWithFile("sheets/ui.plist");
    spriteCache->addSpriteFramesWithFile("sheets/units.plist");
    spriteCache->addSpriteFramesWithFile("sheets/cultist.plist");
}
