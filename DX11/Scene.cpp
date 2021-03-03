/*system header*/
#include "main.h"
#include "manager.h"
#include "renderer.h"
/*tools header*/

/*scenes header*/

/*gameobjects header*/
#include "Light.h"
/*self header*/
#include "Scene.h"

void CScene::Draw()
{
	CLight* light = new CLight(D3DXVECTOR4(1.0f, -1.0f, 1.0f, 0.0f));

	for (int i = 0; i < LAYERNUM; i++)
	{
		if (i == 1)
		{
			light->SetEnable(true);
		}
		if (i == 2)
		{
			light->SetEnable(false);
		}
		for (auto object : m_GameObject[i])
		{
			object->Draw();
		}
	}

	delete light;
	light = NULL;
}