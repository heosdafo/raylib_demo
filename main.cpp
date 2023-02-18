#include <raylib.h>
#include <vector>
#include <iostream>
#define NUM_FRAMES_PER_LINE     5
#define NUM_LINES               5




struct 事件穿透记录参数 {
	int node_conf[10][4] ;//记录每个禁止穿透的节点的xy坐标点,以及长度和宽度
	int 数组当前行;
	bool 是否到达最上层节点;//这个变量作为标记使用,只在游戏屏幕中最上层节点执行event函数时才将其设置为true;
	
};
class node {
public:
	node(int x, int y) {
		size.height = 100;
		size.width = 100;
		size.x = x;
		size.y = y;
		m_pos.x = x;
		m_pos.y = y;
		m_color = RED;
		是否穿透 = true;
		children_list.clear();
	}
	std::vector<node*> children_list;
	bool 是否穿透;
	bool 是否响应事件;
	Rectangle size;
	Vector2 m_pos;
	const char* name;

	Color m_color;
	void set_color(Color c) {
		m_color = c;
	}
	void addchild(node* son_node) {
		children_list.push_back(son_node);
	}
	void event(事件穿透记录参数 &参数1){
		

		for (auto it = children_list.rbegin(); it != children_list.rend(); ++it) {
			(*it)->event(参数1);
		}
		if(参数1.是否到达最上层节点 == false){//这条判断中是否到达最上层节点原始值是false,但在游戏屏幕中最上层节点触发event函数后,将会被设置为true,其作用是为了判断被点击的节点是否为游戏屏幕中最上层的节点.
			Vector2 当前鼠标坐标 = GetMousePosition();
			if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
				if (CheckCollisionPointRec(当前鼠标坐标, size)) 
				{
					std::cout<<"----> is in first node inside\n";
				}
			}
			参数1.是否到达最上层节点 = true;//在游戏屏幕中最上层节点执行完event函数之前,会将这个变量设置为true
		}else{
			Vector2 当前鼠标坐标 = GetMousePosition();
			if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
				if (CheckCollisionPointRec(当前鼠标坐标, size)) 
				{
					bool 是否在重叠位置 = false;
					for(int i=0;i<参数1.数组当前行;i++){
						Rectangle tmp_size;
						tmp_size.x = 参数1.node_conf[i][0];
						tmp_size.y = 参数1.node_conf[i][1];
						tmp_size.width = 参数1.node_conf[i][2];
						tmp_size.height = 参数1.node_conf[i][3];
						if(CheckCollisionPointRec(当前鼠标坐标, tmp_size)){
							是否在重叠位置 = true;
							break;
						}else{
							是否在重叠位置 = false;
						}
					}
					if(是否在重叠位置 == false){
						std::cout<<"--->"<<name<<"\n";
					}
				}
			}
		}
		if(是否穿透 == false){
			参数1.node_conf[参数1.数组当前行][0] = size.x;
			参数1.node_conf[参数1.数组当前行][1] = size.y;
			参数1.node_conf[参数1.数组当前行][2] = size.width;
			参数1.node_conf[参数1.数组当前行][3] = size.height;
			参数1.数组当前行++;
			
		}
	
	};

	
	void draw() {
		DrawRectangle(size.x, size.y, size.width, size.height, m_color);
		for (size_t i = 0; i < children_list.size(); i++) {
			children_list[i]->draw();
		}
	}
};


int main(void)
{
	const int screenWidth = 800;
	const int screenHeight = 450;
	InitWindow(screenWidth, screenHeight, "raylib [textures] example - sprite explosion");
	InitAudioDevice();
	SetTargetFPS(60);
	SetTraceLogLevel(LOG_WARNING);	
	事件穿透记录参数 m_event;
	m_event.数组当前行 = 1;
	node* root = new node(50, 50);
	root->set_color(BLUE);
	// 创建一些子节点
	node* node1 = new node(100, 50);
	node1->set_color(GREEN);
	root->addchild(node1);
	node* node2 = new node(70, 100);
	node2->set_color(RED);
	root->addchild(node2);
	node1->name = "node1";
	node2->name = "node2";
	node* node3 = new node(120
		,120);
	node3->set_color(BLACK);
	node3->name = "node3";
	node2->addchild(node3);
	node2->是否穿透 = false;
	node1->是否穿透 = false;
	node3->size.width = 70;
	node3->size.height = 70;
	node3->是否穿透 = false;
	root->name = "root";

	while (!WindowShouldClose())
	{
		root->event(m_event);
		m_event.数组当前行=1;

		m_event.是否到达最上层节点 = false;
		BeginDrawing();
		ClearBackground(RAYWHITE);
		root->draw();
		DrawText("Click to explode!", 440, 400, 40, DARKGRAY);
		EndDrawing();
		//----------------------------------------------------------------------------------
	}
	CloseAudioDevice();
	CloseWindow();
	return 0;
}
