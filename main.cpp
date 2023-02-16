#include "raylib.h"
#include <cstddef>
#include <cstdlib>
#include <cstdio>
#include <iostream>

class Node {
private:
	// 节点位置
	Vector2 m_size;
	Vector2 m_vPosition;
	// 节点锚点
	//Vector2 m_vAnchor;
	// 节点父节点
	Node* m_pParent;
	// 节点子节点列表
	Node* m_pChildren[10];
	// 节点子节点数量
	int m_iChildCount;
	// 节点事件监听器列表
	void (*m_pEventListeners[10])(void);
	// 节点事件监听器数量
	int m_iEventListenerCount;
	// 节点鼠标禁止穿透
	bool m_bMouseBlock;
	// 节点是否被点击过
	bool m_bClicked;
	
public:
	// 构造函数
	Node() {
		m_size.x = GetScreenWidth();
		m_size.y = GetScreenHeight();
		m_vPosition = {0, 0};
		//m_vAnchor = {0, 0};
		m_pParent = NULL;
		m_iChildCount = 0;
		m_iEventListenerCount = 0;
		m_bMouseBlock = false;
		m_bClicked = false;
	}
	
	virtual ~Node() {}
	// 设置节点位置
	void SetPosition(Vector2 vPosition) {
		m_vPosition = vPosition;
	}
	
	void SetSize(int w, int h){
		if(w<1){
			w = 1;
		};
		if(h<1){
			h = 1;
		};
		m_size.x = w;
		m_size.y = h;
		
	}
	
	Vector2 GetSize(){
		return m_size;
	}
	// 获取节点位置
	Vector2 GetPosition() {
		return m_vPosition;
	}
	
	// 设置节点锚点
//	void SetAnchor(Vector2 vAnchor) {
//		m_vAnchor = vAnchor;
//	}
	
	// 获取节点锚点
//	Vector2 GetAnchor() {
//		return m_vAnchor;
//	}
	// 设置节点父节点
	void SetParent(Node* pParent) {
		m_pParent = pParent;
	}
	
	// 获取节点父节点
	Node* GetParent() {
		return m_pParent;
	}
	
	// 添加节点子节点
	void AddChild(Node* pChild) {
		if (m_iChildCount < 10) {
			m_pChildren[m_iChildCount] = pChild;
			pChild->SetParent(this);
			m_iChildCount++;
		}
	}
	// 获取节点子节点
	Node* GetChild(int iIndex) {
		if (iIndex >= 0 && iIndex < m_iChildCount) {
			return m_pChildren[iIndex];
		}
		else {
			return NULL;
		}
	}
	
	// 获取节点子节点数量
	int GetChildCount() {
		return m_iChildCount;
	}
	// 添加节点事件监听器
	void AddEventListener(void (*pEventListener)(void)) {
		if (m_iEventListenerCount < 10) {
			m_pEventListeners[m_iEventListenerCount] = pEventListener;
			m_iEventListenerCount++;
		}
	}
	
	// 获取节点事件监听器
	void (*GetEventListener(int iIndex))(void) {
		if (iIndex >= 0 && iIndex < m_iEventListenerCount) {
			return m_pEventListeners[iIndex];
		}
		else {
			return NULL;
		}
	}
	// 获取节点事件监听器数量
	int GetEventListenerCount() {
		return m_iEventListenerCount;
	}
	
	// 设置节点鼠标禁止穿透
	void SetMouseBlock(bool bMouseBlock) {
		m_bMouseBlock = bMouseBlock;
	}
	
	// 获取节点鼠标禁止穿透
	bool GetMouseBlock() {
		return m_bMouseBlock;
	}

	// 设置节点是否被点击过
	void SetClicked(bool bClicked) {
		m_bClicked = bClicked;
	}
	
	// 获取节点是否被点击过
	bool GetClicked() {
		return m_bClicked;
	}
	
