/**-------System Header-------*/
#include <opencv2/opencv.hpp>
#include <functional>

namespace util
{
    void writeToCout(cv::Mat &img);
    void forEachPixel(cv::Mat &img, const std::function<void(cv::Mat &m, int y, int x)> &f);
} // namespace util
