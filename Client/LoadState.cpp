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
				ldscreen.Load("3DDATA\\CONTROL\\RES\\EXUI1.DDS");
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

	glMatrixMode (GL_MODELVIEW);
	glPushMatrix ();
	glLoadIdentity ();
	glMatrixMode (GL_PROJECTION);
	glPushMatrix ();
	glLoadIdentity ();

	glBegin (GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glEnd ();

	glPopMatrix ();
	glMatrixMode (GL_MODELVIEW);
	glPopMatrix ();
	
	gWindow->SwapBuffers();

	if(sFirstLoad){
		gScene->Init();

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
