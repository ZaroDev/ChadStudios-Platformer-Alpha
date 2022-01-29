
#include "App.h"
#include "Window.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "Scene.h"
#include "Death.h"
#include "Map.h"
#include "Physics.h"
#include "Intro.h"
#include "FadeToBlack.h"
#include "Pathfinding.h"
#include "UI.h"
#include "EntityManager.h"
#include "Fonts.h"
#include "Scene2.h"
#include "Defs.h"
#include "Log.h"
#include "GuiManager.h"


#include <iostream>
#include <sstream>

// Constructor
App::App(int argc, char* args[]) : argc(argc), args(args)
{
	frames = 0;

	win = new Window(true);
	input = new Input(true);
	render = new Render(true);
	tex = new Textures(true);
	audio = new Audio(true);
	fadeToBlack = new FadeToBlack(true);
	intro = new Intro(true);
	scene = new Scene(false);
	death = new Death(false);
	pathfinding = new PathFinding(false);
	map = new Map(true);
	fonts = new Fonts(true);
	ui = new UI(false);
	physics = new Physics(true);
	scene2 = new Scene2(false);
	entman = new EntityManager(true);
	guiManager = new GuiManager(true);


	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(win);
	AddModule(input);
	AddModule(tex);
	AddModule(audio);
	AddModule(fadeToBlack);
	AddModule(physics);
	AddModule(pathfinding);
	AddModule(intro);
	AddModule(death);
	AddModule(scene);
	AddModule(scene2);
	AddModule(fonts);
	AddModule(map);
	AddModule(entman);
	AddModule(ui);
	AddModule(guiManager);
	// Render last to swap buffer
	AddModule(render);

	ptimer = new PerfTimer();
	frameDuration = new PerfTimer(); 
}

// Destructor
App::~App()
{
	// Release modules
	ListItem<Module*>* item = modules.end;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->prev;
	}

	modules.Clear();
}

void App::AddModule(Module* module)
{
	module->Init();
	modules.Add(module);
}

// Called before render is available
bool App::Awake()
{
	pugi::xml_document configFile;
	pugi::xml_node config;
	pugi::xml_node configApp;

	bool ret = false;

	// L01: DONE 3: Load config from XML
	config = LoadConfig(configFile);

	if (config.empty() == false)
	{
		ret = true;
		configApp = config.child("app");

		// L01: DONE 4: Read the title from the config file
		title.Create(configApp.child("title").child_value());
		organization.Create(configApp.child("organization").child_value());
	
		// L08: DONE 1: Read from config file your framerate cap
		maxFrameRate = configApp.child("frcap").attribute("value").as_int();
	}

	if (ret == true)
	{
		ListItem<Module*>* item;
		item = modules.start;

		while ((item != NULL) && (ret == true))
		{
			// L01: DONE 5: Add a new argument to the Awake method to receive a pointer to an xml node.
			// If the section with the module name exists in config.xml, fill the pointer with the valid xml_node
			// that can be used to read all variables for that module.
			// Send nullptr if the node does not exist in config.xml
			ret = item->data->Awake(config.child(item->data->name.GetString()));

			item = item->next;
		}
	}

	return ret;
}

// Called before the first frame
bool App::Start()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;

	while(item != NULL && ret == true)
	{
		if (item->data->IsEnabled())
			ret = item->data->Start();

		item = item->next;
	}

	return ret;
}

