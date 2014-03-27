/****************************************************************************
 Copyright (c) 2013-2014 George Guy
 Copyright (c) 2013-2014 Casey Loufek
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

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