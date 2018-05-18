#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include "led.h"     
#include "key.h"  
#include "hand_sensor.h"  

 

		
#include "sh_054a.h"
		
 
		


 struct GP2AP054A_params
{                                     //参数结构体
	/* Temporal parameters for Direction Judgement */    //推荐临时参数设置
	float max_x1, min_x1, max_y1, min_y1;
	float max_x2, min_x2, max_y2, min_y2;
	int diff_max_x, diff_max_y;
	int x_plus, x_minus, y_plus, y_minus;
	int gs_state;
	int speed_counts;
	/* Thresholds depending on the performance */        //阀值性能
	s16 ignore_diff_th;
	u16 ignore_z_th;
	//signed char ratio_th;
	float ratio_th;
	/* Parameters for active offset cancelation */        //活动补偿取消参数
	int active_osc_on;
	int allowable_variation;
	int acquisition_num;
	u16 max_aoc;
	u16 min_aoc;
	/* Parameters for Zoom function */                    //变焦高度的参数
	int zoomFuncOn;
	int to_zoom_th;
	int out_zoom_th;
	int zoomModeNow;
	int zoom_mode_th;
	/* Saturation notification */                          //饱和度通告
	int sat_flag[4][5];
	/* Store raw_data */ //
	u16 raw_d[4][5];
	/* Store aoc_data */
	u16 aoc_d[4];
	/* Store aoc_data for Zoom */
	u16 zoom_aoc[5];
	/* Store zoom_data */
	u16 zoom_d[5];
	/* Store sub_data */
	u16 sub_d[5];
	/* GS_result */
	u16 res_gs;
} ;

// *******************************************************************
// * Name	: clearGSparams(Input)
// * Input	: struct GP2AP054A_params pointer
// * Output	:
// * Note	: clear temporal variables for direction judgement//清理临时变量的说明
// *******************************************************************
void clearGSparams(struct GP2AP054A_params *p_gp) {
	p_gp->x_plus		= 0;
	p_gp->x_minus		= 0;
	p_gp->y_plus		= 0;
	p_gp->y_minus		= 0;
	p_gp->max_x1		= 0;
	p_gp->min_x1		= 0;
	p_gp->max_y1		= 0;
	p_gp->min_y1		= 0;
	p_gp->max_x2		= 0;
	p_gp->min_x2		= 0;
	p_gp->max_y2		= 0;
	p_gp->min_y2		= 0;
	p_gp->diff_max_x	= 0;
	p_gp->diff_max_y	= 0;
	p_gp->speed_counts	= 0;
 	p_gp->gs_state		= STATE_ONE;
	return;
}

	

/*******************************************************************
 * Name	: init054Aparams(Input)   结构体
 * Input	: struct GP2AP054A_params pointer
 * Output	: 
 * Note	: Initialize variables
 *******************************************************************/
void initGP2AP054Aparams(struct GP2AP054A_params *p_gp){
	int i,m,n;
	clearGSparams(p_gp);
	p_gp->ignore_z_th 			= 20;
	p_gp->ignore_diff_th 		= 10;
	p_gp->ratio_th 				= 0.1;
	p_gp->active_osc_on 		= 1;
	p_gp->allowable_variation 	= 30;
	p_gp->acquisition_num		= 10;
	p_gp->max_aoc				= 8000;
	p_gp->min_aoc				= 0;
	p_gp->zoomFuncOn 			= 1;
	p_gp->to_zoom_th			= 100;
	p_gp->out_zoom_th			= 30;
	p_gp->zoomModeNow 			= 0;
	p_gp->zoom_mode_th			= 4;
	p_gp->res_gs				= 0;
	 	
	for(i=0; i<4; i++){
		p_gp->sat_flag[0][i]	= 0;
		p_gp->raw_d[0][i]		= 0;
		p_gp->aoc_d[i]			= p_gp->max_aoc/4;
		p_gp->sub_d[i]			= 0;
		p_gp->zoom_aoc[i]		= 0;
		p_gp->zoom_d[i]			= 0;
	}
	p_gp->sat_flag[0][4]		= 0;
	p_gp->raw_d[0][4]			= 0;
	p_gp->sub_d[4]				= 0;
	
	for(m=0; m<4; m++){
		for(n=0; n<4; n++){
			p_gp->sat_flag[m][n]		= 0;
			p_gp->raw_d[m][n]			= 0;
		}
	}
}



