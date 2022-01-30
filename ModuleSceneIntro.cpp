#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "PhysVehicle3D.h"
#include "Globals.h"

#define PI 3.14159265359
#define DEG_TO_RAD (PI / 180)

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;
	floorOffset = 1;

	Cube* cube;
	timer = new Timer();
	playMusic = new Timer();
	playMusic->Stop();

	//Musica
	App->audio->PlayMusic("Assets/mainMusic.ogg");
	winSFX = App->audio->LoadFx("Assets/winSFX.wav");
	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));
	App->window->SetTitle("SuperAnimeGirlRacing");
	// Platforms
	{
		// Done
		{
			//FIRST SECTION
			{
				//First Platform
				cube = new Cube(100, 1, 15);
				cube->SetPos(-90, floorOffset, -86);
				cube->color.Set(0.75, 1, 0.75);
				cubes.add(cube);										// Add in array to be rendered
				pbCheck.add(App->physics->AddBody(*cube, 0));
				plCheck.add(pbCheck.getLast()->data);

				//First Ramp
				cube = new Cube(6, 1, 15);
				cube->SetPos(-40, 1 + floorOffset, -86);
				cube->color.Set(0.75, 1, 0.75);
				cube->SetRotation(25, { 0,0,1 });
				cubes.add(cube);
				pbCheck.add(App->physics->AddBody(*cube, 0));
				// Second Platform
				cube = new Cube(50, 1, 15);
				cube->SetPos(0, floorOffset, -86);
				cube->color.Set(0.75, 1, 0.75);
				cubes.add(cube);
				pbCheck.add(App->physics->AddBody(*cube, 0));

				//Second Ramp
				cube = new Cube(6, 1, 15);
				cube->SetPos(25, 1 + floorOffset, -86.0f);
				cube->color.Set(0.75, 1, 0.75);
				cube->SetRotation(25, { 0,0,1 });
				cubes.add(cube);
				pbCheck.add(App->physics->AddBody(*cube, 0));


				// Third Platform
				cube = new Cube(20, 1, 15);
				cube->SetPos(50, floorOffset, -86);
				cube->color.Set(0.75, 1, 0.75);
				cubes.add(cube);
				pbCheck.add(App->physics->AddBody(*cube, 0));

				//Third Ramp
				cube = new Cube(20, 1, 15);
				cube->SetPos(63, 1 + floorOffset, -86.0f);
				cube->color.Set(0.75, 1, 0.75);
				cube->SetRotation(10, { 0,0,1 });
				cubes.add(cube);
				pbCheck.add(App->physics->AddBody(*cube, 0));

				//Guides
				//Right guide
				cube = new Cube(200, 3, 1);
				cube->SetPos(-40, 2 + floorOffset, -79);
				cube->color.Set(1, 0, 0);
				cubes.add(cube);
				pbCheck.add(App->physics->AddBody(*cube, 0));
				//Left guide
				cube = new Cube(200, 3, 1);
				cube->SetPos(-40, 2 + floorOffset, -93);
				cube->color.Set(1, 0, 0);
				cubes.add(cube);
				pbCheck.add(App->physics->AddBody(*cube, 0));
			}
			//Second Section
			{
				//Platform
				cube = new Cube(15, 1, 30);
				cube->SetPos(80, 2.75 + floorOffset, -78);
				cube->color.Set(1, 0.5, 0.5);
				reference = *cube;
				cubes.add(cube);
				pbCheck.add(App->physics->AddBody(*cube, 0));
				plCheck.add(pbCheck.getLast()->data);

				// First Plank Rigth
				cube = new Cube(1.3, 0.5, 18);
				cube->SetPos(78.5, (reference.GetPos().y + ((reference.size.y / 2) - (cube->size.y))), -60);
				cube->color.Set(0.6, 0.4, 0.2);
				
				reference = *cube;
				cubes.add(cube);
				pbCheck.add(App->physics->AddBody(*cube, 0));
				//Firts Plank Left
				cube = new Cube(1.3, 0.5, 18);
				cube->SetPos(80.5, reference.GetPos().y, -60);
				cube->color.Set(0.6, 0.4, 0.2);
				cubes.add(cube);
				pbCheck.add(App->physics->AddBody(*cube, 0));

				//Platform
				cube = new Cube(15, 1, 15);
				cube->SetPos(80, 2.75 + floorOffset, -50);
				cube->color.Set(1, 0.5, 0.5);
				cube->SetRotation(0, { 0, 1, 0 });
				reference = *cube;
				cubes.add(cube);
				pbCheck.add(App->physics->AddBody(*cube, 0));
				plCheck.add(pbCheck.getLast()->data);

				// First Plank Rigth
				cube = new Cube(1.3, 0.5, 18);
				cube->SetPos(78.5, (reference.GetPos().y + ((reference.size.y / 2) - (cube->size.y))), -40);
				cube->color.Set(0.6, 0.4, 0.2);

				reference = *cube;
				cubes.add(cube);
				pbCheck.add(App->physics->AddBody(*cube, 0));
				//Firts Plank Left
				cube = new Cube(1.3, 0.5, 18);
				cube->SetPos(80.5, reference.GetPos().y, -40);
				cube->color.Set(0.6, 0.4, 0.2);
				cubes.add(cube);
				pbCheck.add(App->physics->AddBody(*cube, 0));
			}
			//Third Part
			{
				cube = new Cube(15, 1, 15);
				cube->SetPos(80, 1 + floorOffset, -24);
				cube->color.Set(0.75, 1, 0.75);
				cube->SetRotation(10, { 1,0,0 });
				cubes.add(cube);
				pbCheck.add(App->physics->AddBody(*cube, 0));


				cube = new Cube(15, 1, 15);
				cube->SetPos(80,  floorOffset - 1, -10);
				cube->color.Set(0.75, 1, 0.75);

				cubes.add(cube);
				pbCheck.add(App->physics->AddBody(*cube, 0));

			}
			//Forth Part
			Looping({ 135, floorOffset - 1.5f, -12 });

			//Fifth Part
			{
				cube = new Cube(20, 1, 25);
				cube->SetPos(135, floorOffset - 1, 0);
				cube->color.Set(3.872222, 0.479737, 0.357043);
				cubes.add(cube);
				pbCheck.add(App->physics->AddBody(*cube, 0));


				cube = new Cube(20, 1, 25);
				cube->SetPos(135, floorOffset, 10);
				cube->SetRotation(-10, { 1, 0, 1 });
				cube->color.Set(3.872222, 0.479737, 0.357043);
				cubes.add(cube);
				pbCheck.add(App->physics->AddBody(*cube, 0));

				cube = new Cube(20, 1, 25);
				cube->SetPos(140, floorOffset, 27);
				cube->SetRotation(20, { 1, 0, 1 });
				cube->color.Set(3.872222, 0.479737, 0.357043);
				cubes.add(cube);
				pbCheck.add(App->physics->AddBody(*cube, 0));
			}

			//Sixt Part
			{
				cube = new Cube(20, 1, 25);
				cube->SetPos(140, floorOffset, 30);
				cube->color.Set(3.872222, 0.479737, 0.357043);
				cubes.add(cube);
				pbCheck.add(App->physics->AddBody(*cube, 0));


				//Elevator
				elevator = new Cube(10, 2, 10);
				elevator->SetPos(140, floorOffset, 35);
				elevator->color.Set(0.5f, 1.0f, 0.5f);
				playerOnElevator = App->physics->AddBody(*elevator, 0);

				cube = new Cube(30, 1, 20);
				cube->SetPos(140, 30 + floorOffset, 50);
				cube->color.Set(2, 0.5, 0.2);
				cubes.add(cube);
				pbCheck.add(App->physics->AddBody(*cube, 0));


				RampFlip({ 135, 30+ floorOffset, 57 }, { 13, 1, 3 });
				RampFlip({ 150, 30 + floorOffset, 57 }, { 13, 1, 5 });

				//Left Side
				cube = new Cube(50, 1, 20);
				cube->SetPos(180, 50 + floorOffset, 80);
				cube->color.Set(0, 0, 0.5);
				
				cubes.add(cube);
				pbCheck.add(App->physics->AddBody(*cube, 0));
				pbCheck.getLast()->data->body->setFriction(-1.0f);

				cube = new Cube(50, 1, 20);
				cube->SetPos(180, 50 + floorOffset, 80);
				cube->color.Set(0, 0, 0.5);

				cubes.add(cube);
				pbCheck.add(App->physics->AddBody(*cube, 0));
				pbCheck.getLast()->data->body->setFriction(-1.0f);


				cube = new Cube(10, 1, 10);
				cube->SetPos(180, 50 + floorOffset, 100);
				cube->color.Set(0, 0, 0.5);

				cubes.add(cube);
				pbCheck.add(App->physics->AddBody(*cube, 0));
				pbCheck.getLast()->data->body->setFriction(-1.0f);

				cube = new Cube(10, 1, 10);
				cube->SetPos(180, 50 + floorOffset, 120);
				cube->color.Set(0, 0, 0.5);

				cubes.add(cube);
				pbCheck.add(App->physics->AddBody(*cube, 0));
				pbCheck.getLast()->data->body->setFriction(-1.0f);

				cube = new Cube(10, 1, 10);
				cube->SetPos(180, 50 + floorOffset, 140);
				cube->color.Set(0, 0, 0.5);

				cubes.add(cube);
				pbCheck.add(App->physics->AddBody(*cube, 0));
				pbCheck.getLast()->data->body->setFriction(-1.0f);

				cube = new Cube(10, 1, 10);
				cube->SetPos(180, 50 + floorOffset, 160);
				cube->color.Set(0, 0, 0.5);

				cubes.add(cube);
				pbCheck.add(App->physics->AddBody(*cube, 0));
				pbCheck.getLast()->data->body->setFriction(-1.0f);

				//Rigth side
				cube = new Cube(50, 1, 20);
				cube->SetPos(100, 45 + floorOffset, 70);
				cube->color.Set(2, 0.5, 1);
				cubes.add(cube);
				pbCheck.add(App->physics->AddBody(*cube, 0));

				cube = new Cube(10, 1, 10);
				cube->SetPos(105, 45 + floorOffset, 90);
				cube->color.Set(2, 0.5, 0.2);
				cubes.add(cube);
				pbCheck.add(App->physics->AddBody(*cube, 0));

				cube = new Cube(10, 1, 10);
				cube->SetPos(95, 45 + floorOffset, 105);
				cube->color.Set(2, 0.75, 0.2);
				cubes.add(cube);
				pbCheck.add(App->physics->AddBody(*cube, 0));

				cube = new Cube(10, 1, 10);
				cube->SetPos(105, 45 + floorOffset, 125);
				cube->color.Set(2, 0.5, 0.2);
				cubes.add(cube);
				pbCheck.add(App->physics->AddBody(*cube, 0));

				cube = new Cube(10, 1, 10);
				cube->SetPos(95, 45 + floorOffset, 145);
				cube->color.Set(2, 0.75, 0.2);
				cubes.add(cube);
				pbCheck.add(App->physics->AddBody(*cube, 0));

				cube = new Cube(10, 1, 10);
				cube->SetPos(95, 45 + floorOffset, 160);
				cube->color.Set(2, 0.75, 0.2);
				cubes.add(cube);
				pbCheck.add(App->physics->AddBody(*cube, 0));
				//Middle

				cube = new Cube(100, 1, 30);
				cube->SetPos(140, 45 + floorOffset, 180);
				cube->color.Set(2, 0.75, 0.2);
				cubes.add(cube);
				pbCheck.add(App->physics->AddBody(*cube, 0));

			}
			//Seventh part	
			{
				cube = new Cube(100, 1, 30);
				cube->SetPos(140, 45 + floorOffset, 180);
				cube->color.Set(2, 0.75, 0.2);
				cubes.add(cube);
				pbCheck.add(App->physics->AddBody(*cube, 0));

				RampFlip({ 130, 45 + floorOffset, 190 }, { 5, 1, 7 });
				MidRamp({ 140, 45 + floorOffset, 190 }, false , { 5, 1, 5 });
				RampFlip({ 150, 45 + floorOffset, 190 }, { 2, 1, 5 });

				cube = new Cube(50, 1, 50);
				cube->SetPos(140, 60 + floorOffset, 250);
				cube->color.Set(2, 0.75, 0.2);
				cubes.add(cube);
				pbCheck.add(App->physics->AddBody(*cube, 0));

				cube = new Cube(10, 1, 30);
				cube->SetPos(140, 65 + floorOffset, 283);
				cube->color.Set(2, 0.75, 0.2);
				cube->SetRotation(-30, { 1, 0, 0 });
				cubes.add(cube);
				pbCheck.add(App->physics->AddBody(*cube, 0));

				cube = new Cube(10, 1, 30);
				cube->SetPos(140, 72 + floorOffset, 310);
				cube->color.Set(2, 0.75, 0.2);
				cubes.add(cube);
				pbCheck.add(App->physics->AddBody(*cube, 0));
			}
			//Eight Part
			{
				cube = new Cube(30, 1, 30);
				cube->SetPos(140, 72 + floorOffset, 340);
				cube->color.Set(0, 0.75, 0.2);
				cubes.add(cube);
				pbCheck.add(App->physics->AddBody(*cube, 0));

				cube = new Cube(20, 1, 10);
				cube->SetPos(162.5, 72 + floorOffset, 345);
				cube->color.Set(0, 0.75, 0.2);
				cube->SetRotation(45, { 0, 1, 0 });
				cubes.add(cube);
				pbCheck.add(App->physics->AddBody(*cube, 0));

				cube = new Cube(20, 1, 10);
				cube->SetPos(175, 72 + floorOffset, 335);
				cube->color.Set(0, 0.75, 0.2);
				cube->SetRotation(-45, { 0, 1, 0 });
				cubes.add(cube);
				pbCheck.add(App->physics->AddBody(*cube, 0));

				cube = new Cube(20, 1, 10);
				cube->SetPos(187, 72 + floorOffset, 345);
				cube->color.Set(0, 0.75, 0.2);
				cube->SetRotation(45, { 0, 1, 0 });
				cubes.add(cube);
				pbCheck.add(App->physics->AddBody(*cube, 0));

				cube = new Cube(30, 1, 10);
				cube->SetPos(200, 68 + floorOffset, 340);
				cube->color.Set(0, 0.75, 0.2);
				cube->SetRotation(45, { 0.5, 1, 0 });
				cubes.add(cube);
				pbCheck.add(App->physics->AddBody(*cube, 0));

				cube = new Cube(30, 1, 10);
				cube->SetPos(200, 60 + floorOffset, 350);
				cube->color.Set(0, 0.75, 0.2);
				cubes.add(cube);
				pbCheck.add(App->physics->AddBody(*cube, 0));

				cube = new Cube(20, 1, 20);
				cube->SetPos(220, 60 + floorOffset, 360);
				cube->color.Set(0, 0.75, 0.2);
				cubes.add(cube);
				pbCheck.add(App->physics->AddBody(*cube, 0));
			}
			//End Part
			{
				cube = new Cube(40, 1, 10);
				cube->SetPos(250, 60 + floorOffset, 360);
				cube->color.Set(0.5, 0.75, 0.2);
				cubes.add(cube);
				pbCheck.add(App->physics->AddBody(*cube, 0));

				cube = new Cube(40, 1, 10);
				cube->SetPos(270, 60 + floorOffset, 360);
				cube->color.Set(0.5, 0.75, 0.2);
				cube->SetRotation(25, { 1, 0, 0 });
				cubes.add(cube);
				pbCheck.add(App->physics->AddBody(*cube, 0));

				cube = new Cube(40, 1, 10);
				cube->SetPos(310, 60 + floorOffset, 360);
				cube->color.Set(0.5, 0.75, 0.2);
				cube->SetRotation(-25, { 1, 0, 0 });
				cubes.add(cube);
				pbCheck.add(App->physics->AddBody(*cube, 0));

				cube = new Cube(40, 1, 10);
				cube->SetPos(340, 60 + floorOffset, 360);
				cube->color.Set(0.5, 0.75, 0.2);
				cubes.add(cube);
				pbCheck.add(App->physics->AddBody(*cube, 0));
			}
			// Win sensor
			{
				cube = new Cube(12, 3, 12);
				cube->SetPos(340, 60 + floorOffset, 360);
				cube->color.Set(0.3, 0.3, 0.35);
				cubes.add(cube);
				pbCheck.add(App->physics->AddBody(*cube, 0));
				pbCheck.getLast()->data->body->setFriction(1000.0f);

				cubeSensor.SetPos(cube->GetPos().x, cube->GetPos().y + 1.6f, cube->GetPos().z);
				cubeSensor.size = { cube->size.x - 0.5f,0.85f,cube->size.z - 0.75f };
				cubeSensor.color.Set(1, 0.35, 0.35);
				bodySensor = App->physics->AddBody(cubeSensor, 0);
				bodySensor->SetAsSensor(true);
				bodySensor->collision_listeners.add(this);
				bodySensor->body->setUserPointer(bodySensor);
			}


		}
		{
			// Checkpoints respawns
			{
				// Checkpoint Start 
				cube = new Cube(1, 1, 1);
				cube->SetPos(-110, 10 + floorOffset, -86);
				cube->SetRotation(90, { 0,1,0 });
				checkpoints.add(cube);										
				pbCheck.add(App->physics->AddBody(*cube, 0));
				pbCheck.getLast()->data->SetAsSensor(true);
				cube->size = { 0,0,0 };								

				// Checkpoint 2 
				cube = new Cube(1, 1, 1);
				cube->SetPos(80, 17 + floorOffset, -86);
				checkpoints.add(cube);
				pbCheck.add(App->physics->AddBody(*cube, 0));
				pbCheck.getLast()->data->SetAsSensor(true);
				cube->size = { 0,0,0 };


				// Checkpoint 3
				cube = new Cube(1, 1, 1);
				cube->SetPos(80, floorOffset + 15, -12);
				checkpoints.add(cube);
				pbCheck.add(App->physics->AddBody(*cube, 0));
				pbCheck.getLast()->data->SetAsSensor(true);
				cube->size = { 0,0,0 };

				// Checkpoint 4
				cube = new Cube(1, 1, 1);
				cube->SetPos(135, floorOffset + 10, 0);
				checkpoints.add(cube);
				pbCheck.add(App->physics->AddBody(*cube, 0));
				pbCheck.getLast()->data->SetAsSensor(true);
				cube->size = { 0,0,0 };

				// Checkpoint 5
				cube = new Cube(1, 1, 1);
				cube->SetPos(140, floorOffset + 30, 50);
				checkpoints.add(cube);
				pbCheck.add(App->physics->AddBody(*cube, 0));
				pbCheck.getLast()->data->SetAsSensor(true);
				cube->size = { 0,0,0 };

				// Checkpoint 6
				cube = new Cube(1, 1, 1);
				cube->SetPos(140, 45 + floorOffset, 180);
				checkpoints.add(cube);
				pbCheck.add(App->physics->AddBody(*cube, 0));
				pbCheck.getLast()->data->SetAsSensor(true);
				cube->size = { 0,0,0 };

				// Checkpoint 7
				cube = new Cube(1, 1, 1);
				cube->SetPos(140, 72 + floorOffset, 340);
				cube->SetRotation(90, { 0, 1, 0 });
				checkpoints.add(cube);
				pbCheck.add(App->physics->AddBody(*cube, 0));
				pbCheck.getLast()->data->SetAsSensor(true);
				cube->size = { 0,0,0 };

				// Checkpoint 8
				cube = new Cube(1, 1, 1);
				cube->SetPos(220, 60 + floorOffset, 360);
				cube->SetRotation(90, { 0, 1, 0 });
				checkpoints.add(cube);
				pbCheck.add(App->physics->AddBody(*cube, 0));
				pbCheck.getLast()->data->SetAsSensor(true);
				cube->size = { 0,0,0 };
			}
		}
	}

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)App->SetDebug();



	if (win)
	{
		Win();
	}


	cubeSensor.Render();
	RenderElevator();

	//Rendering all assets
	for (p2List_item<Cube*>* cubesLooping = looping.getFirst(); cubesLooping; cubesLooping = cubesLooping->next)
	{
		cubesLooping->data->Render();
	}
	
	for (p2List_item<Cube*>* cube = cubes.getFirst(); cube; cube = cube->next)
	{
		cube->data->Render();
	}
	


	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::Win()
{
	App->audio->PlayFx(winSFX);
	cameraWin = true;
	Mix_PauseMusic();
	
	if (win)
	{
		App->window->SetTitle("YOU WON");
	}
	Mix_ResumeMusic();
}
void ModuleSceneIntro::RenderElevator()
{

	elevator->Render();
	elevator->color.Set(cX, cY, cZ);

	posMoveY = elevator->GetPos().y;
	if ((posMoveY <= 30 + floorOffset) && moveToUp)
	{
		posMoveY += 0.10;
		elevator->SetPos(elevator->GetPos().x, posMoveY, elevator->GetPos().z);
		playerOnElevator->SetPos(elevator->GetPos().x, posMoveY, elevator->GetPos().z);

	}
	else if ((posMoveY >= floorOffset) && !moveToUp)
	{
		posMoveY -= 0.16;
		elevator->SetPos(elevator->GetPos().x, posMoveY, elevator->GetPos().z);
		playerOnElevator->SetPos(elevator->GetPos().x, posMoveY, elevator->GetPos().z);
	}
	else
	{
		if (!timerStarted)
		{
			timerStarted = true;
			timer->Start();
		}
		if (timer->Read() > 5000)
		{
			timerStarted = false;
			moveToUp = !moveToUp;
		}

	}
}
void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	if ((body1 == bodySensor || body2 == bodySensor) && !win)
	{
		win = true;
		playMusic->Start();
	}
}

