#ifndef STAGE_H
#define STAGE_H
#include "raylib.h"
#include "object.h"

class stage {
public:
	stage();
	~stage();
	void update();
	void draw();
	void dispatch_mouse_event();
private:
	//在这里创建一个结构体 用来记录禁止穿透的鼠标信息
	mouse_event mouse_event_click;
	std::vector<Rectangle> mouse_propagation_list;
	object* 当前场景;

};

#endif
