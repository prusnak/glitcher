#include <iostream>
#include <vector>
#include <deque>
#include <cstdlib>
#include <cv.h>
#include <highgui.h>
#include <unistd.h>

std::vector<IplImage *> buf;
std::deque<IplImage *> que;

bool fullscreen = false;
int camid = -1;
unsigned int frames = 8;

void process_params(int argc, char **argv)
{
	int c;
	while ((c = getopt(argc, argv, "c:fhn:")) != -1) {
		switch (c) {
			case 'c':
				camid = atoi(optarg);
				break;
			case 'f':
				fullscreen = true;
				break;
			case 'h':
				printf("Usage: glitcher [-c X] [-f] [-n X] [-h]\n\n");
				printf("    -c X    use camera with index X instead of the default\n");
				printf("    -f      use fullscreen\n");
				printf("    -n X    use last X frames (default is 8)\n");
				printf("    -h      this help\n\n");
				exit(1);
				break;
			case 'n':
				frames = atoi(optarg);
				break;
		}
	}
}


int main(int argc, char **argv)
{
	process_params(argc, argv);
	CvCapture *cam = cvCreateCameraCapture(camid);
	if (!cam) {
		std::cerr << "Camera not found" << std::endl;
		return 1;
	}
	cvNamedWindow("glitcher", CV_WINDOW_NORMAL);
	cvResizeWindow("glitcher", 640, 480);
	if (fullscreen) {
			cvSetWindowProperty("glitcher", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
	}
	cvSetCaptureProperty(cam, CV_CAP_PROP_FRAME_WIDTH, 640);
	cvSetCaptureProperty(cam, CV_CAP_PROP_FRAME_HEIGHT, 480);
	for (;;) {
		IplImage *frame = cvQueryFrame(cam);
		if (!frame) {
			std::cerr << "Could not retrieve frame from camera" << std::endl;
			break;
		}
		IplImage *i = cvCloneImage(frame);
		buf.push_back(i);
		if (buf.size() >= frames) {
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
