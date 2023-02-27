#ifndef OBJECT_H
#define OBJECT_H
#include "raylib.h"
//#include "rmath"
#include <stdexcept>
#include <vector>
#include <functional>
#include <cstring>
//问题,是否需要增加一个add_node_event_listener, 把需要执行的事件都添加进去,包括执行次数,比如,更新坐标,更新zorder是不是也可以添加进去?
//另外,需要判断执行事件和是否有父节点之间的关系. 这里的数组是否可以用传统数组来实现?
struct mouse_event {
	Vector2 position;
	MouseButton key;
};
typedef std::function<void(mouse_event)> mouse_event_callback;//给自定义函数换了个名字

class object {
public:
	object(int w,int h);
	~object();
	void set_color(Color c);
	void set_position(int x, int y);
	Vector2 get_position();
	void add_child(object* 子节点);
	void remove_child(object* 子节点);
	void add_mouse_event_listener(int event_type,mouse_event_callback callback);
	//是否要增加个开启或关闭某个事件监听的开关? toggle_mouse_event(参数1 bool,参数2 回调函数)
	void remove_mouse_event_listener(mouse_event_callback callback);
	void add_keyboard_event_listener();
	void remove_keyboard_event_listener();
	void set_visiable(bool v);
	void set_mouse_enabled(bool v);//通过设置布尔值来决定鼠标事件是开启还是停止
	void set_keyboard_enabled(bool v);
	void set_mouse_propagation(bool v);//设置鼠标穿透事件
	void set_keyboard_propagation(bool v);
	object* get_parent();// 是否需要使用智能指针? 是否需要使用转换类型?
	// find_child(); 查找子节点
	virtual void dispatch_mouse_event(int curr_mouse_state,mouse_event curr_mouse_event,std::vector<Rectangle> &curr_mouse_propagation_list);
	virtual void update();
	//virtual void update_position(int x, int y);
	virtual void update_position();
	virtual void draw();
	int type;
	const char* name;
//private:
	//Rectangle size;
	//set_visiable;
	//问题,如何判断鼠标是不是按住?
	
	Color color;
	bool visiable;
	bool position_update_required;//这个变量的意思是坐标是否需要更新 是否取消这个变量 直接把position更新写在update里?
	int node_mouse_left_state;
	int node_mouse_right_state;
	object* parent;
	bool mouse_propagation;//设置鼠标事件是否传播 set_mouse_propagation 对应鼠标穿透 和键盘穿透 需要函数来实现
	bool keyboard_propagation;
	std::vector<object*> children_list;
	Vector2 absolute_position;//绝对坐标
	Vector2 relative_position;//相对坐标
	Vector2 anchor_position;
	//Texture2D texture;//用来存储大纹理  这个应该弄成sprite的独有变量
	Rectangle clickable_area_cfg;//包围盒  node类用这个表示尺寸信息
	//Rectangle texture_clip_cfg;//用来存储从大纹理上截取小纹理的长宽信息  这个应该弄成sprite的独有变量 sprite类用它和包围盒保存尺寸信息
	bool mouse_enabled;//鼠标事件是否开启
	bool keybord_enabled;
	//std::vector<mouse_event_handler*> mouse_single_click_list;//鼠标左键
	std::vector<mouse_event_callback> mouse_down_callback_list;//鼠标左键
	std::vector<mouse_event_callback> mouse_hover_callback_list;//鼠标左键+鼠标右键
	std::vector<mouse_event_callback> mouse_up_callback_list;//鼠标左键+鼠标右键
	//std::vector<mouse_event_handler*> mouse_hover_list;//鼠标左键+鼠标右键

};

#endif
