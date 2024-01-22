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
	ContinueButton = app->tex->Load("Assets/Textures/ContinueTitle-Sheet.png");
	SettingsButton = app->tex->Load("Assets/Textures/SettingsTitle-Sheet.png");
	CreditsButton = app->tex->Load("Assets/Textures/CreditsTitle-Sheet.png");
	ExitButton = app->tex->Load("Assets/Textures/ExitTitle-Sheet.png");

	SDL_Rect NewGamePos = { 120, 80, 128, 32 };
	NewGame = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "New Game", NewGamePos, this, NewGameButton);

	SDL_Rect ContinuePos = { 120, 115, 128, 32 };
	Continue = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "Continue", ContinuePos, this, ContinueButton);

	SDL_Rect SettingsPos = { 120, 150, 128, 32 };
	Settings = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, "Continue", SettingsPos, this, SettingsButton);

	SDL_Rect CreditsPos = { 120, 185, 128, 32 };
	Credits = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 4, "Continue", CreditsPos, this, CreditsButton);

	SDL_Rect ExitPos = { 120, 220, 128, 32 };
	Exit = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 5, "Continue", ExitPos, this, ExitButton);

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


	return true;
}

// Called each loop iteration
bool SceneMenu::PostUpdate()
{

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