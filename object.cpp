#include "object.h"
#include <iostream>

object::object(int w,int h){
	this->clickable_area_cfg.height = h;//默认为0
	this->clickable_area_cfg.width = w;//默认为0 并取消宽度和高度参数的输入
	this->clickable_area_cfg.x = 0;
	this->clickable_area_cfg.y = 0;
	this->absolute_position.x = 0;
	this->absolute_position.y = 0;
	this->relative_position.x = 0;
	this->relative_position.y = 0;
	this->node_mouse_left_state = 0;
	this->node_mouse_right_state = 0;
	this->mouse_propagation = true;
	this->keyboard_propagation = true;
	this->mouse_enabled = false;
	this->keybord_enabled = false;
	this->visiable = true;
	this->type = 0;//普通节点 1为场景节点 2.为ui节点
	this->parent = nullptr;
	this->anchor_position.x = 0;
	this->anchor_position.y = 0;
};
object::~object(){
	
};

void object::set_color(Color c){
	this->color = c;
};
void object::set_position(int x, int y){
	this->relative_position.x = x;
	this->relative_position.y = y;
	if(this->parent!=nullptr){//如果父节点不为空,则调用update_position
		this->update_position();
	}	
};
Vector2 object::get_position(){
	return this->relative_position;
};

void object::add_child(object* 子节点) {
	if (子节点->parent ||!子节点) {
		throw std::runtime_error("无法调用add_child，此子节点已有父节点");
		return;
	}
	
	子节点->parent = this;
	children_list.push_back(子节点);//不需要更改写法 不会创建临时对象
	
	switch (子节点->type) {
	case 0:
		子节点->update_position();
		break;
	case 1:
		// 等于 1 的时候为场景节点，可以将子节点添加到场景节点中
		// 场景节点的添加操作可能包含其他逻辑，可以单独实现一个函数来处理
		//add_to_scene(子节点);
		break;
	case 2:
		// 等于 2 的时候为 UI 节点，可以将子节点添加到 UI 节点中
		// UI 节点的添加操作可能包含其他逻辑，可以单独实现一个函数来处理
		//add_to_ui(子节点);
		break;
	default:
		break;
	}
}


//void object::add_child(object* 子节点){
//	if(子节点 == nullptr){
//		throw std::runtime_error("无法调用add_child,此子节点为nullptr");
//		return;
//	}
//	if(子节点->parent!=nullptr){
//		throw std::runtime_error("无法调用remove_child,此子节点已有父节点");
//		return;
//	}else{
//		子节点->parent = this;
//		this->children_list.push_back(子节点);
//	}
//	if(子节点->type == 0){
//		子节点->update_position();
//	}else if(子节点->type == 1){
//		//等于1的时候为场景节点
//		return;
//	}else if(子节点->type == 2){
//		//等于2的时候为ui节点
//		return;
//	}	
//};

void object::remove_child(object* 子节点){
	if (!子节点) {
		// 参数检查可以不用抛出异常，直接返回
		return;
	}
	if (!子节点->parent) {
		// 参数检查可以不用抛出异常，直接返回
		return;
	}
	
	// 使用迭代器，避免创建临时对象
	auto it = std::find(this->children_list.begin(), this->children_list.end(), 子节点);
	if (it != this->children_list.end()) {
		this->children_list.erase(it);
		子节点->parent = nullptr;
		//需要不需要把子节点的某些属性重置?
	}
}


//void object::remove_child(object* 子节点){
//	if (子节点 == nullptr) {
//		throw std::runtime_error("无法调用remove_child,此子节点为nullptr");
//		return;
//	}
//	if (子节点->parent == nullptr) {
//		throw std::runtime_error("无法调用remove_child,此子节点没有父节点");
//	} else {
//		this->children_list.erase(
//			std::remove(this->children_list.begin(), this->children_list.end(), 子节点),
//			this->children_list.end()
//			);
//		子节点->parent = nullptr;
//		//需要不需要把子节点的某些属性重置?
//	}
//}

