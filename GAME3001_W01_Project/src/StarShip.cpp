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

void StarShip::Seek() {

}

void StarShip::LookWhereYoureGoing(glm::vec2 target_direction) {

}

void StarShip::m_move() {
	const float dt = Game::Instance().GetDeltaTime();

	const glm::vec2 initial_position = GetTransform()->position;

	const glm::vec2 velocity_term = GetRigidBody()->velocity * dt;

	const glm::vec2 acceleration_term = GetRigidBody()->acceleration * 0.5f;

	glm::vec2 final_position = initial_position + velocity_term + acceleration_term;

	GetTransform()->position = final_position;

	GetRigidBody()->velocity += GetRigidBody()->acceleration;

	GetRigidBody()->velocity = Util::Clamp(GetRigidBody()->velocity, GetMaxSpeed());

}

