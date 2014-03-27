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

#ifndef __ZombiesWillInfectYourFamily__CCActionWait__
#define __ZombiesWillInfectYourFamily__CCActionWait__

#include "cocos2d.h"

/*
 * I implemented Wait because ActionInterval was made abstract in Cocos2d-x 3,
 * and I liked using ActionIntervals in sequences to delay other actions.
 *
 * For example, this is how you would use a Wait to make the Sprite "sprite"
 * turn invisible three seconds after you run the Action "sequence":
 * 
 * auto wait = Wait::create( 3 );
 * auto invisible = FadeOut::create( 0 );
 * auto sequence = Sequence::createWithTwoActions( wait, invisible );
 * sprite->runAction( sequence );
 */

class Wait : public cocos2d::ActionInterval
{
public:
    
    inline virtual void update( float delta ) override {};
    
    inline virtual Wait* clone() const
    {
        return Wait::create( this->getDuration() );
    };
    
    inline virtual Wait* reverse() const
    {
        return this->clone();
    };
    
    inline static Wait* create( float duration )
    {
        using namespace cocos2d;
        auto ret = new Wait();
        
        if( ret && ret->initWithDuration(duration) )
        {
            ret->autorelease();
            return ret;
        }
        else
        {
            CC_SAFE_DELETE(ret);
            return NULL;
        }
        
        return ret;
    };
};

#endif /* defined(__ZombiesWillInfectYourFamily__CCActionWait__) */
