#ifndef STAGE_H
#define STAGE_H
#include "unordered_map"
#include "raylib.h"
#include "object.h"

class stage {
public:
	//stage();
	//~stage();
	void update();
	void draw();
	void dispatch_mouse_event();//鼠标事件如果需要发送网络消息则在下面
	void dispatch_keyboard_event();
	mouse_event mouse_event_click;
	std::vector<Rectangle> mouse_propagation_list;
	object* 当前场景;
	static stage* get_instance();
private:
	static stage* instance;
	stage();
	~stage();

};

#endif
