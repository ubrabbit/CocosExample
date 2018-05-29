#include "ActorBase.h"
#include "GameRoot.h"
bool ActorBase::initWithActorData(ActorData *data)
{
 
    bool bRet = false;
    do {
        setActorData(data);
        totalBlood = data->getblood();
        _action_attack = NULL;
        _action_attack_flip = NULL;
        _action_run = NULL;
        _action_run_flip = NULL;
        _action_stand = NULL;
        _action_stand_flip = NULL;
        _action_dead = NULL;
        _action_dead_flip = NULL;

        _currentAnimateAction = NULL;
        
        CCSpriteFrameCache* cache =
        CCSpriteFrameCache::sharedSpriteFrameCache();
        char str[64] = {0};
        char strf[64] = {0};
        
        CCArray* _attackFrames =  CCArray::create();
        CCArray* _attackFrames_flip =  CCArray::create();

        for (int i = 0; i < 4; i++) {
            sprintf(str, "%s_%d.png", data->getActorID().c_str(), i);
            sprintf(strf, "%sf_%d.png", data->getActorID().c_str(), i);
   
            _attackFrames->addObject(cache->spriteFrameByName(str));
            _attackFrames_flip->addObject(cache->spriteFrameByName(strf));
        }

        _action_attack = CCAnimate::create(
                                           CCAnimation::createWithSpriteFrames(_attackFrames,0.3f));
        _action_attack->retain();
        _action_attack_flip = CCAnimate::create(
                                                CCAnimation::createWithSpriteFrames(_attackFrames_flip,0.3f));
        _action_attack_flip->retain();
        
        CCArray* _runFrames =  CCArray::createWithCapacity(10);
        CCArray* _runFrames_flip =  CCArray::createWithCapacity(10);
        for (int i = 4; i < 6; i++) {
            sprintf(str, "%s_%d.png", data->getActorID().c_str(), i);
            sprintf(strf, "%sf_%d.png", data->getActorID().c_str(), i);
            _runFrames->addObject(cache->spriteFrameByName(str));
            _runFrames_flip->addObject(cache->spriteFrameByName(strf));
        }
        _action_run = CCAnimate::create(
                                        CCAnimation::createWithSpriteFrames(_runFrames,0.3f));
        _action_run->retain();
        _action_run_flip = CCAnimate::create(
                                             CCAnimation::createWithSpriteFrames(_runFrames_flip,0.3f));
        _action_run_flip->retain();
        
        CCArray* _standFrames =   CCArray::createWithCapacity(10);
        CCArray* _standFrames_flip =   CCArray::createWithCapacity(10);
        for (int i = 6; i < 7; i++) {
            sprintf(str, "%s_%d.png", data->getActorID().c_str(), i);
            sprintf(strf, "%sf_%d.png", data->getActorID().c_str(), i);
            _standFrames->addObject(cache->spriteFrameByName(str));
            _standFrames_flip->addObject(cache->spriteFrameByName(strf));
        }
        _action_stand = CCAnimate::create(
                                          CCAnimation::createWithSpriteFrames(_standFrames,0.3f));
        _action_stand->retain();
        _action_stand_flip = CCAnimate::create(
                                               CCAnimation::createWithSpriteFrames(_standFrames_flip,0.3f));
        _action_stand_flip->retain();
        
        CCArray* _deadFrames =  CCArray::createWithCapacity(10);
        CCArray* _deadFrames_flip =  CCArray::createWithCapacity(10);
        for (int i = 7; i < 9; i++) {
            sprintf(str, "%s_%d.png", data->getActorID().c_str(), i);
            sprintf(strf, "%sf_%d.png", data->getActorID().c_str(), i);
            _deadFrames->addObject(cache->spriteFrameByName(str));
            _deadFrames_flip->addObject(cache->spriteFrameByName(strf));
        }
        _action_dead = CCAnimate::create(
                                         CCAnimation::createWithSpriteFrames(_deadFrames,0.3f));
        _action_dead->retain();
        _action_dead_flip = CCAnimate::create(
                                              CCAnimation::createWithSpriteFrames(_deadFrames_flip,0.3f));
        _action_dead_flip->retain();
        
        if (mActorData->getGroupID().compare("1")) {
              sprintf(str, "%s_%d.png", data->getActorID().c_str(),6);
            mActorDir = Left;
            
        }else{
              sprintf(str, "%sf_%d.png", data->getActorID().c_str(), 6);
            mActorDir = Right;
        }
       _sprite =  CCSprite::createWithSpriteFrameName(str);
        _sprite->retain();
        this->addChild(_sprite);

        healthBar = CCProgressTimer::create(CCSprite::create("health_bar_green.png"));
        
		healthBar->setType(kCCProgressTimerTypeBar);
		healthBar->setMidpoint(ccp(0, 0));
		healthBar->setBarChangeRate(ccp(1, 0));
		healthBar->setPercentage(100);
		healthBar->setScale(0.1f);
		healthBar->setPosition(ccp(0,_sprite->getContentSize().height* 0.5f));
		this->addChild(healthBar,2);
        CCSprite *redBar = CCSprite::create("health_bar_red.png");
        redBar->setPosition(healthBar->getPosition());
        redBar->setScale(0.1);
        this->addChild(redBar,1);
        winsize = CCDirector::sharedDirector()->getWinSize();
   
        bRet = true;
    } while (0);
    return bRet;
}
ActorBase* ActorBase::create(ActorData* data)
{
    ActorBase* actorbase =new ActorBase();
    if (actorbase&&actorbase->initWithActorData(data)) {
        actorbase->autorelease();
        return actorbase;
    }else
    {
       
        delete actorbase;
        actorbase =NULL;
        return NULL;
    }
}

ActorBase::~ActorBase()
{
    CC_SAFE_RELEASE_NULL(mActorData);
    CC_SAFE_RELEASE_NULL(_action_attack);
    CC_SAFE_RELEASE_NULL(_action_attack_flip);
    CC_SAFE_RELEASE_NULL(_action_dead);
    CC_SAFE_RELEASE_NULL(_action_dead_flip);
    CC_SAFE_RELEASE_NULL(_action_run);
    CC_SAFE_RELEASE_NULL(_action_run_flip);
    CC_SAFE_RELEASE_NULL(_action_stand);
    CC_SAFE_RELEASE_NULL(_action_stand_flip);
}
void ActorBase::setoriginalPos(cocos2d::CCPoint var)
{
     if (mActorData->getGroupID().compare("1")==0)
         var.x += winsize.width/5;
    else
        var.x -=  winsize.width/5;
    _originalPos = var;
}
void ActorBase::start()
{
    this->scheduleOnce(schedule_selector(ActorBase::startRun), 0.5f);
    schedule(schedule_selector(ActorBase::actorLogic), 0.1f,kCCRepeatForever,0.6f);
}
ActorDir ActorBase::getActorDir(void){
    if (_target) {
        
        if (_target->getPositionX()>this->getPositionX()) {
            mActorDir = Right;
        }else{
            mActorDir = Left;
        }
    }
    if (_originalPos.equals(this->getPosition())) {
          if (mActorData->getGroupID().compare("1")==0)
              mActorDir = Right;
        else
            mActorDir = Left;
    }
}
void ActorBase::backToLine()
{
    
    if (isBack) {
        return;
    }
    CCArray* arr;
    if (mActorData->getGroupID().compare("1")==0) {
        arr  =   GameRoot::shareGameRoot()->getactorArrL();
    }else
        arr = GameRoot::shareGameRoot()->getactorArrR();
    
  //  CCSize winsize = CCDirector::sharedDirector()->getWinSize();
    
    moveToPositon( _originalPos);
    isBack = true;
}
void ActorBase::StateToRun()
{
    getActorDir();
	if (mActorDir == Left)
		RunAnimateAction_RepeatForever(_action_run);
	else
		RunAnimateAction_RepeatForever(_action_run_flip);
}

void ActorBase::StateToAttack()
{
    
//	currentAnimateActionStop();

}

void ActorBase::StateToDead()
{
   
	currentAnimateActionStop();
     getActorDir();
	if (mActorDir == Left)
		RunAnimateAction_once(_action_dead,callfunc_selector(ActorBase::dealDead));
	else
		RunAnimateAction_once(_action_dead_flip,callfunc_selector(ActorBase::dealDead));
}

void ActorBase::StateToStand()
{
    getActorDir();
	if (mActorDir == Left)
		RunAnimateAction_RepeatForever(_action_stand);
	else
		RunAnimateAction_RepeatForever(_action_stand_flip);
}

void ActorBase::currentAnimateActionStop()
{
    
	if (_currentAnimateAction != NULL)
		_sprite->stopAction(_currentAnimateAction);
    this->stopAllActions();
    
}

void ActorBase::RunAnimateAction_RepeatForever(CCAnimate* action)
{
	currentAnimateActionStop();
	_currentAnimateAction = _sprite-> runAction(CCRepeatForever::create(action));
}
void ActorBase:: RunAnimateAction_once(CCAnimate* action,SEL_CallFunc selector)
{
    CCSequence *sequece = CCSequence::create(action,CCCallFunc::create(this, selector),NULL);
    _onceAnimateAction =  _sprite->runAction(sequece);
}
 void ActorBase:: oneceAnimteActionStop()
{
   // this->scheduleOnce(schedule_selector(ActorBase::StateToAttack), 1.5f);
}
void ActorBase::actorLogic(){
    if (mActorData->getblood()<=0) {
        stopAllActions();
        unscheduleAllSelectors();
        isDead =true;
        StateToDead();
    }
        if (!isDead&& _target)
        {
            if (_target-> mActorData->getblood()<=0) {
                StateToStand();
                unschedule(schedule_selector(ActorBase::fire));
                _target =NULL;
                //CC_SAFE_RELEASE_NULL(_target);
         //       scheduleOnce(schedule_selector(ActorBase::findAnotherTarget), 2.0f) ;
            }
          }
    
    if (!_target)
    {
        findAnotherTarget();
    }else
    {

    }
    
}
void ActorBase::dealDead()
{
    removeFromParentAndCleanup(true);
    CCArray* arr;
    if (mActorData->getGroupID().compare("1")==0) {
     arr  =   GameRoot::shareGameRoot()->getactorArrL();
    }else
        arr = GameRoot::shareGameRoot()->getactorArrR();
      int index =CC_INVALID_INDEX;
    CCObject* obj;
    CCARRAY_FOREACH(arr, obj)
    {
        ActorBase* actor = (ActorBase*)obj;
        int tag = actor->getTag();
        if (tag == this->getTag()) {
            index = arr->indexOfObject(obj);
            break;
        }
    }
    if (index!=CC_INVALID_INDEX) {
        arr->removeObjectAtIndex(index);
        CCLog("Rcount:%d", GameRoot::shareGameRoot()->getactorArrR()->count());
         CCLog("Lcount:%d", GameRoot::shareGameRoot()->getactorArrL()->count());
    }
}
void ActorBase::findAnotherTarget()
{
    if (isDead) {
        return;
    }
    CCArray *enmeys ;
    if (mActorData->getGroupID().compare("1")) {
        enmeys=GameRoot::shareGameRoot()->getactorArrL();
       
    }else
        enmeys = GameRoot::shareGameRoot()->getactorArrR();
    // CCLog("count:%d",enmeys->count());
    if (enmeys->count()==0)
    {
        backToLine();//敌人此时都消灭了，回到防线内
        return;
    }else
        isBack = false;
    //查找最近距离的敌人
    ActorBase* closestEnemy = NULL;
	double maxDistant = 99999;

	CCObject* temp;
	CCARRAY_FOREACH(enmeys, temp){
		ActorBase* enemy = (ActorBase*)temp;
		if(enemy->getActorData()->getblood() <= 0){
			continue;
		}
        if (enemy->getPositionY()==this->getPositionY()) {
            closestEnemy = enemy;
            break;
        }
		double curDistance = ccpDistance(this->getPosition(), enemy->getPosition());
		if(curDistance < maxDistant){
			closestEnemy = enemy;
			maxDistant = curDistance;
		}
	}
    if (closestEnemy) {
 
	settarget(closestEnemy);
    StateToRun();

        //这里，操作同一“更新”时，先停止更新，在更新
    this->unscheduleUpdate();
    this->scheduleUpdate();
    }
    
}
void ActorBase::startRun()
{
    
    CCArray *arr;
    CCArray *arr1;
    if (getActorData()->getGroupID().compare("1")==0) {
        arr = GameRoot::shareGameRoot()-> getactorArrL();
        arr1 =   GameRoot::shareGameRoot()-> getactorArrR();
    }else{
           arr = GameRoot::shareGameRoot()-> getactorArrR();
        arr1 = GameRoot::shareGameRoot()-> getactorArrL();
    }
//    int index = -1;
//    index = arr->indexOfObject(this);
//    if (index!=CC_INVALID_INDEX) {
//        if (index<arr1->count()) {
//            settarget((ActorBase*)arr1->objectAtIndex(index));
//        }
//    }
    CCLOG("arr count: %d ,arr1 count:%d",arr->count(),arr1->count());
    if (arr1->count()==0) {
        return;
    }
   
 //   moveToTarget();
    CCSize winsize = CCDirector::sharedDirector()->getWinSize();
    moveToPositon(ccp(winsize.width/2, this->getPositionY()));
    this->scheduleUpdate();
}
void ActorBase::moveToPositon(cocos2d::CCPoint pos)
{
    if (this->getPosition().equals(pos)) {
        return;
    }
    _destinationPos = pos;
        if (pos.x>this->getPositionX()) {
            mActorDir = Right;
        }else{
            mActorDir = Left;
        }
    

    float dis = ccpDistance(this->getPosition(), pos);
    float time = dis/mActorData->getspeed();
    CCMoveTo *move = CCMoveTo::create(time, pos);
    _move = move;
     StateToRun();
    this->runAction(CCSequence::create(move,CCCallFunc::create(this, callfunc_selector(ActorBase::StateToStand)),NULL));
}
void ActorBase::moveToTarget()
{
    if (_target) {
        CCPoint pos;
        if (_target->getActorDir()==Left) {

            pos = ccp(_target->getPositionX()+40 ,_target->getPositionY());
            if (pos.x>=winsize.width-20) {
                pos.x = winsize.width-20;
            }
        }else{
             pos = ccp(_target->getPositionX()-40,_target->getPositionY());
            if (pos.x<=20) {
                pos.x =20;
            }
        }
        moveToPositon(pos);
    }
}
void ActorBase::startAttack(){
    int val = mActorData->getdamage();
    if (_target)
   _target ->attackedByEnemy(val, false);
 }
