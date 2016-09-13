#ifndef __SWITCH_OPS_H
#define __SWITCH_OPS_H

void init_switch(void);
int open_switch(unsigned char index);
int close_switch(unsigned char index);

unsigned char get_input_status(unsigned char index);
unsigned char get_light_zouyou_status(void);
unsigned char get_light_shangxia_status(void);

unsigned char get_feiye_fuzi_status(void);
unsigned char get_qingxiye_fuzi_status(void);
unsigned char get_xishiye_fuzi_status(void);

unsigned char get_start_key_status(void);   //��ȡ �������� ״̬
unsigned char get_xckg_ybh_status(void);    //��ȡ ������ �г̿��� ״̬
unsigned char get_xckg_sjk_status(void);    //��ȡ �Լ��� �г̿��� ״̬

unsigned char get_shijika_key_status(void);
unsigned char get_qingxi_key_status(void);
unsigned char get_start_key_status(void);

unsigned char get_go_status(unsigned char index);  //��ȡ ���� ״̬ 
unsigned char get_all_go_status(unsigned char go_type,unsigned char *buf);

void check_go(unsigned char go_type);

#endif
