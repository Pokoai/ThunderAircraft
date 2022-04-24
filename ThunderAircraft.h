#ifndef __THUNDERAIRCRAFT_H__
#define __THUNDERAIRCRAFT_H__


// 定义数据结构

// 1.敌机数据结构
typedef struct enemy {
    int x, y;    // 位置坐标
    int status;  // 是否被击中
//    int speed;   // 速度 
} DS_Enemy;


// 2.战机数据结构
typedef struct army {
    int x, y;    // 位置坐标
    int status;  // 是否被撞击 
} DS_Army;  


// 3. 炮弹数据结构
typedef struct bullet {
    int x, y;    // 位置坐标
    int status;  // 是否击中 
} DS_Bullet; 


typedef int Status;  

// 是否击中
//typedef enum  {
//	noHIT;
//	isHIT;
//} Hit; 

#endif
