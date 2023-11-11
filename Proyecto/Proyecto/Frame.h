#pragma once
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include <gtc\quaternion.hpp>
#include <fstream>

class Frame
{
public:
	Frame();
	Frame(glm::vec3 position, glm::vec3 rotations, glm::vec3 scale);
	Frame(glm::vec3 position, glm::quat rotation, glm::vec3 scale);
	~Frame();

	// Getters

	glm::vec3 getPosition() { return this->position; }
	glm::vec3 getEulerRotation();
	glm::quat getRotation() { return this->rotation; }
	glm::vec3 getScale() { return this->scale; }

	// Setters
	
	void setPosition(glm::vec3 position) { this->position = position; }
	void setRotation(glm::vec3 rotations) { this->rotation = calculateRotation(rotations); }
	void setScale(glm::vec3 scale) { this->scale = scale; }

	// Methods

	static glm::quat calculateRotation(glm::vec3 rotations);
	static glm::quat interpolateRotation(Frame a, Frame b, float t);
	static glm::vec3 interpolatePosition(Frame a, Frame b, float t);
	static glm::vec3 interpolateScale(Frame a, Frame b, float t);
	void writeFrameToFile(std::ofstream& file);

private:
	// Attributes

	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;

	// Methods 

	static glm::vec3 interpolate(glm::vec3 a, glm::vec3 b, float t);
};
