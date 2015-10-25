/* 
 * File 		: window.cpp
 * Author 		: Ahmad Naufal (049) - Tifani Warnita (055) - Asanilta Fahda (079)
 * Description	: Window implementation
 */ 

#include "window.h"
#include <iostream>

//Ctor
Window::Window() : frameBuffer(WINSIZE) {
	
}

//Cctor
Window::Window(const Window& w) : frameBuffer(w.frameBuffer) {
	for (int i=0; i<WINSIZE; i++) {
		timeout[i] = w.timeout[i];
		isAck[i] = w.isAck[i];
	}
}

//Operator assignment
Window& Window::operator=(const Window& w) {
	frameBuffer = w.frameBuffer;
	for (int i=0; i<WINSIZE; i++) {
		timeout[i] = w.timeout[i];
		isAck[i] = w.isAck[i];
	}
	return *this;
}

//Dtor
Window::~Window() {}

//Automatic slide the window
void Window::slideWindow(Frame f) {
	frameBuffer.del();
	timeout[frameBuffer.getTail()] = TIMEOUT;
	isAck[frameBuffer.getTail()] = false; //ack has not received yet
	frameBuffer.add(f);
}

//Add new frame to window
void Window::addFrame(Frame f, int tm) {
	timeout[frameBuffer.getTail()] = tm;
	isAck[frameBuffer.getTail()] = false; //ack has not received yet
	frameBuffer.add(f);
}

//Delete front frame of window
void Window::delFrame() {

}

//Reduce all timeout by 1
void Window::reduceTimeOut() {
	for (int i=0; i<WINSIZE; i++) {
		if (timeout[i] > 0)
			i--;
	}
}

//Have already sent again because NAK, set timeout to max
void Window::setTimeOut(int idx) {
	timeout[idx] = TIMEOUT;
}

//Have received ACK, set isAck to true
void Window::setAckTrue(int idx) {
	isAck[idx] = true;
}

//Get frame buffer
Queue Window::getFrameBuffer() {
	return frameBuffer;
}

//Get timeout by index
int Window::getTimeOut(int idx) {
	return timeout[idx];
}

//Get isAck by index
bool Window::getIsAck(int idx) {
	return isAck[idx];
}