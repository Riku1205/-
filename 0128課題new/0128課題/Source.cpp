#define _CRT_SECURE_NO_WARNINGS

#include "stdio.h"
#include"string.h"
//exit���g���ۂɕK�v
#include "stdlib.h"

  //�\���̂�錾(�\���̂ɕK�v�ȍ���)
typedef struct _images
{
	unsigned char r[1000][1000];
	unsigned char g[1000][1000];
	unsigned char b[1000][1000];
	char path[1000];
	struct _images* next;
}image;


//�摜�T�C�Y���`����
#define X_SIZE 640
#define Y_SIZE 480

//���͂�RGB�l��p��
unsigned char r_in[Y_SIZE][X_SIZE], g_in[Y_SIZE][X_SIZE], b_in[Y_SIZE][X_SIZE];

//�摜�̏o�͐�̔z��
unsigned char out_imgr[Y_SIZE][X_SIZE];
unsigned char out_imgg[Y_SIZE][X_SIZE];
unsigned char out_imgb[Y_SIZE][X_SIZE];
//�O���C�X�P�[���p�z��
unsigned char out_img[Y_SIZE][X_SIZE];

//�֐��̃v���g�^�C�v��錾����
void read_img(unsigned char r_in[Y_SIZE][X_SIZE], unsigned char g_in[Y_SIZE][X_SIZE], unsigned char b_in[Y_SIZE][X_SIZE]);
void gray_img(unsigned char r[Y_SIZE][X_SIZE], unsigned char g[Y_SIZE][X_SIZE], unsigned char b[Y_SIZE][X_SIZE], unsigned char img[Y_SIZE][X_SIZE], image* wp);
void save_img(unsigned char imgr[Y_SIZE][X_SIZE], unsigned char imgg[Y_SIZE][X_SIZE], unsigned char imgb[Y_SIZE][X_SIZE], image* wp);
void inversionWide_img(unsigned char imgr[Y_SIZE][X_SIZE], unsigned char imgg[Y_SIZE][X_SIZE], unsigned char imgb[Y_SIZE][X_SIZE], image* wp);
void inversionHigh_img(unsigned char imgr[Y_SIZE][X_SIZE], unsigned char imgg[Y_SIZE][X_SIZE], unsigned char imgb[Y_SIZE][X_SIZE], image* wp);
void inversionColor_img(unsigned char imgr[Y_SIZE][X_SIZE], unsigned char imgg[Y_SIZE][X_SIZE], unsigned char imgb[Y_SIZE][X_SIZE], image* wp);
//�O���C�X�P�[���p�֐�
void save_img2(unsigned char img[Y_SIZE][X_SIZE], image* wp);


//�\���̕ϐ�1�ڂ�錾
image* jtoi = (image*)malloc(sizeof(image));
image* wp = jtoi, * start = jtoi;


//���C���ƂȂ�֐�
void main(void)
{
	//���͉摜�̓ǂݍ���
	read_img(r_in, g_in, b_in);

	char cmd[10];
	int flg = 0;


	while (1)
	{
		printf("�����R�}���h��I��\n");
		scanf("%s", cmd);
		if (strcmp(cmd, "exit") == 0)
		{
			printf("�I��");
			break;
		}
		wp = start;
		while (wp != NULL)
		{
			if (strcmp(cmd, "gray") == 0)
			{
				//�O���C�X�P�[����
				gray_img(r_in, g_in, b_in, out_img, wp);
			}
			else if (strcmp(cmd, "winv") == 0)
			{
				//�摜�����]
				inversionWide_img(out_imgr, out_imgg, out_imgb, wp);
			}
			else if (strcmp(cmd, "hinv") == 0)
			{
				//�摜�c���]
				inversionHigh_img(out_imgr, out_imgg, out_imgb, wp);
			}
			else if (strcmp(cmd, "cinv") == 0)
			{
				//�F���]
				inversionColor_img(out_imgr, out_imgg, out_imgb, wp);
			}
			else if (strcmp(cmd, "/") == 0)
			{
				printf("�R�}���h�ꗗ\n�O���C�X�P�[���Fgray\n�����]�Fwinv\n�c���]�Fhinv\n\n");
				break;
			}
			else
			{
				printf("�R�}���h�G���[\n�������R�}���h����͂��Ă�������");
				break;
			}
			if (strcmp(cmd, "gray") == 0)
			{
				//�o�͉摜�̕ۑ�
				save_img2(out_img, wp);
			}
			else
			{ 	//�o�͉摜�̕ۑ�
				save_img(out_imgr, out_imgg, out_imgb, wp);
			}
			printf("�ۑ�����");
			wp = wp->next;
		}
	}
}

//���͉摜�̓ǂݍ���
void read_img(unsigned char r[Y_SIZE][X_SIZE], unsigned char g[Y_SIZE][X_SIZE], unsigned char b[Y_SIZE][X_SIZE])
{
	int i, j;
	int flg2 = 0;
	while (1)
	{
		char read_path[1000];

		//�t�@�C�����߂̃t�@�C����p��
		printf("�ǂݍ��ރt�@�C��-->");
		//�W�����͂��o�b�t�@�Ɋi�[����
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

		//�t�@�C�����J��
		fp = fopen(read_path, "rb");

		//�����C�t�@�C���������Ƃ��ɏI��������
		if (fp == NULL)
		{
			printf("�t�@�C�������݂��܂���");
			exit(-1);
		}

		//�p�X�̋L��
		strcpy(jtoi->path, read_path);

		//�摜������RGB�l�̎擾�i���X�^�����j
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
		//�t�@�C�������
		fclose(fp);
	}
}


//�O���C�X�P�[���p�@�o�͉摜�̕ۑ�
void save_img2(unsigned char img[Y_SIZE][X_SIZE], image* wp)
{
	int i, j;

	FILE* fp;


	//�t�@�C�����J��
	fp = fopen(wp->path, "wb");

	////�����C�t�@�C���������Ƃ��ɏI��������
	if (fp == NULL)
	{
		printf("�t�@�C�������݂��܂���", wp->path);
		exit(-1);
	}
	//�摜��RGB�l���t�@�C���ɕۑ�����
	for (j = 0; j < Y_SIZE; j++)
	{
		for (i = 0; i < X_SIZE; i++)
		{
			fputc(img[j][i], fp);
		}
	}
	//�t�@�C�������
	fclose(fp);
}

//�o�͉摜�̕ۑ�
void save_img(unsigned char imgr[Y_SIZE][X_SIZE], unsigned char imgg[Y_SIZE][X_SIZE], unsigned char imgb[Y_SIZE][X_SIZE], image* wp)
{
	int i, j;

	FILE* fp;




	//�t�@�C�����J��
	fp = fopen(wp->path, "wb");

	////�����C�t�@�C���������Ƃ��ɏI��������
	if (fp == NULL)
	{
		printf("�t�@�C�������݂��܂���", wp->path);
		exit(-1);
	}
	//�摜��RGB�l���t�@�C���ɕۑ�����
	for (j = 0; j < Y_SIZE; j++)
	{
		for (i = 0; i < X_SIZE; i++)
		{
			fputc(imgr[j][i], fp);
			fputc(imgg[j][i], fp);
			fputc(imgb[j][i], fp);
		}
	}
	//�t�@�C�������
	fclose(fp);
}

//�摜�����֐�

//�O���C��
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

//�摜�����]
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
			//�er,g,b�l�����ւ�����
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
	//�\���̂֊i�[
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
//�摜�c���]
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
			//�er,g,b�l�����ւ�����
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
	//�\���̂֊i�[
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

//�F�̔��]
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

//�摜�g��
