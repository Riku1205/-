#define _CRT_SECURE_NO_WARNINGS

#include "stdio.h"
#include"string.h"
//exitを使う際に必要
#include "stdlib.h"

  //構造体を宣言(構造体に必要な項目)
typedef struct _images
{
	unsigned char r[1000][1000];
	unsigned char g[1000][1000];
	unsigned char b[1000][1000];
	char path[1000];
	struct _images* next;
}image;


//画像サイズを定義する
#define X_SIZE 640
#define Y_SIZE 480

//入力のRGB値を用意
unsigned char r_in[Y_SIZE][X_SIZE], g_in[Y_SIZE][X_SIZE], b_in[Y_SIZE][X_SIZE];

//画像の出力先の配列
unsigned char out_imgr[Y_SIZE][X_SIZE];
unsigned char out_imgg[Y_SIZE][X_SIZE];
unsigned char out_imgb[Y_SIZE][X_SIZE];
//グレイスケール用配列
unsigned char out_img[Y_SIZE][X_SIZE];

//関数のプロトタイプを宣言する
void read_img(unsigned char r_in[Y_SIZE][X_SIZE], unsigned char g_in[Y_SIZE][X_SIZE], unsigned char b_in[Y_SIZE][X_SIZE]);
void gray_img(unsigned char r[Y_SIZE][X_SIZE], unsigned char g[Y_SIZE][X_SIZE], unsigned char b[Y_SIZE][X_SIZE], unsigned char img[Y_SIZE][X_SIZE], image* wp);
void save_img(unsigned char imgr[Y_SIZE][X_SIZE], unsigned char imgg[Y_SIZE][X_SIZE], unsigned char imgb[Y_SIZE][X_SIZE], image* wp);
void inversionWide_img(unsigned char imgr[Y_SIZE][X_SIZE], unsigned char imgg[Y_SIZE][X_SIZE], unsigned char imgb[Y_SIZE][X_SIZE], image* wp);
void inversionHigh_img(unsigned char imgr[Y_SIZE][X_SIZE], unsigned char imgg[Y_SIZE][X_SIZE], unsigned char imgb[Y_SIZE][X_SIZE], image* wp);
void inversionColor_img(unsigned char imgr[Y_SIZE][X_SIZE], unsigned char imgg[Y_SIZE][X_SIZE], unsigned char imgb[Y_SIZE][X_SIZE], image* wp);
//グレイスケール用関数
void save_img2(unsigned char img[Y_SIZE][X_SIZE], image* wp);


//構造体変数1個目を宣言
image* jtoi = (image*)malloc(sizeof(image));
image* wp = jtoi, * start = jtoi;


//メインとなる関数
void main(void)
{
	//入力画像の読み込み
	read_img(r_in, g_in, b_in);

	char cmd[10];
	int flg = 0;


	while (1)
	{
		printf("処理コマンドを選択\n");
		scanf("%s", cmd);
		if (strcmp(cmd, "exit") == 0)
		{
			printf("終了");
			break;
		}
		wp = start;
		while (wp != NULL)
		{
			if (strcmp(cmd, "gray") == 0)
			{
				//グレイスケール化
				gray_img(r_in, g_in, b_in, out_img, wp);
			}
			else if (strcmp(cmd, "winv") == 0)
			{
				//画像横反転
				inversionWide_img(out_imgr, out_imgg, out_imgb, wp);
			}
			else if (strcmp(cmd, "hinv") == 0)
			{
				//画像縦反転
				inversionHigh_img(out_imgr, out_imgg, out_imgb, wp);
			}
			else if (strcmp(cmd, "cinv") == 0)
			{
				//色反転
				inversionColor_img(out_imgr, out_imgg, out_imgb, wp);
			}
			else if (strcmp(cmd, "/") == 0)
			{
				printf("コマンド一覧\nグレイスケール：gray\n横反転：winv\n縦反転：hinv\n\n");
				break;
			}
			else
			{
				printf("コマンドエラー\n正しいコマンドを入力してください");
				break;
			}
			if (strcmp(cmd, "gray") == 0)
			{
				//出力画像の保存
				save_img2(out_img, wp);
			}
			else
			{ 	//出力画像の保存
				save_img(out_imgr, out_imgg, out_imgb, wp);
			}
			printf("保存完了");
			wp = wp->next;
		}
	}
}

//入力画像の読み込み
void read_img(unsigned char r[Y_SIZE][X_SIZE], unsigned char g[Y_SIZE][X_SIZE], unsigned char b[Y_SIZE][X_SIZE])
{
	int i, j;
	int flg2 = 0;
	while (1)
	{
		char read_path[1000];

		//ファイルためのファイルを用意
		printf("読み込むファイル-->");
		//標準入力をバッファに格納する
		scanf("%s", read_path);
		if (strcmp(read_path, "exit") == 0)
		{
			break;
		}

		if (flg2 != 0)
		{
			jtoi = (image*)malloc(sizeof(image));
			wp->next = jtoi;
			wp = wp->next;
		}
		else flg2 = 1;

		FILE* fp;

		//ファイルを開く
		fp = fopen(read_path, "rb");

		//もし，ファイルが無いときに終了させる
		if (fp == NULL)
		{
			printf("ファイルが存在しません");
			exit(-1);
		}

		//パスの記憶
		strcpy(jtoi->path, read_path);

		//画像内からRGB値の取得（ラスタ走査）
		for (j = 0; j < Y_SIZE; j++)
		{
			for (i = 0; i < X_SIZE; i++)
			{
				jtoi->r[j][i] = fgetc(fp);
				jtoi->g[j][i] = fgetc(fp);
				jtoi->b[j][i] = fgetc(fp);
			}
		}
		wp->next = NULL;
		//ファイルを閉じる
		fclose(fp);
	}
}


//グレイスケール用　出力画像の保存
void save_img2(unsigned char img[Y_SIZE][X_SIZE], image* wp)
{
	int i, j;

	FILE* fp;


	//ファイルを開く
	fp = fopen(wp->path, "wb");

	////もし，ファイルが無いときに終了させる
	if (fp == NULL)
	{
		printf("ファイルが存在しません", wp->path);
		exit(-1);
	}
	//画像のRGB値をファイルに保存する
	for (j = 0; j < Y_SIZE; j++)
	{
		for (i = 0; i < X_SIZE; i++)
		{
			fputc(img[j][i], fp);
		}
	}
	//ファイルを閉じる
	fclose(fp);
}

//出力画像の保存
void save_img(unsigned char imgr[Y_SIZE][X_SIZE], unsigned char imgg[Y_SIZE][X_SIZE], unsigned char imgb[Y_SIZE][X_SIZE], image* wp)
{
	int i, j;

	FILE* fp;




	//ファイルを開く
	fp = fopen(wp->path, "wb");

	////もし，ファイルが無いときに終了させる
	if (fp == NULL)
	{
		printf("ファイルが存在しません", wp->path);
		exit(-1);
	}
	//画像のRGB値をファイルに保存する
	for (j = 0; j < Y_SIZE; j++)
	{
		for (i = 0; i < X_SIZE; i++)
		{
			fputc(imgr[j][i], fp);
			fputc(imgg[j][i], fp);
			fputc(imgb[j][i], fp);
		}
	}
	//ファイルを閉じる
	fclose(fp);
}

//画像処理関数

//グレイ化
void gray_img(unsigned char r[Y_SIZE][X_SIZE], unsigned char g[Y_SIZE][X_SIZE], unsigned char b[Y_SIZE][X_SIZE], unsigned char img[Y_SIZE][X_SIZE], image* wp)
{
	int i, j;

	for (j = 0; j < Y_SIZE; j++)
	{
		for (i = 0; i < X_SIZE; i++)
		{
			img[j][i] = (unsigned char)(0.30 * jtoi->r[j][i] + 0.59 * jtoi->g[j][i] + 0.11 * jtoi->b[j][i]);
		}
	}
}

