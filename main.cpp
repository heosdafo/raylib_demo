#include "raylib.h"
#include <iostream>
class camera {
	
public:
	static camera* get_instance(){
		if(instance == nullptr){
			instance = new camera();
		}
		return instance;
	};
	camera(){
		
	};
	~camera(){
		
	};
	int 绝对坐标x;
	int 绝对坐标y;
	int width;
	int height;
private:
	static camera* instance;
};
camera* camera::instance;

class node {

public:
	node* parent;
	node* children_list[1500];
	int children_list_size;
	int curr_zindex;
	int target_zindex;
	Vector2 相对坐标;
	Vector2 锚点坐标;
	Vector2 绝对坐标;
	Texture2D 纹理;
	Rectangle 帧信息;
	Rectangle 包围盒;
	node(){
		this->curr_zindex = -1;
		this->target_zindex = -1;
		this->children_list_size = 0;
		this->相对坐标.x = 0;
		this->相对坐标.y = 0;
		this->绝对坐标.x = 0;
		this->绝对坐标.y = 0;
		this->锚点坐标.x = 0;
		this->锚点坐标.y = 0;
		this->parent = nullptr;
	};
	~node(){
		
	};
	void set_zindex(int z){
		if(this->parent == NULL){
			if(z<0){
				this->target_zindex = 0;
			}else{
				this->target_zindex = z;
			}
			
		}else{//有父节点
			//z为目标位置
			if(z<0){
				z = 0;
			}else if(z>this->parent->children_list_size-1){
				z = this->parent->children_list_size-1;
			}
			
			if(z == this->curr_zindex){//目标zindex和原有zindex相同
				return;
			}else if(z > this->curr_zindex){//目标zindex大于原有zindex
				node* tmp_node = this;
				for(int i = this->curr_zindex ; i <= z;i++){
					this->parent->children_list[i] = this->parent->children_list[i+1];
					this->parent->children_list[i]->curr_zindex--;
				}
				this->parent->children_list[z] = tmp_node;
				this->curr_zindex = z;
			}else if(z < this->curr_zindex){
				node* tmp_node = this;
				for(int i = this->curr_zindex;i >= z;i--){
					this->parent->children_list[i] = this->parent->children_list[i-1];
					this->parent->children_list[i]++;
				}
				this->parent->children_list[z] = tmp_node;
				this->curr_zindex = z;
			}
		}
	};
	void swap_zindex(node* 目标节点){
		if(this->parent == NULL){
			std::cout<<"---->node::swap_zindex, this node has not parent"<<"\n";
			return;
		}else if(目标节点->parent == NULL){
			std::cout<<"---->node::swap_zindex, target node has not parent"<<"\n";
			return;
		}else if(目标节点->parent != this->parent){
			std::cout<<"---->node::swap_zindex, this parent != target parent"<<"\n";
			return;
		}
		else{
			node* tmp_node = this;
			int tmp_zindex = this->curr_zindex;
			this->parent->children_list[tmp_zindex] = 目标节点;
			this->parent->children_list[tmp_zindex]->curr_zindex = tmp_zindex;
			
			this->parent->children_list[目标节点->curr_zindex] = tmp_node;
			this->parent->children_list[目标节点->curr_zindex]->curr_zindex = tmp_node->curr_zindex;
		}
	};
	int get_zindex(){
		return this->curr_zindex;	
	};
	void set_position(int x, int y){
		this->相对坐标.x = x;
		this->相对坐标.y = y;
	};
	int get_position_x(){
		return this->相对坐标.x;	
	};
	int get_position_y(){
		return this->相对坐标.y;	
	};
	void set_anchor(int x, int y){
		this->锚点坐标.x = x;
		this->锚点坐标.y = y;
	};
	int get_anchor_x(){
		return this->锚点坐标.x;
	};
	int get_anchor_y(){
		return this->锚点坐标.y;
	};
	int get_width(){
		return this->包围盒.width;
	};
	int get_height(){
		return this->包围盒.height;
	};
	void add_child(node* 子节点){
		if(子节点->parent != NULL){
			std::cout<<"---->node::add_child, this son node has a parent"<<"\n";
			return;
		}else if(1){
			//忘记了另外一种判断
		}
		if(子节点->target_zindex != -1){//在addchild之前 就已经使用了set_zindex;
			//TODO
			if(子节点->target_zindex >= this->children_list_size){
				this->children_list[this->children_list_size] = 子节点;
				子节点->curr_zindex = this->children_list_size;
				子节点->target_zindex = -1;
			}else{
				for(int i = this->children_list_size-1;i >= 子节点->target_zindex; i--){
					this->children_list[i+1] = this->children_list[i];
				}
				this->children_list[子节点->target_zindex] = 子节点;
				子节点->curr_zindex = 子节点->target_zindex;
				子节点->target_zindex = -1;
			}
			this->children_list_size++;
		}else{//在addchild之前 未使用set_zindex
			this->children_list[this->children_list_size] = 子节点;
			子节点->curr_zindex = this->children_list_size;
			子节点->target_zindex = -1;
			this->children_list_size++;
		}
	};
	void remove_child(node* 子节点){
		if(子节点->parent == NULL){
			std::cout<<"---->node::remove_child, this son node has not a parent"<<"\n";
			return;
		}
		for(int i = 子节点->curr_zindex;i <= this->children_list_size-1;i++){
			this->children_list[i] = this->children_list[i+1];
		}
		this->children_list_size--;
		子节点->curr_zindex = -1;
		子节点->target_zindex = -1;
		//需要补需要考虑释放 子节点的内存啥的 比如释放纹理内存.
	};
	void remove_frome_parent(node* 父节点){
		if(this->parent!=父节点){
			std::cout<<"---->node::remove_frome_parent, this node parent != param 1"<<"\n";
			return;
		}else if(this->parent == NULL){
			std::cout<<"---->node::remove_frome_parent,this node has not parent"<<"\n";
			return;
		}
		for(int i = this->curr_zindex;i <= 父节点->children_list_size-1;i++){
			父节点->children_list[i] = 父节点->children_list[i+1];
		}
		父节点->children_list_size--;
		this->curr_zindex = -1;
		this->target_zindex = -1;
		//需要补需要考虑释放 子节点的内存啥的 比如释放纹理内存.
	};
	void set_texture(const char* file_name){
		
	};
	void set_texture_frome_spritesheet(const char* png_name, const char* conf_name,const char* ele_name){
		
	};
	virtual void update(){
		
	};
	virtual void update_position(int psx_and_ax, int psx_and_ay){
		this->绝对坐标.x = psx_and_ax + this->相对坐标.x - this->锚点坐标.x;
		this->绝对坐标.y = psx_and_ay + this->相对坐标.y - this->锚点坐标.y;
		for(int i=0;i<this->children_list_size;i++){
			//children_list[i]->坐标是否更改 这里是否需要设置? 锚点更改是否需要更改子节点?
			this->children_list[i]->update_position(this->绝对坐标.x+this->锚点坐标.x,this->绝对坐标.y+this->锚点坐标.y);
		}
	};
	virtual void draw(){
		DrawTextureRec(this->纹理,this->帧信息,this->绝对坐标,WHITE);
		for(int i=0;i<this->children_list_size;i++){
			this->children_list[i]->draw();
		}
	};

};

