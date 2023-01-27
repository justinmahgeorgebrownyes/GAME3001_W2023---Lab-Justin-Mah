#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"
#include "InputType.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"
#include "Util.h"

#include <glm/gtc/type_ptr.hpp>


PlayScene::PlayScene()
{
	PlayScene::Start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::Draw()
{
	DrawDisplayList();
	SDL_SetRenderDrawColor(Renderer::Instance().GetRenderer(), 255, 255, 255, 255);
}

void PlayScene::Update()
{
	UpdateDisplayList();

	m_mousePosition.x;

	m_pTarget->Update();
	m_pShip->Update();
	(*m_pShip).Update();

	//glm::vec2 mousePosition = EventManager::Instance().GetMousePosition();
	//float distance = Util::Distance(m_pPlayer->GetTransform()->position, m_pPlaneSprite->GetTransform()->position);
	//if (distance > 100.0f) {
	//	Game::Instance().ChangeSceneState(SceneState::END);
	//}
}

void PlayScene::Clean()
{
	RemoveAllChildren();
}


void PlayScene::HandleEvents()
{
	EventManager::Instance().Update();

	GetPlayerInput();

	GetKeyboardInput();
}

void PlayScene::GetPlayerInput()
{
	switch (m_pCurrentInputType)
	{
	case static_cast<int>(InputType::GAME_CONTROLLER):
	{
		// handle player movement with GameController
		if (SDL_NumJoysticks() > 0)
		{
			if (EventManager::Instance().GetGameController(0) != nullptr)
			{
				constexpr auto dead_zone = 10000;
				if (EventManager::Instance().GetGameController(0)->STICK_LEFT_HORIZONTAL > dead_zone)
				{
					m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_RUN_RIGHT);
					m_playerFacingRight = true;
				}
				else if (EventManager::Instance().GetGameController(0)->STICK_LEFT_HORIZONTAL < -dead_zone)
				{
					m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_RUN_LEFT);
					m_playerFacingRight = false;
				}
				else
				{
					if (m_playerFacingRight)
					{
						m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_IDLE_RIGHT);
					}
					else
					{
						m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_IDLE_LEFT);
					}
				}
			}
		}
	}
	break;
	case static_cast<int>(InputType::KEYBOARD_MOUSE):
	{
		// handle player movement with mouse and keyboard
		if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_A))
		{
			m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_RUN_LEFT);
			m_playerFacingRight = false;
			m_pPlayer->GetTransform()->position += glm::vec2(-10.0f, 0.0f);
			
		}
		else if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_D))
		{
			m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_RUN_RIGHT);
			m_playerFacingRight = true;
			m_pPlayer->GetTransform()->position += glm::vec2(10.0f, 0.0f);
			
		}
		else
		{
			if (m_playerFacingRight)
			{
				m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_IDLE_RIGHT);
			}
			else
			{
				m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_IDLE_LEFT);
			}
		}
	}
	break;
	case static_cast<int>(InputType::ALL):
	{
		if (SDL_NumJoysticks() > 0)
		{
			if (EventManager::Instance().GetGameController(0) != nullptr)
			{
				constexpr auto dead_zone = 10000;
				if (EventManager::Instance().GetGameController(0)->STICK_LEFT_HORIZONTAL > dead_zone
					|| EventManager::Instance().IsKeyDown(SDL_SCANCODE_D))
				{
					m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_RUN_RIGHT);
					m_playerFacingRight = true;
				}
				else if (EventManager::Instance().GetGameController(0)->STICK_LEFT_HORIZONTAL < -dead_zone
					|| EventManager::Instance().IsKeyDown(SDL_SCANCODE_A))
				{
					m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_RUN_LEFT);
					m_playerFacingRight = false;
				}
				else
				{
					if (m_playerFacingRight)
					{
						m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_IDLE_RIGHT);
					}
					else
					{
						m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_IDLE_LEFT);
					}
				}
			}
		}
		else if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_A))
		{
			m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_RUN_LEFT);
			m_playerFacingRight = false;
		}
		else if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_D))
		{
			m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_RUN_RIGHT);
			m_playerFacingRight = true;
		}
		else
		{
			if (m_playerFacingRight)
			{
				m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_IDLE_RIGHT);
			}
			else
			{
				m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_IDLE_LEFT);
			}
		}
	}
	break;
	}
}

void PlayScene::GetKeyboardInput()
{
	if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_ESCAPE))
	{
		Game::Instance().Quit();
	}

	if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_1))
	{
		Game::Instance().ChangeSceneState(SceneState::START);
	}

	if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_2))
	{
		Game::Instance().ChangeSceneState(SceneState::END);
	}
}

