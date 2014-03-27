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

#ifndef __ZombiesWillInfectYourFamily__EventAction__
#define __ZombiesWillInfectYourFamily__EventAction__

#include "cocos2d.h"
#include "GGUtil.h"

/*
 * Useful when you want to post an event repeatedly, or after a delay. I kept
 * the class names from Cocos2d-x-2.x.x because CCNotificationCenter was
 * deprecated in Cocos2d-x-3.0.
 */

class EventAction : public cocos2d::CCActionInstant
{
public:
    typedef cocos2d::CCActionInstant super;
    
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, _event, Event);
    CC_SYNTHESIZE_READONLY(cocos2d::CCObject*, _argument, Argument);
    
    EventAction() : super(), _argument(NULL) {};
    virtual ~EventAction() { CC_SAFE_RELEASE_NULL(_argument); };
    virtual bool init(const std::string& event, cocos2d::CCObject* argument = NULL);
    static EventAction* create(const std::string& event, cocos2d::CCObject* argument = NULL);
    virtual void update( float delta );
    
    inline virtual EventAction* clone() const
    {
        return EventAction::create( this->getEvent(), this->getArgument() );
    };
    
    inline virtual EventAction* reverse() const
    {
        return EventAction::create( this->getEvent(), this->getArgument() );
    };
};

#endif /* defined(__ZombiesWillInfectYourFamily__EventAction__) */
