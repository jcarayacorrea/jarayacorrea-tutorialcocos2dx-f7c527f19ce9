#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "CCParallaxNodeExtra.h"

using namespace cocos2d;
USING_NS_CC;

typedef enum{
    KYOUWIN,
    KYOULOSE
}FinJuego;

class HelloWorld : public CCLayer
{
private:
    
    CCSpriteBatchNode * _batchNode;
    CCSprite * _nave;
    //Parallax
    CCParallaxNodeExtras * _nodoFondo;
    CCSprite * _polvoEstelar1;
    CCSprite * _polvoEstelar2;
    CCSprite * _anomEspacial;
    CCSprite * _anomEspacial2;
    CCSprite * _amanecerPlanet;
    CCSprite * _galaxia;
    //End Parallax
    // Movimiento con el aceleromentro
    float _navePorSegundoY;
    
    //Asteroides
    CCArray * asteroides;
    int _proxAsteroideAgenerar;
    int _proxAsteroide;
    
    //Rayo Laser
    CCArray * rayosLaser;
    int proxRayoLaser;
    
    //Vidas
    int _vidas;
    
    //GAMEOVER
    double tiempoGameOver;
    bool gameOver;
    
    
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();

    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static CCScene* scene();
    
    //Agregar scroll al parallax
    void update (float dt);
    // DidAccelarate para modifique el acelerometro
    virtual void didAccelerate(CCAcceleration * pValorAceleracion);

    float randomValor(float min, float max);
    void setInvisible(CCNode * nodo);
    float  getTimeTick();
    
    //Habilitar toques para lanzamiento de Rayos Laseer
    virtual void ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    
    void escenaFin(FinJuego finJuego);
    void reiniciarJuego();
    
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
