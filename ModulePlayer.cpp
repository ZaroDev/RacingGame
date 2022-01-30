#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"


#define PI 3.1415

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled), vehicle(NULL)
{
	turn = acceleration = brake = 0.0f;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");
	floorOffset = App->scene_intro->floorOffset;

	// Load Fx
	dead = App->audio->LoadFx("Assets/ahh.wav");
	boing = App->audio->LoadFx("Assets/bing.wav");
	broom = App->audio->LoadFx("Assets/brom.wav");
	drift = App->audio->LoadFx("Assets/drift.wav");


	jumpTime = new Timer();
	jumpTime->Start();
	VehicleInfo car;
	color.Set(1.0f, 1.0f, 1.0f, 1.0f);
	cameraLoop.Set(-8, 63 + floorOffset, 50);
	// ----------------------------------------Vehicle chassis----//
	car.chassis_size.Set(1, 0.75, 2);
	car.chassis_offset.Set(0, 0.5f, 0.05);

	car.cockPitSize.Set(0.90, 0.35, 1);
	car.cockPitOffset.Set(0, car.chassis_offset.y + 0.35, car.chassis_offset.z - 0.1);

	car.frontLightSize.Set(1.10, 0.25, 0.25);
	car.frontLigthOffset.Set(0, car.chassis_offset.y, car.chassis_offset.z + car.chassis_size.z / 2.05f);

	car.backLightSize.Set(1.10, 0.25, 0.25);
	car.backLightOffset.Set(0, car.chassis_offset.y, car.chassis_offset.z - car.chassis_size.z / 2.05f);

	car.lowerAntenaSize.Set(0.1, 0.60, 0.1);
	car.lowerAntenaOffset.Set(-0.4, car.chassis_offset.y + 0.5, car.chassis_offset.z - 0.75);

	car.upperAntenaSize.Set(0.075, 0.3, 0.75);
	car.upperAntenaOffset.Set(-0.4, car.chassis_offset.y + 0.95, car.chassis_offset.z - 1);
	// Car properties ----------------------------------------

	car.mass = 130.0f;
	car.suspensionStiffness = 26.10f;
	car.suspensionCompression = 1.42f;
	car.suspensionDamping = 2.35f;
	car.maxSuspensionTravelCm = 510;
	car.frictionSlip = 100.5;
	car.maxSuspensionForce = 1000.0f;


	// Wheel properties ---------------------------------------
	float connection_height = car.chassis_size.y - car.chassis_offset.z + 0.75;
	float wheel_radius = 0.6f;
	float wheel_width = 0.75f;
	float suspensionRestLength = 1.25f;

	// Don't change anything below this line ------------------

	float half_width = car.chassis_size.x + 0.28f;

	float half_length = car.chassis_size.z * 0.6f;

	vec3 direction(0, -1, 0);
	vec3 axis(-1, 0, 0);

	car.num_wheels = 4;
	car.wheels = new Wheel[4];

	// FRONT-LEFT ------------------------
	car.wheels[0].connection.Set(half_width - 0.4f * wheel_width, connection_height, half_length - wheel_radius + 0.2 + car.chassis_offset.z);
	car.wheels[0].direction = direction;
	car.wheels[0].axis = axis;
	car.wheels[0].suspensionRestLength = suspensionRestLength;
	car.wheels[0].radius = wheel_radius * 0.95;
	car.wheels[0].width = wheel_width * 0.9;
	car.wheels[0].front = true;
	car.wheels[0].drive = true;
	car.wheels[0].brake = false;
	car.wheels[0].steering = true;
	// FRONT-RIGHT ------------------------
	car.wheels[1].connection.Set(-half_width + 0.4f * wheel_width, connection_height, half_length - wheel_radius + 0.2 + car.chassis_offset.z);
	car.wheels[1].direction = direction;
	car.wheels[1].axis = axis;
	car.wheels[1].suspensionRestLength = suspensionRestLength;
	car.wheels[1].radius = wheel_radius * 0.95;
	car.wheels[1].width = wheel_width * 0.9;
	car.wheels[1].front = true;
	car.wheels[1].drive = true;
	car.wheels[1].brake = false;
	car.wheels[1].steering = true;

	// REAR-LEFT ------------------------
	car.wheels[2].connection.Set(half_width - 0.3f * wheel_width, connection_height, -half_length + wheel_radius - 0.3 + car.chassis_offset.z);
	car.wheels[2].direction = direction;
	car.wheels[2].axis = axis;
	car.wheels[2].suspensionRestLength = suspensionRestLength;
	car.wheels[2].radius = wheel_radius;
	car.wheels[2].width = wheel_width;
	car.wheels[2].front = false;
	car.wheels[2].drive = false;
	car.wheels[2].brake = true;
	car.wheels[2].steering = false;

	// REAR-RIGHT ------------------------
	car.wheels[3].connection.Set(-half_width + 0.3f * wheel_width, connection_height, -half_length + wheel_radius - 0.3 + car.chassis_offset.z);
	car.wheels[3].direction = direction;
	car.wheels[3].axis = axis;
	car.wheels[3].suspensionRestLength = suspensionRestLength;
	car.wheels[3].radius = wheel_radius;
	car.wheels[3].width = wheel_width;
	car.wheels[3].front = false;
	car.wheels[3].drive = false;
	car.wheels[3].brake = true;
	car.wheels[3].steering = false;


	// Sensors
	{
		VehicleInfo sensorInf;

		sensorInf.sensor_size.Set(0.75f, 0.5f, 0.75f);
		sensorInf.sensor_offset.Set(0, 0, 0);
		sensorInf.mass = 0.001f;
		sensorInf.num_wheels = 0;


		vehicleSensor = App->physics->AddVehicle(sensorInf);
		vehicleSensor->body->setGravity({ 0,0,0 });
		vehicleSensor->color.Set(1, 0.5, 0.5);
		vehicleSensor->SetAsSensor(true);
		vehicleSensor->body->setUserPointer(vehicleSensor);
		vehicleSensor->body->setCollisionFlags(vehicleSensor->body->getCollisionFlags() | btCollisionObject::CO_GHOST_OBJECT);


		cubeSensor.SetPos(0, 10, 0);
		cubeSensor.size = { 0.25,0.25,0.25 };
		cubeSensor.color = Color(1, 0.5, 0.3);
		bodySensor = App->physics->AddBody(cubeSensor, 0);

		bodySensor->collision_listeners.add(this);
		bodySensor->SetAsSensor(true);
		bodySensor->body->setCollisionFlags(bodySensor->body->getCollisionFlags() | btCollisionObject::CO_GHOST_OBJECT);

		bodySensor->SetPos(0, 10, 0);
	}


	vehicle = App->physics->AddVehicle(car);

	vehicle->body->setFriction(0.5);
	vehicle->collision_listeners.add(this);
	vehicle->body->setUserPointer(vehicle);
	vec3 cam = App->scene_intro->checkpoints.at(0).data->GetPos();
	float* pos = App->scene_intro->checkpoints.at(0).data->transform.M;
	vehicle->SetTransform(pos);
	vehicle->SetPos(cam.x, cam.y, cam.z);

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");
	return true;
}

