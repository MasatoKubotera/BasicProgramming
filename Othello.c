/*------------------------------------------------------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <drawlib.h>
#include <time.h>
#include <math.h>
/*------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define dl_Black dl_color_from_name("black")
#define dl_White dl_color_from_name("white")
#define dl_Red   dl_color_from_name("red")
/*------------------------------------------------------------------------------------------------------------------------------------------------------*/
//石配置
int sta[8][8];

//盤の大きさ
int y_frame = 0, under = DL_HEIGHT, x_frame = 160, x_right = 640;

//マス目の大きさ
int size = DL_HEIGHT / 8;

//石数(石表示時にカウント)
int white_num ,black_num;

//表示座標
int x, y;

//押座標リセット
int x_save = -1, y_save = -1;

//順番
int now_turn;

//枠線・中心線
int x_line[9],y_line[9], x_center_line[8], y_center_line[8];

 //パス・降参・リセット
int but_sta[3];

int but_x1, but_x2, rect_xsize, but_y1[3], but_y2[3];

//石の半径
int circle_size = 25;

//置くことが可能か
int change , possible;
/*------------------------------------------------------------------------------------------------------------------------------------------------------*/	
void circle(); //円表示

void setup(); //初期盤

void reversi();

void click();

void display();
/*------------------------------------------------------------------------------------------------------------------------------------------------------*/
int main(void){
/*======================================================================================================================================================*/
	//初期設定
	dl_initialize(1.5);
		
	for(int i = 0; i < 9; i++){
		x_line[i] = x_frame + size * i;
		y_line[i] = y_frame + size * i;
	}
	
	for(int i = 0; i < 8; i++){
		x_center_line[i] = x_frame + size / 2 + size * i;
		y_center_line[i] = y_frame + size / 2 + size * i;
	}
	
	//勝利パターン
	int win_case;
/*======================================================================================================================================================*/
	start:
/*======================================================================================================================================================*/

	//初期盤表示
	setup();
	display();
	
	//初手設定
	now_turn = -1;
	
	//ボタン状態初期化
	for(int i = 0; i < 3; i++) but_sta[i] = 0;
/*======================================================================================================================================================*/
	while(1){ //w1
	
		while(1){ //w2

			display();
			
			//終了条件
			if(white_num + black_num >= 64 || white_num * black_num == 0)
				goto end;
			
			//アクション状態取得
			click();
			
			
			//座標設定終了＆石が置ける状態
			if(x * y >= 0 && sta[y][x] == 0)
				break;
				
			//パス
			if(but_sta[0] != 0) now_turn *= -1;
			
			//降参
			if(but_sta[1] != 0){
				but_sta[1] = now_turn;
				goto end;
			}
				
			//リセット
			if(but_sta[2] != 0)
				goto start;
			
		} //w2
		
		
		//座標に石を配置
		sta[y][x] = now_turn;
		
		//リバーシ
		reversi();
		
		//次のターン
		if(possible == 1) now_turn *= -1;
		else sta[y][x] = 0;

	}//w1
	
	end:
	
	//降参
	if(but_sta[1] != 0){
		if(but_sta[1] > 0) 
			win_case = -1;
		if(but_sta[1] < 0) 
			win_case =  1;
	}
	else {
		if(white_num > black_num)	
			win_case = -1;
		else if(white_num < black_num)
			win_case =  1;
		else
			win_case =  0;
	}
	
	switch(win_case){
		case -1:
			dl_text( "White Win!!", 80, y_line[4], 3, dl_Red, 3); break;
		case  1:
			dl_text( "Black Win!!", 80, y_line[4], 3, dl_Red, 3); break;
		case  0:
			dl_text( "   Draw!   ", 80, y_line[4], 3, dl_Red, 3); break;
	}
	
	while(1){
	  //アクション状態を取得
		click();
		
		//リセット
		if(but_sta[2] != 0) goto start;
	}
	
	return 0;
}
/*------------------------------------------------------------------------------------------------------------------------------------------------------*/
void click(){
	//アクション変数
	int event_type, key, x_click, y_click;
	
	//押座標リセット
	x_save = -1, y_save = -1;
			
	while(x_save + y_save < -1) {

		//記録されているアクションを照会
		while (dl_get_event(&event_type, &key, &x_click, &y_click)){
						
			//押されたとき
			if(event_type == 3 || event_type == 5) {
						
				//押座標を更新
				x_save = x_click; y_save = y_click;
			}
		}
	}
			
	//表示座標のリセット
	x = -1; y = -1;
			
	//座標の設定
	for(int i = 0; i < 8; i++){
		if(x_save >= x_line[i] && x_save < x_line[i+1]) x = i;
		if(y_save >= y_line[i] && y_save < y_line[i+1]) y = i;
	}
				
	//ボタン
	for(int i = 0; i < 3; i++){
		if(x_save > but_x1 && x_save  < but_x2 && y_save > but_y1[i] && y_save < but_y2[i])
			but_sta[i] = 1;
		else
			but_sta[i] = 0; 
	}
}
/*------------------------------------------------------------------------------------------------------------------------------------------------------*/
//表示
void display(){

	//全消去
	dl_rectangle( 0, 0, DL_WIDTH, DL_HEIGHT, dl_White, 0, 1);
	
	//ボタン左右座標
	but_x1 = 10, but_x2 = x_frame - 10;
	rect_xsize = 20; // x/2
	for (int i = 0; i < 3; i++){
		but_y1[i] = y_line[i+5] - rect_xsize;
		but_y2[i] = y_line[i+5] + rect_xsize;
	}

	//ボタン枠表示
	for(int i = 0; i < 3; i++){
		dl_rectangle( but_x1, but_y1[i], but_x2, but_y2[i], dl_Black, 1, 0);
	}
			
	//パスボタン
	dl_text("Pass", 50, y_line[5]+8, 0.8, dl_Black, 1);

	//降参ボタン
	dl_text("Resignation", 16, y_line[6]+8, 0.7, dl_Black, 1);

	//リセットボタン
	dl_text("Reset", 47, y_line[7]+8, 0.7, dl_Black, 1);

	//盤表示
	dl_rectangle( x_frame , y_frame, x_right, under, DL_RGB( 25 ,115 ,50), 1, 1);
	dl_rectangle( x_frame , y_frame, x_right, under, dl_Black, 2, 0);
	
	//縦横線表示
	for(int i = 1; i < 8; i++){
		dl_line( x_line[i], y_frame, x_line[i], under, dl_Black, 1);
		dl_line( x_frame, y_line[i], x_right, y_line[i], dl_Black, 1);
	}
	
	//点表示
	dl_circle(x_line[2], y_line[2], 5, dl_Black, 1, 1);
	dl_circle(x_line[2], y_line[6], 5, dl_Black, 1, 1);
	dl_circle(x_line[6], y_line[2], 5, dl_Black, 1, 1);
	dl_circle(x_line[6], y_line[6], 5, dl_Black, 1, 1);
	
	//数リセット
	white_num = 0; black_num = 0;
	
	for(int i = 0; i < 8; i++){
		for(int j = 0; j < 8; j++){
			
			//白
			if(sta[i][j] < 0){
				white_num++; dl_circle(x_center_line[j], y_center_line[i], circle_size, dl_White, 1, 1);
			}
			
			//黒
			if(sta[i][j] > 0){
				black_num++; dl_circle(x_center_line[j], y_center_line[i], circle_size, dl_Black, 1, 1);
			}
		}
	}
	//表示文字列
	char str[9];

	//順番表示
	if(now_turn < 0) dl_circle( 10, y_line[1]-30, 5, dl_Black, 1, 1);
	if(now_turn > 0) dl_circle( 10, y_line[2]-30, 5, dl_Black, 1, 1);
	
	//石数表示
	snprintf( str, 9, "White %2d", white_num); dl_text( str, 20, y_line[1]-20, 1, dl_Black, 2);
	snprintf( str, 9, "Black %2d", black_num); dl_text( str, 20, y_line[2]-20, 1, dl_Black, 2);	
}
/*------------------------------------------------------------------------------------------------------------------------------------------------------*/
void circle(){	
	//数リセット
	white_num = 0; black_num = 0;
	
	for(int i = 0; i < 8; i++){
		for(int j = 0; j < 8; j++){
			
			//白
			if(sta[i][j] < 0){
				white_num++; dl_circle(x_center_line[j], y_center_line[i], circle_size, dl_White, 1, 1);
			}
			
			//黒
			if(sta[i][j] > 0){
				black_num++; dl_circle(x_center_line[j], y_center_line[i], circle_size, dl_Black, 1, 1);
			}
		}
	}
}
/*------------------------------------------------------------------------------------------------------------------------------------------------------*/
//初期盤
void setup(){
	//初期石配置
	for(int i = 0; i < 8; i++)
		for(int j = 0; j < 8; j++)
				sta[i][j]=0;
	sta[3][3] = -1; sta[4][4] = -1; sta[3][4] = 1; sta[4][3] = 1;
}
/*------------------------------------------------------------------------------------------------------------------------------------------------------*/
void reversi(){
	int i;
	
	possible = 0;
	
	change = 0;
	
	//右側
	if(x <= 5 && sta[y][x+1] + sta[y][x] == 0){
		for(i = 2, change = 1; sta[y][x+i] != sta[y][x]; change++, i++){
			if(x+i > 7 || sta[y][x+i] == 0) {
				change = -1; break;
			}
		}
		for(; change > 0; change--){
			sta[y][x + change] = sta[y][x]; possible = 1;
		}
	}
			
	//左側
	if(x >= 2 && sta[y][x-1] + sta[y][x] == 0){
		for(i = 2, change = 1; sta[y][x-i] != sta[y][x]; change++, i++){
			if(x-i < 0 || sta[y][x-i] == 0) {
				change = -1; break;
			}
		}
		for(; change > 0; change--){
			sta[y][x - change] = sta[y][x]; possible = 1;
		}
	}
			
	//下側
	if(y <= 5 && sta[y+1][x] + sta[y][x] == 0){
		for(i = 2, change = 1; sta[y+i][x] != sta[y][x]; change++, i++){
			if(y+i > 7 || sta[y+i][x] == 0) {
				change = -1; break;
			}
		}
		for(; change > 0; change--){
			sta[y + change][x] = sta[y][x]; possible = 1;
		}
	}	
			
	//上側
	if(y >= 2 && sta[y-1][x] + sta[y][x] == 0){
		for(i = 2, change = 1; sta[y-i][x] != sta[y][x]; change++, i++){
			if(y-i < 0 || sta[y-i][x] == 0) {
				change = -1; break;
			}
		}
		for(; change > 0; change--){
			sta[y - change][x] = sta[y][x]; possible = 1;
		}
	}
			
	//斜め右下
	if(x <= 5 && y <= 5 && sta[y+1][x+1] + sta[y][x] == 0){
		for(i = 2, change = 1; sta[y+i][x+i] != sta[y][x]; change++, i++){
			if(x+i > 7 || y+i > 7 || sta[y+i][x+i] == 0) {
				change = -1; break;
			}
		}
		for(; change > 0; change--){
			sta[y + change][x + change] = sta[y][x]; possible = 1;
		}
	}
			
	//斜め左上
	if(x >= 2 && y >= 2 && sta[y-1][x-1] + sta[y][x] == 0){
		for(i = 2, change = 1; sta[y-i][x-i] != sta[y][x]; change++, i++){
			if(x-i < 0 || y-i < 0 || sta[y-i][x-i] == 0) {
				change = -1; break;
			}
		}
		for(; change > 0; change--){
			sta[y - change][x - change] = sta[y][x]; possible = 1;
		}
	}

	//斜め右上
	if(x <= 5 && y >= 2 && sta[y-1][x+1] + sta[y][x] == 0){
		for(i = 2, change = 1; sta[y-i][x+i] != sta[y][x]; change++, i++){
			if(x+i > 7 || y-i < 0 || sta[y-i][x+i] == 0) {
				change = -1; break;
			}
		}
		for(; change > 0; change--){
			sta[y - change][x + change] = sta[y][x]; possible = 1;
		}
	}
			
	//斜め左下
	if(x >= 2 && y <= 5 && sta[y+1][x-1] + sta[y][x] == 0){
		for(i = 2, change = 1; sta[y+i][x-i] != sta[y][x]; change++, i++){
			if(x-i < 0 || y+i > 7 || sta[y+i][x-i] == 0) {
				change = -1; break;
			}
		}
		for(; change > 0; change--){
			sta[y + change][x - change] = sta[y][x]; possible = 1;
		}
	}
}
