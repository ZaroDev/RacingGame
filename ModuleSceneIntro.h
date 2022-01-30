#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"

#define MAX_SNAKE 2

struct PhysBody3D;
struct PhysMotor3D;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	void Win();
	void RenderElevator();
	
	bool CleanUp();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);

	void Looping(vec3 position);
	void RampFlip(vec3 position, vec3 size);
	void Ramp(vec3 position, bool inverse, vec3 size);
	void MidRamp(vec3 position, bool inverse, vec3 size);
public:

	float angle;
	float radi;
	float floorOffset = 0;

	int winSFX;

	p2List<Cube*> looping;
	p2List<Cube*> cubes;
	p2List<PhysBody3D*> physBodies;

	p2List<Cube*> checkpoints;
	p2List<PhysBody3D*> pbCheck;
	p2List<PhysBody3D*> plCheck;
	PhysBody3D* checkPoint;

	p2List<PhysBody3D*> listeners;

	Cube reference;
	Cube ground;

	Cube* elevator;
	PhysBody3D* playerOnElevator;
	float posMoveY;


	PhysBody3D* pb_chassis;
	Cube p_chassis;

	PhysBody3D* pb_wheel;
	Cylinder p_wheel;

	PhysBody3D* pb_wheel2;
	Cylinder p_wheel2;

	PhysMotor3D* left_wheel;
	PhysMotor3D* right_wheel;


	PhysBody3D* body;

	Cube cubeSensor;
	PhysBody3D* bodySensor;

	Cylinder cylinder1;

	float cX = 1;
	float cY = 1;
	float cZ = 1;

	float aux;
	float increment1 = 0.01f;
	float increment2 = 0.02f;
	float increment3 = 0.015f;

	bool changeColor = true;
	bool moveToUp = true;

	bool win = false;
	bool won = false;
	bool cameraWin = false;
	bool timerStarted;
	Timer* playMusic;

private:
	Timer* timer;
};