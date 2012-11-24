#include <vector>
#include <deque>
#include <cv.h>
#include <highgui.h>

const unsigned int LEN = 8;

std::vector<IplImage *> buf;
std::deque<IplImage *> que;

int main()
{
	cvNamedWindow("glitcher", CV_WINDOW_NORMAL);
	cvResizeWindow("glitcher", 640, 480);
	cvSetWindowProperty("glitcher", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
	CvCapture *cam = cvCreateCameraCapture(0);
	cvSetCaptureProperty(cam, CV_CAP_PROP_FRAME_WIDTH, 640);
	cvSetCaptureProperty(cam, CV_CAP_PROP_FRAME_HEIGHT, 480);
	for (;;) {
		IplImage *frame = cvQueryFrame(cam);
		if (!frame) break;
		IplImage *i = cvCloneImage(frame);
		buf.push_back(i);
		if (buf.size() >= LEN) {
			std::vector<IplImage *>::reverse_iterator rit;
			for (rit = buf.rbegin(); rit < buf.rend(); rit++) {
				que.push_back(*rit);
			}
			buf.clear();
		}
		if (que.size() > 0) {
			cvShowImage("glitcher", que.front());
			cvReleaseImage(&que.front());
			que.pop_front();
		}
		if(cvWaitKey(10) == 27) break;
	}
	cvReleaseCapture(&cam);
	cvDestroyWindow("glitcher");
	return 0;
}