//void object::remove_child(object* 子节点){
//	if(子节点 == nullptr){
//		throw std::runtime_error("无法调用remove_child,此子节点为nullptr");
//		return;
//	}
//	if(子节点->parent == nullptr){
//		throw std::runtime_error("无法调用remove_child,此子节点没有父节点");
//	}else{
//		auto it = std::find(this->children_list.begin(), this->children_list.end(), 子节点);
//		if(it!=this->children_list.end()){
//			子节点->parent = nullptr;
//			this->children_list.erase(it);
//			//需要不需要把子节点的某些属性重置?
//		}else{
//			throw std::runtime_error("无法调用remove_child,78行出错");
//		}
//	}
//};

void object::add_mouse_event_listener(int event_type, mouse_event_callback callback) {
	//参数2的定义方法 mouse_event_callback my_callback = [](mouse_event event) {}
	if (event_type == 1) {//mouse_down
		auto it = std::find_if(mouse_down_callback_list.begin(), mouse_down_callback_list.end(),
			[&](const mouse_event_callback& cb) { return cb.target_type() == callback.target_type(); });
		if (it == mouse_down_callback_list.end()) {
			mouse_down_callback_list.push_back(callback);
		} else {
			throw std::runtime_error("mouse_down_callback列表中已经存在相同的回调函数");
		}
	}else if (event_type == 2) {//mouse_hover
		auto it = std::find_if(mouse_hover_callback_list.begin(), mouse_hover_callback_list.end(),
			[&](const mouse_event_callback& cb) { return cb.target_type() == callback.target_type(); });
		if (it == mouse_hover_callback_list.end()) {
			mouse_hover_callback_list.push_back(callback);
		} else {
			throw std::runtime_error("mouse_hover_callback列表中已经存在相同的回调函数");
		}
	}else if (event_type == 3) {//mouse_up
		auto it = std::find_if(mouse_up_callback_list.begin(), mouse_up_callback_list.end(),
			[&](const mouse_event_callback& cb) { return cb.target_type() == callback.target_type(); });
		if (it == mouse_up_callback_list.end()) {
			mouse_up_callback_list.push_back(callback);
		} else {
			throw std::runtime_error("mouse_up_callback列表中已经存在相同的回调函数");
		}
	}
}

void object::remove_mouse_event_listener(mouse_event_callback callback_function) {
	auto remove_from_list = [&](std::vector<mouse_event_callback>& list, auto it) {
		list.erase(it);
	};
	
	for (auto& list : {&mouse_down_callback_list, &mouse_hover_callback_list, &mouse_up_callback_list}) {
		auto it = std::find_if(list->begin(), list->end(),
			[&](const mouse_event_callback& cb) { return cb.target_type() == callback_function.target_type(); });
		if (it != list->end()) {
			remove_from_list(*list, it);
			return;
		}
	}
}

//void object::remove_mouse_event_listener(mouse_event_callback callback_function){
//	
//}
void object::add_keyboard_event_listener(){
	
}
void object::remove_keyboard_event_listener(){
	
}

void object::set_mouse_propagation(bool v){
	this->mouse_propagation = v;
}

void object::set_keyboard_propagation(bool v){
	this->keyboard_propagation = v;
}

void object::set_visiable(bool v){
	this->visiable = v;
	for (auto& child : this->children_list) {
		child->set_visiable(v);
	};
	//连同子节点一起设置visiable
}

void object::set_mouse_enabled(bool v){
	this->mouse_enabled = v;
};
void object::set_keyboard_enabled(bool v){
	this->keybord_enabled = v;
};
object* object::get_parent(){
	return this->parent;
};