	void Update() {
		// 如果节点没有被点击过，才进行事件分发
		if (!m_bClicked) {
			// 获取鼠标位置
			Vector2 vMousePosition = GetMousePosition();
			Vector2 mNodeSize = GetSize();
			Vector2 mNodePos = GetPosition();
			// 判断鼠标是否在节点范围内
			if (vMousePosition.x>=mNodePos.x&&vMousePosition.y>=mNodePos.y&&vMousePosition.x<=mNodePos.x+mNodeSize.x &&vMousePosition.y<=mNodePos.y+mNodeSize.y) {
				// 遍历所有事件监听器，执行回调函数
				for (int i = 0; i < m_iEventListenerCount; i++) {
					(*m_pEventListeners[i])();
				}
				// 如果鼠标左键按下，设置节点为已点击状态
				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
					SetClicked(true);
				}
			}
		}
		else {
			// 如果鼠标左键释放，设置节点为未点击状态
			if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
				SetClicked(false);
			}
		}
		
		// 更新所有子节点状态
		for (int i = 0; i < m_iChildCount; i++) {
			m_pChildren[i]->Update();
		}
	}
	
	// 渲染节点
	void Render() {
		// 渲染自身
		DrawSelf();
		// 渲染所有子节点
		for (int i = 0; i < m_iChildCount; i++) {
			m_pChildren[i]->Render();
		}
	}
	
	// 绘制自身（虚函数，由子类实现）
	virtual void DrawSelf() = 0;
};

// 定义一个继承自Node的子类，实现DrawSelf方法
class Button : public Node {
private:
	// 按钮颜色
	Color m_cColor;
	// 按钮文字
	const char* m_sText;
	
public:
	// 构造函数
	Button(Color cColor, const char* sText) {
		m_cColor = cColor;
		m_sText = sText;
	}
	
	// 绘制自身
	void DrawSelf() override {
		// 获取节点位置和锚点
		Vector2 vPosition = GetPosition();
		Vector2 mSize = GetSize();
		//Vector2 vAnchor = GetAnchor();
		// 绘制一个矩形作为按钮背景
		DrawRectangle(vPosition.x,vPosition.y,mSize.x,mSize.y,m_cColor);//需要宽度和高度
		// 绘制按钮文字
		DrawText(m_sText, GetScreenWidth() / 2 - MeasureText(m_sText, 20) / 2, GetScreenHeight() / 2 - 10, 20, BLACK);
	}
};

// 定义一个回调函数，用于打印鼠标点击事件
void OnClick() {
	std::cout<<"asdasdasdasd";
	printf("You clicked the button!\n");
}

void OnClick1() {
	std::cout<<"123123123";
	printf("You 123123123ton!\n");
}

// 在主函数中创建一个Button对象，并添加鼠标事件监听器
int main() {
	// 初始化窗口和绘图环境
	InitWindow(800, 600, "Example");
	SetTargetFPS(60);
	
	// 创建一个Button对象，设置位置、锚点和颜色等属性
	Button* pButton = new Button(LIGHTGRAY, "Click Me");
	Button* b2 = new Button(BLACK, "bt");
	b2->SetSize(100,50);
	b2->AddEventListener(&OnClick1);
	b2->SetPosition({120.0f,120.0f});
	b2->SetMouseBlock(false);
	
	pButton->AddChild(b2);
	pButton->SetSize(100,100);
	pButton->SetPosition({100.0f, 100.0f});
	//pButton->SetAnchor({200.0f, 100.0f});
	pButton->SetMouseBlock(true);
	// 添加鼠标事件监听器，传入回调函数的指针
	pButton->AddEventListener(&OnClick);
	
	// 主循环
	while (!WindowShouldClose()) {
		// 更新节点状态
		pButton->Update();
		
		// 开始绘制
		BeginDrawing();
		ClearBackground(RAYWHITE);
		
		// 渲染节点
		pButton->Render();
		
		// 结束绘制
		EndDrawing();
	}
	
	// 释放资源和关闭窗口
	delete pButton;
	CloseWindow();
	
	return 0;
}
	