void ModulePlayer::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{
	positionCM = vehicle->body->getCenterOfMassPosition();
	brake = 2.5f;
	turn = acceleration = 0.0f;
	AssistDirection(90.0f);

	vehicle->vehicle->getChassisWorldTransform();

	btQuaternion q = vehicle->vehicle->getChassisWorldTransform().getRotation();

	cubeSensor.SetPos(positionCM.getX(), positionCM.getY() - 0.55, positionCM.getZ());
	vehicle->vehicle->getChassisWorldTransform().getOpenGLMatrix(&cubeSensor.transform);
	btVector3 offset(0, -0.55, 0);
	offset = offset.rotate(q.getAxis(), q.getAngle());

	cubeSensor.transform.M[12] += offset.getX();
	cubeSensor.transform.M[13] += offset.getY();
	cubeSensor.transform.M[14] += offset.getZ();
	float* pos = cubeSensor.transform.M;
	bodySensor->SetTransform(pos);
	vehicleSensor->SetTransform(pos);
	
	if (vehicle->body->getCenterOfMassPosition().getY() < floorOffset - 1)
	{
		if (!falling)App->audio->PlayFx(dead);
		falling = true;
	}
	
	if (vehicle->body->getCenterOfMassPosition().getY() < floorOffset - 40) respawn = true;

	CheckPoints();

	vehicle->SetColor(color);
	color.Set(1.0f, 1.0f, 1.0f, 1.0f);
	forwardVector = vehicle->vehicle->getForwardVector().normalize();

	perpendicularVector = { -forwardVector.getZ(), forwardVector.getY(), forwardVector.getX() };

	if (!App->GetDebug() && !App->scene_intro->win)PlayerControls();


	vehicle->ApplyEngineForce(acceleration);

	if (!App->scene_intro->cameraWin) CameraPlayer(dt);
	if (App->scene_intro->cameraWin)CameraWin(dt);
	//Render
	vehicle->Turn(turn);
	vehicle->Brake(brake);
	vehicle->Render();

	return UPDATE_CONTINUE;
}

