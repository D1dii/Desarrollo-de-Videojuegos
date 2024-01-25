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
#include "SDL_mixer/include/SDL_mixer.h"


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

	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

	


	return ret;
}

// Called before the first frame
bool SceneMenu::Start()
{

	NewGameButton = app->tex->Load("Assets/Textures/NewGameTitle-Sheet.png");
	ContinueButton = app->tex->Load("Assets/Textures/ContinueTitle-Sheet.png");
	SettingsButton = app->tex->Load("Assets/Textures/SettingsTitle-Sheet.png");
	CreditsButton = app->tex->Load("Assets/Textures/CreditsTitle-Sheet.png");
	ExitButton = app->tex->Load("Assets/Textures/ExitTitle-Sheet.png");
	Cartel = app->tex->Load("Assets/Textures/SettingsCartel.png");
	CreditsCartel = app->tex->Load("Assets/Textures/CreditsCartel.png");
	SliderTex = app->tex->Load("Assets/Textures/SliderSprite.png");
	ButtonSlider = app->tex->Load("Assets/Textures/ButtonSlider.png");
	Background = app->tex->Load("Assets/Textures/tittle_screen_1.png");

	SDL_Rect NewGamePos = { 120, -520, 128, 32 };
	NewGame = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "New Game", NewGamePos, this, NewGameButton);

	SDL_Rect ContinuePos = { 120, -485, 128, 32 };
	Continue = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "Continue", ContinuePos, this, ContinueButton);

	SDL_Rect SettingsPos = { 120, -450, 128, 32 };
	Settings = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, "Settings", SettingsPos, this, SettingsButton);

	SDL_Rect CreditsPos = { 120, -420, 128, 32 };
	Credits = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 4, "Credits", CreditsPos, this, CreditsButton);

	SDL_Rect ExitPos = { 120, -380, 128, 32 };
	Exit = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 5, "Exit", ExitPos, this, ExitButton);

	SDL_Rect SliderPos = { 305, -410, 100, 10 };
	Slider = (GuiSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 6, "Slider", SliderPos, this, SliderTex, ButtonSlider);

	SDL_Rect VSyncPos = { 310, -450, 25, 25 };
	VSync = (GuiCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 7, "VSync", VSyncPos, this, ExitButton);
	VSync->checked = true;
	isVSyncCheck = true;

	SDL_Rect FullScreenPos = { 375, -450, 25, 25 };
	FullScreen = (GuiCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 8, "FullScreen", FullScreenPos, this, ExitButton);
	isFullScreenCheck = false;

	app->render->camera.x = 0;
	app->render->camera.y = 1200;

	audioPath = "Assets/Audio/Music/music_spy.ogg";

	app->audio->PlayMusic(audioPath.GetString(), 0.0f);

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
	int scale = app->win->GetScale();

	if ((isSettingsActive && app->scene->player->options) && positionCartel > 245)
	{
		positionCartel -= 2;
	}
	else if ((!isSettingsActive && app->scene->player->options == false) && positionCartel < 550)
	{
		positionCartel += 2;
	}

	if ((isSettingsActive && app->scene->player->options) && positionCartel <= 245)
	{
		showOptions = true;
	}
	else {
		showOptions = false;
	}

	if (isCreditsActive && positionCredits > 245)
	{
		positionCredits -= 2;
	}
	else if (!isCreditsActive && positionCredits < 475)
	{
		positionCredits += 2;
	}

	if (isSettingsActive)
	{
		SettingsTimer++;
	}
	else
	{
		SettingsTimer2++;
	}
	
	if (isCreditsActive)
	{
		CreditsTimer++;
	}
	else
	{
		CreditsTimer2++;
	}
	

	app->render->DrawTexture(Background, 0, -600);

	app->render->DrawTexture(Cartel, positionCartel, -550);
	app->render->DrawTexture(CreditsCartel, positionCredits, -480);

	
	
	
	

	return true;
}

// Called each loop iteration
bool SceneMenu::PostUpdate()
{

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	//volume sounds
	Mix_VolumeMusic(app->scene->volume);
	Mix_Volume(-1, app->scene->volume);

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