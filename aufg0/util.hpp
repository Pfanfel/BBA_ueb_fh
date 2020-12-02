/**-------System Header-------*/
#include <opencv2/opencv.hpp>

namespace util
{
void writeToCout(cv::Mat &img);
void writeToCout2(cv::Mat &img);
void forEachPixel(cv::Mat &img, void f(cv::Mat &m, int y, int x));
} // namespace util
