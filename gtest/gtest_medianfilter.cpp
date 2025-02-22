#include "gtest_medianfilter.hpp"

#include <QDir>
#include <QElapsedTimer>

using ::testing::AtLeast;

namespace gtest_medianfilter
{
    TEST_F(GTest_medianfilter, test_medianFilter1)
    {
        QElapsedTimer timer;
        timer.start();
        // cv::Mat
        cv::Mat frame = cv::Mat(500, 500, CV_8UC1, cv::Scalar(0));
        MedianFilter* imgMedianObj = new MedianFilter(frame, 11);
        int x = 0;
        int y = 0;
        for (int j = 0; j < 5; j++)
        {
            for (int i = 0; i < 40; i++)
            {
                frame.at<unsigned char>(cv::Point(x, y)) = i;
                imgMedianObj->removeNode();
                imgMedianObj->addImage(frame);
                cv::Mat mMedian = imgMedianObj->getMedian();
                // imgMedianObj.showMedianNode(0);
            }
            qDebug() << "median operation takes:" << timer.restart() << "[ms]";
        }
    }

    TEST_F(GTest_medianfilter, test_medianFilter2)
    {
        
    }

}  // namespace gtest_medianfilter
