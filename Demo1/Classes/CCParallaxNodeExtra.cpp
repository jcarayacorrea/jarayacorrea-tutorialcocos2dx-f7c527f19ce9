//
//  CCParallaxNodeExtra.cpp
//  Demo1
//
//  Created by Juan Carlos Araya on 07-09-13.
//
//

#include "CCParallaxNodeExtra.h"
/*  Dado que la clases CCObject no es public, es necesario modificarla en otra clase
    */
using namespace cocos2d;

class CCPointObject : CCObject {
    CC_SYNTHESIZE(CCPoint, m_tRatio, Ratio);
    CC_SYNTHESIZE(CCPoint, m_tOffSet, Offset);
    CC_SYNTHESIZE(CCNode *, m_pChild, Child);
};

//Genera un Constructor

CCParallaxNodeExtras * CCParallaxNodeExtras::node(){
    return new CCParallaxNodeExtras();
}

void CCParallaxNodeExtras::incrementOffSet(CCPoint offset, CCNode *nodo){
    
    for( unsigned int i = 0; i < m_pParallaxArray->num; i++) { // recorre arreglo del paralax
        CCPointObject *point = (CCPointObject *)m_pParallaxArray->arr[i]; //
        CCNode * curNode = point->getChild();
        if( curNode->isEqual(nodo) ) {
            point->setOffset( ccpAdd(point->getOffset(), offset) );
            break;
        }
    }
}