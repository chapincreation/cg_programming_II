#include "Player.h"
#include "Object.h"

Player::Player(GLuint width, GLuint height) : Object(){
	BuildTriangles(this->width = width, this->height = height);
	//BuildTriangles(1, 1);
	//SetScale(vec3(0.025f, 0.25f, 1.0f));
	//SetPosition(vec3(0.0f, 0.0f, 0.0f));
	//SetPosition(vec3(0));
	upKey = GLFW_KEY_UP;
	downKey = GLFW_KEY_DOWN;
	leftKey = GLFW_KEY_LEFT;
	rightKey = GLFW_KEY_RIGHT;
}

Player::~Player(){

}

void Player::Update(const float& deltaTime){
	//Object::Update(deltaTime);
	if(glfwGetKey(window, upKey) == GLFW_PRESS)
		this->SetPosition(this->GetPosition() + vec3(0.0f, 10.0f * deltaTime, 0.0f));
	if(glfwGetKey(window, downKey) == GLFW_PRESS)
		this->SetPosition(this->GetPosition() + vec3(0.0f, -10.0f * deltaTime, 0.0f));
	if(glfwGetKey(window, rightKey) == GLFW_PRESS)
		this->SetPosition(this->GetPosition() + vec3(10.0f * deltaTime, 0.0f, 0.0f));
	else if(glfwGetKey(window, leftKey) == GLFW_PRESS)
		this->SetRotation(this->GetRotation() + vec3(10.0f * deltaTime, 0.0f, 0.0f));
		

	
}

void Player::SetKeys(const int& upKey, const int&downKey, const int& leftKey, const int&rightKey){
	this->upKey = upKey;
	this->downKey = downKey;
	this->leftKey = leftKey;
	this->rightKey = rightKey;
}