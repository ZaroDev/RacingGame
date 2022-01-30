#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "Color.h"


struct PhysVehicle3D;

#define MAX_ACCELERATION 700.0f
#define TURN_DEGREES 17.0f * DEGTORAD
#define BRAKE_POWER 50.0f



class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update(float dt);
	void PlayerControls();
	void CheckPoints();
	void Teleport(int num);
	void CameraPlayer(float dt);
	void AssistDirection(float hardness);
	bool CleanUp();
	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);
	void CameraWin(float dt);

public:

	PhysVehicle3D* vehicle;
	PhysVehicle3D* vehicleSensor;

	float turn;
	float acceleration;
	float brake;
	bool jumping = false;

	Cube cubeSensor;
	PhysBody3D* bodySensor;
	int lastCheckpoint = 0;

private:

	Timer* jumpTime;

	btVector3 forwardVector;
	btVector3 perpendicularVector;
	
	int velocity;
	float assistDirection;
	float calc;
	float floorOffset;
	
	vec3 cameraLoop;
	vec3 lastCamera;
	btVector3 positionCM;
	
	//Gameplay conditioners

	bool CPactive = false;
	bool respawn = false;
	bool falling = false;
	//Apearence

	Color color;

	//SFX
	uint dead;
	uint boing;
	uint broom;
	uint drift;
	



	float angle = -0.25;
};