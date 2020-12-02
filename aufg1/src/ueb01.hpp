/**-------System Header-------*/
#include <opencv2/opencv.hpp>

namespace bba
{
    typedef enum
    {
        HIST_NOT_ANALYZED,
        HIST_BRIGHTNESS_LOW,
        HIST_BRIGHTNESS_HIGH,
        HIST_CONTRAST_HIGH,
        HIST_CONTRAST_OK

    } HistResult;

    bool
    identity(cv::Mat &img);
    bool brightness(cv::Mat &img, float value);
    bool factor(cv::Mat &img, float value);
    bool linear(cv::Mat &img, float slope, float y_intercept);
    bool contrast(cv::Mat &img, float value);
    cv::Mat normalize(cv::Mat &img);
    bool gamma(cv::Mat &img, float value);
    bool clamp(cv::Mat &img);
    cv::Mat add(cv::Mat &img1, cv::Mat &img2);
    cv::Mat sub(cv::Mat &img1, cv::Mat &img2);
    cv::Mat adiff(cv::Mat &img1, cv::Mat &img2);
    bool histogramm(cv::Mat &img, int bins);
    bool sumHistogramm(cv::Mat &img, int bins);
    bool histogrammCheck(cv::Mat &img);
}; // namespace bba