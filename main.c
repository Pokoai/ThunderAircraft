// ����ս����Ϸ
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

// ȫ�ֱ����� MVC�е� M )

// �л����ڵ���������ʱ�������飬�ɸ�Ϊ����ṹ 
DS_Enemy enemys[1000];   
DS_Bullet bullets[1000];
DS_Army armys; 

int enemyCnt = 0;  // �л����� 
int bulletCnt = 0; // �ӵ�����

int score = 0;     // ����������һ�ܵл���1��
double speed = 1;  // �л��½��ٶȣ���ʱ���ݵ÷ֵ��٣��ɸ�Ϊ������Ϸʱ����� 

ACL_Image bkImg;  // ����ͼƬ
ACL_Image endImg;  // GAME OVERͼƬ
ACL_Image bulletImg;  // �ӵ� 
ACL_Image enemyImg;  // �л� 
ACL_Image armyImg;  // ս�� 

ACL_Sound bgm;    // BGM
ACL_Sound fire;   //�������� 



// ս��ģ��
void drawArmy(DS_Army army)
{
//    rectangle(army.x-20, army.y, army.x+20, army.y+5);
//    rectangle(army.x-5, army.y-10, army.x+5, army.y);
	putImage(&armyImg, army.x, army.y);
}

// �л�ģ�� 
void drawEnemy(DS_Enemy enemy)
{
    line(enemy.x-10, enemy.y, enemy.x+10, enemy.y);
    line(enemy.x, enemy.y, enemy.x, enemy.y+10);
//	putImage(&enemyImg, enemy.x, enemy.y);
}

// �ڵ�ģ�� 
void drawBullet(DS_Bullet bullet)    
{
//    rectangle(bullet.x-3, bullet.y-3, bullet.x+3, bullet.y+3);
    putImage(&bulletImg, bullet.x+2, bullet.y+3);
    putImage(&bulletImg, bullet.x+32, bullet.y+3);
}


// ����ת��Ϊ�ַ���
char * intToString(int d)
{
		
} 


// MVCģ���е� View
void drawAll()
{
    beginPaint();
    // �����������Ȼ��ƣ� 
    putImage(&bkImg, 0, 0); 
    
    // ��Ϸ���������� GAME OVER ��Ļ 
	if ( armys.status ) {
		putImage(&endImg, 50, HEIGHT/2-40);
	}
	
	// ����
//	paintText(0, 0, "12"); 
     
	// ս�� 
    drawArmy(armys);
    // �л� 
    for ( int i = 0; i < enemyCnt; i++ ) {
    	drawEnemy(enemys[i]);
	}
    // �ڵ� 
	for ( int i = 0; i < bulletCnt; i++ ) {
    	drawBullet(bullets[i]);
	}   

    endPaint();
} 


// �л��½��ٶ�
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


// �� enemys[] ������ɾ��ĳ���л�
void delEnemy(int idx) 
{
	// ɾ�������һ��Ԫ�أ���Ҫ�ƶ� idx ���������Ԫ�� 
	if ( idx < enemyCnt-1)	{
		for (int i = idx; i < enemyCnt; i++ ) {
			enemys[i-1] = enemys[i]; 
		} 
	}
	enemyCnt--;
} 

// �� bullets[] ������ɾ��ĳ���ڵ� 
void delBullet(int idx) 
{
	// ɾ�������һ��Ԫ�أ���Ҫ�ƶ� idx ���������Ԫ�� 
	if ( idx < bulletCnt-1)	{
		for (int i = idx; i < bulletCnt; i++ ) {
			bullets[i-1] = bullets[i]; 
		} 
	}
	bulletCnt--;
}


// �ڵ����ел�������״̬���� 1 
// �л�ײ��ս��������״̬���� 1 
void isHit()
{		
	for ( int i = 0; i < enemyCnt; i++ ) {
		if ( ~enemys[i].status ) { // enemys[i]û�б����е�����²ż��������ж� 
		
			// �ڵ����ел�������״̬���� 1
			for ( int j = 0; j < bulletCnt; j++ ) {
				if ( ~bullets[j].status \
					&& abs(enemys[i].x - bullets[j].x ) < 13 \
			        && abs(enemys[i].y - bullets[j].y) < 13 ) {
		
				    	enemys[i].status = 1;
						bullets[j].status = 1;
						
						score++;  // �÷�	  
				}	
			}
			
			// �л�ײ��ս��������״̬���� 1 
			if ( abs( enemys[i].x - armys.x ) < 20
			    && abs( enemys[i].y - armys.y ) < 20) {
			    	enemys[i].status = 1;
					armys.status = 1;
			}
       }
    }
			
}


