#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //inicio Batch de imagenes
    _batchNode = CCSpriteBatchNode::create("Spritesheets/Sprites.pvr.ccz");
    //Defino datos plist
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Spritesheets/Sprites.plist");
    //Creo nave
    _nave = CCSprite::createWithSpriteFrameName("SpaceFlier_sm_1.png");
    //Obtengo tamaño pantalla
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    //seteo posicion sprite
    _nave->setPosition(ccp(winSize.width*0.1,winSize.height*0.5));
    //Añado nave al nodo
    this->addChild(_nave,1);
    //Crear Parallax
    _nodoFondo = CCParallaxNodeExtras::node(); //Crea Nodo Paralax
    this->addChild(_nodoFondo,-1); //Lo agrega como para del Nodo principal
    
    //Crear los sprites que se agregan al nodo Parallax
    _polvoEstelar1 = CCSprite::create("Backgrounds/bg_front_spacedust.png");
    _polvoEstelar2 = CCSprite::create("Backgrounds/bg_front_spacedust.png");
    _amanecerPlanet = CCSprite::create("Backgrounds/bg_planetsunrise.png");
    _anomEspacial = CCSprite::create("Backgrounds/bg_spacialanomaly.png");
    _anomEspacial2 = CCSprite::create("Backgrounds/bg_spacialanomaly2.png");
    _galaxia = CCSprite::create("Backgrounds/bg_galaxy.png");
    
    //Determinar la velocidad del Parallax y del Polvo Espacial
    CCPoint velPolvo = ccp(0.1,0.1);
    CCPoint velNodo = ccp(0.05,0.05);
    
    //Agregar nodos al parallax
    _nodoFondo->addChild(_polvoEstelar1, 0, velPolvo, ccp(0,winSize.height/2)); // agrega polvo estelar1
    _nodoFondo->addChild(_polvoEstelar2, 0, velPolvo, ccp(_polvoEstelar1->getContentSize().width,winSize.height/2)); //agrga polvo estelar 2
    _nodoFondo->addChild(_galaxia, -1, velNodo, ccp(0, winSize.height * 0.7)); // agrega galaxia
    _nodoFondo->addChild(_amanecerPlanet, -1, velNodo, ccp(600, winSize.height * 0)); //agrega amanecer planetario
    _nodoFondo->addChild(_anomEspacial, -1, velNodo, ccp(900, winSize.height * 0.3));//agrega anomalia espacial
    _nodoFondo->addChild(_anomEspacial2, -1, velNodo, ccp(1500, winSize.height * 0.9)); //agrega anomalia espacial 2
    
    //agregar Estrellas
    HelloWorld::addChild(CCParticleSystemQuad::create("Particles/Stars1.plist"));
    HelloWorld::addChild(CCParticleSystemQuad::create("Particles/Stars2.plist"));
    HelloWorld::addChild(CCParticleSystemQuad::create("Particles/Stars3.plist"));
    //Se agrega el update del scroll al nodo.
    this->scheduleUpdate();
    
    //Habilita el accelerometro
    this->setAccelerometerEnabled(true);
    
    //Asteroides
    #define KNUMASTEROIDS 15 // Cantidad de asteroides del nivel
    
    /* Se crea un arreglo donde se agregaran todos los asteroides de la etapa*/
    asteroides = new CCArray();
    /*Se crea un ciclo for para crear cada asteroide*/
    for(int i = 0; i < KNUMASTEROIDS; ++i) {
        CCSprite *spriteAsteroide = CCSprite::createWithSpriteFrameName("asteroid.png");
        spriteAsteroide->setVisible(false);
        //SEta error, se debe añadir al  nodo principal
        this->addChild(spriteAsteroide,1);
        
      //  _batchNode->addChild(asteroid,2);
        asteroides->addObject(spriteAsteroide);
    }
    
    //Rayo Laser
    #define KNUMLASERS 5
    rayosLaser = new CCArray();
    for(int i = 0; i < KNUMLASERS; ++i) {
        CCSprite * spriteRayoLaser = CCSprite::createWithSpriteFrameName("laserbeam_blue.png");
        spriteRayoLaser->setVisible(false);
        this->addChild(spriteRayoLaser);
        rayosLaser->addObject(spriteRayoLaser);
    }
    // Habilita pantalla touch
    this->setTouchEnabled(true);
    //setea Vidas
    _vidas = 5;
    double tiempo = getTimeTick();
    tiempoGameOver = tiempo + 3000;
    
    return true;
}

/* En el update se setea el valor del scroll con ccpoint y luego
   se setea la posicion del nodo fondo agregando la opsicion inicial y la constante scroll
  por el dt del update*/