/*******************************************************************
 * Name	: getActiveOffset(Input1, Output, Input2)
 * Input	: Input1 = p_gp->raw_d[5], Input2 = struct gp_params pointer
 * Output	: p_gp->aoc_d[4]
 * Note	: to get offset value from acquisition times of raw data.  从捕获原始数据的次数来获取位移的值
 *******************************************************************/
void getOffsetData(struct GP2AP054A_params *p_gp, int j){
	int i;
	static int num_counts=0;
	static u16 max_d[4]={0};
	static u16 min_d[4]={0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF};
	static u32 avg_d[4]={0};
	
	if( p_gp->sat_flag[j][4]){
		num_counts = 0;
		return;
	}
	if(!((p_gp->raw_d[j][4] <= p_gp->max_aoc) && (p_gp->raw_d[j][4] >= p_gp->min_aoc))){
		num_counts = 0;
		return;
	}
	//// The last measurement of offset
	if(num_counts == p_gp->acquisition_num -1){
		for(i=0;i<4;i++){
			avg_d[i] = avg_d[i] + p_gp->raw_d[j][i];
			if(p_gp->raw_d[j][i] > max_d[i]){
				max_d[i] = p_gp->raw_d[j][i];
			}
			if(p_gp->raw_d[j][i] < min_d[i]){
				min_d[i] = p_gp->raw_d[j][i];
			}
		}
		
		for(i=0;i<4;i++){
			avg_d[i] = avg_d[i] - max_d[i] - min_d[i];
			avg_d[i] = avg_d[i]/(p_gp->acquisition_num-2);
			
			//// If data variation is large, offset data doesn't update and uses the former data.
			if( (max_d[i] < avg_d[i] + p_gp->allowable_variation) && 
				(min_d[i] + p_gp->allowable_variation > avg_d[i])){
					p_gp->aoc_d[i] = avg_d[i] + p_gp->allowable_variation;
				}
		}
		num_counts = 0;
	//// The first measurement of offset
	}else if(num_counts == 0){
		for(i=0;i<4;i++){
			avg_d[i] = p_gp->raw_d[j][i];
			max_d[i] = p_gp->raw_d[j][i];
			min_d[i] = p_gp->raw_d[j][i];
		}
		num_counts++;
	}else{
		for(i=0;i<4;i++){
			avg_d[i] = avg_d[i] + p_gp->raw_d[j][i];;
			if(p_gp->raw_d[j][i] > max_d[i]){
				max_d[i] = p_gp->raw_d[j][i];;
			}
			if(p_gp->raw_d[j][i] < min_d[i]){
				min_d[i] = p_gp->raw_d[j][i];;
			}
		}
		num_counts++;
	}
	return;
}
/*******************************************************************
 * Name	: getActiveOffset(Input1, Output, Input2)   得到动态位移的结果
 * Input	: Input1 = p_gp->raw_d[5], Input2 = struct GP2AP054A_params pointer
 * Output	: p_gp->aoc_d[4]
 * Note	: to get offset value from acquisition times of raw data.     从捕获原始数据的次数来获取位移的值
 *******************************************************************/
void getActiveOffset(struct GP2AP054A_params *p_gp, int j){
	int i;
	
	if((p_gp->raw_d[j][4] <= p_gp->max_aoc) && (p_gp->raw_d[j][4] >= p_gp->min_aoc)){
		getOffsetData(p_gp, j);
	}
	for(i=0;i<4;i++){
		if(p_gp->raw_d[j][i] > p_gp->aoc_d[i]){
			p_gp->sub_d[i] = p_gp->raw_d[j][i] - p_gp->aoc_d[i];
		}else{
			p_gp->sub_d[i] = 0;
		}
	}
	p_gp->sub_d[4] = 0;
	for(i=0;i<4;i++){
		p_gp->sub_d[4] += p_gp->sub_d[i];
	}
	return;
}