// ɨ������ Object, �������ݣ� MVC�е� C�� 
void scanAll()
{
	// 1.�������ел���ս���������ӵ��Ļ���״̬
	isHit(bullets, enemys);
	
	// 2.����ս������ 
	// ��ս�������У��ӽ������Ƴ�ս�� 
	if ( armys.status ) {
		armys.x = 800;
		armys.y = 800;
	} 
	 
	// 3.�����ڵ����� 
	// �ӵ�ÿ������ BULLETSTEP 
	for ( int i = 0; i < bulletCnt; i++ ) {
    	bullets[i].y -= BULLETSTEP;
	}
    // ����ӵ�����Ŀ����߳������淶Χ����������ɾ�� 
    for ( int i = 0; i < bulletCnt; i++ ) {
    	if ( bullets[i].status || bullets[i].y < 0 ) {
			// �� bullets[] �������Ƴ����ӵ�
			delBullet(i);
		}
	} 
	
	// 4.���µл�����
	// ������ӵл�
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
		// �л�ÿ���½� ENEMYSTEP * speed;
    	enemys[i].y += ENEMYSTEP * speed;
    	
    	// ����л������л��߳������淶Χ����������ɾ�� 
		if ( enemys[i].status || enemys[i].y > HEIGHT ) {
			// �� enemys[] �������Ƴ��õл� 
			delEnemy(i); 
		}
	}
	
	{
//		printf("ս����\nx:%d, y:%d, status:%d\n", armys.x, armys.y, armys.status);
		printf("�л�1�ţ�\nx:%d, y:%d, status:%d\n", \
				enemys[0].x, enemys[0].y, enemys[0].status);
		printf("enemyCnt:%d\n", enemyCnt);
		printf("bulletCnt:%d\n", bulletCnt);
	}
}



// ��ʱ���ص�����
// 1.ɨ������ Object, �������жϣ�����������
// 2.���»��� 
void timerListener(int timerID)
{
	scanAll();
    drawAll();
}




// ���̻ص������� MVC�е� C )
// 1.���������ս��λ�����ꣻ 
// 2.�ո�����һ���ӵ��������У� 
// 3. ���»��ƣ� 
void keyListener(int key, int event)
{ 
    if ( event == KEY_DOWN ) {
        switch( key ) {
        	// �ո�� 
            case VK_SPACE: {
            	for ( int i = 0; i < 2; i++ ) {
            		bullets[bulletCnt++] = 
					(DS_Bullet){armys.x+3, armys.y-10, 0};  // �������� 
				
            		playSound(fire, 0); 
				}
                break; 
            }
            // �� 
            case VK_UP:
            case VK_W : {
                armys.y -= ARMYSTEP;
                break;
            } 
            // �� 
            case VK_DOWN:
            case VK_S   : {
            	armys.y += ARMYSTEP;
                break;
            }
            // ��
            case VK_LEFT:
            case VK_A   : {
            	armys.x -= ARMYSTEP;
                break;
            }
            // ��
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
	// ��ʼ��ս�� 
    armys = (DS_Army){125, 330, 0};
    drawAll();
    
    beginPaint();
    
    // ����ͼƬ 
    loadImage("D:\\Project\\ThunderAircraft\\sources\\bg.bmp", &bkImg);
    loadImage("D:\\Project\\ThunderAircraft\\sources\\gameover.bmp", &endImg);
    loadImage("D:\\Project\\ThunderAircraft\\sources\\bullet.bmp", &bulletImg);
    loadImage("D:\\Project\\ThunderAircraft\\sources\\army.bmp", &armyImg);
    loadImage("D:\\Project\\ThunderAircraft\\sources\\enemy.bmp", &enemyImg);
    // ���Ʊ��� 
    putImage(&bkImg, 0, 0);
    endPaint(); 
    
    // �������� 
    loadSound("D:\\Project\\ThunderAircraft\\sources\\bgm1.mp3", &bgm);
    loadSound("D:\\Project\\ThunderAircraft\\sources\\fire.wav", &fire);
    // ѭ������ BGM 
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



 