void HelloWorld::update(float dt){
    //Implementar Scroll
    CCPoint fondoVerticalScl = ccp(-1000,0);
    _nodoFondo->setPosition(ccpAdd(_nodoFondo->getPosition(), ccpMult(fondoVerticalScl, dt)));
    
    CCArray *spaceDusts = CCArray::createWithCapacity(2);
    spaceDusts->addObject(_polvoEstelar1);
    spaceDusts->addObject(_polvoEstelar2);
    for ( int ii = 0; ii <spaceDusts->count(); ii++ ) {
        CCSprite * spaceDust = (CCSprite *)(spaceDusts->objectAtIndex(ii));
        float xPosition = _nodoFondo->convertToWorldSpace(spaceDust->getPosition()).x;
        float size = spaceDust->getContentSize().width;
        if ( xPosition < -size/2 ) {
            _nodoFondo->incrementOffSet(ccp(spaceDust->getContentSize().width*2,0),spaceDust);
        }
    }
    
    CCArray *backGrounds = CCArray::createWithCapacity(4);
    backGrounds->addObject(_galaxia);
    backGrounds->addObject(_amanecerPlanet);
    backGrounds->addObject(_anomEspacial);
    backGrounds->addObject(_anomEspacial2);
    for ( int ii = 0; ii <backGrounds->count(); ii++ ) {
        CCSprite * background = (CCSprite *)(backGrounds->objectAtIndex(ii));
        float xPosition = _nodoFondo->convertToWorldSpace(background->getPosition()).x;
        float size = background->getContentSize().width;
        if ( xPosition < -size ) {
            _nodoFondo->incrementOffSet(ccp(2000,0),background);
        }
    }
    /*Obtiene el valor de la pantalla*/
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    /*Setea el maximo valor que puede alcanzar la nave en movimiento*/
    float maxY = winSize.height - _nave->getContentSize().height/2;
    
    /*Setea el minimo valor que puede moverse la nave para que no salga de la pantalla*/
    float minY = _nave->getContentSize().height/2;
    /*Calcula la diferencia entre el valor calculado por cada movimiento del acelerometro
     por el valor que se resfresca el update*/
    float diff = (_navePorSegundoY * dt);
    
    /*Establece la nueva posicion, a traves del calculo del moviemiento del acelerometro 
     y la posicion actual de la nave*/
    float newY = _nave->getPosition().y + diff;
    
    /*Verifica que el valor no pase los establecido previamente*/
    newY = MIN(MAX(newY, minY), maxY);
    
    /*Cambia la posicion de la nave*/
    _nave->setPosition(ccp(_nave->getPosition().x, newY));
    
    // ************ ASTEROIDES *************
    
    float curTimeMillis = getTimeTick(); //Obtiene el tiempo actual
    
    //Si el tiempo actual es mayor al proximo a generar

    if (curTimeMillis > _proxAsteroideAgenerar) {         
        float randMillisecs = randomValor(0.20,1.0) * 1000; //Genera random entre 0.2 y 1
        
        /* Setea este valor con el valor del tiempo actual y el generado anteriormente*/
        _proxAsteroideAgenerar = randMillisecs + curTimeMillis; 
        
        /*Obtiene un valor random entre la primera y ultima posicion del eje Y*/
        float randY = randomValor(0.0,winSize.height);
        /*Genera el tiempo de duracion de la accion del asteroide*/
        float randDuration = randomValor(2.0,8.0);
        
        //Obtiene Asteroide desde el arreglo de asterioides de manera secuencia
        CCSprite *actualAsteroide = (CCSprite *)asteroides->objectAtIndex(_proxAsteroide);
        _proxAsteroide++;
        
        // en caso de que este valor pase el tamaño del arreglo lo setea en cero/
        if (_proxAsteroide >= asteroides->count())
            _proxAsteroide = 0;
        
        // Para todas las acciones en caso en que se ejecute una actual
        actualAsteroide->stopAllActions();
        
        // Setea la posicion Y generada anteriormente
        actualAsteroide->setPosition( ccp(winSize.width+actualAsteroide->getContentSize().width/2, randY));
        // Hace visible el asteroide
        actualAsteroide->setVisible(true);
        //Ejecuta accion del asteroide
        actualAsteroide->runAction(CCSequence::create(CCMoveBy::create(randDuration, ccp(-winSize.width-actualAsteroide->getContentSize().width, 0)), CCCallFuncN::create(this, callfuncN_selector(HelloWorld::setInvisible)), NULL));
    }
    
    //Coalisiones Basicas
    // Asteroids
    CCObject* objAsteroide;
    CCObject* objRayoLaser;
    CCARRAY_FOREACH(asteroides, objAsteroide){
        if (!((CCSprite *) objAsteroide)->isVisible() )
            continue;
        CCARRAY_FOREACH(rayosLaser, objRayoLaser){
            if (!((CCSprite *) objRayoLaser)->isVisible())
                continue;
            if (((CCSprite *) objRayoLaser)->boundingBox().intersectsRect(((CCSprite *)objAsteroide)->boundingBox()) ) {
                ((CCSprite *)objRayoLaser)->setVisible(false);
                ((CCSprite *)objAsteroide)->setVisible(false);
                continue;
            }
        }
        if (_nave->boundingBox().intersectsRect(((CCSprite *)objAsteroide)->boundingBox()) ) {
            ((CCSprite *)objAsteroide)->setVisible(false);
            _nave->runAction( CCBlink::create(1.0, 9));
            _vidas--;
        }
    }
    
    // Vidas GameOver
    if (_vidas <= 0) { //Si las vidas son menoes que 0
        _nave->stopAllActions(); // para la nave
        _nave->setVisible(false); // la oculta
        this->escenaFin(KYOULOSE); // Crea escena fin Perdiste
    } else if (curTimeMillis >= tiempoGameOver) { // Si el tiempo es mayor al de game over
        this->escenaFin(KYOUWIN); // crea escena ganaste!!!!!!!
    }
    
}

