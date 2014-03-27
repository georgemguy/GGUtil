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

#ifndef __ZombiesWillInfectYourFamily__SoundAction__
#define __ZombiesWillInfectYourFamily__SoundAction__

#include "GGUtil.h"

/*
 * Useful when one must make a sound repeatedly, or after a delay.
 */

class SoundAction : public cocos2d::ActionInterval
{
public:
    typedef cocos2d::ActionInterval super;
    
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, _fileName, FileName);
    CC_SYNTHESIZE_READONLY(unsigned int, _id, ID);

    SoundAction() : super(), _id(0) {};
    virtual ~SoundAction() {};
    virtual bool init(const std::string& filename, const float duration);
    static SoundAction* create(const std::string& filename, const float duration);
    
    virtual void startWithTarget(cocos2d::Node* target);
    virtual void stop();
    
    virtual SoundAction* clone() const override;
    virtual SoundAction* reverse() const override;
};

#endif /* defined(__ZombiesWillInfectYourFamily__SoundAction__) */
