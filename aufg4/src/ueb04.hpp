/**-------System Header-------*/
#include <functional>
#include <opencv2/opencv.hpp>
#include "argparse.hpp"

namespace bba
{
    typedef std::function<bool(std::vector<cv::Mat> &imagestack)> stack_operation;
    typedef std::vector<cv::Mat> imagestack;

    const int SUBREGION_WIDTH = 301;
    const int LOCATION_BUFFER_SIZE = 3;
    const int TEMPLATE_UPDATE_COUNT = 10;

    struct TrackParameters
    {
        cv::Point trackLoc;
        int templateWidth;
        std::vector<float> textColorRGB;
        std::string trackText;
    };

    cv::Mat getTemplateFromImage(cv::Mat &img, int x, int y, int width);
    cv::Rect getSubregionAroundPoint(cv::Mat &img, cv::Point &trackLoc, int width);
    cv::Mat trackFeature(cv::Mat &img, bba::TrackParameters &params, bool &first, cv::Mat &templ, int &counter, std::deque<cv::Point> &prevMatchLocations);
    cv::Point trackFeatureGUI(cv::Mat &img);
    TrackParameters createTrackParams(Operation op);
} // namespace bba