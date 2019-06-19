// Transform.cpp: implementation of the Transform class.

#include <stdio.h>
#include "Transform.h"

//Please implement the following functions:

// Helper rotation function.  
mat3 Transform::rotate(const float degrees, const vec3& axis) {
	float x, y, z;
	x = axis[0];
	y = axis[1];
	z = axis[2];
	float radian = degrees * pi / 180.0;

	mat3 identity_matrix(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0);
	mat3 projection_matrix(x*x, x*y, x*z, y*x, y*y, y*z, z*x, z*y, z*z);
	mat3 dual_matrix(0.0, z, -y, -z, 0.0, x, y, -x, 0.0);

	mat3 rotate_matrix = float(cos(radian)) * identity_matrix + float(1-cos(radian)) * projection_matrix + float(sin(radian)) * dual_matrix;
	return rotate_matrix;
}

// Transforms the camera left around the "crystal ball" interface
void Transform::left(float degrees, vec3& eye, vec3& up) {
	mat3 rotation_matrix = rotate(degrees, up);
	eye = rotation_matrix * eye;
}

// Transforms the camera up around the "crystal ball" interface
void Transform::up(float degrees, vec3& eye, vec3& up) {
	vec3 axis = normalize(cross(eye, up));
	mat3 rotation_matrix = rotate(degrees, axis);
	eye = rotation_matrix * eye;
	up = normalize(rotation_matrix * up);	
}

// Your implementation of the glm::lookAt matrix
mat4 Transform::lookAt(vec3 eye, vec3 up) {
	vec3 w = normalize(eye);
	vec3 u = normalize(cross(up, w));
	vec3 v = cross(w, u);

	mat4 rotate(u.x, v.x, w.x, 0.0, u.y, v.y, w.y, 0.0, u.z, v.z, w.z, 0.0, 0.0, 0.0, 0.0, 1.0);
	mat4 translate(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, -eye.x, -eye.y, -eye.z, 1.0);	
	mat4 result = rotate * translate;
	return result;
}

Transform::Transform()
{

}

Transform::~Transform()
{

}
