#include "object.h"
//#include "tcp_client.cpp"

object::object(){
	this->clickable_area_cfg.height = 0;//默认为0
	this->clickable_area_cfg.width = 0;//默认为0 
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
	if(this->parent!=nullptr){//
		this->update_position();
	}	
};

Vector2 object::get_position(){
	return this->relative_position;
};

Rectangle object::get_size(){
	return this->clickable_area_cfg;
}

void object::add_child(object* 子节点) {
	if (子节点->parent ||!子节点) {
		throw std::runtime_error("无法调用add_child，此子节点已有父节点");
		return;
	}
	
	子节点->parent = this;
	children_list.push_back(子节点);//
	
	switch (子节点->type) {
	case 0:
		子节点->update_position();
		break;
	case 1:
		break;
	case 2:
		break;
	default:
		break;
	}
}

void object::remove_child(object* 子节点){
	if (!子节点) {
		return;
	}
	if (!子节点->parent) {
		return;
	}
	
	auto it = std::find(this->children_list.begin(), this->children_list.end(), 子节点);
	if (it != this->children_list.end()) {
		this->children_list.erase(it);
		子节点->parent = nullptr;
	}
}

void object::add_mouse_event_listener(int event_type, mouse_event_callback callback) {
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

void object::remove_mouse_event_listener(mouse_event_callback callback) {
	auto remove_from_list = [&](std::vector<mouse_event_callback>& list, auto it) {
		list.erase(it);
	};
	
	for (auto& list : {&mouse_down_callback_list, &mouse_hover_callback_list, &mouse_up_callback_list}) {
		auto it = std::find_if(list->begin(), list->end(),
			[&](const mouse_event_callback& cb) { return cb.target_type() == callback.target_type(); });
		if (it != list->end()) {
			remove_from_list(*list, it);
			return;
		}
	}
}

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
					return;//
				}
			}
			if(curr_mouse_state == 1 && curr_mouse_event.key == MOUSE_LEFT_BUTTON && this->node_mouse_left_state == 0){
				this->node_mouse_left_state = 1;//
				for (const auto& cb : mouse_down_callback_list) {
					cb(curr_mouse_event);
				};
			}else if(curr_mouse_state == 2 && curr_mouse_event.key == MOUSE_LEFT_BUTTON && (this->node_mouse_left_state == 1||this->node_mouse_left_state == 2)){
				//std::cout<<"hover";
				this->node_mouse_left_state = 2;//
				for (const auto& cb : mouse_hover_callback_list) {
					cb(curr_mouse_event);
				};
			}else if(curr_mouse_state == 0 && curr_mouse_event.key == MOUSE_LEFT_BUTTON && (this->node_mouse_left_state == 1 || this->node_mouse_left_state == 2)){
				this->node_mouse_left_state = 0;//
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
			
			if(mouse_propagation == false){//
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
	for (auto& child : this->children_list) {
		child->update();
	};	
};

void object::draw(){
	for (auto& child : this->children_list) {
		child->draw();
	};	
};