void ModulePlayer::PlayerControls()
{

	if (jumpTime->Read() > 500)
	{
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && vehicle->state != State::IN_AIR && !jumping)
		{
			jumpTime->Start();
			jumping = true;
			vehicle->vehicle->getRigidBody()->applyCentralForce({ 0,51000,0 });
			App->audio->PlayFx(boing);
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT &&
		(vehicle->state != State::IN_AIR || vehicle->state == State::TURBO) &&
		App->input->GetKey(SDL_SCANCODE_S) != KEY_REPEAT)
	{
		velocity = MAX_ACCELERATION * 2;
		vehicle->state = TURBO;
		vehicle->vehicle->getRigidBody()->applyCentralForce({ 0,-99,0 });
	}
	else
	{
		velocity = MAX_ACCELERATION;
		if (vehicle->state != State::IN_AIR)vehicle->state = State::IDLE;
	}

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		if (vehicle->state != State::TURBO && vehicle->state != State::IN_AIR)vehicle->state = State::WALK;
		vehicle->vehicle->getRigidBody()->applyCentralForce({ 0,-70,0 });
		if (vehicle->vehicle->getCurrentSpeedKmHour() <= -2.25)
		{
			vehicle->vehicle->getRigidBody()->applyCentralForce({ 0,-50,0 });
			if (vehicle->state != State::IN_AIR)App->audio->PlayFx(drift);
			brake = BRAKE_POWER / 1.5f;
			color.Set(1.0f, 0.0f, 0.0f, 1.0f);
		}
		else
			acceleration = velocity;
		if(vehicle->state != State::IN_AIR) App->audio->PlayFx(broom);
		vehicle->body->applyTorque(perpendicularVector * -40);

	}

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		if (vehicle->state != State::TURBO && vehicle->state != State::IN_AIR)vehicle->state = State::WALK;
		if (vehicle->vehicle->getCurrentSpeedKmHour() > +2.25)
		{
			brake = BRAKE_POWER / 1.5f;
			color.Set(1.0f, 0.0f, 0.0f, 1.0f);
			vehicle->vehicle->getRigidBody()->applyCentralForce({ 0,-50,0 });
			if (vehicle->state != State::IN_AIR)App->audio->PlayFx(drift);

		}
		else
			acceleration = velocity * -1;
		if (vehicle->state != State::IN_AIR) App->audio->PlayFx(broom);
		vehicle->body->applyTorque(perpendicularVector * 80);
	}

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		if (turn < TURN_DEGREES)
			turn += (TURN_DEGREES)-assistDirection;
		brake = 15;

		if (vehicle->state == State::IN_AIR)
		{
			vehicle->body->applyTorque(forwardVector * -45);
		}
		else
		{
			vehicle->body->applyTorque(forwardVector * -200);
		}

	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		if (turn > -TURN_DEGREES)
			turn -= (TURN_DEGREES)-assistDirection;
		brake = 15;

		if (vehicle->state == State::IN_AIR)
		{
			vehicle->body->applyTorque(forwardVector * 45);
		}
		else
		{
			vehicle->body->applyTorque(forwardVector * 200);
		}
	}
}

