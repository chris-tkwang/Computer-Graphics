// Transform.cpp: implementation of the Transform class.

// Note: when you construct a matrix using mat4() or mat3(), it will be COLUMN-MAJOR
// Keep this in mind in readfile.cpp and display.cpp
// See FAQ for more details or if you're having problems.

#include "Transform.h"

// Helper rotation function.  Please implement this.  
mat3 Transform::rotate(const float degrees, const vec3& axis) 
{
	vec3 normalized_axis = normalize(axis);
	float x, y, z;
	x = normalized_axis[0];
	y = normalized_axis[1];
	z = normalized_axis[2];
	float radian = degrees * pi / 180.0;

	mat3 identity_matrix(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0);
	mat3 projection_matrix(x*x, x*y, x*z, y*x, y*y, y*z, z*x, z*y, z*z);
	mat3 dual_matrix(0.0, z, -y, -z, 0.0, x, y, -x, 0.0);

	mat3 rotate_matrix = float(cos(radian)) * identity_matrix + float(1-cos(radian)) * projection_matrix + float(sin(radian)) * dual_matrix;
	return rotate_matrix;
}

void Transform::left(float degrees, vec3& eye, vec3& up) 
{
	mat3 rotation_matrix = rotate(degrees, up);
	eye = rotation_matrix * eye;
}

void Transform::up(float degrees, vec3& eye, vec3& up) 
{
	vec3 axis = normalize(cross(eye, up));
	mat3 rotation_matrix = rotate(degrees, axis);
	eye = rotation_matrix * eye;
	up = normalize(rotation_matrix * up);	  
}

mat4 Transform::lookAt(const vec3 &eye, const vec3 &center, const vec3 &up) 
{
	vec3 w = normalize(eye);
	vec3 u = normalize(cross(up, w));
	vec3 v = cross(w, u);

	mat4 rotate(u.x, v.x, w.x, 0.0, u.y, v.y, w.y, 0.0, u.z, v.z, w.z, 0.0, 0.0, 0.0, 0.0, 1.0);
	mat4 translate(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, -eye.x, -eye.y, -eye.z, 1.0);	
	mat4 result = rotate * translate;
	return result;
}

mat4 Transform::perspective(float fovy, float aspect, float zNear, float zFar)
{
	float radian = fovy * pi / 180.0;
	float d = 1 / tan(radian / 2);
	float A = (zNear + zFar) / (zNear - zFar);
	float B = 2 * zNear * zFar / (zNear - zFar);
	
	mat4 ret(d/aspect, 0.0, 0.0, 0.0, 0.0, d, 0.0, 0.0, 0.0, 0.0, A, -1.0, 0.0, 0.0, B, 0.0);
	return ret;
}

mat4 Transform::scale(const float &sx, const float &sy, const float &sz) 
{
	mat4 ret(sx, 0.0, 0.0, 0.0, 0.0, sy, 0.0, 0.0, 0.0, 0.0, sz, 0.0, 0.0, 0.0, 0.0, 1.0);
	return ret;
}

mat4 Transform::translate(const float &tx, const float &ty, const float &tz) 
{
	mat4 ret(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, tx, ty, tz, 1.0);
	return ret;
}

// To normalize the up direction and construct a coordinate frame.  
// As discussed in the lecture.  May be relevant to create a properly 
// orthogonal and normalized up. 
// This function is provided as a helper, in case you want to use it. 
// Using this function (in readfile.cpp or display.cpp) is optional.  

vec3 Transform::upvector(const vec3 &up, const vec3 & zvec) 
{
  vec3 x = glm::cross(up,zvec); 
  vec3 y = glm::cross(zvec,x); 
  vec3 ret = glm::normalize(y); 
  return ret; 
}


Transform::Transform()
{

}

Transform::~Transform()
{

}
