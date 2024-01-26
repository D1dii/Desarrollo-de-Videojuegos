#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "SceneMenu.h"
#include "ScreenEnd.h"
#include "ScreenInitial.h"
#include "Map.h"
#include "GuiControl.h"
#include "GuiManager.h"
#include "ModuleFadeToBlack.h"
#include "SDL_mixer/include/SDL_mixer.h"


#include "Defs.h"
#include "Log.h"

ScreenInitial::ScreenInitial(bool startEnabled) : Module(startEnabled)
{
	name.Create("sceneInitial");
}

// Destructor
ScreenInitial::~ScreenInitial()
{}

// Called before render is available
bool ScreenInitial::Awake(pugi::xml_node& config)
{
	LOG("Loading SceneInitial");
	bool ret = true;

	sceneParameter = config;

	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);




	return ret;
}

// Called before the first frame
bool ScreenInitial::Start()
{

	img = app->tex->Load("Assets/Textures/Start Screen.png");

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	audioPath = "Assets/Audio/Music/Let_Him_Cook.mp3";

	app->audio->PlayMusic(audioPath.GetString(), 0.0f);

	return true;
}

// Called each loop iteration
bool ScreenInitial::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool ScreenInitial::Update(float dt)
{
	int scale = app->win->GetScale();


	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		app->initial->Disable();
		app->sceneMenu->Enable();
		showImage = false;
	}

	if (showImage) app->render->DrawTexture(img, 0, 0);

	return true;
}

// Called each loop iteration
bool ScreenInitial::PostUpdate()
{

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	//volume sounds
	Mix_VolumeMusic(app->scene->volume);
	Mix_Volume(-1, app->scene->volume);

	return ret;
}

// Called before quitting
bool ScreenInitial::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

bool ScreenInitial::OnGuiMouseClickEvent(GuiControl* control)
{
	LOG("Press Gui Control: %d", control->id);

	return true;
}