void HelloWorld::didAccelerate(CCAcceleration *pValorAceleracion){
#define KFILTERINGFACTOR 0.1
#define KRESTACCELX -0.6
#define KSHIPMAXPOINTPERSEC (winSize.height*0.5)
#define KMAXDIFFX 0.2
    
    double rollingX;
    
/* En cocos el invertir los ejes depende de la orientacion del dispositivo*/
    pValorAceleracion->x = pValorAceleracion->y;
    /*CAlculo del movimiento de la nave*/
    
    rollingX = (pValorAceleracion->x * KFILTERINGFACTOR) + (rollingX * (1.0 - KFILTERINGFACTOR));
    /*Acelarecion del movimiento*/
    float acelX = pValorAceleracion->x - rollingX;
    /*Obtengo el tamaño de la pantalla*/
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    /*Diferencia de aceleracion factor aceleracion - constante aceleracion*/
    float acelDif = acelX - KRESTACCELX;
    float fraccionAcel = acelDif / KMAXDIFFX;
    _navePorSegundoY = KSHIPMAXPOINTPERSEC * fraccionAcel;
}

float HelloWorld::randomValor(float min, float max){
    /* Genera un numero entre los establecido*/
    return (((float) arc4random() / 0xFFFFFFFFu) * (max - min)) + min;
}

float HelloWorld::getTimeTick() {
    /*Obtiene el tiempo n milisegundos*/
    timeval time;
    gettimeofday(&time, NULL);
    unsigned long millisecs = (time.tv_sec * 1000) + (time.tv_usec/1000);
    return (float) millisecs;
}

void HelloWorld::setInvisible(CCNode * node) {
    node->setVisible(false);
}

void HelloWorld::ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    // Obtiene el rayo segun el index definido
    CCSprite *shipLaser = (CCSprite *)rayosLaser->objectAtIndex(proxRayoLaser);
    
    // Si es mayor a la cantidad de arreglos setea 0
    if ( proxRayoLaser >= rayosLaser->count() )
        proxRayoLaser = 0;
    
    shipLaser->setPosition( ccpAdd( _nave->getPosition(), ccp(shipLaser->getContentSize().width/2, 0)));
    shipLaser->setVisible(true);
    shipLaser->stopAllActions();
    shipLaser->runAction(CCSequence::create(CCMoveBy::create(0.5,ccp(winSize.width, 0)), CCCallFuncN::create(this, callfuncN_selector(HelloWorld::setInvisible)), NULL  // DO NOT FORGET TO TERMINATE WITH NULL
                                            ));
}
/* Funcion para reiniciar juego*/
void HelloWorld::reiniciarJuego(){
    // Reemplaza la escena con una accion flip a la funcion escena
    CCDirector::sharedDirector()->replaceScene(CCTransitionZoomFlipX::create(0.5, this->scene()));
    //Habilitar nuevamente update
    this->scheduleUpdate();
}

void HelloWorld::escenaFin(FinJuego finJuego){
    if(gameOver){
        return;
    }
    gameOver = true;
    
    //Obtengo eltamaño de la pantalla
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    //Mensaje Victoria
    char mensaje[10] = "Ganaste!!";
    if (finJuego== KYOULOSE){
        strcpy(mensaje, "Perdiste!");
    }
    //Crea label a mensaje victoria
    CCLabelBMFont * texto ;
    texto = CCLabelBMFont::create(mensaje, "Arial.fnt");
    texto->setScale(0.1);
    texto->setPosition(ccp(winSize.width/2 , winSize.height*0.6));
    this->addChild(texto);
    
    //Crea label restart!!!!!
    CCLabelBMFont * textoReiniciar;
    strcpy(mensaje,"Restart");
    textoReiniciar = CCLabelBMFont::create(mensaje, "Arial.fnt");
    
    //crea item restart fin del juego
    CCMenuItemLabel *restartItem = CCMenuItemLabel::create(textoReiniciar, this, menu_selector(HelloWorld::reiniciarJuego) );
    restartItem->setScale(0.1);
    restartItem->setPosition( ccp(winSize.width/2, winSize.height*0.4));
    
    //Menu Restart
    CCMenu *menu = CCMenu::create(restartItem, NULL);
    menu->setPosition(CCPointZero);
    this->addChild(menu);
    
    //Limpia MEnu y TExto
    restartItem->runAction(CCScaleTo::create(0.5, 1.0));
    texto ->runAction(CCScaleTo::create(0.5, 1.0));
    
    //Para el juego, parando el update
    this->unscheduleUpdate();
    
    
}
