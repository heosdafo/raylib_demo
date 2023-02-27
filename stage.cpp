#include "stage.h"
#include <iostream>
#include <chrono>

stage::stage(){
	this->mouse_event_click.position =  Vector2{ 0.0f, 0.0f };
	auto start = std::chrono::high_resolution_clock::now();
	object* abc[100005];
	for(int i=0;i<=100000;i++){
		abc[i] = new object(10,10);
	};
	auto end = std::chrono::high_resolution_clock::now();
	
	// 计算时间差
	std::chrono::duration<double> elapsed_time = end - start;
	
	// 输出计时结果
	std::cout << "Elapsed time: " << elapsed_time.count() << " seconds" << std::endl;
	std::cout<<"asdasdasdasd";
//	object* node1 = new object(100,100);
//	node1->set_color(GREEN);
//	node1->type = 1;
//	node1->name = "node1";
//	node1->set_position(100,100);
//	
//	object* node2 = new object(150,150);
//	node2->set_color(BLUE);
//	node2->set_position(50,50);
//	node1->add_child(node2);
//	std::cout<<node2->type;
//	mouse_event_callback my_callback = [](mouse_event event) {
//		if(event.key == MOUSE_LEFT_BUTTON){
//			std::cout<<"node1--------\n";
//		}
//	};
//	mouse_event_callback my_callback1 = [](mouse_event event) {
//		if(event.key == MOUSE_LEFT_BUTTON){
//			std::cout<<"node2--\n";
//		}
//	};
//	node1->set_mouse_enabled(true);
//	node2->set_mouse_enabled(true);
//	node1->set_mouse_propagation(false);
//	node2->set_mouse_propagation(false);
//	node2->add_mouse_event_listener(1,my_callback1);
//	node1->add_mouse_event_listener(1,my_callback);
//
//	this->当前场景 = node1;
}

stage::~stage(){
	
}

void stage::update(){
	//this->当前场景->update_position(0,0);
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
		//this->mouse_event_click = mouse_event{ Vector2{ 0.0f, 0.0f }, "" };//重置
		this->mouse_propagation_list.clear();//重置
		//传入的3个参数 1.当前鼠标状态,按下,按住,抬起,2.当前鼠标事件,鼠标坐标+鼠标哪个按键,3.当前鼠标穿透列表,用来记录不可穿透的节点信息
		//参数3的类型为 std::vector<Rectangle>& rects
		//问题,mouse_event如何带入callback执行?
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
	
//	if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)){
//		Vector2 curr_mouse_position = GetMousePosition();
//		this->当前场景->dispatch_mouse_event();
//	}else if(IsMouseButtonDown(MOUSE_RIGHT_BUTTON)){
//		Vector2 curr_mouse_position = GetMousePosition();
//		this->当前场景->dispatch_mouse_event();
//	}else if(IsMouseButtonUp(MOUSE_RIGHT_BUTTON)){
//		Vector2 curr_mouse_position = GetMousePosition();
//		this->当前场景->dispatch_mouse_event();
//	}
}
