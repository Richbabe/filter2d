#include <opencv2\opencv.hpp>
#include <iostream>    
#include <stdio.h> 
using namespace std;
using namespace cv;

Mat filter2d(Mat picture , int select) {

	//均值滤波器
	if (select == 1) {
		int div;
		cout << "输入均值滤波器的维数:";
		cin >> div;
		int **averageFilter = new int*[div];//均值滤波器
		for (int i = 0;i < div;i++) {
			averageFilter[i] = new int[div];
		}
		//将均值滤波器每个值初始化为1
		for (int i = 0;i < div;i++) {
			for (int j = 0;j < div;j++) {
				averageFilter[i][j] = 1;
			}
		}
		int row = picture.rows;//原图片行数
		int col = picture.cols;//原图片列数
		int **picPixel = new int*[row];//原图片像素矩阵
		for (int m = 0;m < row;m++) {
			picPixel[m] = new int[col];
		}
		//获取原图片每一点像素值
		for (int n = 0;n < row;n++) {
			for (int z = 0;z < col;z++) {
				picPixel[n][z] = int(picture.at<Vec3b>(n, z)[0]);
			}
		}
		int extendRow = row + div * 2 - 2;//补0后的矩阵行数
		int extendCol = col + div * 2 - 2;//补0后的矩阵列数
		int **extendMatrix = new int*[extendRow];//补0后的矩阵,且初始化为0
		for (int i = 0;i < extendRow;i++) {
			extendMatrix[i] = new int[extendCol];
		}
		//将原图片矩阵放入补0后的矩阵
		for (int i = 0;i < row;i++) {
			for (int j = 0;j < col;j++) {
				extendMatrix[i + div - 1][j + div - 1] = picPixel[i][j];
			}
		}
		//补边界,否则会出现边缘过亮
		for (int i = 0;i < extendRow;i++) {
			for (int j = 0;j < extendCol;j++) {
				if ((i < div - 1 && j <= col + div - 1) || (i <= row + div - 1 && j < div - 1)) {
					extendMatrix[i][j] = picPixel[0][0];
				}
				else if ((i > row + div - 1 && j <= col + div - 1) || j > col + div - 1) {
					extendMatrix[i][j] = picPixel[row - 1][col - 1];
				}
				continue;
			}
		}
		int a = (div - 1) / 2;
		int b = (div - 1) / 2;
		Mat newPicture = Mat(row, col, CV_8UC3);//目标图片
		//均值滤波
		for (int i = 0;i < row;i++) {
			for (int j = 0;j < col;j++) {
				int grey = 0;
				for (int k = -a;k <= a;k++) {
					for (int l = -b;l <= b;l++) {
						grey += extendMatrix[i + div - 1 + k][j + div - 1 + l] * averageFilter[k + a][l + b];//相关操作
					}
				}
				grey = grey / (div * div);
				if (i == 0) {
					cout << grey << endl;
				}	
				newPicture.at<Vec3b>(i, j) = Vec3b(grey, grey, grey);
			}
		}
		return newPicture;
		}
			
	//拉普拉斯滤波器
	if (select == 2) {
		int filter[3][3] = { 0,1,0,
						     1,-4,1,
							 0,1,0 };
		int row = picture.rows;//原图片行数
		int col = picture.cols;//原图片列数
		int **picPixel = new int*[row];//原图片像素矩阵
		for (int m = 0;m < row;m++) {
			picPixel[m] = new int[col];
		}
		//获取原图片每一点像素值
		for (int n = 0;n < row;n++) {
			for (int z = 0;z < col;z++) {
				picPixel[n][z] = int(picture.at<Vec3b>(n, z)[0]);
			}
		}
		int extendrow = row + 3 * 2 - 2;//补0后的矩阵行数
		int extendcol = col + 3 * 2 - 2;//补0后的矩阵列数
		int **extendMatrix = new int*[extendrow];//补0后的矩阵,且初始化为0
		for (int i = 0;i < extendrow;i++) {
			extendMatrix[i] = new int[extendcol];
		}
		//将原图片矩阵放入补0后的矩阵
		for (int i = 0;i < row;i++) {
			for (int j = 0;j < col;j++) {
				extendMatrix[i + 3 - 1][j + 3 - 1] = picPixel[i][j];
			}
		}
		//补边界,否则会出现边缘过亮
		for (int i = 0;i < extendrow;i++) {
			for (int j = 0;j < extendcol;j++) {
				if ((i < 3 - 1 && j <= col + 3 - 1) || (i <= row + 3 - 1 && j < 3 - 1)) {
					extendMatrix[i][j] = picPixel[0][0];
				}
				else if ((i > row + 3 - 1 && j <= col + 3 - 1) || j > col + 3 - 1) {
					extendMatrix[i][j] = picPixel[row - 1][col - 1];
				}
				continue;
			}
		}
		int a = (3 - 1) / 2;
		int b = (3 - 1) / 2;
		Mat newPicture = Mat(row, col, CV_8UC3);//目标图片
		//拉普拉斯滤波
		for (int i = 0;i < row;i++) {
			for (int j = 0;j < col;j++) {
				int grey = 0;
				for (int k = -a;k <= a;k++) {
					for (int l = -b;l <= b;l++) {
						grey += extendMatrix[i + 3 - 1 + k][j + 3 - 1 + l] * filter[k + a][l + b];
					}
				}
				grey = grey + picPixel[i][j];
				newPicture.at<Vec3b>(i, j) = Vec3b(grey, grey, grey);
			}
		}
		return newPicture;
		}

	//高提升滤波
	if (select == 3) {
		int div = 3;
		int **averageFilter = new int*[div];//均值滤波器
		for (int i = 0;i < div;i++) {
			averageFilter[i] = new int[div];
		}
		//将均值滤波器每个值初始化为1
		for (int i = 0;i < div;i++) {
			for (int j = 0;j < div;j++) {
				averageFilter[i][j] = 1;
			}
		}
		int row = picture.rows;//原图片行数
		int col = picture.cols;//原图片列数
		int **picPixel = new int*[row];//原图片像素矩阵
		for (int m = 0;m < row;m++) {
			picPixel[m] = new int[col];
		}
		//获取原图片每一点像素值
		for (int n = 0;n < row;n++) {
			for (int z = 0;z < col;z++) {
				picPixel[n][z] = int(picture.at<Vec3b>(n, z)[0]);
			}
		}
		int extendRow = row + div * 2 - 2;//补0后的矩阵行数
		int extendCol = col + div * 2 - 2;//补0后的矩阵列数
		int **extendMatrix = new int*[extendRow];//补0后的矩阵,且初始化为0
		for (int i = 0;i < extendRow;i++) {
			extendMatrix[i] = new int[extendCol];
		}
		//将原图片矩阵放入补0后的矩阵
		for (int i = 0;i < row;i++) {
			for (int j = 0;j < col;j++) {
				extendMatrix[i + div - 1][j + div - 1] = picPixel[i][j];
			}
		}
		//补边界,否则会出现边缘过亮
		for (int i = 0;i < extendRow;i++) {
			for (int j = 0;j < extendCol;j++) {
				if ((i < div - 1 && j <= col + div - 1) || (i <= row + div - 1 && j < div - 1)) {
					extendMatrix[i][j] = picPixel[0][0];
				}
				else if ((i > row + div - 1 && j <= col + div - 1) || j > col + div - 1) {
					extendMatrix[i][j] = picPixel[row - 1][col - 1];
				}
				continue;
			}
		}
		int a = (div - 1) / 2;
		int b = (div - 1) / 2;
		Mat newPicture = Mat(row, col, CV_8UC3);//目标图片
		//高提升滤波
		for (int i = 0;i < row;i++) {
			for (int j = 0;j < col;j++) {
				int grey = 0;
				for (int k = -a;k <= a;k++) {
					for (int l = -b;l <= b;l++) {
						grey += extendMatrix[i + div - 1 + k][j + div - 1 + l] * averageFilter[k + a][l + b];
					}
				}
				grey = grey / (div * div);//平滑后的图
				grey = picPixel[i][j] - grey;//得到细节gmask(x,y)
				grey = picPixel[i][j] + 2 * grey;//细节增强g(x,y)，k取2
				//防止越界
				if (grey > 255) {
					grey = 255;
				}
				if (grey < 0) {
					grey = 0;
				}
				newPicture.at<Vec3b>(i, j) = Vec3b(grey, grey, grey);
			}
		}
		return newPicture;
	}
}
int main() {
	Mat picture = imread("01.png");
	cout<<"Please enter 1---均值滤波器   2---拉普拉斯滤波器   3---高提升滤波：";
	int select;
	cin>>select;
	Mat newPicture = filter2d(picture,select);
	cout<<"Please enter the name of your target picture which will save in the root:";
	string name;
	cin>>name;
	imwrite(name+".png",newPicture);
	cout<<"save successfully!";
	imshow("newPicture", newPicture);
	waitKey(0);
}
