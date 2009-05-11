#include "console.h"
#include "Creature.hpp"
#include <math.h>

/**
 * The constructor. Sets coordinates to 0,0,0.
 * @param tag The Objects tag.
 * @param model The Objects RCBC model pointer.
 */
Creature::Creature(const string tag, Visual* model) {
	setTag(tag);
	setVisual(*model);
	setMass(1.0f);
	turn_angle_ = 0.0;
	walk_velocity_ = 2.0f;
	running_multiplier_ = 2.0f;
	setTurnRate(1.0f);
	backward_ = false;
	forward_ = false;
	turn_left_ = false;
	turn_right_ = false;
	strafe_left_ = false;
	strafe_right_ = false;
	running_ = false;
	controller_ = NULL;
	controller_ghost_ = NULL;
	setShape(NULL);
}

Creature::~Creature() {
	#warning ['TODO']: deregisterObject me from physics engine, area list, delete shapes...
}

/**
 * Copy constructor.
 * @returns deep copy.
 */
Object* Creature::clone() {
	#warning ['TODO']: Copy constructor.
	return NULL;
}

void Creature::setShape(btCollisionShape* shape = NULL) {
	// If there is already a shape
	if(shape_) {
		delete(shape_);
	}

	// Set the shape...
	if(shape != NULL) { // If we are overiding the default...
		RigidBody::setShape(shape);
	} else { // A default shape
#warning ['TODO']: Don't hardcode these numbers? (although it can be overridden...)
		
		btScalar characterWidth = 0.35;
		btScalar characterHeight = 0.8f-characterWidth;
		shape_ = new btCapsuleShape(characterWidth,characterHeight);
		//shape_ = new btBoxShape(btVector3(0.45f, 0.5f, 0.2f));
	}
	
	btTransform transform;
	transform.setIdentity();
	
	//btTransform transform = getBody()->getWorldTransform();
	delete(body_);
	body_ = new btPairCachingGhostObject();
	body_->setWorldTransform(transform);
	body_->setCollisionShape(shape_);
	body_->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
	
	/*getBody().setSleepingThresholds (0.0, 0.0);
	getBody().setAngularFactor (0.0);*/
	
	btScalar stepHeight = btScalar(0.01);
	delete(controller_);
	controller_ = new btKinematicCharacterController(
						(btPairCachingGhostObject*)body_,
						(btConvexShape*)shape_,
						stepHeight);
}

btVector3& Creature::getPos() {
	return body_->getWorldTransform().getOrigin();
}

void Creature::setArea(Area& area) {
	Area* old_area = getArea();
	if(old_area) { /* If its already in an area */
		if(old_area->getPhysics() == area.getPhysics()) {
			return;
		}
			#warning ['TODO']: If not remove shape from old area physics...
	}
	
	Object::setArea(area);
	Area* newarea = getArea();
	newarea->getPhysics()->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
	btDynamicsWorld* world = newarea->getPhysics()->getWorld();
	
	if(body_) {
		world->addCollisionObject(body_,btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter|btBroadphaseProxy::DefaultFilter);
	}
	if(controller_) {
		world->addCharacter(controller_);            
	}
}

/**
 * Sets the creature as moving backwards.
 * @param turn_rate
 */
void Creature::setTurnRate(const float turn_rate) {
	turn_rate_ = turn_rate;
}

/**
 * Performs a jump action.
 */
void Creature::Jump() {
	LOG("Robots carn't jump %s...", SYMBOL_SADLEY);
}

/**
 * Sets the creature running.
 * @see Run()
 * @see Reverse()
 * @see Forward()
 * @see TurnLeft()
 * @see TurnRight()
 * @see StrafeLeft()
 * @see StrafeRight()
 */
void Creature::Run(const bool isRunning) {
	running_ = isRunning;
}

/**
 * Sets the creature as moving forwards.
 * @see Run()
 * @see Reverse()
 * @see Forward()
 * @see TurnLeft()
 * @see TurnRight()
 * @see StrafeLeft()
 * @see StrafeRight()
 */
void Creature::Forward(const bool state) {
	forward_ = state;
}

/**
 * Sets the creature as moving backwards.
 * @see Run()
 * @see Reverse()
 * @see Forward()
 * @see TurnLeft()
 * @see TurnRight()
 * @see StrafeLeft()
 * @see StrafeRight()
 */
void Creature::Reverse(const bool state) {
	backward_ = state;
}

/**
 * Sets the creature as turning left.
 * @see Run()
 * @see Reverse()
 * @see Forward()
 * @see TurnLeft()
 * @see TurnRight()
 * @see StrafeLeft()
 * @see StrafeRight()
 */
void Creature::TurnLeft(const bool state) {
	turn_left_ = state;
}
/**
 * Sets the creature as turning right.
 * @see Run()
 * @see Reverse()
 * @see Forward()
 * @see TurnLeft()
 * @see TurnRight()
 * @see StrafeLeft()
 * @see StrafeRight()
 */
void Creature::TurnRight(const bool state) {
	turn_right_ = state;
}

/**
 * Sets the creature as strafing left.
 * @see Run()
 * @see Reverse()
 * @see Forward()
 * @see TurnLeft()
 * @see TurnRight()
 * @see StrafeLeft()
 * @see StrafeRight()
 */
void Creature::StrafeLeft(const bool state) {
	strafe_left_ = state;
}

/**
 * Sets the creature as strafing right.
 * @see Run()
 * @see Reverse()
 * @see Forward()
 * @see TurnLeft()
 * @see TurnRight()
 * @see StrafeLeft()
 * @see StrafeRight()
 */
void Creature::StrafeRight(const bool state) {
	strafe_right_ = state;
}

void Creature::update(const int time) {
	float dt = ((float)time - getLastupdate()) / 1000;

	// Get the current position and rotation
	btTransform xform;
	xform = body_->getWorldTransform();
	btVector3 forwardDir = xform.getBasis()[2];
	btVector3 upDir = xform.getBasis()[1];
	btVector3 strafeDir = xform.getBasis()[0];	

	btVector3 walkDirection = btVector3(0.0f, 0.0f, 0.0f);
	btScalar walkSpeed = walk_velocity_ * dt;

	forwardDir[0] = -forwardDir[0];
	// Get any movement flags
	if(running_) {
		walkSpeed *= running_multiplier_;
	}
	if(forward_) {
		walkDirection -= forwardDir;
	}
	if(backward_) {
		walkDirection += forwardDir;
	}
	if(strafe_left_) {
		walkDirection += strafeDir;
	}
	if(strafe_right_) {
		walkDirection -= strafeDir;
	}
	if(turn_left_) {
		turn_angle_ -= turn_rate_ * dt;
	}
	if(turn_right_) {
		turn_angle_ += turn_rate_ * dt;
	}

	// update the position
	xform.setRotation(btQuaternion (btVector3(0.0, 1.0, 0.0), turn_angle_));
	((btPairCachingGhostObject*)body_)->setWorldTransform (xform);
	controller_->setWalkDirection(walkDirection * walkSpeed);

	RigidBody::update(time);
}
