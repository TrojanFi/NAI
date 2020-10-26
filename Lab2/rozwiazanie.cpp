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

    int LowH = 0;
    int HighH = 0;
    int LowS = 0;
    int HighS = 0;
    int LowV = 0;
    int HighV = 0;

    createTrackbar("LowH", "MojeRange", &LowH, 180);
    createTrackbar("HighH", "MojeRange", &HighH, 180);
    createTrackbar("LowS", "MojeRange", &LowS, 255);
    createTrackbar("HighS", "MojeRange", &HighS, 255);
    createTrackbar("LowV", "MojeRange", &LowV, 255);
    createTrackbar("HighV", "MojeRange", &HighV, 255);


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


           inRange(MojeRange, Scalar(LowH, LowV , LowS), Scalar(HighS, HighH, HighV),MojeRange);

       
        // wys wart
        putText(pokazaniehsv, to_string(LowH), { 10,30 }, FONT_HERSHEY_PLAIN, 1.0, { 255,255,255,8 });
        putText(pokazaniehsv, to_string(HighH), { 10,50 }, FONT_HERSHEY_PLAIN, 1.0, { 255,255,255,8 });
        putText(pokazaniehsv, to_string(LowS), { 10,70 }, FONT_HERSHEY_PLAIN, 1.0, { 255,255,255,8 });
        putText(pokazaniehsv, to_string(HighS), { 10,90 }, FONT_HERSHEY_PLAIN, 1.0, { 255,255,255,8 });
        putText(pokazaniehsv, to_string(LowV), { 10,110 }, FONT_HERSHEY_PLAIN, 1.0, { 255,255,255,8 });
        putText(pokazaniehsv, to_string(HighV), { 10,130 }, FONT_HERSHEY_PLAIN, 1.0, { 255,255,255,8 });
       
        

        imshow("MojeRange", MojeRange);
        imshow("HSV4", pokazaniehsv);



        if ((waitKey(1000.0 / 60.0) & 0x0ff) == 27) {
            capturing = false;
        }
    } while (capturing);
    return 0;
}