//画像横反転
void inversionWide_img(unsigned char imgr[Y_SIZE][X_SIZE], unsigned char imgg[Y_SIZE][X_SIZE], unsigned char imgb[Y_SIZE][X_SIZE], image* wp)
{
	int i, j;
	unsigned char tmp;
	//
	for (j = 0; j < Y_SIZE; j++)
	{
		for (i = 0; i < X_SIZE; i++)
		{
			imgr[j][i] = (jtoi->r[j][i]);
			imgg[j][i] = (jtoi->g[j][i]);
			imgb[j][i] = (jtoi->b[j][i]);

		}
	}
	for (j = 0; j < Y_SIZE; j++)
	{
		for (i = 0; i < X_SIZE / 2; i++)
		{
			//各r,g,b値を入れ替え処理
			tmp = imgr[j][i];
			imgr[j][i] = imgr[j][X_SIZE - i - 1];
			imgr[j][X_SIZE - i - 1] = tmp;

			tmp = imgg[j][i];
			imgg[j][i] = imgg[j][X_SIZE - i - 1];
			imgg[j][X_SIZE - i - 1] = tmp;


			tmp = imgb[j][i];
			imgb[j][i] = imgb[j][X_SIZE - i - 1];
			imgb[j][X_SIZE - i - 1] = tmp;
		}
	}
	//構造体へ格納
	for (j = 0; j < Y_SIZE; j++)
	{
		for (i = 0; i < X_SIZE; i++)
		{
			jtoi->r[j][i] = imgr[j][i];
			jtoi->g[j][i] = imgg[j][i];
			jtoi->b[j][i] = imgb[j][i];
		}
	}
}
//画像縦反転
void inversionHigh_img(unsigned char imgr[Y_SIZE][X_SIZE], unsigned char imgg[Y_SIZE][X_SIZE], unsigned char imgb[Y_SIZE][X_SIZE], image* wp)
{
	int i, j;
	unsigned char tmp;
	for (j = 0; j < Y_SIZE; j++)
	{
		for (i = 0; i < X_SIZE; i++)
		{
			imgr[j][i] = (jtoi->r[j][i]);
			imgg[j][i] = (jtoi->g[j][i]);
			imgb[j][i] = (jtoi->b[j][i]);

		}
	}
	for (j = 0; j < Y_SIZE / 2; j++)
	{
		for (i = 0; i < X_SIZE; i++)
		{
			//各r,g,b値を入れ替え処理
			tmp = imgr[j][i];
			imgr[j][i] = imgr[Y_SIZE - j - 1][i];
			imgr[Y_SIZE - j - 1][i] = tmp;

			tmp = imgg[j][i];
			imgg[j][i] = imgg[Y_SIZE - j - 1][i];
			imgg[Y_SIZE - j - 1][i] = tmp;

			tmp = imgb[j][i];
			imgb[j][i] = imgb[Y_SIZE - j - 1][i];
			imgb[Y_SIZE - j - 1][i] = tmp;
		}
	}
	//構造体へ格納
	for (j = 0; j < Y_SIZE; j++)
	{
		for (i = 0; i < X_SIZE; i++)
		{
			jtoi->r[j][i] = imgr[j][i];
			jtoi->g[j][i] = imgg[j][i];
			jtoi->b[j][i] = imgb[j][i];
		}
	}
}

//色の反転
void inversionColor_img(unsigned char imgr[Y_SIZE][X_SIZE], unsigned char imgg[Y_SIZE][X_SIZE], unsigned char imgb[Y_SIZE][X_SIZE], image* wp)
{
	int i, j;
	unsigned char tmp;
	for (j = 0; j < Y_SIZE; j++)
	{
		for (i = 0; i < X_SIZE; i++)
		{
			imgr[j][i] = (jtoi->r[j][i]);
			imgg[j][i] = (jtoi->g[j][i]);
			imgb[j][i] = (jtoi->b[j][i]);

		}
	}
	for (j = 0; j < X_SIZE; j++)
	{
		for (i = 0; i < Y_SIZE; i++)
		{
			imgr[j][i] = 255 - imgr[j][i];
			imgg[j][i] = 255 - imgg[j][i];
			imgb[j][i] = 255 - imgb[j][i];
		}
	}
}

//画像拡大