// Called each loop iteration
bool App::Update()
{
	bool ret = true;
	PrepareUpdate();

	if(input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if(ret == true)
		ret = PreUpdate();

	if(ret == true)
		ret = DoUpdate();

	if(ret == true)
		ret = PostUpdate();

	FinishUpdate();
	return ret;
}

// Load config from XML file
// NOTE: Function has been redesigned to avoid storing additional variables on the class
pugi::xml_node App::LoadConfig(pugi::xml_document& configFile) const
{
	pugi::xml_node ret;

	pugi::xml_parse_result result = configFile.load_file(CONFIG_FILENAME);

	if (result == NULL) LOG("Could not load xml file: %s. pugi error: %s", CONFIG_FILENAME, result.description());
	else ret = configFile.child("config");

	return ret;
}



// ---------------------------------------------
void App::PrepareUpdate()
{
	OPTICK_EVENT();
	frameCount++;
	lastSecFrameCount++;

	// L08: DONE 4: Calculate the dt: differential time since last frame
	dt = frameDuration->ReadMs();
	frameDuration->Start();

	if (fpsCap)
		maxFrameRate = 28;
	else
		maxFrameRate = 16;

}

// ---------------------------------------------
void App::FinishUpdate()
{
	OPTICK_EVENT();
	// L02: DONE 1: This is a good place to call Load / Save methods
	if (loadGameRequested == true) LoadGame();
	if (saveGameRequested == true) SaveGame();



	float secondsSinceStartup = startupTime.ReadSec();

	if (lastSecFrameTime.Read() > 1000) {
		lastSecFrameTime.Start();
		framesPerSecond = lastSecFrameCount;
		lastSecFrameCount = 0;
		averageFps = (averageFps + framesPerSecond) / 2;
	}

	static char title[256];
	sprintf_s(title, 256, "FPS %i / Avg. FPS  %.2f / Last-frame %f MS / Vsync: %s", 
	framesPerSecond, averageFps, dt, app->render->vsync ? "on" : "off");

	// L08: DONE 2: Use SDL_Delay to make sure you get your capped framerate
	float delay = float(maxFrameRate) - frameDuration->ReadMs();
	//LOG("F: %f Delay:%f", frameDuration->ReadMs(), delay);

	// L08: DONE 3: Measure accurately the amount of time SDL_Delay() actually waits compared to what was expected
	if (!app->render->vsync) {
		PerfTimer* delayt = new PerfTimer();
		delayt->Start();
		if (maxFrameRate > 0 && delay > 0) SDL_Delay(delay);
		LOG("Expected %f milliseconds and the real delay is % f", delay, delayt->ReadMs());
	}
	app->win->SetTitle(title);
}

// Call modules before each loop iteration
bool App::PreUpdate()
{
	OPTICK_EVENT();
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}
		if (item->data->IsEnabled())
			ret = item->data->PreUpdate();
	}

	return ret;
}

// Call modules on each loop iteration
bool App::DoUpdate()
{
	OPTICK_EVENT();
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}
		if (item->data->IsEnabled())
			ret = item->data->Update(dt);
	}
	return ret;
}

// Call modules after each loop iteration
bool App::PostUpdate()
{
	OPTICK_EVENT();
	bool ret = true;
	ListItem<Module*>* item;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}
		if (item->data->IsEnabled())
			ret = item->data->PostUpdate();
	}

	return ret;
}

// Called before quitting
bool App::CleanUp()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.end;

	while(item != NULL && ret == true)
	{
		if (item->data->IsEnabled())
			ret = item->data->CleanUp();
		item = item->prev;
	}

	return ret;
}

// ---------------------------------------
int App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* App::GetArgv(int index) const
{
	if(index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* App::GetTitle() const
{
	return title.GetString();
}

// ---------------------------------------
const char* App::GetOrganization() const
{
	return organization.GetString();
}

// Load / Save
void App::LoadGameRequest()
{
	// NOTE: We should check if SAVE_STATE_FILENAME actually exist
	loadGameRequested = true;
}

// ---------------------------------------
void App::SaveGameRequest() const
{
	// NOTE: We should check if SAVE_STATE_FILENAME actually exist and... should we overwriten
	saveGameRequested = true;
}



// ---------------------------------------
// L02: DONE 5: Create a method to actually load an xml file
// then call all the modules to load themselves
bool App::LoadGame()
{
	bool ret = true;

	pugi::xml_parse_result result = gameStateFile.load_file("save_game.xml");

	if (result == NULL)
	{
		LOG("Could not load map xml file config.xml. pugi error: %s", result.description());
		ret = false;
	}
	

	ListItem<Module*>* item;
	item = modules.start;

	while (item != NULL && ret == true)
	{
		ret = item->data->LoadState(gameStateFile.child("game_state").child(item->data->name.GetString()));
		item = item->next;
	}

	loadGameRequested = false;


	return ret;
}

// L02: DONE 7: Implement the xml save method for current state
bool App::SaveGame() const
{
	bool ret = true;

	pugi::xml_document* saveDoc = new pugi::xml_document();
	pugi::xml_node saveStateNode = saveDoc->append_child("game_state");

	

	ListItem<Module*>* item;
	item = modules.start;

	while (item != NULL)
	{
		ret = item->data->SaveState(saveStateNode.append_child(item->data->name.GetString()));
		item = item->next;
	}
	ret = saveDoc->save_file("save_game.xml");

	saveGameRequested = false;
	return ret;
}





