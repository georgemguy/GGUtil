//
//  GGUtil.cpp
//  ZombiesWillInfectYourFamily
//
//  Created by George Guy on 12/14/13.
//
//

#include "GGUtil.h"

// Copied from CCNS.cpp
// string toolkit
static inline void split(std::string src, const char* token, std::vector<std::string>& vect)
{
    int nend=0;
    int nbegin=0;
    while(nend != -1)
    {
        nend = src.find(token, nbegin);
        if(nend == -1)
            vect.push_back(src.substr(nbegin, src.length()-nbegin));
        else
            vect.push_back(src.substr(nbegin, nend-nbegin));
        nbegin = nend + strlen(token);
    }
}

// Copied from CCNS.cpp
// first, judge whether the form of the string like this: {x,y}
// if the form is right,the string will be split into the parameter strs;
// or the parameter strs will be empty.
// if the form is right return true,else return false.
inline bool splitWithForm(const char* pStr, std::vector<std::string>& strs)
{
    bool bRet = false;
    
    do
    {
        CC_BREAK_IF(!pStr);
        
        // string is empty
        std::string content = pStr;
        CC_BREAK_IF(content.length() == 0);
        
        int nPosLeft  = content.find('{');
        int nPosRight = content.find('}');
        
        // don't have '{' and '}'
        CC_BREAK_IF(nPosLeft == (int)std::string::npos || nPosRight == (int)std::string::npos);
        // '}' is before '{'
        CC_BREAK_IF(nPosLeft > nPosRight);
        
        std::string pointStr = content.substr(nPosLeft + 1, nPosRight - nPosLeft - 1);
        // nothing between '{' and '}'
        CC_BREAK_IF(pointStr.length() == 0);
        
        int nPos1 = pointStr.find('{');
        int nPos2 = pointStr.find('}');
        // contain '{' or '}'
        CC_BREAK_IF(nPos1 != (int)std::string::npos || nPos2 != (int)std::string::npos);
        
        split(pointStr, ",", strs);
        if (strs.size() != 3 || strs[0].length() == 0 || strs[1].length() == 0 || strs[2].length() == 0)
        {
            strs.clear();
            break;
        }
        
        bRet = true;
    } while (0);
    
    return bRet;
}

cocos2d::Color3B Color3BFromString(const char* pszContent)
{
    std::vector<std::string> strs;
    if(!splitWithForm(pszContent, strs)) return cocos2d::ccBLACK;
    
    int r = (int) atoi(strs[0].c_str());
    int g = (int) atoi(strs[1].c_str());
    int b = (int) atoi(strs[2].c_str());
    
    return cocos2d::Color3B(r,g,b);
}