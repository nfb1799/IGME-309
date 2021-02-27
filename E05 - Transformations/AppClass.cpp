#include "AppClass.h"
void Application::InitVariables(void)
{
	//init the mesh
	//m_pMesh = new MyMesh();
	//m_pMesh->GenerateCube(1.0f, C_WHITE);
	//m_pMesh->GenerateSphere(1.0f, 5, C_WHITE);

	for (int i = 0; i < 46; i++)
	{
		m_pMesh = new MyMesh();
		m_pMesh->GenerateCube(1.0f, C_BLACK);
		m_pMeshes.push_back(m_pMesh);
	}
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	matrix4 m4View = m_pCameraMngr->GetViewMatrix();
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();

	static float x = 0;
	vector3 v3Position(x, 0, -5.0f);
	x += .01f;
	matrix4 m4Position = glm::translate(vector3(-10.0f, 1.0f, 1.0f)) * glm::translate(v3Position);
	//m_pMesh->Render(m4Projection, m4View, m4Model);
	
	//1 row
	m_pMeshes[0]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(-3.0f, 4.0f, 0.0f)));
	m_pMeshes[1]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(3.0f, 4.0f, 0.0f)));

	//2 row
	m_pMeshes[2]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(-2.0f, 3.0f, 0.0f)));
	m_pMeshes[3]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(2.0f, 3.0f, 0.0f)));

	//3 row, index 4 - 10
	for (int i = 0; i < 7; i++)
	{
		m_pMeshes[i + 4]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(-3.0f + (float)i, 2.0f, 0.0f)));
	}

	//4 row
	m_pMeshes[11]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(-4.0f, 1.0f, 0.0f)));
	m_pMeshes[12]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(-3.0f, 1.0f, 0.0f)));
	//-------------------------------------------------------------------------------------------------
	m_pMeshes[13]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(-1.0f, 1.0f, 0.0f)));
	m_pMeshes[14]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(0.0f, 1.0f, 0.0f)));
	m_pMeshes[15]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(1.0f, 1.0f, 0.0f)));
	//-------------------------------------------------------------------------------------------------
	m_pMeshes[16]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(3.0f, 1.0f, 0.0f)));
	m_pMeshes[17]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(4.0f, 1.0f, 0.0f)));

	//5 row, index 18 - 28
	for (int i = 0; i < 11; i++)
	{
		m_pMeshes[18 + i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(-5.0f + (float)i, 0.0f, 0.0f)));
	}

	//6 row
	m_pMeshes[29]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(-5.0f, -1.0f, 0.0f)));
	//--------------------------------------------------------------------------------------------------
	for (int i = 0; i < 7; i++) // index 30 - 36
	{
		m_pMeshes[30 + i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(-3.0f + (float)i, -1.0f, 0.0f)));
	}
	//--------------------------------------------------------------------------------------------------
	m_pMeshes[37]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(5.0f, -1.0f, 0.0f)));

	//7 row
	m_pMeshes[38]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(-5.0f, -2.0f, 0.0f)));
	m_pMeshes[39]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(-3.0f, -2.0f, 0.0f)));
	m_pMeshes[40]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(3.0f, -2.0f, 0.0f)));
	m_pMeshes[41]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(5.0f, -2.0f, 0.0f)));

	//final row
	m_pMeshes[42]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(-2.0f, -3.0f, 0.0f)));
	m_pMeshes[43]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(-1.0f, -3.0f, 0.0f)));
	m_pMeshes[44]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(1.0f, -3.0f, 0.0f)));
	m_pMeshes[45]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(2.0f, -3.0f, 0.0f)));

	
	// draw a skybox
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
	SafeDelete(m_pMesh);

	//release GUI
	ShutdownGUI();
}