void object::dispatch_mouse_event(int curr_mouse_state,mouse_event curr_mouse_event,std::vector<Rectangle> &curr_mouse_propagation_list){//这里需要传入参数记录上层节点是否穿透
	for(auto it = this->children_list.rbegin(); it != this->children_list.rend(); ++it) {
		(*it)->dispatch_mouse_event(curr_mouse_state,curr_mouse_event,curr_mouse_propagation_list);
	}
	if(this->visiable && this->mouse_enabled){
		if(CheckCollisionPointRec(curr_mouse_event.position,this->clickable_area_cfg)){
			for (auto& element : curr_mouse_propagation_list) {
				if(CheckCollisionPointRec(curr_mouse_event.position,element)){
					return;//当前节点包围盒如果和参数3中的包围盒如果有重合,则返回.
				}
			}
			if(curr_mouse_state == 1 && curr_mouse_event.key == MOUSE_LEFT_BUTTON && this->node_mouse_left_state == 0){
				this->node_mouse_left_state = 1;//遍历调用mouse_down列表里的回调函数
				for (const auto& cb : mouse_down_callback_list) {
					cb(curr_mouse_event);
				};
			}else if(curr_mouse_state == 2 && curr_mouse_event.key == MOUSE_LEFT_BUTTON && (this->node_mouse_left_state == 1||this->node_mouse_left_state == 2)){
				//std::cout<<"hover";
				this->node_mouse_left_state = 2;//遍历调用mouse_hover列表里的回调函数
				for (const auto& cb : mouse_hover_callback_list) {
					cb(curr_mouse_event);
				};
			}else if(curr_mouse_state == 0 && curr_mouse_event.key == MOUSE_LEFT_BUTTON && (this->node_mouse_left_state == 1 || this->node_mouse_left_state == 2)){
				this->node_mouse_left_state = 0;//遍历调用mouse_up列表里的回调函数
				for (const auto& cb : mouse_up_callback_list) {
					cb(curr_mouse_event);
				};
			}else if(curr_mouse_state == 1 && curr_mouse_event.key == MOUSE_RIGHT_BUTTON && this->node_mouse_right_state == 0){
				this->node_mouse_right_state = 1;
				for (const auto& cb : mouse_down_callback_list) {
					cb(curr_mouse_event);
				};
			}else if(curr_mouse_state == 2 && curr_mouse_event.key == MOUSE_RIGHT_BUTTON && (this->node_mouse_right_state == 2 || this->node_mouse_right_state == 1)){
				this->node_mouse_right_state = 2;
				for (const auto& cb : mouse_hover_callback_list) {
					cb(curr_mouse_event);
				};
			}else if(curr_mouse_state == 0 && curr_mouse_event.key == MOUSE_RIGHT_BUTTON && (this->node_mouse_right_state == 2 || this->node_mouse_right_state == 1)){
				this->node_mouse_right_state = 0;
				for (const auto& cb : mouse_up_callback_list) {
					cb(curr_mouse_event);
				};
			}
			
			if(mouse_propagation == false){//只有点击到当前节点的范围内时,才会写入到参数2
				curr_mouse_propagation_list.push_back(this->clickable_area_cfg);
			}
		}else{
			if(curr_mouse_state == 0 && curr_mouse_event.key == MOUSE_LEFT_BUTTON && (this->node_mouse_left_state == 2 || this->node_mouse_left_state == 1)){
				this->node_mouse_left_state = 0;
			}else if(curr_mouse_state == 0 && curr_mouse_event.key == MOUSE_RIGHT_BUTTON && (this->node_mouse_right_state == 2 || this->node_mouse_right_state == 1)){
				this->node_mouse_right_state = 0;
			}
		}
	}
};

void object::update_position(){
	this->absolute_position.x = this->parent->absolute_position.x;
	this->absolute_position.x = this->parent->absolute_position.x + this->parent->anchor_position.x + this->relative_position.x - this->anchor_position.x;
	this->absolute_position.y = this->parent->absolute_position.y + this->parent->anchor_position.y + this->relative_position.y - this->anchor_position.y;
	this->clickable_area_cfg.x = this->absolute_position.x;
	this->clickable_area_cfg.y = this->absolute_position.y;
	for (auto& child : this->children_list) {
		child->update_position();
	};	
}

void object::update(){
	
};

void object::draw(){
	DrawRectangle(absolute_position.x,absolute_position.y,clickable_area_cfg.width,clickable_area_cfg.height,this->color);
	for (auto& child : this->children_list) {
		child->draw();
	}
};
