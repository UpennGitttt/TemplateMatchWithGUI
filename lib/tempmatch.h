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
    TrainTemplateMatch(const TrainTemplateMatch&);

    bool setAngleParams(float angle_start, float angle_range, float angle_step);
    bool setCannyThreshold(int thresh_low, int thresh_high);
    bool setPyramidLevel(int level);
    bool setTimeLimit(int limit);
    bool trainTemplateMatch(cv::Mat train_image, cv::Mat mask_image);

    std::shared_ptr<smartmore::vitip::PatternPlanar>& getFMTool();
    std::shared_ptr<smartmore::vitip::TrainParamsPlanar>& getTrainParmas();

private:
    class Impl;
    std::shared_ptr<Impl> impl_;
};

class RunTemplateMatch
{
public:
    RunTemplateMatch();
    ~RunTemplateMatch();
    RunTemplateMatch(const RunTemplateMatch&);

    bool setOverlapThreshold(float ratio);
    bool setTimeLimit(int limit);
    bool setScoreThreshold(float score_thresh);
    bool setPyramidLevel(int level);
    bool runTemplateMatch(cv::Mat run_img, std::shared_ptr<smartmore::vitip::PatternPlanar>& pattern);

    const std::shared_ptr<smartmore::vitip::OutputPlanar>& getOuput();

private:
    class Impl;
    std::shared_ptr<Impl> impl_;

};

//class TrainOutput{
//public:
//    TrainOutput();
//    ~TrainOutput();
//    TrainOutput(const TrainOutput&);
//
//
//    std::vector<cv::Point2f> getTemplateCurves() const;
//
//private:
//    class Impl;
//    std::shared_ptr<Impl> impl_;
//};
//
//class RunOutput{
//public:
//    RunOutput();
//    ~RunOutput();
//    RunOutput(const RunOutput&);
//
//    bool getNG();
//    std::vector<std::vector<cv::Point2f>> getCurves() const;
//    std::vector<cv::Point2f> getCenters() const;
//    std::vector<double> getScores() const;
//    std::vector<cv::Mat> getHomographys() const;
//    std::vector<float> getAngles() const;
//
//private:
//    class Impl;
//    std::shared_ptr<Impl> impl_;
//};
//
//class TemplateMatchTool {
//    TemplateMatchTool();
//    ~TemplateMatchTool();
//    const RunOutput& RunOutput() const;
//    const TrainOutput& TrainOutput() const;
//    class Impl;                                
//};
//
//TrainTemplateMatch& trainParams();
//
//RunTemplateMatch& runParams();
} //mark

#endif // TEMPMATCH_H
