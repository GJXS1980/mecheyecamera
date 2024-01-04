/**********************************************************
Author: 
微信公众号: 
QQ群: 
Email: 
LICENSE: MIT
**********************************************************/
#include "QtMecheyeCamera.h"
#include <QApplication>

#include <opencv2/opencv.hpp>
#include "area_scan_3d_camera/Camera.h"
#include "area_scan_3d_camera/api_util.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QVideoWidget widget;
    QtCamera camera(&widget);
    widget.show();

    // 搜索设备
    mmind::eye::Camera mecheyecamera;
    if (!findAndConnect(mecheyecamera))
        return -1;

    if (!confirmCapture3D()) {
        mecheyecamera.disconnect();
        return 0;
    }

    //  2D图像
    mmind::eye::Frame2D frame2D;
    showError(mecheyecamera.capture2D(frame2D));

    //  3D图像
    mmind::eye::Frame3D frame3D;
    showError(mecheyecamera.capture3D(frame3D));

    cv::Mat image2D;
    const std::string imageFile = "2DImage.png";
    const std::string pointCloudFile = "PointCloud.ply";

    // 保存点云
    showError(frame3D.saveUntexturedPointCloud(mmind::eye::FileFormat::PLY, pointCloudFile));

    mmind::eye::Frame2DAnd3D frame2DAnd3D;
    showError(mecheyecamera.capture2DAnd3D(frame2DAnd3D));

    const std::string texturedPointCloudFile = "TexturedPointCloud.ply";

    showError(
        frame2DAnd3D.saveTexturedPointCloud(mmind::eye::FileFormat::PLY, texturedPointCloudFile));
    std::cout << "Capture and save the textured point cloud: " << texturedPointCloudFile << std::endl;

    switch (frame2D.colorType()) 
    {
        case mmind::eye::ColorTypeOf2DCamera::Monochrome:
        {
            mmind::eye::GrayScale2DImage grayImage = frame2D.getGrayScaleImage();
            image2D = cv::Mat(grayImage.height(), grayImage.width(), CV_8UC1, grayImage.data());
        }
        case mmind::eye::ColorTypeOf2DCamera::Color:
        {
            mmind::eye::Color2DImage colorImage = frame2D.getColorImage();
            image2D = cv::Mat(colorImage.height(), colorImage.width(), CV_8UC3, colorImage.data());
        }
    }

    cv::imshow(imageFile, image2D);
    cv::imwrite(imageFile, image2D);
    std::cout << "Capture and save the 2D image: " << imageFile << std::endl;
    cv::waitKey(0);

    mecheyecamera.disconnect();
    std::cout << "Disconnected from the camera successfully." << std::endl;

    return a.exec();
}
