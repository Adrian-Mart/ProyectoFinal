#include "Frame.h"

/**
 * @brief Instantiates a new frame.
 * @param position The position of the object.
 * @param rotations The rotations in x, y, and z axis of the object.
 * @param scale The scale of the object.
 * @param lenght The lenght of the frame.
 * */
Frame::Frame(glm::vec3 position, glm::vec3 rotations, glm::vec3 scale)
{
	this->position = position;
	this->rotation = calculateRotation(rotations);
	this->scale = scale;
}

/**
 * @brief Instantiates a new frame.
 * @param position The position of the object.
 * @param rotation The rotation of the object.
 * @param scale The scale of the object.
 * @param lenght The lenght of the frame.
 * */
Frame::Frame(glm::vec3 position, glm::quat rotation, glm::vec3 scale)
{
	this->position = position;
	this->rotation = rotation;
	this->scale = scale;
}

Frame::Frame()
{
	this->position = glm::vec3(0.0f, 0.0f, 0.0f);
	this->rotation = calculateRotation(glm::vec3(0.0f, 0.0f, 0.0f));
	this->scale = glm::vec3(1.0f, 1.0f, 1.0f);
}

/**
 * @brief Interpolates the rotation between two frames.
 * @param a The first frame.
 * @param b The second frame.
 * @param t The interpolation factor.
 * @return The interpolated rotation.
 */
glm::quat Frame::interpolateRotation(Frame a, Frame b, float t)
{
	return glm::slerp(a.rotation, b.rotation, t);
}

/**
 * @brief Interpolates the position between two frames.
 * @param a The first frame.
 * @param b The second frame.
 * @param t The interpolation factor.
 * @return The interpolated position.
 */
glm::vec3 Frame::interpolatePosition(Frame a, Frame b, float t)
{
	return interpolate(a.position, b.position, t);
}

/**
 * @brief Interpolates the scale between two frames.
 * @param a The first frame.
 * @param b The second frame.
 * @param t The interpolation factor.
 * @return The interpolated scale.
 */
glm::vec3 Frame::interpolateScale(Frame a, Frame b, float t)
{
	return interpolate(a.scale, b.scale, t);
}

/**
 * @brief Writes the frame to a file.
 * @param file The file to write to.
 */
void Frame::writeFrameToFile(std::ofstream& file)
{
	file << position.x << " " << position.y << " " << position.z << " ";
	file << rotation.x << " " << rotation.y << " " << rotation.z << " " << rotation.w << " ";
	file << scale.x << " " << scale.y << " " << scale.z << " " << std::endl;
}

/**
 * @brief Calculates the rotation quaternion from the rotation vector.
 * @param rotations The rotation vector.
 * @return The rotation quaternion.
 */
glm::quat Frame::calculateRotation(glm::vec3 rotations)
{
	glm::quat quatX = glm::angleAxis(rotations.x, glm::vec3(1, 0, 0));
	glm::quat quatY = glm::angleAxis(rotations.y, glm::vec3(0, 1, 0));
	glm::quat quatZ = glm::angleAxis(rotations.z, glm::vec3(0, 0, 1));
	return quatZ * quatY * quatX;
}

/**
 * @brief Interpolates between two vectors.
 * @param a The first vector.
 * @param b The second vector.
 * @param t The interpolation factor.
 * @return The interpolated vector.
 */
glm::vec3 Frame::interpolate(glm::vec3 a, glm::vec3 b, float t)
{
	return a + (b - a) * t;
}

Frame::~Frame() {}

/**
 * @brief Gets the euler rotation from the quaternion rotation.
 * @return The euler rotation in x, y and z axis.
 */
glm::vec3 Frame::getEulerRotation()
{
	return glm::eulerAngles(rotation) * 3.14159f / 180.f;
}