void ActorBase::fire(){
    getActorDir();
    if (mActorDir == Left)
    RunAnimateAction_once(_action_attack,callfunc_selector(ActorBase::startAttack));
    else
         RunAnimateAction_once(_action_attack_flip,callfunc_selector(ActorBase::startAttack));
}

void ActorBase::attackedByEnemy(int damageval,bool isBoom)
{
    int bloodval = mActorData->getblood();
    bloodval -= damageval;
    mActorData->setblood(bloodval);
    
    CCString *str = CCString::createWithFormat("-%d",damageval);
    //  CCLog(str->getCString());
    float font = 30;
    if (isBoom) {
        font = 60;
    }
    CCLabelTTF* demageLab = CCLabelTTF::create(str->getCString(),  "Marker Felt", font);
    if (isBoom) {
        demageLab->setColor(ccYELLOW);
    }else
        demageLab->setColor(ccRED);
    float x = CCRANDOM_0_1()*40;
    float y = CCRANDOM_0_1()*40;
    demageLab->setPositionX(x);
    demageLab->setPositionY(y);
    this->addChild(demageLab);
    CCActionInterval* scale=CCScaleTo::create(10, 0.5);
    CCActionInterval *fade = CCFadeOut::create(0.5);
    demageLab->runAction(CCSpawn::create(scale,fade,NULL));
    
    float f = (float)mActorData->getblood()/totalBlood;
    healthBar->setPercentage(f*100);
}
void ActorBase:: update(float dt){
    if (!_target) {
        return;
    }
 //碰撞检测
    CCRect targetRect =
    CCRectMake(
                                   _target->getPosition().x,
                                   _target->getPosition().y,
                                   _target->_sprite->getContentSize().width,
                                  _target->_sprite->getContentSize().height);

    CCRect actorRect = CCRectMake(
                                   this->getPosition().x ,
                                  this->getPosition().y ,
                                   _sprite->getContentSize().width+mActorData->getattackRange(),
                                   _sprite->getContentSize().height+mActorData->getattackRange());
    int YY=_target->getPositionY()-this->getPositionY();
    if (actorRect.intersectsRect(targetRect)&& abs(YY)<5) {
        this->unscheduleUpdate();
        this->StateToStand();
        //遭遇敌人，在攻击范围内，开始攻击
        schedule( schedule_selector(ActorBase::fire), 1.0f);
    }else
    {
        // 跑向敌人，跟踪算法请参考： http://blog.csdn.net/looffer/article/details/8846159
        CCPoint targetPos = _target->getPosition();
        CCPoint myPos = this->getPosition();
        double deltax = targetPos.x - myPos.x;
        double deltay = targetPos.y - myPos.y;
        if( deltax == 0 )
        {
            if( targetPos.y >= myPos.y )             // 子弹需要下移
                deltax = 0.0000001;
            else                                 // 子弹需要上移
                deltax = -0.0000001;
        }
        
        //   同理，对deltay作判断
        
        if( deltay == 0 )
        {
            if( targetPos.x >= myPos.x )             // 子弹需要右移
                deltay = 0.0000001;
            else                                 // 子弹需要左移
                deltay = -0.0000001;
        }
        
        //   现在对角度所处的项限作判断
        float angle;
        if( deltax>0 && deltay>0 )
            angle = atan(fabs(deltay/deltax));           // 第一项限
        
        else if( deltax<0 && deltay>0 )
            angle =  kmPI-atan(fabs(deltay/deltax))  ;        // 第二项限
        
        else if( deltax<0 && deltay<0 )
            angle = kmPI+atan(fabs(deltay/deltax)) ;         // 第三项限
        else
            angle = 2*kmPI-atan(fabs(deltay/deltax));
        
        float x =cosf(angle) * mActorData->getspeed()* dt;
        float y = sinf(angle) * mActorData->getspeed() * dt;
        
        this->setPosition(ccp(myPos.x + x, myPos.y + y));
    }
}
int ActorBase::less(const CCObject* in_pCcObj0, const CCObject* in_pCcObj1) {
    return ((ActorBase*)in_pCcObj0)->getPositionY() < ((ActorBase*)in_pCcObj1)->getPositionY();
}

void ActorBase::sortActors(CCArray* array) {
    std::sort(array->data->arr, array->data->arr + array->data->num, ActorBase::less);
}