/*******************************************************************
 * Name	: getDirection(Input1, Input2)  得到方向
 * Input	: Input1 = sub_os_data[4], Input2 = struct GP2AP054A_params pointer
 * Output	: Output return value is unsigned 16bits integer = 0x0ZSD
            Z(4bits from the higer 5th to 8th bit) ; Zoom results
            S(4bits from the lower 5th to 8th bit) ; Speed results
            D(lower 4bits) ; Direction results
 * Note	: get the result of Direction or Zoom 得到方向或变焦的结果
 *******************************************************************/
void getDirection(struct GP2AP054A_params *p_gp){
	s16 data_x = 0, data_y = 0;
	float temp_ratio = 0;
	float ratio_x = 0, ratio_y = 0;

	p_gp->res_gs = 0;
	//// Diff calculation ////
	data_y = p_gp->sub_d[2] + p_gp->sub_d[3] - p_gp->sub_d[0] - p_gp->sub_d[1];
	if( ((data_y > -(p_gp->ignore_diff_th)) && (data_y < (p_gp->ignore_diff_th))) ||
		(p_gp->sub_d[0] == OVER_FLOW_DATA) || (p_gp->sub_d[1] == OVER_FLOW_DATA) ||
		(p_gp->sub_d[2] == OVER_FLOW_DATA) || (p_gp->sub_d[3] == OVER_FLOW_DATA) )
	{
		data_y = 0;
	}
	data_x = p_gp->sub_d[1] + p_gp->sub_d[2] - p_gp->sub_d[0] - p_gp->sub_d[3];
	if( ((data_x > -(p_gp->ignore_diff_th)) && (data_x < (p_gp->ignore_diff_th))) ||
		(p_gp->sub_d[0] == OVER_FLOW_DATA) || (p_gp->sub_d[1] == OVER_FLOW_DATA) ||
		(p_gp->sub_d[2] == OVER_FLOW_DATA) || (p_gp->sub_d[3] == OVER_FLOW_DATA) )
	{
		data_x = 0;
	}
	
	//// Ratio calculation ////
	if(p_gp->sub_d[4] < p_gp->ignore_z_th){
		temp_ratio = 0;
	}else{
		temp_ratio = ((float)data_y / (float)p_gp->sub_d[4]);
	}
	ratio_y = temp_ratio;
	if(p_gp->sub_d[4] < p_gp->ignore_z_th){
		temp_ratio = 0;
	}else{
		temp_ratio = ((float)data_x / (float)p_gp->sub_d[4]);
	}
	ratio_x = temp_ratio;

	//// Judgement FSM start ////
	switch (p_gp->gs_state)
	{
		case STATE_ONE:
			if(p_gp->sub_d[4] >= p_gp->ignore_z_th){
				if(ratio_x > (p_gp->ratio_th)){
					p_gp->x_plus = 1;
					p_gp->max_x1 = ratio_x;
				}else{
					p_gp->x_plus = 0;
					p_gp->max_x1 = 0;
				}
					
				if(ratio_x < -(p_gp->ratio_th)){
					p_gp->x_minus = 1 ;
					p_gp->min_x1 = ratio_x;
				}else{
					p_gp->x_minus = 0;
					p_gp->min_x1 = 0;
				}
				
				if(ratio_y > (p_gp->ratio_th)){
					p_gp->y_plus = 1;
					p_gp->max_y1 = ratio_y;
				}else{
					p_gp->y_plus = 0;
					p_gp->max_y1 = 0;
				}
				
				if(ratio_y < -(p_gp->ratio_th)){
					p_gp->y_minus = 1;
					p_gp->min_y1 = ratio_y;
				}else{
					p_gp->y_minus = 0;
					p_gp->min_y1 = 0;
				}
			}

			if( (p_gp->x_plus > 0) || (p_gp->x_minus > 0) ||
				(p_gp->y_plus > 0) || (p_gp->y_minus > 0) )
			{
				p_gp->gs_state = STATE_TWO;
			}else{
				p_gp->gs_state = STATE_ONE;
			}
			
		break;
		
		case STATE_TWO:
			if( (p_gp->sub_d[4] < p_gp->ignore_z_th) )
			{
				clearGSparams(p_gp);
			}else if( 
		      ((p_gp->x_plus ) && (ratio_x < -(p_gp->ratio_th))) ||
		      ((p_gp->x_minus) && (ratio_x >  (p_gp->ratio_th))) ||
		      ((p_gp->y_plus ) && (ratio_y < -(p_gp->ratio_th))) ||
		      ((p_gp->y_minus) && (ratio_y >  (p_gp->ratio_th))) )
			{
				if(ratio_x > (p_gp->ratio_th)){
					p_gp->max_x2 = ratio_x;
				}else{
					p_gp->max_x2 = 0;
				}
			
				if(ratio_x < -(p_gp->ratio_th)){
					p_gp->min_x2 = ratio_x;
				}else{
					p_gp->min_x2 = 0;
				}
			
				if(ratio_y > (p_gp->ratio_th)){
					p_gp->max_y2 = ratio_y;
				}else{
					p_gp->max_y2 = 0;
				}
			
				if(ratio_y < -(p_gp->ratio_th)){
					p_gp->min_y2 = ratio_y;
				}else{
					p_gp->min_y2 = 0;
				}
				p_gp->gs_state = STATE_THREE;
			
			}else {
				if( (ratio_x > (p_gp->max_x1)) && (ratio_x > (p_gp->ratio_th))){
					p_gp->max_x1 = ratio_x;
					p_gp->x_plus = 1;
				}else if( (ratio_x < (p_gp->min_x1)) && (ratio_x < -(p_gp->ratio_th)) ){
					p_gp->min_x1 = ratio_x;
					p_gp->x_minus = 1;
				}
				if( (ratio_y > (p_gp->max_y1)) && (ratio_y > (p_gp->ratio_th)) ){
					p_gp->max_y1 = ratio_y;
					p_gp->y_plus = 1;
				}else if( (ratio_y < (p_gp->min_y1)) && (ratio_y < -(p_gp->ratio_th)) ){
					p_gp->min_y1 = ratio_y;
					p_gp->y_minus =1;
				}
				if( p_gp->x_plus && p_gp->x_minus){
					if((p_gp->max_x1) > -(p_gp->min_x1)) {
						p_gp->x_plus  = 1;
						p_gp->x_minus = 0;
					}else {		 
						p_gp->x_plus  = 0;
						p_gp->x_minus = 1;
					}
				}
				if( p_gp->y_plus && p_gp->y_minus){
					if((p_gp->max_y1) > -(p_gp->min_y1)) {
						p_gp->y_plus  = 1;
						p_gp->y_minus = 0;
					}else {
						p_gp->y_plus  = 0;
						p_gp->y_minus = 1;
					}
				}
				p_gp->gs_state = STATE_TWO;
			}
		break;
		
		case STATE_THREE:
			if( p_gp->sub_d[4] < (p_gp->ignore_z_th) )
			{
				if( (p_gp->x_plus) && (p_gp->min_x2 < -(p_gp->ratio_th))){
					p_gp->diff_max_x = p_gp->max_x1 - p_gp->min_x2;
				}else if( (p_gp->x_minus) && (p_gp->max_x2 > p_gp->ratio_th) ){
					p_gp->diff_max_x = p_gp->max_x2 - p_gp->min_x1;
				}else {
					p_gp->diff_max_x = 0;
				}
				
				if( (p_gp->y_plus) && (p_gp->min_y2 < -(p_gp->ratio_th)) ){
					p_gp->diff_max_y = p_gp->max_y1 - p_gp->min_y2;
				}else if( (p_gp->y_minus) && (p_gp->max_y2 > p_gp->ratio_th) ){
					p_gp->diff_max_y = p_gp->max_y2 - p_gp->min_y1;
				}else{
					p_gp->diff_max_y = 0;
				}
				
				//// Final direction Judgement ////
				if( p_gp->diff_max_x >= p_gp->diff_max_y){
					if(p_gp->x_plus == 1){
						p_gp->res_gs = DIR_RIGHT;
					}else {
						p_gp->res_gs = DIR_LEFT;
					}
				}else{
					if(p_gp->y_plus == 1){
						p_gp->res_gs = DIR_TOP;
					}else {
						p_gp->res_gs = DIR_BOTTOM;
					}
				}
				
				if(p_gp->speed_counts < (u16)COUNTS_HIGH_SP){
					p_gp->res_gs |=((u16)SPEED_HIGH<<4);
				}else if(p_gp->speed_counts < (u16)COUNTS_MID_SP){
					p_gp->res_gs |=((u16)SPEED_MID<<4);
				}
				clearGSparams(p_gp);
			
			}else {
				if( (ratio_x > p_gp->max_x2) && (ratio_x > p_gp->ratio_th) ){
					p_gp->max_x2 = ratio_x;
				}else if ( (ratio_x < (p_gp->min_x2)) && (ratio_x < -(p_gp->ratio_th))){
					p_gp->min_x2 = ratio_x;
				}
				if( (ratio_y > (p_gp->max_y2)) && (ratio_y > (p_gp->ratio_th)) ){
					p_gp->max_y2 = ratio_y;
				}else if( (ratio_y < (p_gp->min_y2)) && (ratio_y < -(p_gp->ratio_th))){
					p_gp->min_y2 = ratio_y;
				}
				p_gp->gs_state = STATE_THREE;
			}
			
		break;
		default:
		break;
	}

	//// Speed Judgement counts////
	if(p_gp->gs_state > STATE_ONE){
		p_gp->speed_counts++;
	}else{
		p_gp->speed_counts = 0;
	}
	return;
}// End of getDirection()

