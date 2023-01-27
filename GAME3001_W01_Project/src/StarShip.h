#pragma once

#ifndef __STARSHIP__
#define __STARSHIP__

#include "DisplayObject.h"
#include "Agent.h"

class StarShip : public Agent
{
public:

	StarShip();
	~StarShip();

	void Draw() override;
	void Update() override;
	void Clean() override;

	//gettoer settor

	float GetMaxSpeed() const;
	float GetTurnRate() const;
	float GetAccelerationRate() const;
	glm::vec2 GetDesiredVelocity() const;

	void SetMaxSpeed(float speed);
	void SetTurnRate(float angle);
	void SetAccelerationRate(float rate);
	void SetDesiredVelocity(glm::vec2 target_position);

	//puglib functions

	void Seek();
	void LookWhereYoureGoing(glm::vec2 target_direction);


private:

	float m_maxSpeed;
	float m_turnRate;
	float m_accelerationRate;

	glm::vec2 m_desiredVelocity;

	void m_move();

};


#endif