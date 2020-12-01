# pragma once

# include "Vector4.h"
# include "Vector3.h"

class Light
{
public:
	Light() {} // Default constructor , we 'll be needing this later !
	Light(const Vector3& position, const Vector4& colour, float radius)
	{
		this->position = position;
		this->colour = colour;
		this->radius = radius;
	}

	~Light(void) {};

	Vector3 GetPosition() const                { return position; }
	void SetPosition(const Vector3& val)       { position = val; }

	float GetRadius() const                    { return radius; }
	void SetRadius(float val)                  { radius = val; }

	Vector4 GetColour() const                  { return colour; }
	void SetColour(const Vector4& val)         { colour = val; }


	Vector3 GetDirection() const               { return direction; }
	void SetDirection(const Vector3& dir)      { direction = dir; }

	float GetOuterCutoff() const               { return outerCutoff; }
	void  SetOuterCutoff(float val)            { outerCutoff = val; }

	float GetCutOff() const                    { return cutOff; }
	void  SetCutOff(float val)                 { cutOff = val; }

	float GetAngle() const                    { return angle; }
	void  SetAngle(float val)                 { angle = val; }


protected:
	Vector3 position;
	float radius;
	Vector4 colour;

	Vector3 direction;
	float outerCutoff;
	float cutOff;
	float angle;

};