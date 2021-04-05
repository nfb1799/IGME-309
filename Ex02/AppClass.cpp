#include "AppClass.h"
using namespace Simplex;
void Application::RenderFromInsidePumpkin(matrix4 m4PumpkinToWorld)
{
	//make sure you have these vectors in the Pumpkin space, as they are, the
	//argument is not involved
	vector3 v3Position = m_v3Pumpkin;
	vector3 v3Target = v3Position + vector3(m4PumpkinToWorld * vector4(AXIS_Z, 1.0f));
	vector3 v3Above = v3Position + vector3(m4PumpkinToWorld * vector4(AXIS_Y, 1.0f));
	
#pragma region DOES NOT NEED CHANGES
	//Set the position and target of the camera
	m_pCameraMngr->SetPositionTargetAndUpward(v3Position, v3Target, v3Above - v3Position);
#pragma endregion
}
#pragma region DOES NOT NEED CHANGES
void Application::InitVariables(void)
{
	m_bCameraSwitch = false;
	m_pCameraMngr->SetFPS(false);

	//Set the position of the light
	m_pLightMngr->SetPosition(vector3(10.0f));

	///Initialize models

	//steve
	Model* pSteve = new Model();
	pSteve->Load("Minecraft\\Steve.obj");
	MyRigidBody* pSteveRB = new MyRigidBody(pSteve->GetVertexList());
	m_modelList.push_back(pSteve);
	m_RigidBodyList.push_back(pSteveRB);

	//Zombie
	Model* pZombie = new Model();
	pZombie->Load("Minecraft\\Zombie.obj");
	MyRigidBody* pZombieRB = new MyRigidBody(pZombie->GetVertexList());
	m_modelList.push_back(pZombie);
	m_RigidBodyList.push_back(pZombieRB);

	//Cow
	Model* pCow = new Model();
	pCow->Load("Minecraft\\Cow.obj");
	MyRigidBody* pCowRB = new MyRigidBody(pCow->GetVertexList());
	m_modelList.push_back(pCow);
	m_RigidBodyList.push_back(pCowRB);

	//Creeper
	Model* pPig = new Model();
	pPig->Load("Minecraft\\Creeper.obj");
	MyRigidBody* pCreeperRB = new MyRigidBody(pPig->GetVertexList());
	m_modelList.push_back(pPig);
	m_RigidBodyList.push_back(pCreeperRB);

	m_pEnderman = new Model();
	m_pEnderman->Load("Minecraft\\Enderman.obj");

	//Pumpkin
	m_pPumpkin = new Model();
	m_pPumpkin->Load("Minecraft\\Pumpkin.obj");
	m_pPumpkinRB = new MyRigidBody(m_pPumpkin->GetVertexList());

	double fDelta = 2.0 * PI / static_cast<double>(m_modelList.size());
	for (uint i = 0; i < m_modelList.size(); i++)
	{
		vector3 v3Position(glm::cos(fDelta * i) * 5.0f, glm::sin(fDelta * i) * 5.0f, 0.0f);
		matrix4 m4Position = glm::translate(v3Position);
		m_modelMatrixList.push_back(m4Position);
	}
	

	m_qArcBall = glm::angleAxis(glm::radians(45.0f), glm::normalize(vector3(1.0f, 1.0f, 1.0f)));
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();

	if (m_bCameraSwitch)
	{
		m_v3Pumpkin = ZERO_V3;
	}

	//Set model matrix to the creeper
	matrix4 m4PumpkingToWorld = glm::translate(m_v3Pumpkin) * ToMatrix4(m_qArcBall);
	m_pPumpkin->SetModelMatrix(m4PumpkingToWorld);
	m_pPumpkinRB->SetModelMatrix(m4PumpkingToWorld);
	//Render the pumpkin
	m_pPumpkin->AddToRenderList();
	if (!m_bCameraSwitch)
	{
		m_pMeshMngr->AddAxisToRenderList(m4PumpkingToWorld);
		m_pPumpkinRB->AddToRenderList();
	}

	m_pEnderman->SetModelMatrix(
		glm::translate(vector3(0.0f, 0.0f, 20.0f)) * 
		glm::scale(vector3(2.0f)) * 
		glm::rotate(glm::radians(180.0f), AXIS_Y));
	m_pEnderman->AddToRenderList();
	
	//Set model matrix to the Models
	uint uCount = m_modelList.size();
	for (uint i = 0; i < uCount; i++)
	{
		matrix4 m4Model;
		if (!m_bCameraSwitch)
			m4Model = m_modelMatrixList[i] * ToMatrix4(m_qArcBall);
		else
			m4Model = m_modelMatrixList[i];
		m_modelList[i]->SetModelMatrix(m4Model);
		m_modelList[i]->AddToRenderList();
		m_RigidBodyList[i]->SetModelMatrix(m4Model);
		m_RigidBodyList[i]->AddToRenderList();
		m_pMeshMngr->AddAxisToRenderList(m4Model);
	}
	
	for (uint i = 0; i < uCount; i++)
	{
		m_pPumpkinRB->IsColliding(m_RigidBodyList[i]);
	}

	if (!m_bCameraSwitch)
	{
		//Set the position and target of the camera
		m_pCameraMngr->SetPositionTargetAndUpward(
			vector3(0.0f, 0.0f, 13.0f), //Position
			vector3(0.0f, 0.0f, 12.0f),	//Target
			AXIS_Y);					//Up
	}
	else
	{
		RenderFromInsidePumpkin(m4PumpkingToWorld);
	}
}
void Application::Display(void)
{
	//Clear the screen
	ClearScreen();

	//Add grid to the scene
	m_pMeshMngr->AddGridToRenderList();

	//Add skybox
	m_pMeshMngr->AddSkyboxToRenderList();
	
	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();
		
	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	//release the model
	SafeDelete(m_pPumpkin);
	SafeDelete(m_pEnderman);

	//release the rigid body for the model
	SafeDelete(m_pPumpkinRB);

	uint uCount = m_modelList.size();
	for (uint i = 0; i < uCount; i++)
	{
		delete m_modelList[i];
		m_modelList[i] = nullptr;

		delete m_RigidBodyList[i];
		m_RigidBodyList[i] = nullptr;
	}
		
	//release GUI
	ShutdownGUI();
}
#pragma endregion