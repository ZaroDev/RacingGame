#include "PhysVehicle3D.h"
#include "Primitive.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

// ----------------------------------------------------------------------------
VehicleInfo::~VehicleInfo()
{
	//if(wheels != NULL)
		//delete wheels;
}

// ----------------------------------------------------------------------------
PhysVehicle3D::PhysVehicle3D(btRigidBody* body, btRaycastVehicle* vehicle, const VehicleInfo& info) : PhysBody3D(body), vehicle(vehicle), info(info)
{
}

// ----------------------------------------------------------------------------
PhysVehicle3D::~PhysVehicle3D()
{
	delete vehicle;
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Render()
{
	Cylinder wheel;

	wheel.color = Black;
	wheel.axis = false;
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		wheel.radius = info.wheels[0].radius;
		wheel.height = info.wheels[0].width;

		vehicle->updateWheelTransform(i);
		vehicle->getWheelInfo(i).m_worldTransform.getOpenGLMatrix(&wheel.transform);

		wheel.Render();
	}

	Cube chassis(info.chassis_size.x, info.chassis_size.y, info.chassis_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis.transform);
	btQuaternion q = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset(info.chassis_offset.x, info.chassis_offset.y, info.chassis_offset.z);
	offset = offset.rotate(q.getAxis(), q.getAngle());

	chassis.transform.M[12] += offset.getX();
	chassis.transform.M[13] += offset.getY();
	chassis.transform.M[14] += offset.getZ();
	chassis.color.Set(1, 0, 1);
	Cube cockPit(info.cockPitSize.x, info.cockPitSize.y, info.cockPitSize.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&cockPit.transform);
	btVector3 offset2(info.cockPitOffset.x, info.cockPitOffset.y, info.cockPitOffset.z);
	offset2 = offset2.rotate(q.getAxis(), q.getAngle());

	cockPit.transform.M[12] += offset2.getX();
	cockPit.transform.M[13] += offset2.getY();
	cockPit.transform.M[14] += offset2.getZ();
	cockPit.color.Set(0, 0.75, 1);


	Cube backLight(info.backLightSize.x, info.backLightSize.y, info.backLightSize.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&backLight.transform);
	btVector3 offset3(info.backLightOffset.x, info.backLightOffset.y, info.backLightOffset.z);
	offset3 = offset3.rotate(q.getAxis(), q.getAngle());


	backLight.transform.M[12] += offset3.getX();
	backLight.transform.M[13] += offset3.getY();
	backLight.transform.M[14] += offset3.getZ();
	backLight.color = this->GetColor();

	Cube frontLight(info.frontLightSize.x, info.frontLightSize.y, info.frontLightSize.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&frontLight.transform);
	btVector3 offset6(info.frontLigthOffset.x, info.frontLigthOffset.y, info.frontLigthOffset.z);
	offset3 = offset6.rotate(q.getAxis(), q.getAngle());


	frontLight.transform.M[12] += offset3.getX();
	frontLight.transform.M[13] += offset3.getY();
	frontLight.transform.M[14] += offset3.getZ();
	frontLight.color.Set(1, 1, 0, 1.0f);

	Cube loweAntena(info.lowerAntenaSize.x, info.lowerAntenaSize.y, info.lowerAntenaSize.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&loweAntena.transform);
	btVector3 offset4(info.lowerAntenaOffset.x, info.lowerAntenaOffset.y, info.lowerAntenaOffset.z);
	offset4 = offset4.rotate(q.getAxis(), q.getAngle());

	loweAntena.transform.M[12] += offset4.getX();
	loweAntena.transform.M[13] += offset4.getY();
	loweAntena.transform.M[14] += offset4.getZ();
	loweAntena.color.Set(0.5f, 0.5f, 0.5f, 1.0f);


	Cube upperAntena(info.upperAntenaSize.x, info.upperAntenaSize.y, info.upperAntenaSize.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&upperAntena.transform);
	btVector3 offset5(info.upperAntenaOffset.x, info.upperAntenaOffset.y, info.upperAntenaOffset.z);
	offset5 = offset5.rotate(q.getAxis(), q.getAngle());

	upperAntena.transform.M[12] += offset5.getX();
	upperAntena.transform.M[13] += offset5.getY();
	upperAntena.transform.M[14] += offset5.getZ();
	upperAntena.color.Set(1, 1, 0, 1.0f);


	chassis.Render();
	cockPit.Render();
	frontLight.Render();
	backLight.Render();
	loweAntena.Render();
	upperAntena.Render();
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::ApplyEngineForce(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].drive == true)
		{
			vehicle->applyEngineForce(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Brake(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].brake == true)
		{
			vehicle->setBrake(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Turn(float degrees)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].steering == true)
		{
			vehicle->setSteeringValue(degrees, i);
		}
	}
}

// ----------------------------------------------------------------------------
float PhysVehicle3D::GetKmh() const
{
	return vehicle->getCurrentSpeedKmHour();
}