/*******************************************************************
 * Name	: getZoom(Input1, Input2, Input3)                  得到变焦距离的结果
 * Input	: Input1 = raw_data, Input1 = aoc_data, Input3 = struct GP2AP054A_params pointer
 * Output	: Output return value is unsigned 16bits integer = 0x0Z00
            Z(4bits from the higer 5th to 8th bit) ; Zoom results
 * Note	: get the result of Zoom                             得到变焦距离的结果
 *******************************************************************/
void getZoom(struct GP2AP054A_params *p_gp, int j){
	int i;
	const u16 zoom_z_th[5] = {
		1000,
		2000,
		4000,
	    6000,
		8000
	};
	static int now_level=ZOOM_LEVEL_DEF, temp_level=ZOOM_LEVEL_DEF, prev_level=ZOOM_LEVEL_DEF;
	static int to_zoom_counts = 0, out_zoom_counts=0, zoom_mode_counts=0;
	u16 to_zoom_th, out_zoom_th, zoom_mode_th;
	to_zoom_th = (u16)(p_gp->to_zoom_th);
	out_zoom_th = (u16)(p_gp->out_zoom_th);
	zoom_mode_th = (u16)(p_gp->zoom_mode_th);

	if( (p_gp->zoomModeNow==0) && (to_zoom_counts==0)){
		p_gp->zoom_aoc[4] = 0;
		for(i=0;i<4;i++){
			p_gp->zoom_aoc[i] =  p_gp->aoc_d[i];
			p_gp->zoom_aoc[4] += p_gp->zoom_aoc[i];
		}
	}

	for(i=0;i<5;i++){
		if(p_gp->raw_d[j][i] > p_gp->zoom_aoc[i]){
			p_gp->zoom_d[i] = p_gp->raw_d[j][i] - p_gp->zoom_aoc[i];
		}else{
			p_gp->zoom_d[i] = 0;
		}
	}
	
	if(p_gp->zoomModeNow){
		clearGSparams(p_gp);
		if(p_gp->zoom_d[4] > zoom_z_th[4]){
			temp_level = ZOOM_LEVEL5;
		}else if(p_gp->zoom_d[4] > zoom_z_th[3]){
			temp_level = ZOOM_LEVEL4;
		}else if(p_gp->zoom_d[4] > zoom_z_th[2]){
			temp_level = ZOOM_LEVEL3;
		}else if(p_gp->zoom_d[4] > zoom_z_th[1]){
			temp_level = ZOOM_LEVEL2;
		}else if(p_gp->zoom_d[4] > zoom_z_th[0]){
			temp_level = ZOOM_LEVEL1;
		}else{
			temp_level = 0;
		}

		if((now_level!=temp_level) && (temp_level!=0) && 
			(temp_level==prev_level)){
			zoom_mode_counts++;
		}else{
			zoom_mode_counts = 0;
		}

		if(zoom_mode_counts >= zoom_mode_th){
			if(zoom_mode_counts >= zoom_mode_th){
				now_level = temp_level;
				zoom_mode_counts = 0;
			}
			p_gp->res_gs = ((u16)now_level)<<8;
		}

		prev_level = temp_level;
		if( (temp_level==0) &&
			(out_zoom_counts >= out_zoom_th)){
			out_zoom_counts=0;
			zoom_mode_counts = 0;
			p_gp->zoomModeNow = 0;
		}else if(temp_level==0){
			out_zoom_counts++;
		}else{
			out_zoom_counts=0;
		}
	}else{
		if( (p_gp->zoom_d[4] > zoom_z_th[0]) &&
			(to_zoom_counts >= to_zoom_th)){
			if(p_gp->zoom_d[4] > zoom_z_th[4]){
				now_level = ZOOM_LEVEL5;
			}else if(p_gp->zoom_d[4] > zoom_z_th[3]){
				now_level = ZOOM_LEVEL4;
			}else if(p_gp->zoom_d[4] > zoom_z_th[2]){
				now_level = ZOOM_LEVEL3;
			}else if(p_gp->zoom_d[4] > zoom_z_th[1]){
				now_level = ZOOM_LEVEL2;
			}else if(p_gp->zoom_d[4] > zoom_z_th[0]){
				now_level = ZOOM_LEVEL1;
			}
			p_gp->res_gs = ((u16)now_level)<<8;
			p_gp->zoomModeNow = 1;
			to_zoom_counts = 0;
			clearGSparams(p_gp);
		}else if((p_gp->zoom_d[4] > zoom_z_th[0])){
			to_zoom_counts++;
		}else{
			to_zoom_counts = 0;
		}
	}
	return ;
}




