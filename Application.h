#pragma once

#include "p2List.h"
#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModulePhysics3D.h"
#include "ModulePlayer.h"


class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleAudio* audio;
	ModuleSceneIntro* scene_intro;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModulePhysics3D* physics;
	ModulePlayer* player;

	uint startTime;
private:
	float	dt;
	p2List<Module*> list_modules;
	Timer ms_timer;
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool GetDebug();
	void SetDebug();
	bool CleanUp();

private:
	bool debug = false;
	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};