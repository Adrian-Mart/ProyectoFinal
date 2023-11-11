#pragma once
#include "Frame.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <gtc\quaternion.hpp>
#include <gtx\quaternion.hpp>
#include <fstream>
#include <string>
#include <sstream>

class Animation
{
public:
	Animation(int steps);
	Animation(std::string filename);
	~Animation();

	// Methods
	void saveFrame(Frame frame, int index);
	void savePosition(glm::vec3 position, int index);
	void saveRotation(glm::vec3 rotations, int index);
	void saveScale(glm::vec3 scale, int index);
	void copyPosition(glm::vec3 position, int index);
	void copyRotation(glm::vec3 rotations, int index);
	void copyScale(glm::vec3 scale, int index);
	Frame getKeyframeAt(int keyframeIndex);
	void saveAnimationToFile(const std::string& filename);
	glm::mat4 getTransformMatrix(int frame);
	glm::mat4 getTransformMatrix(glm::vec3 rootPosition, glm::vec3 rootRotation, glm::vec3 rootScale, int frame);
	glm::mat4 getInitialTransformMatrix(int frame);
	glm::mat4 getInitialTransformMatrix(glm::vec3 rootPosition, glm::vec3 rootRotation, glm::vec3 rootScale, int frame);

private:
	static const int FRAMES = 100;
	int keyframesCount;
	int steps;
	Frame keyframes[FRAMES];

	// Methods
	bool validateFrame(int index);
	void getSurroundingKeyframes(int frame, Frame &actualFrame, Frame &nextFrame, float& interpolationFactor);
};
