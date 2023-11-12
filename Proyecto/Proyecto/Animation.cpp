#include "Animation.h"

Animation::Animation(int steps)
{
	if (steps < 1)
	{
		printf("Error: Invalid steps [%d]", steps);
		exit(1);
	}
	this->keyframesCount = 0;
	this->steps = steps;
}

Animation::Animation(std::string filename)
{
	this->keyframesCount = 0;
	this->steps = 1;
	std::ifstream file(filename);
	if (!file.is_open())
	{
		printf("Error: Could not open file [%s]", filename.c_str());
		return;
	}

	std::string line;
	std::getline(file, line);
	std::istringstream iss(line);
	if(!(iss >> this->steps))
	{
		printf("Error: Could not parse frame [%d] on file [%s]", 0, filename.c_str());
		exit(1);
	}
	int lineCount = 0;
	while(std::getline(file, line))
	{
		iss = std::istringstream(line);
		glm::vec3 p, s;
		glm::quat q;
		if (!(iss >> p.x >> p.y >> p.z >> q.x >> q.y >> q.z >> q.w >> s.x >> s.y >> s.z))
		{
			printf("Error: Could not parse frame [%d] on file [%s]", lineCount, filename.c_str());
			exit(1);
		}
		Frame frame(p, q, s);
		saveFrame(frame, lineCount);
		lineCount++;
	}

}

Animation::~Animation() {}

void Animation::saveFrame(Frame frame, int index)
{
	if (index >= FRAMES || index < 0 || index > this->keyframesCount)
	{
		printf("Error: Invalid frame index [%d]", index);
		return;
	}
	else
		this->keyframes[index] = frame;

	if (this->keyframesCount <= index)
		this->keyframesCount++;
}

void Animation::savePosition(glm::vec3 position, int index)
{
	if (validateFrame(index) == true)
		this->keyframes[index].setPosition(position);
}

void Animation::saveRotation(glm::vec3  rotations, int index)
{
	if (validateFrame(index) == true)
		this->keyframes[index].setRotation(rotations);
}

void Animation::saveScale(glm::vec3 scale, int index)
{
	if (validateFrame(index) == true)
		this->keyframes[index].setScale(scale);
}

void Animation::copyPosition(glm::vec3 position, int index)
{
	if (validateFrame(index) == true)
		this->keyframes[index].setPosition(glm::vec3(position.x, position.y, position.z));
}

void Animation::copyRotation(glm::vec3  rotations, int index)
{
	if (validateFrame(index) == true)
		this->keyframes[index].setRotation(glm::vec3(rotations.x, rotations.y, rotations.z));
}

void Animation::copyScale(glm::vec3 scale, int index)
{
	if (validateFrame(index) == true)
		this->keyframes[index].setScale(glm::vec3(scale.x, scale.y, scale.z));
}

Frame Animation::getKeyframeAt(int index)
{
	if (validateFrame(index) == true)
		return keyframes[index];

}

void Animation::saveAnimationToFile(const std::string& filename)
{
	std::ofstream file(filename);
	if (file.is_open() == false)
	{
		printf("Error: Could not open file [%s]", filename.c_str());
		return;
	}
	file << steps << std::endl;
	for (int i = 0; i < this->keyframesCount; i++)
	{
		keyframes[i].writeFrameToFile(file);
	}
	file.close();
}

/**
 * @brief Returns the interpolated model matrix for the given frame
 * @param frame: The frame to interpolate
 * @return The interpolated model matrix
 */
glm::mat4 Animation::getTransformMatrix(int frame)
{
	glm::mat4 model(1.0f);
	Frame actualFrame, nextFrame;
	float interpolationFactor;

	getSurroundingKeyframes(frame, actualFrame, nextFrame, interpolationFactor);

	model = glm::translate(model, Frame::interpolatePosition(actualFrame, nextFrame, interpolationFactor));
	model *= glm::toMat4(Frame::interpolateRotation(actualFrame, nextFrame, interpolationFactor));
	model = glm::scale(model, Frame::interpolateScale(actualFrame, nextFrame, interpolationFactor));

	return model;
}

/**
 * @brief Returns the interpolated model matrix for the given frame
 * @param rootPosition: The intial position of the object
 * @param rootRotation: The intial rotation of the object
 * @param rootScale: The intial scale of the object
 * @param frame: The frame to interpolate
 * @return The interpolated model matrix
 */
glm::mat4 Animation::getTransformMatrix(glm::vec3 rootPosition, glm::vec3 rootRotation, glm::vec3 rootScale, int frame)
{
	glm::mat4 model(1.0f);
	Frame actualFrame, nextFrame;
	float interpolationFactor;

	getSurroundingKeyframes(frame, actualFrame, nextFrame, interpolationFactor);
	model = glm::translate(model, rootPosition);
	model = glm::translate(model, Frame::interpolatePosition(actualFrame, nextFrame, interpolationFactor));
	model *= glm::toMat4(Frame::calculateRotation(rootRotation));
	model *= glm::toMat4(Frame::interpolateRotation(actualFrame, nextFrame, interpolationFactor));
	model = glm::scale(model, rootScale);
	model = glm::scale(model, Frame::interpolateScale(actualFrame, nextFrame, interpolationFactor));

	return model;
}

glm::mat4 Animation::getInitialTransformMatrix(int frame)
{
	glm::mat4 model(1.0f);

	model = glm::translate(model, keyframes[0].getPosition());
	model *= glm::toMat4(keyframes[0].getRotation());
	model = glm::scale(model, keyframes[0].getScale());

	return model;
}

glm::mat4 Animation::getInitialTransformMatrix(glm::vec3 rootPosition, glm::vec3 rootRotation, glm::vec3 rootScale, int frame)
{
	glm::mat4 model(1.0f);

	model = glm::translate(model, rootPosition);
	model = glm::translate(model, keyframes[0].getPosition());
	model *= glm::toMat4(Frame::calculateRotation(rootRotation));
	model *= glm::toMat4(keyframes[0].getRotation());
	model = glm::scale(model, rootScale);
	model = glm::scale(model, keyframes[0].getScale());

	return model;
}

bool Animation::validateFrame(int index)
{
	if (index >= this->keyframesCount || index < 0)
	{
		printf("Error: Invalid frame index [%d]", index);
		return false;
	}
	else
		return true;
}

void Animation::getSurroundingKeyframes(int frame, Frame& actualFrame, Frame& nextFrame, float& interpolationFactor)
{
	actualFrame = keyframes[0];
	nextFrame = keyframes[0];
	int frames = 0;

	for (size_t i = 1; i < keyframesCount; i++)
	{
		actualFrame = keyframes[i - 1];
		nextFrame = keyframes[i];
		frames += steps;
		if (frames > frame)
			break;
	}

	interpolationFactor = (float)(frame - (frames - steps)) / (float)steps;
}