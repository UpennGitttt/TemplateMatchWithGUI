#include "tempmatch.h"
#include "opencv2/opencv.hpp"
#include "memory.h"
#include "filesystem"
#include <QMessageBox>
#include <qdebug.h>

namespace fs = std::filesystem;
namespace mark{

class TrainTemplateMatch::Impl{
public:
    smartmore::vitip::TrainParamsPlanar trainParams;
    smartmore::vitip::PatternPlanar fm_tool_pattern;
    std::shared_ptr<smartmore::vitip::TemplateTrain> fm_tool_train;
    std::shared_ptr<smartmore::vitip::TrainParamsPlanar> trainParams_ptr;
    std::shared_ptr<smartmore::vitip::PatternPlanar> fm_tool_train_ptr;
    Impl(){
        trainParams.SetAngleParams(0, 360, 0.5);
        trainParams.SetCannyThreshold(10, 105);
        trainParams.SetContourLengthProportion(1);
        trainParams.SetPyramidLevel(5);
        trainParams.SetTimeLimit(5000);
        fm_tool_train = std::make_shared<smartmore::vitip::TemplateTrain>();
    };
    ~Impl(){};
};

TrainTemplateMatch::TrainTemplateMatch(): impl_(std::make_shared<TrainTemplateMatch::Impl>()){}
TrainTemplateMatch::~TrainTemplateMatch(){}

bool TrainTemplateMatch::setAngleParams(float angle_start, float angle_range, float angle_step)
{
    auto code = impl_->trainParams.SetAngleParams(angle_start, angle_range, angle_step);
    if(code != smartmore::vitip::ErrorCode::kResultOk){
        return false;
    }
    return true;
}

bool TrainTemplateMatch::setCannyThreshold(int thresh_low, int thresh_high)
{
    auto code = impl_->trainParams.SetCannyThreshold(thresh_low, thresh_high);
    if(code != smartmore::vitip::ErrorCode::kResultOk){
        return false;
    }
    return true;
}

bool TrainTemplateMatch::setPyramidLevel(int level)
{
    auto code = impl_->trainParams.SetPyramidLevel(level);
    if(code != smartmore::vitip::ErrorCode::kResultOk){
        return false;
    }
    return true;
}

bool TrainTemplateMatch::setTimeLimit(int limit)
{
    auto code = impl_->trainParams.SetTimeLimit(limit);
    if(code != smartmore::vitip::ErrorCode::kResultOk){
        return false;
    }
    return true;
}

bool TrainTemplateMatch::trainTemplateMatch(cv::Mat image, cv::Mat mask){
    smartmore::vitip::ErrorCode code;
    try{
        if(mask.empty()){
            code = impl_->fm_tool_train->templateTrain(image, impl_->fm_tool_pattern, impl_->trainParams, image);
        }else{
            code = impl_->fm_tool_train->templateTrain(image, impl_->fm_tool_pattern, impl_->trainParams, mask);
        }
        if(code != smartmore::vitip::ErrorCode::kResultOk){
            return false;
        }
    }catch(const std::exception& e){
        std::cerr << "Standard exception: " << e.what() << std::endl;
    }catch (...) {
        std::cerr << "Unknown exception occurred" << std::endl;
    }

    std::cout << impl_->trainParams.GetAngleStep() << std::endl;
    qDebug() << "num of features after train: " << impl_->fm_tool_pattern.getOriginalTemplateCurve().size();

    return true;
}

std::shared_ptr<smartmore::vitip::PatternPlanar>& TrainTemplateMatch::getFMTool()
{
    impl_->fm_tool_train_ptr = std::make_shared<smartmore::vitip::PatternPlanar>(impl_->fm_tool_pattern);
    return impl_->fm_tool_train_ptr;
}

std::shared_ptr<smartmore::vitip::TrainParamsPlanar>& TrainTemplateMatch::getTrainParmas()
{
    impl_->trainParams_ptr = std::make_shared<smartmore::vitip::TrainParamsPlanar>(impl_->trainParams);
    return impl_->trainParams_ptr;
}

class RunTemplateMatch::Impl{
public:
    smartmore::vitip::RunParamsPlanar runParams;
    smartmore::vitip::OutputPlanar output;
    smartmore::vitip::TemplateRun fm_tool_run;
    std::shared_ptr<smartmore::vitip::OutputPlanar> output_ptr;

