#include "stage.h"
#include <iostream>
#include <chrono>
stage* stage::instance;

stage::stage(){
	object* node1 = new object();
	this->当前场景 = node1;
}

stage::~stage(){
	
}

stage* stage::get_instance(){
	if(instance == nullptr){
		instance = new stage();
	}
	return instance;
}

void stage::update(){

}

void stage::draw(){
	this->当前场景->draw();
}

void stage::dispatch_mouse_event(){
	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) == true)//按下
	{
		Vector2 curr_mouse_position = GetMousePosition();
		mouse_event_click.position = curr_mouse_position;
		mouse_event_click.key = MOUSE_LEFT_BUTTON;
		this->当前场景->dispatch_mouse_event(1,this->mouse_event_click,this->mouse_propagation_list);
		this->mouse_event_click.position = Vector2{ 0.0f, 0.0f };
		this->mouse_propagation_list.clear();//重置
	}
	else if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) == true)//按住
	{
		Vector2 curr_mouse_position = GetMousePosition();
		mouse_event_click.position = curr_mouse_position;
		mouse_event_click.key = MOUSE_LEFT_BUTTON;
		
		this->当前场景->dispatch_mouse_event(2,this->mouse_event_click,this->mouse_propagation_list);
		this->mouse_event_click.position = Vector2{ 0.0f, 0.0f };
		this->mouse_propagation_list.clear();//重置
	}
	else if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) == true)//抬起释放
	{
		Vector2 curr_mouse_position = GetMousePosition();
		mouse_event_click.position = curr_mouse_position;
		mouse_event_click.key = MOUSE_LEFT_BUTTON;
		this->当前场景->dispatch_mouse_event(0,this->mouse_event_click,this->mouse_propagation_list);
		this->mouse_event_click.position = Vector2{ 0.0f, 0.0f };
		this->mouse_propagation_list.clear();//重置
	}
	
}


