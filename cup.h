/////////////////////////////////////////////////////////////////////////////

// this application is made by HeeSeungKIM in Soc Lab of KTNU university   //
/////////////////////////////////////////////////////////////////////////////

#ifndef CUP_H
#define CUP_H

#include<opencv2/opencv.hpp>
#include<iostream>
#include<raspicam/raspicam_cv.h>
using namespace cv;
using namespace std;

//kind of cups
#define CUP 0
#define MUGCUP 1

//cup shape
#define CIRCLE 0

//method of calculate center point 
#define ONCE 0
#define AVERAGE 1

#define HoughThreshValue 30
class Cup{

private:

	//variables

	int numberOfCircles;
	int method;
	int shape;
	int type;
	float radius;
	float area;
	
  	Point realCenter;

	char Scenter[30];
	char Sradius[30];
	char Sarea[30];
	char SdiffPixel[30];

	//function that not be offered
	inline void getMinCenter(vector<Point> vec);
	inline void getMinRadius(vector<float> vec);

public:
	int diffPixel;
	Point center;
	inline Cup();
	inline Cup(Mat src, int method = ONCE, int type = CUP, int shape=CIRCLE);
	inline ~Cup();
	inline bool calCircle(Mat& src,int param1,int param2);
	inline bool calCircle(Mat& src);
	inline bool avgCalCircle(Mat& src);
	inline void setMethod(int method);
	inline void setShape(int shape);
	inline void setType(int type);
	inline float getRadius();
	inline Point getCenter();
	inline float getArea();
	inline bool draw(Mat& image, Scalar center, Scalar circle);
	inline void showInfo(Mat& image);
	inline void showInfo();
	inline int length(Point pt1, Point pt2);

};

