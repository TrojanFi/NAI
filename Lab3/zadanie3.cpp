#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

int main(int argc, char** argv) {

	int loRange[3] = { 20,100,100 };
	int hiRange[3] = { 30,255,255 };

	namedWindow("jakostam", WINDOW_AUTOSIZE);
	createTrackbar("loRange0", "jakostam", &(loRange[0]), 255);
	createTrackbar("loRange1", "jakostam", &(loRange[1]), 255);
	createTrackbar("loRange2", "jakostam", &(loRange[2]), 255);
	createTrackbar("hiRange0", "jakostam", &(hiRange[0]), 255);
	createTrackbar("hiRange1", "jakostam", &(hiRange[1]), 255);
	createTrackbar("hiRange2", "jakostam", &(hiRange[2]), 255);
	VideoCapture camera(1);
	//VideoCapture backgroundvid("Multiwave.wmv");
	Mat background = imread("wc.jpg", IMREAD_COLOR);
	while (waitKey(1) != 27) {

		int dilation_size = 5;
		auto structElem = getStructuringElement(MORPH_ELLIPSE, // jądro przekształcenia (krzyż panie dzieju)
			Size(2 * dilation_size + 1, 2 * dilation_size + 1), // bigger white space
			Point(dilation_size, dilation_size));

		Mat frame;
		Mat backgroundScaled;
		Mat frameMask, frameNegMask;
		Mat frameWithMask, backgroundScaledWithMask;
		Mat meinniceplace;
		//(with animated background) Mat background;
		//(with animated background) backgroundvid >> background;
		camera >> frame;
		flip(frame, frame, 1); // odbicie
		resize(background, backgroundScaled, { frame.cols, frame.rows }); // rozmiar

		// obraz kolorowy
		//morphologyEx(frame, frame, MORPH_CLOSE, structElem);
		//morphologyEx(frame, frame, MORPH_OPEN, structElem);
		//imshow("dilate", frame);
		//imshow("dilate", structElem); // tu te ++


		
		cvtColor(frame, frameMask, COLOR_BGR2HSV); // Ustawienie koloru barw hsv
		inRange(frameMask, Scalar(loRange[0], loRange[1], loRange[2]), // wyciecie fragmentów w danych zakresach
			Scalar(hiRange[0], hiRange[1], hiRange[2]), frameNegMask);
		
		
		//imshow("orig", frameNegMask); // + szum
		//erode(frameNegMask, frameNegMask, structElem);
		//imshow("erode",frameNegMask); // + szum w kropy
		//dilate(frameNegMask, frameNegMask, structElem);
		//imshow("dilate", frameNegMask); // + szum ujednolici białe kropy
//lub
		morphologyEx(frameNegMask, frameNegMask, MORPH_CLOSE, structElem);
		morphologyEx(frameNegMask, frameNegMask, MORPH_OPEN, structElem);
		imshow("dilate", frameNegMask);

		  //DODANIE NEGACJA I DOBRA ZABAWA
		//bitwise_not(frameNegMask, frameMask); // negation
		//frame.copyTo(frameWithMask, frameMask); // copy with mask (keying) only 1 not 0 
		//imshow("maskedme", frameWithMask);
		//backgroundScaled.copyTo(backgroundScaledWithMask, frameNegMask);
		//imshow("maskebg", backgroundScaledWithMask);
		//meinniceplace = backgroundScaledWithMask + frameWithMask;
		//imshow("jakostam", meinniceplace);
		
		vector<vector<Point>> contours;
		findContours(frameNegMask, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
		

		// sort konturów
		sort(contours.begin(), contours.end(), [](auto& a, auto& b) {
			return contourArea(a, false) > contourArea(b, false);
			});
		// kontury
		for (int i = 0; i < contours.size(); i++) {
			approxPolyDP(contours.at(i), contours.at(i), 5, true);
			//drawContours(frame, contours, i, { 0, 255, 0, 255 });
			//auto txtpos = contours.at(i).at(0);
			//putText(frame, to_string(contours.at(0).size()), txtpos, FONT_HERSHEY_PLAIN, 2, { 0,255,0,255 });
			//txtpos.y += 30; 
			//putText(frame, to_string(contourArea(contours.at(i),false)), txtpos, FONT_HERSHEY_PLAIN, 2, { 0,255,0,255 });
			//
			//txtpos.y -= 60;
			//putText(frame, to_string(i), txtpos, FONT_HERSHEY_PLAIN, 2, { 0,255,0,255 });

		}
		

		
		//Point avgA,avgB;
		// środek konturu 0
		Point avg[3] = {};
		double currentArea = 0;
		for (int i = 0; i < contours.size(); i++)
		{
			if (i <= 1) {
				

				//Rect r = boundingRect(contours.at(0));
				//avg[i].x = r.x + r.width / 2;
				//avg[i].y = r.y + r.height / 2;
				//putText(frame, "0", avg[i], FONT_HERSHEY_PLAIN, 2, { 0,0,255,255 });

				Rect r = boundingRect(contours.at(i));
				avg[i].x = r.x + r.width / 2;
				avg[i].y = r.y + r.height / 2;
				putText(frame, "0", avg[i], FONT_HERSHEY_PLAIN, 2, { 0,0,255,255 });
				currentArea += contourArea(contours.at(i), false);


			}
			else break;
		}
		//if (contours.size()) {
		//	Point avgk;
		//
		//	Rect k = boundingRect(contours.at(1));
		//	avgk.x = k.x + k.width / 2;
		//	avgk.y = k.y + k.height / 2;
		//	putText(frame, "1", avgk, FONT_HERSHEY_PLAIN, 2, { 0,0,255,255 });
		//	avg.x = r.x + r.width ;
		//	avg.y = r.y + r.height;
		//	avgA = avg;
		//	avgB = avg;
		//}




		
		//putText(frame, "BANG",avg, FONT_HERSHEY_PLAIN, 2, { 255,0,255,255 });// dwa srodki A i B 
		avg[2].y = avg[0].y;
		if ((avg[0].x > avg[1].x)) {
			avg[2].x = ((avg[0].x - avg[1].x)/2) + avg[1].x;
		}
		else avg[2].x = ((avg[1].x - avg[0].x) / 2) + avg[0].x;
		putText(frame, "Bang", avg[0], FONT_HERSHEY_PLAIN, 2, { 255,0,255,255 });// dwa srodki A i B 
		arrowedLine(frame, avg[0], avg[1], Scalar(0, 255, 0), 3);
		arrowedLine(frame, avg[1], avg[0], Scalar(240, 700), 3);

		imshow("contours", frame);
		
	}
	return 0;
}
