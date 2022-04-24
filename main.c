// 雷霆战机游戏
// Author: Guyue

#include "ThunderAircraft.h"
#include "acllib.h"
#include "key.h"
#include <stdio.h>
#include <math.h>

#define WIDTH 300
#define HEIGHT 400

#define ARMYSTEP 5 
#define BULLETSTEP 20
#define ENEMYSTEP 1

// 全局变量（ MVC中的 M )

// 敌机、炮弹变量，暂时利用数组，可改为链表结构 
DS_Enemy enemys[1000];   
DS_Bullet bullets[1000];
DS_Army armys; 

int enemyCnt = 0;  // 敌机数量 
int bulletCnt = 0; // 子弹数量

int score = 0;     // 分数，击中一架敌机得1分
double speed = 1;  // 敌机下降速度，暂时根据得分调速，可改为根据游戏时间调速 

ACL_Image bkImg;  // 背景图片
ACL_Image endImg;  // GAME OVER图片
ACL_Image bulletImg;  // 子弹 
ACL_Image enemyImg;  // 敌机 
ACL_Image armyImg;  // 战机 

ACL_Sound bgm;    // BGM
ACL_Sound fire;   //发炮音乐 



// 战机模型
void drawArmy(DS_Army army)
{
//    rectangle(army.x-20, army.y, army.x+20, army.y+5);
//    rectangle(army.x-5, army.y-10, army.x+5, army.y);
	putImage(&armyImg, army.x, army.y);
}

// 敌机模型 
void drawEnemy(DS_Enemy enemy)
{
    line(enemy.x-10, enemy.y, enemy.x+10, enemy.y);
    line(enemy.x, enemy.y, enemy.x, enemy.y+10);
//	putImage(&enemyImg, enemy.x, enemy.y);
}

// 炮弹模型 
void drawBullet(DS_Bullet bullet)    
{
//    rectangle(bullet.x-3, bullet.y-3, bullet.x+3, bullet.y+3);
    putImage(&bulletImg, bullet.x+2, bullet.y+3);
    putImage(&bulletImg, bullet.x+32, bullet.y+3);
}


// 整数转换为字符串
char * intToString(int d)
{
		
} 


// MVC模型中的 View
void drawAll()
{
    beginPaint();
    // 背景（必须先绘制） 
    putImage(&bkImg, 0, 0); 
    
    // 游戏结束，呈现 GAME OVER 字幕 
	if ( armys.status ) {
		putImage(&endImg, 50, HEIGHT/2-40);
	}
	
	// 分数
//	paintText(0, 0, "12"); 
     
	// 战机 
    drawArmy(armys);
    // 敌机 
    for ( int i = 0; i < enemyCnt; i++ ) {
    	drawEnemy(enemys[i]);
	}
    // 炮弹 
	for ( int i = 0; i < bulletCnt; i++ ) {
    	drawBullet(bullets[i]);
	}   

    endPaint();
} 


// 敌机下降速度
void downSpeed()
{
	if ( score > 30 ) {
		speed = 1.2;
	}	else if ( score > 50 ) {
		speed = 1.5;
	}	else if ( score > 100 ) {
		speed = 2;
	}	
} 


// 从 enemys[] 数组中删除某个敌机
void delEnemy(int idx) 
{
	// 删除非最后一个元素，需要移动 idx 后面的所有元素 
	if ( idx < enemyCnt-1)	{
		for (int i = idx; i < enemyCnt; i++ ) {
			enemys[i-1] = enemys[i]; 
		} 
	}
	enemyCnt--;
} 

// 从 bullets[] 数组中删除某个炮弹 
void delBullet(int idx) 
{
	// 删除非最后一个元素，需要移动 idx 后面的所有元素 
	if ( idx < bulletCnt-1)	{
		for (int i = idx; i < bulletCnt; i++ ) {
			bullets[i-1] = bullets[i]; 
		} 
	}
	bulletCnt--;
}


// 炮弹击中敌机：两者状态均置 1 
// 敌机撞到战机：两者状态均置 1 
void isHit()
{		
	for ( int i = 0; i < enemyCnt; i++ ) {
		if ( ~enemys[i].status ) { // enemys[i]没有被击中的情况下才继续后续判断 
		
			// 炮弹击中敌机：两者状态均置 1
			for ( int j = 0; j < bulletCnt; j++ ) {
				if ( ~bullets[j].status \
					&& abs(enemys[i].x - bullets[j].x ) < 13 \
			        && abs(enemys[i].y - bullets[j].y) < 13 ) {
		
				    	enemys[i].status = 1;
						bullets[j].status = 1;
						
						score++;  // 得分	  
				}	
			}
			
			// 敌机撞到战机：两者状态均置 1 
			if ( abs( enemys[i].x - armys.x ) < 20
			    && abs( enemys[i].y - armys.y ) < 20) {
			    	enemys[i].status = 1;
					armys.status = 1;
			}
       }
    }
			
}


