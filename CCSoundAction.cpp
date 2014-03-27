//
//  SoundAction.cpp
//  ZombiesWillInfectYourFamily
//
//  Created by George Guy on 1/30/14.
//
//

#include "CCSoundAction.h"
#include "SimpleAudioEngine.h"

bool SoundAction::init(const std::string& filename, const float duration)
{
    if( super::initWithDuration(duration) )
    {
        this->_fileName = filename;
        return true;
    }
    
    return false;
}

SoundAction* SoundAction::create(const std::string& filename, const float duration)
{
    auto ret = new SoundAction();
    
    if(ret && ret->init(filename, duration))
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        CC_SAFE_DELETE(ret);
        return NULL;
    }
}

SoundAction* SoundAction::clone() const
{
    return SoundAction::create(this->getFileName(), this->getDuration());
}

SoundAction* SoundAction::reverse() const
{
    return SoundAction::create(this->getFileName(), this->getDuration());
}

void SoundAction::startWithTarget(cocos2d::Node *target)
{
    super::startWithTarget(target);
    
    CocosDenshion::SimpleAudioEngine* audio = CocosDenshion::SimpleAudioEngine::getInstance();
    _id = audio->playEffect(this->getFileName().c_str());
}

void SoundAction::stop()
{
    super::stop();
    
    CocosDenshion::SimpleAudioEngine* audio = CocosDenshion::SimpleAudioEngine::getInstance();
    audio->stopEffect(_id);
}