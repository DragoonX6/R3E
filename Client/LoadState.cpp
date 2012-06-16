#include "Global.h"

#include "Fonts.hpp"
#include "Player.hpp"
#include "LoadState.hpp"
#include "UserInterface.hpp"
#include "CharacterList.hpp"
#include "Window.hpp"

#include "..\R3E\RoseData.hpp"
#include "..\R3E\MapManager.hpp"
#include "..\R3E\SceneManager.hpp"
#include "..\R3E\STB.hpp"
#include "..\RGE\Image.hpp"

LoadState::LoadState(int map){
	mLoadMap = map;
}

LoadState::~LoadState(){
}

bool LoadState::CanEnterState(){
	return true;
}

void LoadState::EnterState(){
}

bool LoadState::LeaveState(){
	return true;
}

int LoadState::HandleEvent(GuiEvent* /*gevt*/){
	return 0;
}

int LoadState::HandleEvent(GameEvent* /*gevt*/){
	printf("LoadState::HandleGameEvent should never be called!\n");
	return 0;
}

int LoadState::HandleEvent(NetworkEvent* /*nevt*/){
	printf("LoadState::HandleNetworkEvent should never be called!\n");
	return 0;
}

int LoadState::Run(){
	static bool sFirstLoad = true;
	Texture ldscreen;
	ROSE::STB loadstb;
	char loadstr[MAX_PATH];
	int ranval = 0;
	int rows = 0;

	strcpy_s(loadstr, "3DDATA\\STB\\LIST_LOADING.STB");

	while(1)
	{
		if(sFirstLoad)
		{
			if(!ldscreen.Load("3DDATA\\CONTROL\\RES\\r3elogo.png"))
			{
				ldscreen.Load("3DDATA\\CONTROL\\RES\\EXUI1.DDS");
			}
			break;
		}
		else
		{
			loadstb.Load(loadstr);
			rows = loadstb.Rows();
			srand(time(NULL));
			ranval = rand() % rows;

			memset(loadstr, 0, sizeof(loadstr));
			strcpy_s(loadstr, loadstb.GetString(ranval, 0));
			if(strcmp(loadstr, ""))
			{
				ldscreen.Load(loadstr);
				break;
			}
		}
	}

	if(sFirstLoad){
		gScene->Init();
	}

	gScene->BeginScene();
	gScene->RenderScene();
	gScene->EndScene();

	glEnable(GL_TEXTURE_2D);
	glColor3f(1.0f, 1.0f, 1.0f);

	gScene->Begin2D();
	IImage *screen = new IImage(ldscreen);
	UiRender *ren = new UiRender();
	int x = (gWindow->Width()/2)-(ldscreen.GetWidth()/2);
	int y = (gWindow->Height()/2)-(ldscreen.GetHeight()/2);
	ren->PushTranslate(x, y);
	ren->DrawImage(screen, 0, 0, ldscreen.GetWidth(), ldscreen.GetHeight(), ldscreen.GetWidth(), ldscreen.GetHeight());
	// we dont have to pop
	gScene->End2D();

	gScene->UpdateScene();

	gWindow->SwapBuffers();

	if(sFirstLoad)
	{
		ROSE::Data::Load();
		GameFonts::Load();
		gInterface->Init();
		sFirstLoad = false;
	}

	if(gPlayer)
		gScene->RemoveEntity(gPlayer);//No deletion of us thanks

	gScene->ClearScene();
	gCharacterList->Clear();

	Map* map = 0;
	SkyBox* skybox = 0;
	if(!MapManager::Instance().LoadMap(mLoadMap, skybox, map)) return -1;

	gScene->SetMap(map);
	gScene->SetSkyBox(skybox);
	return 0;
}
