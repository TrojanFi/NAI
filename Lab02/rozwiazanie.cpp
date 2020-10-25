#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

int main(int argc, char** argv) {

    bool capturing = true;
    cv::VideoCapture cap(0);
    cv::VideoCapture cap2(1);

    if (!cap.isOpened()) {
        cerr << "error while opening" << endl;
        return -1;
    }

    //hehe napis
    //cout << "ROZMIARY KAMERY: " << cap.get(CAP_PROP_FRAME_WIDTH) << "x" << cap.get(CAP_PROP_FRAME_HEIGHT) << endl;
    
 
    int szerokosc = 0;
    int wysokosc = 0;
    cout << "podaj szerokość okna" << endl;
    cin >> szerokosc;
    cout << "podaj wysokość okna" << endl;
    cin >> wysokosc;

    namedWindow("MojeRange", WINDOW_AUTOSIZE);
    namedWindow("HSV4", WINDOW_AUTOSIZE);

    int uRangeOne = 0;
    int uRangeTwo = 0;
    int uRangeThree = 0;

    int lRangeOne = 0;
    int lRangeTwo = 0;
    int lRangeThree = 0;

    createTrackbar("range1", "MojeRange", &uRangeOne, 255);
    createTrackbar("range2", "MojeRange", &uRangeTwo, 255);
    createTrackbar("range3", "MojeRange", &uRangeThree, 255);

    createTrackbar("range4", "MojeRange", &lRangeOne, 255);
    createTrackbar("range5", "MojeRange", &lRangeTwo, 255);
    createTrackbar("range6", "MojeRange", &lRangeThree, 255);


    do {
        Mat frame, moje, blur, MojeRange, pokazaniehsv;
        cap >> frame;
        Mat dst = frame;
       


        if (cap.read(frame)) {
           
            if (szerokosc > 0 && wysokosc > 0) resize(frame, moje, { szerokosc,wysokosc });
            else resize(frame, moje, { 320,200 });
            putText(frame, "Dobry Gracz ", { 300,30 }, FONT_HERSHEY_PLAIN, 1.0, { 255,4,4,4 });
            imshow("Okno z rozmiarem", moje);
            imshow("Okno 1", frame);
            GaussianBlur(moje, blur, Size(5, 5), 0);
            imshow("Gałsian", blur);
        }
        else {
            //stream finished
            capturing = false;
        }


        if (waitKey(30) == 'x') {

            Mat frame2;
            frame2 = frame.clone(); // klonem frame jest frame2
            auto r = selectROI("złapana klatka", frame2);
            Mat roi = frame2(r); // wycianie z orginału 
            imshow("ROI", roi);
            imwrite("wolnaPolska.JPG", roi);
            
           
        }
        

        cvtColor(frame, MojeRange, COLOR_BGR2HSV); // (frame,docelowyframe,rodzajkolorów)
        cvtColor(frame, pokazaniehsv, COLOR_BGR2HSV); // (frame,docelowyframe,rodzajkolorów)


           inRange(MojeRange, Scalar(uRangeOne, uRangeTwo, uRangeThree),
               Scalar(lRangeOne, lRangeTwo, lRangeThree),
              MojeRange);

       
        // wys wart
        putText(pokazaniehsv, to_string(uRangeOne), { 10,30 }, FONT_HERSHEY_PLAIN, 1.0, { 255,4,4,4 });
        putText(pokazaniehsv, to_string(uRangeTwo), { 10,50 }, FONT_HERSHEY_PLAIN, 1.0, { 255,4,4,4 });
        putText(pokazaniehsv, to_string(uRangeThree), { 10,70 }, FONT_HERSHEY_PLAIN, 1.0, { 255,4,4,4 });
        putText(pokazaniehsv, to_string(lRangeOne), { 10,90 }, FONT_HERSHEY_PLAIN, 1.0, { 255,4,4,4 });
        putText(pokazaniehsv, to_string(lRangeTwo), { 10,110 }, FONT_HERSHEY_PLAIN, 1.0, { 255,4,4,4 });
        putText(pokazaniehsv, to_string(lRangeThree), { 10,130 }, FONT_HERSHEY_PLAIN, 1.0, { 255,4,4,4 });
       
        

        imshow("MojeRange", MojeRange);
        imshow("HSV4", pokazaniehsv);



        if ((waitKey(1000.0 / 60.0) & 0x0ff) == 27) {
            capturing = false;
        }
    } while (capturing);
    return 0;
}