// 扫描所有 Object, 更新数据（ MVC中的 C） 
void scanAll()
{
	// 1.更新所有敌机、战机、顶部子弹的击中状态
	isHit(bullets, enemys);
	
	// 2.更新战机数据 
	// 若战机被击中，从界面中移除战机 
	if ( armys.status ) {
		armys.x = 800;
		armys.y = 800;
	} 
	 
	// 3.更新炮弹数据 
	// 子弹每次上升 BULLETSTEP 
	for ( int i = 0; i < bulletCnt; i++ ) {
    	bullets[i].y -= BULLETSTEP;
	}
    // 如果子弹击中目标或者超出界面范围，从数组中删除 
    for ( int i = 0; i < bulletCnt; i++ ) {
    	if ( bullets[i].status || bullets[i].y < 0 ) {
			// 从 bullets[] 数组中移除该子弹
			delBullet(i);
		}
	} 
	
	// 4.更新敌机数据
	// 随机增加敌机
	static int cc = 0;
	if ( cc++ % 9 == 8 ) {
		cc = 0;

		int x = rand() % WIDTH;
		if ( x < 30 )
			x = 30;
		if ( x > WIDTH - 30 )
			x = WIDTH - 30;
		enemys[enemyCnt++]= (DS_Enemy){ x, 0, 0};
	}
	 
	for ( int i = 0; i < enemyCnt; i++ ) {
		// 敌机每次下降 ENEMYSTEP * speed;
    	enemys[i].y += ENEMYSTEP * speed;
    	
    	// 如果敌机被击中或者超出界面范围，从数组中删除 
		if ( enemys[i].status || enemys[i].y > HEIGHT ) {
			// 从 enemys[] 数组中移除该敌机 
			delEnemy(i); 
		}
	}
	
	{
//		printf("战机：\nx:%d, y:%d, status:%d\n", armys.x, armys.y, armys.status);
		printf("敌机1号：\nx:%d, y:%d, status:%d\n", \
				enemys[0].x, enemys[0].y, enemys[0].status);
		printf("enemyCnt:%d\n", enemyCnt);
		printf("bulletCnt:%d\n", bulletCnt);
	}
}



// 定时器回调函数
// 1.扫描所有 Object, 做各种判断，并更新数据
// 2.重新绘制 
void timerListener(int timerID)
{
	scanAll();
    drawAll();
}




// 键盘回调函数（ MVC中的 C )
// 1.方向键更新战机位置坐标； 
// 2.空格键添加一个子弹到数组中； 
// 3. 重新绘制； 
void keyListener(int key, int event)
{ 
    if ( event == KEY_DOWN ) {
        switch( key ) {
        	// 空格键 
            case VK_SPACE: {
            	for ( int i = 0; i < 2; i++ ) {
            		bullets[bulletCnt++] = 
					(DS_Bullet){armys.x+3, armys.y-10, 0};  // 更新数据 
				
            		playSound(fire, 0); 
				}
                break; 
            }
            // 上 
            case VK_UP:
            case VK_W : {
                armys.y -= ARMYSTEP;
                break;
            } 
            // 下 
            case VK_DOWN:
            case VK_S   : {
            	armys.y += ARMYSTEP;
                break;
            }
            // 左
            case VK_LEFT:
            case VK_A   : {
            	armys.x -= ARMYSTEP;
                break;
            }
            // 右
            case VK_RIGHT:
            case VK_D    : {
            	armys.x += ARMYSTEP;
                break;
            }  
        }
    	drawAll();
    }
}

void initGame()
{
	// 初始化战机 
    armys = (DS_Army){125, 330, 0};
    drawAll();
    
    beginPaint();
    
    // 加载图片 
    loadImage("D:\\Project\\ThunderAircraft\\sources\\bg.bmp", &bkImg);
    loadImage("D:\\Project\\ThunderAircraft\\sources\\gameover.bmp", &endImg);
    loadImage("D:\\Project\\ThunderAircraft\\sources\\bullet.bmp", &bulletImg);
    loadImage("D:\\Project\\ThunderAircraft\\sources\\army.bmp", &armyImg);
    loadImage("D:\\Project\\ThunderAircraft\\sources\\enemy.bmp", &enemyImg);
    // 绘制背景 
    putImage(&bkImg, 0, 0);
    endPaint(); 
    
    // 加载音乐 
    loadSound("D:\\Project\\ThunderAircraft\\sources\\bgm1.mp3", &bgm);
    loadSound("D:\\Project\\ThunderAircraft\\sources\\fire.wav", &fire);
    // 循环播放 BGM 
    playSound(bgm, 1);
    
    
}

int Setup() 
{
    initWindow("ThunderAircraft", DEFAULT, DEFAULT, WIDTH, HEIGHT);
    initGame();
//    initConsole();
    
    registerKeyboardEvent(keyListener);
    registerTimerEvent(timerListener); 
    
    startTimer(0, 100);
    
    return 0; 
 } 



 