class stage {
	int YourVar;
private:
	static stage* instance;
public:
	node* 当前场景;
	Vector2 相对坐标;
	Vector2 绝对坐标;
	stage(){
		相对坐标.x = 0;
		相对坐标.y = 0;
		绝对坐标.x = 0;
		绝对坐标.y = 0;
		//创建当前场景
		camera::get_instance()->绝对坐标x = 0;
		camera::get_instance()->绝对坐标y = 0;
		this->init();	
	};
	~stage(){

	};
	static stage* get_instance(){
		if(instance == nullptr){
			instance = new stage();
		}
		return instance;
	};
	
	void init(){
		
	};
	void update(){
		
	};
	
	void update_position(){
		当前场景->update_position(绝对坐标.x-camera::get_instance()->绝对坐标x,绝对坐标.y-camera::get_instance()->绝对坐标y);
	};
	void draw(){
		当前场景->draw();
	};

};
stage* stage::instance;

int main(void)
{
	// Initialization
	//--------------------------------------------------------------------------------------
	const int screenWidth = 800;
	const int screenHeight = 450;
	
	InitWindow(screenWidth, screenHeight, "demo");
	InitAudioDevice();
	SetTargetFPS(120);
	SetTraceLogLevel(LOG_WARNING);
	
	
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		
		BeginDrawing();
		stage::get_instance()->update_position();
		stage::get_instance()->update();
		stage::get_instance()->draw();
		ClearBackground(RAYWHITE);
		
		//DrawText("Click to explode!", 440, 400, 40, DARKGRAY);
		
		EndDrawing();
		//----------------------------------------------------------------------------------
	}
	CloseAudioDevice();
	CloseWindow();              // Close window and OpenGL context
	return 0;
}