    Impl(){
        runParams.setOverlapThreshold(0.5);
        runParams.setScoreThreshold(0.5);
        runParams.SetPyramidLevel(1);
        runParams.SetTimeLimit(3000);
    };
    ~Impl(){};
};

RunTemplateMatch::RunTemplateMatch(): impl_(std::make_shared<RunTemplateMatch::Impl>()){}
RunTemplateMatch::~RunTemplateMatch(){}

bool RunTemplateMatch::setOverlapThreshold(float ratio)
{
    auto code = impl_->runParams.setOverlapThreshold(ratio);
    if(code != smartmore::vitip::ErrorCode::kResultOk){
        return false;
    }
    return true;
}
bool RunTemplateMatch::setTimeLimit(int limit)
{
    auto code = impl_->runParams.SetTimeLimit(limit);
    if(code != smartmore::vitip::ErrorCode::kResultOk){
        return false;
    }
    return true;
}
bool RunTemplateMatch::setScoreThreshold(float score_thresh)
{
    auto code = impl_->runParams.setScoreThreshold(score_thresh);
    if(code != smartmore::vitip::ErrorCode::kResultOk){
        return false;
    }
    return true;
}
bool RunTemplateMatch::setPyramidLevel(int level)
{
    auto code = impl_->runParams.SetPyramidLevel(level);
    if(code != smartmore::vitip::ErrorCode::kResultOk){
        return false;
    }
    return true;
}

bool RunTemplateMatch::runTemplateMatch(cv::Mat run_img, std::shared_ptr<smartmore::vitip::PatternPlanar>& pattern)
{
    smartmore::vitip::PatternPlanar fm_tool_pat = *pattern.get();
    auto code = impl_->fm_tool_run.templateRun(run_img, impl_->output, fm_tool_pat, impl_->runParams, run_img);
    if(code != smartmore::vitip::ErrorCode::kResultOk){
        return false;
    }

    qDebug() << "Run success: " << impl_->output.getScores()[0];
    return true;

}

const std::shared_ptr<smartmore::vitip::OutputPlanar>& RunTemplateMatch::getOuput()
{
    impl_->output_ptr = std::make_shared<smartmore::vitip::OutputPlanar>(impl_->output);
    return impl_->output_ptr;
}

//class TrainOutput::Impl{
//public:
//    TrainTemplateMatch trainParams_;
//    std::shared_ptr<smartmore::vitip::PatternPlanar> pattern_tool;
//
//    Impl(){
//        pattern_tool = trainParams_.getFMTool();
//    };
//    ~Impl(){};
//};
//
//TrainOutput::TrainOutput(): impl_(std::make_shared<TrainOutput::Impl>()){}
//TrainOutput::~TrainOutput(){}
//
//std::vector<cv::Point2f> TrainOutput::getTemplateCurves() const
//{
//    return impl_->pattern_tool->getOriginalTemplateCurve();
//}
//
//class RunOutput::Impl{
//public:
//    RunTemplateMatch runParams;
//    smartmore::vitip::OutputPlanar output;
//
//
//    Impl(){
//        output = runParams.getOuput();
//    };
//    ~Impl(){};
//};
//
//RunOutput::RunOutput(): impl_(std::make_shared<RunOutput::Impl>()){}
//RunOutput::~RunOutput(){}
//
//bool RunOutput::getNG()
//{
//    return impl_->output.getNG();
//}
//
//std::vector<std::vector<cv::Point2f>> RunOutput::getCurves() const
//{
//    return impl_->output.getCurves();
//}
//
//std::vector<cv::Point2f> RunOutput::getCenters() const
//{
//    return impl_->output.getCenters();
//}
//
//std::vector<double> RunOutput::getScores() const
//{
//    return impl_->output.getScores();
//}
//
//std::vector<cv::Mat> RunOutput::getHomographys() const
//{
//    return impl_->output.getHomographys();
//}
//
//std::vector<float> RunOutput::getAngles() const
//{
//    return impl_->output.getAngles();
//}

} //namespace mark
