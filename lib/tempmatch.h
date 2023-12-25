#ifndef TEMPMATCH_H
#define TEMPMATCH_H
// #include

#include "TemplateMatching.h"
#include "core/ErrorCode.h"
#include "core/Outputs.h"
#include "core/Parameters.h"
#include "core/Patterns.h"
#include "opencv2/opencv.hpp"
namespace mark{
class TrainTemplateMatch
{
public:
    TrainTemplateMatch();
    ~TrainTemplateMatch();

    bool setAngleParams(float angle_start, float angle_range, float angle_step);
    bool setCannyThreshold(int thresh_low, int thresh_high);
    bool setPyramidLevel(int level);
    bool setTimeLimit(int limit);
    bool trainTemplateMatch(cv::Mat train_image, cv::Mat mask_image);

    const smartmore::vitip::PatternPlanar& getFMTool();
    const smartmore::vitip::TrainParamsPlanar& getTrainParmas();

private:
    class Impl;
    std::shared_ptr<Impl> impl_;
};

class RunTemplateMatch
{
public:
    RunTemplateMatch();
    ~RunTemplateMatch();

    bool setOverlapThreshold(float ratio);
    bool setTimeLimit(int limit);
    bool setScoreThreshold(float score_thresh);
    bool setPyramidLevel(int level);
    bool runTemplateMatch(cv::Mat run_img);
    const smartmore::vitip::OutputPlanar& getOuput();

private:
    class Impl;
    std::shared_ptr<Impl> impl_;

};

class TrainOutput{
public:
    TrainOutput();
    ~TrainOutput();

    std::vector<cv::Point2f> getTemplateCurves() const;

private:
    class Impl;
    std::shared_ptr<Impl> impl_;
};

class RunOutput{
public:
    RunOutput();
    ~RunOutput();

    bool getNG();
    std::vector<std::vector<cv::Point2f>> getCurves() const;
    std::vector<cv::Point2f> getCenters() const;
    std::vector<double> getScores() const;
    std::vector<cv::Mat> getHomographys() const;
    std::vector<float> getAngles() const;

private:
    class Impl;
    std::shared_ptr<Impl> impl_;
};
} //mark

#endif // TEMPMATCH_H