/*******************************************************************
 * Name	: output_gs_debug1(Input)   输出调试
 * Input	: Input = getDirection() results
 * Output	: 
 * Note	: display the direction results on the terminal window  在终端窗口显示方向的结果
 *******************************************************************/
void output_GSresult(u16 res){
	
	switch(res & 0x0FFF){
		case (DIR_RIGHT|((u16)SPEED_HIGH<<4)):
			printf("RF");	/* RIGHT FAST_SPEED */
		break;
		case (DIR_RIGHT|((u16)SPEED_MID<<4)):
			printf("RM");
		break;
		case (DIR_RIGHT):
			printf("RS");
		break;
		case (DIR_LEFT|((u16)SPEED_HIGH<<4)):
			printf("LF");
		break;
		case (DIR_LEFT|((u16)SPEED_MID<<4)):
			printf("LM");	/* LEFT MIDDLE_SPEED */
		break;						
		case (DIR_LEFT):
			printf("LS");
		break;
		case (DIR_TOP|((u16)SPEED_HIGH<<4)):
			printf("TF");	
		break;
		case (DIR_TOP|((u16)SPEED_MID<<4)):
			printf("TM");
		break;						
		case (DIR_TOP):
			printf("TS");	/* TOP SLOW_SPEED */
		break;
		case (DIR_BOTTOM|((u16)SPEED_HIGH<<4)):
			printf("BF");	/* BOTTOM FAST_SPEED */
		break;
		case (DIR_BOTTOM|((u16)SPEED_MID<<4)):
			printf("BM");
		break;						
		case (DIR_BOTTOM):
			printf("BS");
		break;

		case ((u16)ZOOM_LEVEL5<<8):
			printf("Z5");	/* ZOOM_LEVEL5 */
		break;
		case ((u16)ZOOM_LEVEL4<<8):
			printf("Z4");
		break;
		case ((u16)ZOOM_LEVEL3<<8):
			printf("Z3");
		break;
		case ((u16)ZOOM_LEVEL2<<8):
			printf("Z2");
		break;
		case ((u16)ZOOM_LEVEL1<<8):
			printf("Z1");	/* ZOOM_LEVEL1 */
		break;
		
		default:
		break;
	}
	//printf("\r\n");
	return;
}



	
		

 
int main(void)
{		int rbuf[32];  //存储寄存器数据的数组
 
	  int sh_054a_int;
		int k,i,j;
	 	/* Initialize 054A params *///初始化结构体  ///////////////////必须放在程序的前面定义
	   struct GP2AP054A_params st_gp ;
	   initGP2AP054Aparams(&st_gp);
	/////////////////////////////////////////////////////////////////必须放在程序的前面定义
	
	
	
	  sh_054a_int=1;  //设置中断

	
  	Stm32_Clock_Init(9);	//系统时钟设置
  	uart_init(72,115200);	 	//串口初始化为9600
	  delay_init(72);	   	 	//延时初始化 
	  LED_Init();		  		//初始化与LED连接的硬件接口

	  KEY_Init();				//按键初始化		 	
 
    IIC_Init();
	
		delay_ms(100);   //手势模块开机必须等待稳定才能读数否则出错
		
	 printf("Z1");	/* ZOOM_LEVEL1 */

	while(1)
	{  	//delay_ms(500);   //手势模块开机必须等待稳定才能读数否则出错
		 sh_054a_sensor_init();
		 sh_054a_sensor_read(rbuf, 32);
	
		
		//for(i=0; i<32; i++)
    //{
       //printf(",");
       //printf("%d",rbuf[i]);
			
    //}
		 //printf("NA\r\n");
     
		
				/******************************************************
		 * Read data and Determine the Directions and Zoom_level  读取数据并确定方向和变焦层级
		 ******************************************************/
		if(sh_054a_int){
			/* Init_Read_data */
			for(k=0;k<4;k++){
				st_gp.raw_d[k][4] = 0;
				st_gp.sat_flag[k][4] = 0;
			}
			/* Read_data REG_PRE3_D0_LSB to REG_D3_MSB */
					sh_054a_sensor_read(rbuf, 32);
			for(i=0;i<4;i++){
				st_gp.raw_d[0][i]		= (u16)rbuf[2*i] + (((u16)(rbuf[2*i+1] & 0x7F))<<8);		/* pre3_data */
				st_gp.raw_d[0][4]		+= st_gp.raw_d[0][i];
				st_gp.sat_flag[0][i]	= rbuf[2*i+1]>>7;
				st_gp.sat_flag[0][4]	|= st_gp.sat_flag[0][i];
				
				st_gp.raw_d[1][i]		= (u16)rbuf[2*i+8] + (((u16)(rbuf[2*i+9] & 0x7F))<<8);	/* pre2_data */
				st_gp.raw_d[1][4]		+= st_gp.raw_d[1][i];
				st_gp.sat_flag[1][i]	= rbuf[2*i+9]>>7;
				st_gp.sat_flag[1][4]	|= st_gp.sat_flag[1][i];
				
				st_gp.raw_d[2][i]		= (u16)rbuf[2*i+16] + (((u16)(rbuf[2*i+17] & 0x7F))<<8);	/* pre1_data */
				st_gp.raw_d[2][4]		+= st_gp.raw_d[2][i];
				st_gp.sat_flag[2][i]	= rbuf[2*i+17]>>7;
				st_gp.sat_flag[2][4]	|= st_gp.sat_flag[2][i];
				
				st_gp.raw_d[3][i]		= (u16)rbuf[2*i+24] + (((u16)(rbuf[2*i+25] & 0x7F))<<8);	/* now_data */
				st_gp.raw_d[3][4]		+= st_gp.raw_d[3][i];
				st_gp.sat_flag[3][i]	= rbuf[2*i+25]>>7;
				st_gp.sat_flag[3][4]	|= st_gp.sat_flag[3][i];
			}
			for(j=0; j<=3; j++){
				/* ActiveOffsetCancel */
				getActiveOffset(&st_gp, j);
				/* DetermineDirection */
				getDirection(&st_gp);
				/* DetermineZoomLevel */
				// getZoom(&st_gp, j); // Remove the comment out if you use ZOOM FUNCTION
				for(i=0;i<4;i++){
					/* Output_rawdata */
					//printf("%u,", st_gp.raw_d[j][i]);
					/* Output_data_after_ActiveOffsetCancel */
					//printf("%Lu,", st_gp.sub_d[i]);
				}
				/* Output Direction_and_Speed or ZoomLevel*/
				output_GSresult(st_gp.res_gs);
			}
			//sh_054a_int = 0;
			//enable_interrupts(INT_EXT_H2L);
		}
		
	}
}







