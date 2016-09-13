#ifndef __PUBLIC_H
#define __PUBLIC_H

#define MOTOR_1   0
#define MOTOR_2   1
#define MOTOR_3   2
#define MOTOR_4   3
#define MOTOR_5   4
#define MOTOR_6   5
#define MOTOR_7   6
#define MOTOR_8   7
#define MOTOR_9   8
#define MOTOR_10  9
#define MOTOR_11  10


#define RUN_MODE_RESET 1
#define RUN_MODE_RUN   2

#define MOTOR_STATUS_RUN   1
#define MOTOR_STATUS_STOP  0

//�������ģʽ�����ͣ�����
#define MOTOR_RUN_MODE_CIRCLE 1 
#define MOTOR_RUN_MODE_LINE   2

typedef struct tag_Motor_Info {
	unsigned char motor_type;   //������,��Ϊ���������ö�ʱ��
	unsigned char dir;   		//���з���
	unsigned int steps;  	 	//���еĲ���
	unsigned int max_steps;  	//�������Ĳ���
	unsigned char qk_count;     //  ȱ������
	unsigned char run_mode;     //����ģʽ���������С���λ
	unsigned char motor_run_mode; //�������ģʽ�����ͣ�����
	unsigned char use_go;         //�Ƿ� ʹ�ù����־
	unsigned char status_1,status_2,temp_count;
}MOTOR_INFO;


#endif
