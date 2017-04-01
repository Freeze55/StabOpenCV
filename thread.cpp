#include "thread.h"
#include <QThread>
#include <iostream>
#include <QString>



#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/video.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videostab.hpp>
#include <QDebug>


using namespace std;
using namespace cv;
using namespace cv::videostab;

Thread::Thread()
{

}
void Thread::Stabilize(QString inputPath,QString outputPath,int outputFps, bool Type_Pass_Stabilizer, bool quietMode )
{


    Ptr<IFrameSource> stabilizedFrames;
    StabilizerBase *stabilizer;
    GaussianMotionFilter *motionFilter = new GaussianMotionFilter();
    motionFilter->setParams(15, 4);
    if (Type_Pass_Stabilizer = 0)
    {
         OnePassStabilizer *onePassStabilizer = new OnePassStabilizer();
         onePassStabilizer->setMotionFilter(motionFilter);
         stabilizer = onePassStabilizer;
    }
    else
    {
        TwoPassStabilizer *twoPassStabilizer = new TwoPassStabilizer();
        twoPassStabilizer->setMotionStabilizer(motionFilter);
        stabilizer = twoPassStabilizer;
    }


    VideoFileSource *source = new VideoFileSource(inputPath.toStdString());

    cout << "frame count: " << source->count() << endl;
    stabilizer->setFrameSource(source);

    Ptr<MotionEstimatorRansacL2> est = makePtr<MotionEstimatorRansacL2>(MM_AFFINE);
    RansacParams ransac = est->ransacParams();
    ransac.eps = 0.5;
    est->setRansacParams(ransac);
    est->setMinInlierRatio(0.1);
    Ptr<IOutlierRejector> outlierRejector = makePtr<NullOutlierRejector>();
    Ptr<KeypointBasedMotionEstimator> motionEstimator = makePtr<KeypointBasedMotionEstimator>(est);

    motionEstimator->setDetector(GFTTDetector::create(1000));
    motionEstimator->setOutlierRejector(outlierRejector);



    stabilizer->setMotionEstimator(motionEstimator);
    stabilizer->setRadius(15);
    stabilizer->setBorderMode(BORDER_REPLICATE);
    WeightingDeblurer *debluer = new WeightingDeblurer();
    debluer->setSensitivity(0.1);
    stabilizer->setDeblurer(debluer);

    stabilizedFrames.reset(dynamic_cast<IFrameSource*>(stabilizer));




    VideoWriter writer;
    Mat stabilizedFrame;
    int nframes = 0;


    qDebug() << "OK";


    while (!(stabilizedFrame = stabilizedFrames->nextFrame()).empty())
    {
        // qDebug() << "OK7";
         nframes++;


        if (!outputPath.isEmpty())
        {

            if (!writer.isOpened())
                writer.open(outputPath.toStdString(), VideoWriter::fourcc('X', 'V', 'I', 'D'),
                    outputFps, stabilizedFrame.size());
            writer << stabilizedFrame;

        }


        if (!quietMode)
        {
            imshow("stabilizedFrame", stabilizedFrame);

            char key = static_cast<char>(waitKey(3));
            if (key == 27) { cout << endl; break; }
        }
    }


     qDebug() <<"КОНЕЦ";

     restart();
}
