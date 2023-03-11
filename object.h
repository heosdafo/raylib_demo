#ifndef OBJECT_H
#define OBJECT_H
#include "raylib.h"
#include <stdexcept>
#include <vector>
#include <functional>
#include <cstring>
#include <iostream>

struct mouse_event {
	Vector2 position;
	MouseButton key;
};
typedef std::function<void(mouse_event)> mouse_event_callback;//给自定义函数换了个名字

class object {
public:
	object();
	~object();
	void set_color(Color c);
	void set_position(int x, int y);
	Vector2 get_position();
	Rectangle get_size();
	void add_child(object* 子节点);
	void remove_child(object* 子节点);
	void add_mouse_event_listener(int event_type,mouse_event_callback callback);
	void remove_mouse_event_listener(mouse_event_callback callback);
	void add_keyboard_event_listener();
	void remove_keyboard_event_listener();
	void set_visiable(bool v);
	void set_mouse_enabled(bool v);//
	void set_keyboard_enabled(bool v);
	void set_mouse_propagation(bool v);//
	void set_keyboard_propagation(bool v);
	object* get_parent();

	virtual void dispatch_mouse_event(int curr_mouse_state,mouse_event curr_mouse_event,std::vector<Rectangle> &curr_mouse_propagation_list);
	virtual void update();
	virtual void update_position();
	virtual void draw();
	int type;
	Color color;
	bool visiable;
	bool position_update_required;//
	int node_mouse_left_state;
	int node_mouse_right_state;
	object* parent;
	bool mouse_propagation;//
	bool keyboard_propagation;
	std::vector<object*> children_list;
	Vector2 absolute_position;//绝对坐标
	Vector2 relative_position;//相对坐标
	Vector2 anchor_position;
	Rectangle clickable_area_cfg;//
	bool mouse_enabled;//
	bool keybord_enabled;
	std::vector<mouse_event_callback> mouse_down_callback_list;//鼠标左键
	std::vector<mouse_event_callback> mouse_hover_callback_list;//鼠标左键+鼠标右键
	std::vector<mouse_event_callback> mouse_up_callback_list;//鼠标左键+鼠标右键


};

#endif
