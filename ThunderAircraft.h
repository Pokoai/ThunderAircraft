#ifndef __THUNDERAIRCRAFT_H__
#define __THUNDERAIRCRAFT_H__


// �������ݽṹ

// 1.�л����ݽṹ
typedef struct enemy {
    int x, y;    // λ������
    int status;  // �Ƿ񱻻���
//    int speed;   // �ٶ� 
} DS_Enemy;


// 2.ս�����ݽṹ
typedef struct army {
    int x, y;    // λ������
    int status;  // �Ƿ�ײ�� 
} DS_Army;  


// 3. �ڵ����ݽṹ
typedef struct bullet {
    int x, y;    // λ������
    int status;  // �Ƿ���� 
} DS_Bullet; 


typedef int Status;  

// �Ƿ����
//typedef enum  {
//	noHIT;
//	isHIT;
//} Hit; 

#endif
