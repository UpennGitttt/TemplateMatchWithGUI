#include "tempmatch.h"
#include "opencv2/opencv.hpp"
#include "memory.h"
#include "filesystem"

namespace fs = std::filesystem;
namespace mark{

class TrainTemplateMatch::Impl{
public:
    smartmore::vitip::TrainParamsPlanar trainParams;
    smartmore::vitip::PatternPlanar fm_tool_pattern;
    std::shared_ptr<smartmore::vitip::TemplateTrain> fm_tool_train;
    Impl(){
        trainParams.SetAngleParams(0, 360, 0.5);
        trainParams.SetCannyThreshold(10, 100);
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
    std::cout << "num of features before train: " << impl_->fm_tool_pattern.getOriginalTemplateCurve().size() << std::endl;
    std::cout << "Template size before train: " << impl_->fm_tool_pattern.getOriginalTemplateSize() << std::endl;
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
    std::cout << "Template size after train: " << impl_->fm_tool_pattern.getOriginalTemplateSize() << std::endl;
    std::cout << impl_->fm_tool_pattern.getOriginalTemplateCurve().size() << std::endl;
    // std::cout << "num of features after train " << numf << std::endl;

    return true;
}

const smartmore::vitip::PatternPlanar& TrainTemplateMatch::getFMTool()
{
    return impl_->fm_tool_pattern;
}

const smartmore::vitip::TrainParamsPlanar& TrainTemplateMatch::getTrainParmas()
{
    return impl_->trainParams;
}

class RunTemplateMatch::Impl{
public:
    TrainTemplateMatch trainParams;
    smartmore::vitip::PatternPlanar fm_tool_pattern;
    smartmore::vitip::RunParamsPlanar runParams;
    smartmore::vitip::OutputPlanar output;
    smartmore::vitip::TemplateRun fm_tool_run;

    Impl(){
        fm_tool_pattern = trainParams.getFMTool();
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

bool RunTemplateMatch::runTemplateMatch(cv::Mat run_img)
{
    auto code = impl_->fm_tool_run.templateRun(run_img, impl_->output, impl_->fm_tool_pattern, impl_->runParams, run_img);
    if(code != smartmore::vitip::ErrorCode::kResultOk){
        return false;
    }
    return true;
}

const smartmore::vitip::OutputPlanar& RunTemplateMatch::getOuput()
{
    return impl_->output;
}

class TrainOutput::Impl{
public:
    TrainTemplateMatch trainParams;
    smartmore::vitip::PatternPlanar pattern_tool;

    Impl(){
        pattern_tool = trainParams.getFMTool();
    };
    ~Impl(){};
};

TrainOutput::TrainOutput(): impl_(std::make_shared<TrainOutput::Impl>()){}
TrainOutput::~TrainOutput(){}

std::vector<cv::Point2f> TrainOutput::getTemplateCurves() const
{
    return impl_->pattern_tool.getOriginalTemplateCurve();
}


class RunOutput::Impl{
public:
    RunTemplateMatch runParams;
    smartmore::vitip::OutputPlanar output;


    Impl(){
        output = runParams.getOuput();
    };
    ~Impl(){};
};

RunOutput::RunOutput(): impl_(std::make_shared<RunOutput::Impl>()){}
RunOutput::~RunOutput(){}

bool RunOutput::getNG()
{
    return impl_->output.getNG();
}

std::vector<std::vector<cv::Point2f>> RunOutput::getCurves() const
{
    return impl_->output.getCurves();
}

std::vector<cv::Point2f> RunOutput::getCenters() const
{
    return impl_->output.getCenters();
}

std::vector<double> RunOutput::getScores() const
{
    return impl_->output.getScores();
}

std::vector<cv::Mat> RunOutput::getHomographys() const
{
    return impl_->output.getHomographys();
}

std::vector<float> RunOutput::getAngles() const
{
    return impl_->output.getAngles();
}

} //namespace mark
