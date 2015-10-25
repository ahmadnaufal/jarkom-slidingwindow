/* 
 * File 		: window.h
 * Author 		: Ahmad Naufal (049) - Tifani Warnita (055) - Asanilta Fahda (079)
 * Description	: Header of window for transmitter
 */ 

#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "queue.h"
#include "slidingwindow.h"

#define TIMEOUT 5

class Window {
public:
	Window(); //Ctor
	Window(const Window& w); //Cctor
	Window& operator=(const Window& w); //Operator assignment
	~Window(); //Dtor

	void slideWindow(Frame f);
	void addFrame(Frame f, int tm);
	void delFrame();
	void reduceTimeOut();
	void setTimeOut(int idx);
	void setAckTrue(int idx);

	Queue& getFrameBuffer();
	int getTimeOut(int idx);
	bool getIsAck(int idx);

private:
	Queue frameBuffer;
	int timeout[WINSIZE];
	bool isAck[WINSIZE]; 
};

#endif
 	