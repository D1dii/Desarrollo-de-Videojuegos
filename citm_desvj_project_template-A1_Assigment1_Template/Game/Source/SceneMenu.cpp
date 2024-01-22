#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "SceneMenu.h"
#include "Map.h"
#include "GuiControl.h"
#include "GuiManager.h"
#include "ModuleFadeToBlack.h"


#include "Defs.h"
#include "Log.h"

SceneMenu::SceneMenu(bool startEnabled) : Module(startEnabled)
{
	name.Create("sceneMenu");
}

// Destructor
SceneMenu::~SceneMenu()
{}

// Called before render is available
bool SceneMenu::Awake(pugi::xml_node& config)
{
	LOG("Loading SceneMenu");
	bool ret = true;

	sceneParameter = config;


	return ret;
}

// Called before the first frame
bool SceneMenu::Start()
{

	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

	NewGameButton = app->tex->Load("Assets/Textures/NewGameTitle-Sheet.png");

	SDL_Rect btPos = { 120, 80, 128, 32 };
	gcButtom = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "MyButton", btPos, this, NewGameButton);

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	return true;
}

// Called each loop iteration
bool SceneMenu::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneMenu::Update(float dt)
{


	if (app->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN)
	{
		/*app->scene->Enable();
		app->sceneMenu->Disable();
		app->physics->Enable();
		app->map->Enable();
		app->entityManager->Enable();*/
	}

	return true;
}

// Called each loop iteration
bool SceneMenu::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool SceneMenu::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

bool SceneMenu::OnGuiMouseClickEvent(GuiControl* control)
{
	LOG("Press Gui Control: %d", control->id);

	return true;
}