#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "SceneMenu.h"
#include "ScreenEnd.h"
#include "Map.h"
#include "GuiControl.h"
#include "GuiManager.h"
#include "ModuleFadeToBlack.h"
#include "SDL_mixer/include/SDL_mixer.h"


#include "Defs.h"
#include "Log.h"

ScreenEnd::ScreenEnd(bool startEnabled) : Module(startEnabled)
{
	name.Create("sceneEnd");
}

// Destructor
ScreenEnd::~ScreenEnd()
{}

// Called before render is available
bool ScreenEnd::Awake(pugi::xml_node& config)
{
	LOG("Loading SceneEnd");
	bool ret = true;

	sceneParameter = config;

	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);




	return ret;
}

// Called before the first frame
bool ScreenEnd::Start()
{

	img = app->tex->Load("Assets/Textures/End Screen.png");

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	audioPath = "Assets/Audio/Music/music_spy.ogg";

	app->audio->PlayMusic(audioPath.GetString(), 0.0f);

	return true;
}

// Called each loop iteration
bool ScreenEnd::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool ScreenEnd::Update(float dt)
{
	int scale = app->win->GetScale();

	


	app->render->DrawTexture(img, 0, 0);

	return true;
}

// Called each loop iteration
bool ScreenEnd::PostUpdate()
{

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	//volume sounds
	Mix_VolumeMusic(app->scene->volume);
	Mix_Volume(-1, app->scene->volume);

	return ret;
}

// Called before quitting
bool ScreenEnd::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

bool ScreenEnd::OnGuiMouseClickEvent(GuiControl* control)
{
	LOG("Press Gui Control: %d", control->id);

	return true;
}