void PlayScene::Start()
{
	// Set GUI Title
	m_guiTitle = "Play Scene";
	m_bDebugView = false; // turn off all colliders

	// Player Sprite
	m_pPlayer = new Player();
	m_pPlayer->GetTransform()->position = glm::vec2(50.0f, 50.0f);
	AddChild(m_pPlayer);
	
	m_playerFacingRight = true;

	m_pTarget = new Target();

	m_pTarget->GetTransform()->position = glm::vec2(500.0f, 100.0f);

	AddChild(m_pTarget);
	
	m_pShip = new Ship();
	AddChild(m_pShip);

	m_pStarShip = new StarShip();
	m_pStarShip->GetTransform()->position = glm::vec2(100.0f, 400.0f);
	m_pStarShip->SetTargetPosition(m_pTarget->GetTransform()->position);
	//m_pStarShip->GetCurrentDirection(glm::vec2(1.0f, 0.0f));
	//m_pStarShip->SetEnabled(false);
	AddChild(m_pStarShip);



	/* DO NOT REMOVE */
	ImGuiWindowFrame::Instance().SetGuiFunction([this] { GUI_Function(); });
}


void PlayScene::GUI_Function() 
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	static glm::vec2 targetPosition;
	if (ImGui::SliderFloat2("Target Position", glm::value_ptr(targetPosition), 0.0f, 500.0f)) {
		m_pTarget->GetTransform()->position = targetPosition;
	}

	static glm::vec2 targetVelocity;
	if (ImGui::SliderFloat2("Target Velocity", glm::value_ptr(targetVelocity), -5.0f, 5.0f)) {
		m_pTarget->GetRigidBody()->velocity = targetVelocity;
	}

	if (ImGui::Button("Target Reset")) {
		m_pTarget->GetTransform()->position = glm::vec2(100.0f, 100.0f);
		m_pTarget->GetRigidBody()->velocity = glm::vec2(0.0f, 0.0f);

		targetPosition = m_pTarget->GetTransform()->position;
		targetVelocity = m_pTarget->GetRigidBody()->velocity;
	}



	ImGui::Separator();

	static bool toggleDebug = false;
	if (ImGui::Checkbox("Toggle Debug", &toggleDebug)) {
		m_bDebugView = toggleDebug;
	}

	ImGui::Separator();

	static bool toggleSeek = m_pStarShip->IsEnabled();
	if (ImGui::Checkbox("Toggle Seek", &toggleSeek)) {
		m_pStarShip->SetEnabled(toggleSeek);
	}




	/*ImGui::Begin("Your Window Title Goes Here", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);

	ImGui::Text("Player Input");
	ImGui::RadioButton("Keyboard / Mouse", &m_pCurrentInputType, static_cast<int>(InputType::KEYBOARD_MOUSE)); ImGui::SameLine();
	ImGui::RadioButton("Game Controller", &m_pCurrentInputType, static_cast<int>(InputType::GAME_CONTROLLER)); ImGui::SameLine();
	ImGui::RadioButton("Both", &m_pCurrentInputType, static_cast<int>(InputType::ALL));

	ImGui::Separator();

	//if(ImGui::Button("My Button"))
	//{
		//std::cout << "My Button Pressed" << std::endl;
	//}

	ImGui::Separator();


	static float float3[3] = { m_pPlayer->GetTransform()->position.x, m_pPlayer->GetTransform()->position.y };
	if (ImGui::SliderFloat3("Player position", float3, 0.0f, 2.0f))
	{
		std::cout << float3[0] << std::endl;
		std::cout << float3[1] << std::endl;
		std::cout << float3[2] << std::endl;
		std::cout << "---------------------------\n";
	}
	*/

	/*
	static float float2[2] = {m_pPlayer->GetTransform()->position.x, m_pPlayer->GetTransform()->position.y};
	if(ImGui::SliderFloat2("Player position", float2, 0.0f, 2.0f))
	{
		std::cout << float2[0] << std::endl;
		std::cout << float2[1] << std::endl;
		//std::cout << float2[2] << std::endl;
		std::cout << "---------------------------\n";
	}
	
	
	*/
	


	//float xy[2] = { 0.0f, 0.0f };
	//glm::vec2 xy;
	//ImGui::SliderFloat2("Plane slider", xy, -1.0f, 1.0);
	//m_pPlaneSprite->GetTransform()->position += glm::vec2(xy[0], xy[1]);





	//ImGui::End();
}
