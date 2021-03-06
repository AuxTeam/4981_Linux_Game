#include "Camera.h"
#include "Player.h"
#include "Window.h"
#include "Marine.h"

Camera::Camera(float w, float h) {
	this->viewport = {0, 0, 0, 0};
	this->setViewSize(w, h);
}

Camera::~Camera() {

}

// Move camera to player
void Camera::move(float x, float y) {
	this->viewport.x = ( x + MARINE_HEIGHT / 2 ) - this->viewport.w / 2; 
	this->viewport.y = ( y + MARINE_WIDTH / 2 ) - this->viewport.h / 2;
}

// Set camera width and height
void Camera::setViewSize(float w, float h) {
	this->viewport.w = w;
	this->viewport.h = h;
}

float Camera::getX() {
	return this->viewport.x;	
}

float Camera::getY() {
	return this->viewport.y;
}