void ModuleSceneIntro::Looping(vec3 position)
{
	Cube* cube;
	int numCubes = 236;
	float alpha = 0;
	float radio = 2.5f;
	float rad = 0;
	float offset = 0;
	float posX = position.x;
	float posY = position.y;
	float posZ = -position.z;
	vec3 size = { 20,1.25,radio };
	vec3 axis = { size.x,size.z,size.z };

	for (int i = 0; i < numCubes; i++)
	{
		alpha += (360 * 2) / numCubes;

		rad = alpha * PI / 180;

		posZ += radio * cos(rad);
		posY += radio * sin(rad);

		cube = new Cube();
		cube->SetPos(posX + offset, posY, -posZ);
		cube->size = size;

	
		cube->color.Set(1, 0.75, 1);

		cube->SetRotation(alpha + 1.5f, { 1,0,0 });

		looping.add(cube);
		pbCheck.add(App->physics->AddBody(*cube, 0));

		offset -= (size.x * 2.7) / numCubes;
	}

}

void ModuleSceneIntro::Ramp(vec3 position, bool inverse, vec3 size)
{
	Cube* cube;
	float numCubes = 12;
	float alpha = 0;
	float auxAngle = 90.0f / numCubes;
	float offset = 0;
	float rad = 0;
	float radio = size.x;

	float posX = position.x;
	float posY = position.y;
	float posZ = -position.z;
	vec3 axis = { size.x,size.z,size.z };

	for (int i = 0; i < numCubes; i++)
	{
		rad = alpha * PI / 180;

		posX += radio * cos(rad);
		posY += radio * sin(rad);

		cube = new Cube();
		if (alpha > 42.5)
			cube->SetPos(posX, posY, -posZ - (offset * 1.15f));
		else
			cube->SetPos(posX, posY, -posZ - offset);

		cube->size = size;
		if (alpha > 42.5)
			cube->size = { size.x,size.y,size.z + (i * 5.5f) + (10 - ((posY - floorOffset) / 10)) };
		else
			cube->size = { size.x,size.y,size.z + (i * 4) + (10 - ((posY - floorOffset) / 10)) };;

		if (posZ < 0)
			cube->color.Set(40 / posX + 1.65, 40 / (posY - floorOffset + 35), 40 / posZ);
		else
			cube->color.Set(40 / posX + 1.65, 40 / (posY - floorOffset + 35), 40 / -posZ + 0.7);

		cube->SetRotation(alpha + (auxAngle / 2), { 0,0,1 });

		looping.add(cube);
		pbCheck.add(App->physics->AddBody(*cube, 0));

		alpha += auxAngle;

		if (inverse)offset -= size.z * 2 / numCubes;
		else offset += size.z * 2 / numCubes;
	}

}

void ModuleSceneIntro::MidRamp(vec3 position, bool inverse, vec3 size)
{
	Cube* cube;
	float numCubes = 12;
	float alpha = 0;
	float auxAngle = 90.0f / numCubes;
	float offset = 0;
	float rad = 0;
	float radio = size.z;

	float posX = position.x;
	float posY = position.y;
	float posZ = -position.z;
	vec3 axis = { size.x,size.z,size.z };

	for (int i = 0; i < numCubes; i++)
	{
		rad = alpha * PI / 180;

		posZ -= radio * cos(rad);
		posY += radio * sin(rad);

		cube = new Cube();
		if (alpha > 42.5)
			cube->SetPos(posX + (offset * 1.15f), posY, -posZ);
		else
			cube->SetPos(posX + offset, posY, -posZ);

		cube->size = size;
		if (alpha > 42.5)
			cube->size = { size.x + (i * 5.5f) + (10 - (posY / 10)),size.y,size.z };
		else
			cube->size = { size.x + (i * 4) + (10 - (posY / 10)),size.y,size.z };
		cube->color = White;

		cube->SetRotation(-alpha - (auxAngle / 2), { 1,0,0 });

		looping.add(cube);
		pbCheck.add(App->physics->AddBody(*cube, 0));

		alpha += auxAngle;

		if (inverse)offset -= size.x * 2 / numCubes;
		else offset += size.x * 2 / numCubes;
	}

}

void ModuleSceneIntro::RampFlip(vec3 position, vec3 size)
{
	Cube* cube;
	float numCubes = 10;
	float alpha = 0;
	float auxAngle = 90.0f / numCubes;
	float offset = 0;
	float rad = 0;
	float radio = size.z;

	float posX = position.x;
	float posY = position.y;
	float posZ = -position.z;

	for (int i = 0; i < numCubes; i++)
	{
		alpha += auxAngle;

		rad = alpha * PI / 180;

		posZ -= radio * cos(rad);
		posY += radio * sin(rad);

		cube = new Cube();
		cube->SetPos(posX + offset, posY, -posZ);
		cube->size.Set(size.x, size.y, size.z + 0.25);
		cube->color = White;

		cube->SetRotation(-alpha - (auxAngle / 2), { 1,0,0 });
		cube->color.Set(40 / posX + 1.65, 40 / (posY - floorOffset + 30), 40 / posZ);

		looping.add(cube);
		pbCheck.add(App->physics->AddBody(*cube, 0));
	}

}