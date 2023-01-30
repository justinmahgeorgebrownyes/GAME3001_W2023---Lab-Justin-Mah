#include "StarShip.h"
#include "TextureManager.h"
#include "Util.h"
#include "Game.h"

StarShip::StarShip() {
	TextureManager::Instance().Load("../Assets/textures/ncl.png", "starship");

	const auto size = TextureManager::Instance().GetTextureSize("starship");
	SetWidth(static_cast<int>(size.x));
	SetHeight(static_cast<int>(size.y));
	GetTransform()->position = glm::vec2(100.0f, 100.0f);


	GetRigidBody()->bounds = glm::vec2(GetWidth(), GetHeight());
	GetRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	GetRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	GetRigidBody()->isColliding = false;

	SetType(GameObjectType::AGENT);

	//starting motion

	m_maxSpeed = 50.0f;
	m_turnRate = 5.0f;
	m_accelerationRate = 4.0f;

	SetCurrentDirection(glm::vec2(1.0f, 0.0f));

}

StarShip::~StarShip()
= default;

void StarShip::Draw() {
	TextureManager::Instance().Draw("starship", GetTransform()->position, static_cast<double>(GetCurrentHeading()), 255, true);

}

void StarShip::Update() {
	m_move();
}

void StarShip::Clean() {

}

float StarShip::GetMaxSpeed() const
{
	return 0.0f;
}

float StarShip::GetTurnRate() const
{
	return 0.0f;
}

float StarShip::GetAccelerationRate() const
{
	return 0.0f;
}

glm::vec2 StarShip::GetDesiredVelocity() const
{
	return glm::vec2();
}

void StarShip::SetMaxSpeed(float speed)
{
}

void StarShip::SetTurnRate(float angle)
{
	m_turnRate = angle;

}

void StarShip::SetAccelerationRate(float rate)
{
	m_accelerationRate = rate;

}

void StarShip::SetDesiredVelocity(glm::vec2 target_position)
{

	SetTargetPosition(target_position);
	m_desiredVelocity = Util::Normalize(target_position - GetTransform()->position) * GetMaxSpeed();
	GetRigidBody()->velocity = m_desiredVelocity - GetRigidBody()->velocity;



}

void StarShip::Seek() {

	SetDesiredVelocity(GetTargetPosition());

	const glm::vec2 steering_direction = GetDesiredVelocity() - GetCurrentDirection();

	LookWhereYoureGoing(steering_direction);

	GetRigidBody()->acceleration = GetCurrentDirection() * GetAccelerationRate();
}

void StarShip::LookWhereYoureGoing(glm::vec2 target_direction) {

	const float target_rotation = Util::SignedAngle(GetCurrentDirection(), target_direction);

	const float turn_sensitivity = 5.0f;

	if (abs(target_rotation) > turn_sensitivity) {

		if (target_rotation > 0.0f) {

			//turn right

			SetCurrentHeading(GetCurrentHeading() + GetTurnRate());
		}

		else if(target_rotation < 0.0f) {


			//turn left

			SetCurrentHeading(GetCurrentHeading() - GetTurnRate());

		}

	}






}

void StarShip::m_move() {
	
	Seek();

	
	const float dt = Game::Instance().GetDeltaTime();

	const glm::vec2 initial_position = GetTransform()->position;

	const glm::vec2 velocity_term = GetRigidBody()->velocity * dt;

	const glm::vec2 acceleration_term = GetRigidBody()->acceleration * 0.5f;

	glm::vec2 final_position = initial_position + velocity_term + acceleration_term;

	GetTransform()->position = final_position;

	GetRigidBody()->velocity += GetRigidBody()->acceleration;

	GetRigidBody()->velocity = Util::Clamp(GetRigidBody()->velocity, GetMaxSpeed());
	
	
	

}

