/*system header*/
#include "main.h"
#include "manager.h"
#include "renderer.h"
/*tools header*/
#include "Mathf.h"
/*scenes header*/

/*gameobjects header*/
#include "Field.h"
/*self header*/
#include "SkyBox.h"

void CSkyBox::Init()
{
	CScene* scene = CManager::GetScene();

	float pi2 = 3.14159f * 0.5f;
	float pi = 3.14159f;

	float size = 500.0f;
	//front
	CField* front = scene->AddGameObject<CField>(1);
	front->SetPosition(D3DXVECTOR3(0, 0, size));
	front->SetRotation(D3DXVECTOR3(-pi2, 0, 0));
	//back
	CField* back = scene->AddGameObject<CField>(1);
	back->SetPosition(D3DXVECTOR3(0, 0, -size));
	back->SetRotation(D3DXVECTOR3(-pi2, pi, 0));
	//left
	CField* left = scene->AddGameObject<CField>(1);
	left->SetPosition(D3DXVECTOR3(-size, 0, 0));
	left->SetRotation(D3DXVECTOR3(-pi2, 0, -pi2));
	//right
	CField* right = scene->AddGameObject<CField>(1);
	right->SetPosition(D3DXVECTOR3(size, 0, 0));
	right->SetRotation(D3DXVECTOR3(-pi2, 0, pi2));
	//up
	CField* up = scene->AddGameObject<CField>(1);
	up->SetPosition(D3DXVECTOR3(0, size, 0));
	up->SetRotation(D3DXVECTOR3(pi, 0, 0));
	//down
	CField* down = scene->AddGameObject<CField>(1);

	down->SetPosition(D3DXVECTOR3(0, -size, 0));
	down->SetRotation(D3DXVECTOR3(0, 0, 0));

	//random file
	int n = Mathf::Random(1, 2);
	if (n == 1)
	{
		const WCHAR* filename = L"./asset/texture/sky01.png";
		front->SetFile(filename);
		back->SetFile(filename);
		left->SetFile(filename);
		right->SetFile(filename);
		up->SetFile(filename);
		down->SetFile(filename);
	}
	else if (n == 2)
	{
		const WCHAR* filename = L"./asset/texture/sky02.png";
		front->SetFile(filename);
		back->SetFile(filename);
		left->SetFile(filename);
		right->SetFile(filename);
		up->SetFile(filename);
		down->SetFile(filename);
	}
}

void CSkyBox::Uninit()
{
}

void CSkyBox::Update()
{
}

void CSkyBox::Draw()
{
}