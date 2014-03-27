//
//  CCSoundAction.h
//  ZombiesWillInfectYourFamily
//
//  Created by George Guy on 1/30/14.
//
//

#ifndef __ZombiesWillInfectYourFamily__SoundAction__
#define __ZombiesWillInfectYourFamily__SoundAction__

#include "GGUtil.h"

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
