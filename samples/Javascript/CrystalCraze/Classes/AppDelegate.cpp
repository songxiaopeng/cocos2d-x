#include "AppDelegate.h"

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "ScriptingCore.h"
#include "generated/jsb_cocos2dx_auto.hpp"
#include "generated/jsb_cocos2dx_extension_auto.hpp"
#include "jsb_cocos2dx_extension_manual.h"
#include "cocos2d_specifics.hpp"
#include "js_bindings_ccbreader.h"
#include "js_bindings_system_registration.h"
#include "js_bindings_chipmunk_registration.h"
#include "jsb_opengl_registration.h"

USING_NS_CC;
using namespace CocosDenshion;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
    ScriptEngineManager::purgeSharedManager();
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    Director *pDirector = Director::getInstance();
    pDirector->setOpenGLView(EGLView::getInstance());
    pDirector->setProjection(kDirectorProjection2D);


    Size screenSize = EGLView::getInstance()->getFrameSize();

    Size designSize = Size(320, 480);
    Size resourceSize = Size(320, 480);
    
    std::vector<std::string> searchPaths;
    std::vector<std::string> resDirOrders;
    
    TargetPlatform platform = Application::getInstance()->getTargetPlatform();
    if (platform == kTargetIphone || platform == kTargetIpad || platform == kTargetMacOS)
    {
        searchPaths.push_back("Published-iOS"); // Resources/Published-iOS
        FileUtils::getInstance()->setSearchPaths(searchPaths);

        if (screenSize.height > 480)
        {
            resourceSize = Size(640, 960);
            resDirOrders.push_back("resources-iphonehd");
        }
        else
        {
            resDirOrders.push_back("resources-iphone");
        }
        
        FileUtils::getInstance()->setSearchResolutionsOrder(resDirOrders);
    }
    else if (platform == kTargetAndroid || platform == kTargetWindows)
    {
        // Comments it since opengles2.0 only supports texture size within 2048x2048.
//        if (screenSize.height > 1024)
//        {
//            resourceSize = Size(1280, 1920);
//            resDirOrders.push_back("resources-xlarge");
//            resDirOrders.push_back("");
//        }
//        else 
        if (screenSize.height > 960)
        {
            resourceSize = Size(640, 960);
            resDirOrders.push_back("resources-large");
            resDirOrders.push_back("resources-medium");
            resDirOrders.push_back("resources-small");
        }
        else if (screenSize.height > 480)
        {
            resourceSize = Size(480, 720);
            resDirOrders.push_back("resources-medium");
            resDirOrders.push_back("resources-small");
        }
        else
        {
            resourceSize = Size(320, 568);
            resDirOrders.push_back("resources-small");
        }
        
        FileUtils::getInstance()->setSearchResolutionsOrder(resDirOrders);
    }
    pDirector->setContentScaleFactor(resourceSize.width/designSize.width);

    EGLView::getInstance()->setDesignResolutionSize(designSize.width, designSize.height, kResolutionShowAll);
    
    // turn on display FPS
    pDirector->setDisplayStats(true);
    
    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);
    
    ScriptingCore* sc = ScriptingCore::getInstance();
    sc->addRegisterCallback(register_all_cocos2dx);
    sc->addRegisterCallback(register_all_cocos2dx_extension);
    sc->addRegisterCallback(register_cocos2dx_js_extensions);
    sc->addRegisterCallback(jsb_register_chipmunk);
    sc->addRegisterCallback(register_all_cocos2dx_extension_manual);
    sc->addRegisterCallback(register_CCBuilderReader);
    sc->addRegisterCallback(jsb_register_system);
    sc->addRegisterCallback(JSB_register_opengl);
    
    sc->start();

    js_log("RUNNING Main");
    ScriptEngineProtocol *pEngine = ScriptingCore::getInstance();
    ScriptEngineManager::sharedManager()->setScriptEngine(pEngine);
    ScriptingCore::getInstance()->runScript("main.js");
       
    return true;
}

void handle_signal(int signal) {
    static int internal_state = 0;
    ScriptingCore* sc = ScriptingCore::getInstance();
    // should start everything back
    Director* director = Director::getInstance();
    if (director->getRunningScene()) {
        director->popToRootScene();
    } else {
        PoolManager::sharedPoolManager()->finalize();
        if (internal_state == 0) {
            //sc->dumpRoot(NULL, 0, NULL);
            sc->start();
            internal_state = 1;
        } else {
            sc->runScript("hello.js");
            internal_state = 0;
        }
    }
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    Director::getInstance()->stopAnimation();
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->pauseAllEffects();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    Director::getInstance()->startAnimation();
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->resumeAllEffects();
}