//Resource Part
inline Cup::Cup(){
    numberOfCircles=0;
	method=ONCE;
	shape=CIRCLE;
	type = CUP;
	radius=0.0;
	area = 0.0;
	center = Point(0, 0);
    realCenter = Point(98,67);
    diffPixel=0;

}
inline Cup::Cup(Mat src, int method,int type,int shape){
	if (method == ONCE){
		Cup::method = ONCE;
		if (!calCircle(src))
			printf("Cannot find a cup");
	}
	else if (method == AVERAGE){
		Cup::method = AVERAGE;
		if (!avgCalCircle(src))
			printf("Cannot find a cup");
	}
}
inline Cup::~Cup(){
}
inline bool Cup::calCircle(Mat& src){

	Mat image;
	int param1 = 200;
	cvtColor(src, image, CV_BGR2GRAY);


	while (1){

		vector<Vec3f> circles;
		HoughCircles(image, circles, HOUGH_GRADIENT, 1, HoughThreshValue, param1);
		//cout << circles.size() << endl;

		if (circles.size() == 1){
			center.x = cvRound(circles[0][0]);
			center.y = cvRound(circles[0][1]);
			radius = cvRound(circles[0][2]);
			area = CV_PI * radius * radius;
			return true;
		}
		else{
			if (param1 >= 50)
				param1 -= 10;
			else
				return false;
		}
	}

}
inline bool Cup::calCircle(Mat& src,int param1,int param2){

	Mat image;
	cvtColor(src, image, CV_BGR2GRAY);

		vector<Vec3f> circles;
		HoughCircles(image, circles, HOUGH_GRADIENT, 1, param1,param2);

		if (circles.size()){
			center.x = cvRound(circles[0][0]);
			center.y = cvRound(circles[0][1]);
			radius = cvRound(circles[0][2]);
			area = CV_PI * radius * radius;
	        if(Cup::center.x>10 && Cup::center.y>10){
                if(length(center,realCenter)>diffPixel)
                    diffPixel = length(center,realCenter);
            }
			return true;
        }
        else
            return false;

}
inline bool Cup::avgCalCircle(Mat& src){
	Mat image;

	cvtColor(src, image, CV_BGR2GRAY);
	vector<Point> center;
	vector<float> radius;


	for (int param1 = 200; param1 >= 120; param1 -= 10){
		for (int param2 = 200; param1 >= 120; param1 -= 10){

			vector<Vec3f> circles;
			HoughCircles(image.clone(), circles, HOUGH_GRADIENT, 1,HoughThreshValue, param1, 100, 0, src.rows);

			if (circles.size() == 1){
				center.push_back(Point(cvRound(circles[0][0]), cvRound(circles[0][1])));
				radius.push_back(cvRound(circles[0][2]));
			}
		}

	}
	numberOfCircles = center.size();
	if (center.size()){

		getMinCenter(center);
		getMinRadius(radius);

		area = CV_PI * Cup::radius * Cup::radius;

		return true;
	}
	else
		return false;


}
inline void Cup::setMethod(int method){
	Cup::method = method;
}
inline void Cup::setShape(int shape){
	Cup::shape = shape;
}
inline void Cup::setType(int type){
	Cup::type = type;
}
inline float Cup::getRadius(){
	return Cup::radius;
}
inline Point Cup::getCenter(){
	return Cup::center;
}
inline float Cup::getArea(){
	return Cup::area;
}
inline bool Cup::draw(Mat& image, Scalar center, Scalar circle){

	if(Cup::center.x>10 && Cup::center.y>10){
		cv::circle(image, Cup::center, 0, center, 2);
		cv::circle(image, Cup::center, radius, circle, 2);
        return true;
	}
    return false;
}
inline void Cup::showInfo(Mat& image){

//	for (int i = 0; i < 80; i++)
//		for (int j = 0; j < 180; j++){
//			image.at<Vec3b>(i, j)[0] /= 1.5;
//			image.at<Vec3b>(i, j)[1] /= 1.5;
//			image.at<Vec3b>(i, j)[2] /= 1.5;
//		}

	sprintf(Cup::Scenter, "center : (%d , %d) pixel", Cup::center.x, Cup::center.y);
	sprintf(Cup::Sradius, "radius : %.2f pixel", Cup::radius);
	sprintf(Cup::Sarea, "area : %.2f pixel", Cup::area);
    sprintf(Cup::SdiffPixel,"diffPixel : %d pixel",Cup::diffPixel);

	putText(image, "Multimedia Soc Lab , HeeSeung", Point(15, 20+150), FONT_HERSHEY_DUPLEX, 0.3, Scalar(180, 180, 180), 1);
	putText(image, Cup::Scenter, Point(15, 30+150), FONT_HERSHEY_DUPLEX, 0.3, Scalar(180, 180, 180), 1);
	putText(image, Cup::Sradius, Point(15, 40+150), FONT_HERSHEY_DUPLEX, 0.3, Scalar(180, 180, 180), 1);
	putText(image, Cup::Sarea, Point(15, 50+150), FONT_HERSHEY_DUPLEX, 0.3, Scalar(180, 180, 180), 1);
    putText(image,Cup::SdiffPixel,Point(15,70+150),FONT_HERSHEY_DUPLEX,0.3,Scalar(180,180,180),1);

	if (method == AVERAGE)
		cout << "numberOfCircles : " << numberOfCircles << endl;
	//cout << "center : " << center << endl;
	//cout << "radius : " << radius << endl;
	//cout << "area : " << area << endl;
}
inline void Cup::showInfo(){
	if (method == AVERAGE)
		cout << "numberOfCircles : " << numberOfCircles << endl;
	cout << "center : " << center << endl;
	cout << "radius : " << radius << endl;
	cout << "area : " << area << endl;
}
inline void Cup::getMinCenter(vector<Point> vec){
	float sumX = 0, sumY = 0;
	int size = vec.size();

	for (int i = 0; i < size; i++){
		Point tempPoint = vec.back();
		sumX += tempPoint.x;
		sumY += tempPoint.y;
		vec.pop_back();
	}
	center = Point(sumX / size, sumY / size);
}
inline void Cup::getMinRadius(vector<float> vec){
	float sumR = 0;
	int size = vec.size();

	for (int i = 0; i < size; i++){
		sumR += vec.back();
		vec.pop_back();
	}
	radius = sumR / size;
}
inline int Cup::length(Point pt1, Point pt2)
{
	return cvRound(sqrt((float)pow((pt1.x - pt2.x), 2) + pow((pt1.y - pt2.y), 2)));
}


#endif