void ModulePlayer::CheckPoints()
{
	if (respawn) Teleport(lastCheckpoint);
	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN && App->GetDebug())
	{
		vec3 cam = App->camera->Position;
		vehicle->SetPos(cam.x, cam.y - 5, cam.z);
	}
	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN) Teleport(0);
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN) Teleport(1);
	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN) Teleport(2);
	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN) Teleport(3);
	if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN) Teleport(4);
	if (App->input->GetKey(SDL_SCANCODE_5) == KEY_DOWN) Teleport(5);
	if (App->input->GetKey(SDL_SCANCODE_6) == KEY_DOWN) Teleport(6);
	if (App->input->GetKey(SDL_SCANCODE_7) == KEY_DOWN) Teleport(7);
}

void ModulePlayer::Teleport(int num)
{
	respawn = false;
	vehicle->body->setLinearVelocity({ 0,0,0 });
	vehicle->body->setAngularVelocity({ 0,0,0 });
	vec3 cam = App->scene_intro->checkpoints.at(num).data->GetPos();
	float* pos = App->scene_intro->checkpoints.at(num).data->transform.M;
	vehicle->SetTransform(pos);
	vehicle->SetPos(cam.x, cam.y, cam.z);
	falling = false;
}

void ModulePlayer::CameraPlayer(float dt)
{
	if (!App->GetDebug())
	{
		vec3 myCamera;
		vec3 myCameraLook;
		float distanceCamara2CM = -12;

		if (((cameraLoop.x - 36) < positionCM.getX() && (cameraLoop.x + 36) > positionCM.getX())
			&& (((cameraLoop.y) - 54 < positionCM.getY() && (cameraLoop.y + 54) > positionCM.getY()))
			&& (((cameraLoop.z - 54) < positionCM.getZ() && (cameraLoop.z + 54) > positionCM.getZ())))
		{
			myCamera = cameraLoop;
		}
		else
		{
			if (!falling)
			{
				myCamera.x = positionCM.getX() + forwardVector.getX() * distanceCamara2CM;
				myCamera.y = positionCM.getY() + forwardVector.getY() + 6;
				myCamera.z = positionCM.getZ() + forwardVector.getZ() * distanceCamara2CM;

				lastCamera = myCamera;
			}
			else
			{
				if (lastCamera.y < 1035) lastCamera.y += 0.50;
				myCamera = lastCamera;
			}
		}

		myCameraLook.x = vehicle->body->getCenterOfMassPosition().getX();
		myCameraLook.y = vehicle->body->getCenterOfMassPosition().getY() + 4;
		myCameraLook.z = vehicle->body->getCenterOfMassPosition().getZ();

		App->camera->Position = myCamera;
		App->camera->LookAt(myCameraLook);
	}
}

void ModulePlayer::CameraWin(float dt)
{

	vehicle->body->setLinearVelocity({ 0,0,0 });

	vec3 myCamera;
	vec3 myCameraLook = { 340, 60 + floorOffset, 360 };
	float distanceCamara2CM = -300;

	angle -= dt / 4;
	myCamera.x = distanceCamara2CM * sin(angle);
	myCamera.y = 70 + floorOffset;
	myCamera.z = distanceCamara2CM * cos(angle);

	App->camera->Position = myCamera;
	App->camera->LookAt(myCameraLook);

	if (angle < -(4 * PI) || App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		App->scene_intro->cameraWin = false;
		angle = 0;
		App->scene_intro->win = false;
		Teleport(0);
	}
}
void ModulePlayer::AssistDirection(float hardness)
{
	float turnDegrees = (TURN_DEGREES / DEGTORAD);
	calc = (vehicle->GetKmh() / 16) * (hardness / 100.0f);
	if (calc <= turnDegrees - 5)
		assistDirection = calc * DEGTORAD;
	else assistDirection = (turnDegrees - 5) * DEGTORAD;

}