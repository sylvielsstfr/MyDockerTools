#include "thermalvideocamera.h"
#include <iostream>
#include <chrono>
#include <QDebug>
#include <math.h>

//Set path for image saving here (the actual image saving is done in readStream():
std::string path = "/home/username/sample-folder/";

ThermalVideoCamera::ThermalVideoCamera(QObject *parent) : QObject(parent)
{
    m_cameraBuffer.resize(0);
    m_videoFrame.load("file://" + QGuiApplication::applicationDirPath() + QString("cross.png"));
}

ThermalVideoCamera::ThermalVideoCamera(Camera *camera)
{
    m_cameraBuffer.resize(0);
    setCamera(camera);
    m_videoFrame.load("file://" + QGuiApplication::applicationDirPath() + QString("cross.png"));
    m_cameraSpec = QString::fromStdString(camera->getCameraSpecs());

    //initialize temperature radius
    m_temperatureRadius.append(0.);
    m_temperatureRadius.append(0.);
    m_temperatureRadius.append(0.);
    m_temperatureRadius.append(0.);
    m_temperatureRadius.append(0.);

    QString parameterNames[9] = {
        QString("MODEL:"),
        QString("STATUS:"),
        QString("SHUTTER:"),
        QString("SENSOR:"),
        QString("REFLECT:"),
        QString("ATMOSPHERE:"),
        QString("LENS:"),
        QString("EMISSIVITY:"),
        QString("HUMIDITY:")
    };
    for (QString name : parameterNames)
        ThermalVideoCamera::addCameraData(name);

}

ThermalVideoCamera::~ThermalVideoCamera()
{
    if (!m_stream)
        delete m_stream;
    if (m_cameraConnected)
        ThermalVideoCamera::disconnectCamera();
    if (!m_camera)
        delete m_camera;

}

QString ThermalVideoCamera::cameraSpec()
{
    return m_cameraSpec;
}

QQmlListProperty<CameraData> ThermalVideoCamera::cameraData()
{
    return QQmlListProperty<CameraData>(this, m_cameraData);
}

void ThermalVideoCamera::setCamera(Camera *camera)
{
    m_camera = camera;
}
QByteArray& ThermalVideoCamera::getBuffer()
{
    return m_cameraBuffer;
}

void ThermalVideoCamera::setMinMaxTemp(int min, int max)
{
    m_stream->setPresetTemps(min, max);
}

//This feature was added based on requests from customers. It was added later on in this code. 
//To ensure threadsafety this had to be moved withing the applications logic into ThermalVideoCamera::readStream() 
bool savingImage = false; 
void ThermalVideoCamera::saveRadiometricImage() 
{ 
    savingImage = true; 
} 

void ThermalVideoCamera::findCameraSettings()
{
    m_resX = m_camera->getSettings()->getResolutionX();
    m_resY = m_camera->getSettings()->getResolutionY();
    m_cameraBuffer.resize(m_resX * m_resY * 2);

    ThermalVideoCamera::changeCameraData(0, QString::fromStdString(m_camera->getSettings()->getModel()));
    ThermalVideoCamera::changeCameraData(1, QString::fromStdString(m_camera->getStatus()));

    ThermalVideoCamera::findCameraData();
}

void ThermalVideoCamera::findCameraData()
{
    //Get camera data;
    ThermalVideoCamera::changeCameraData(2, QString::number(m_camera->getSettings()->getShutterTemperature()));
    ThermalVideoCamera::changeCameraData(3, QString::number(m_camera->getSettings()->getSensorTemperature()));
    ThermalVideoCamera::changeCameraData(4, QString::number(m_camera->getSettings()->getReflectedTemperatureC()));
    ThermalVideoCamera::changeCameraData(5, QString::number(m_camera->getSettings()->getAtmospericTemperatureC()));
    ThermalVideoCamera::changeCameraData(6, QString::fromStdString(m_camera->getSettings()->getCurrentLense()));
    ThermalVideoCamera::changeCameraData(7, QString::number(m_camera->getSettings()->getEmissivity()));
    ThermalVideoCamera::changeCameraData(8, QString::number(m_camera->getSettings()->getHumidity()));

    emit cameraDataChanged();
}

void ThermalVideoCamera::connectCamera()
{
    if(m_camera->connect() == 0) {
        m_cameraConnected = true;
        ThermalVideoCamera::findCameraSettings();
        ThermalVideoCamera::initializeCameraContext();
    } else
    {
        cout << "ERROR! Couldn't connect to camera" << endl
             << "HINT: If you're using an USB3 camera, make sure that the eBUS deamon is up and running"
             << endl;
    }
}

void ThermalVideoCamera::disconnectCamera()
{
    ThermalVideoCamera::changeCameraData(1, QString("Disconnected"));
    // Because we are acquiring the buffer on a different QThread 
    // we need to put the thread to sleep for a brief period before disconnecting. 
    // By doing so we prevent the thread from trying to acquire a buffer from a disconnected camera. 
    // There are ofcourse other options on how to achieve a similar effect e.g. Mutex, Lockguards etc. 
    // Which is handled well in the standard library. We thus recommend 
    // using std::threads rather than QThreads, however QT applications do not 
    // like non-QThreads. There is a mutex example within this application aswell. 
    // This is just one of many possible solutions. 
    m_cameraConnected = false; 
    m_cameraThread->msleep(300); 
    m_camera->disconnect(); 

}
/* Create a new thread and move the buffer processing worker class inside */
void ThermalVideoCamera::initializeCameraContext()
{
    ThermalVideoCamera::changeCameraData(1, QString("Acquiring"));
    m_stream = new CameraStreamer(m_camera, m_resX * m_resY * 2, &g_conditionMet);
    m_cameraThread = new QThread;
    m_stream->moveToThread(m_cameraThread);
    connect(m_cameraThread, SIGNAL (started()), m_stream, SLOT (startStream()));
    connect(m_stream, SIGNAL (finished()), m_cameraThread, SLOT (quit()));
    connect(m_stream, SIGNAL (finished()), m_stream, SLOT (deleteLater()));
    connect(m_cameraThread, SIGNAL (finished()), m_cameraThread, SLOT (deleteLater()));

    connect(this, SIGNAL (cameraStopped()), m_stream, SLOT (terminateThread()));
    connect(m_stream, SIGNAL (newBufferAvailable()), this, SLOT (readStream()));

    m_cameraThread->start();
}
/* Retrieve the buffers from the worker class once signaled to do so */
void ThermalVideoCamera::readStream()
{
    m_cameraBuffer = (m_stream->getCameraBuffer());
    m_paletteBuffer = (m_stream->getPaletteBuffer());
    if (savingImage) 
    { 
        static uint16_t* l_imageBuffer; 
        if(!l_imageBuffer) 
        { 
            l_imageBuffer = new uint16_t[m_resX * m_resY * 2]; 
        } 
        memcpy(l_imageBuffer, (uint16_t*)m_cameraBuffer.data(),m_resX * m_resY * 2); 
        m_camera->saveRadiometricImage(l_imageBuffer, (path + "SampleRadiometricImage.jpeg"));
        savingImage = false; 
    } 
    ThermalVideoCamera::setTemperatureRadius();
    ThermalVideoCamera::createVideoFrame();
}

void ThermalVideoCamera::restoreFactorySettings()
{
    m_camera->getSettings()->restoreCameraSettings();
    m_stream->disablePresetTemps();
    ThermalVideoCamera::findCameraData();
}

void ThermalVideoCamera::createVideoFrame()
{
    static uchar* data = nullptr;
     if(!data){
         data = new uchar[m_resX * m_resY * 2];
     }
    memcpy(data,(uchar*)m_cameraBuffer.data(), m_resX * m_resY * 2);

    //two different approaches to constructing a QImage. Note that you need to create a copy by calling copy() because of the restrictions given on the data buffer by QImage.
    m_videoFrame = QImage(data, m_resX, m_resY, m_resX * 2, QImage::Format_Grayscale8).copy();
    m_paletteFrame = QImage(reinterpret_cast<uchar*>(m_paletteBuffer.data()), m_resX, m_resY, m_resX, QImage::Format_Indexed8).copy();
    emit newFrameAvailable();
    //Wake up the buffer processing thread
    g_conditionMet.wakeAll();
}

QImage ThermalVideoCamera::videoFrame()
{
    return m_videoFrame;
}

QImage ThermalVideoCamera::paletteFrame()
{
    return m_paletteFrame;
}

void ThermalVideoCamera::deleteCameraData()
{
    QList<CameraData*>::iterator i;
    for (i = m_cameraData.begin(); i!= m_cameraData.end(); ++i)
    {
        delete *i;
    }
}
void ThermalVideoCamera::addCameraData(QString name)
{
    CameraData* data = new CameraData(name);
    m_cameraData.append(data);
}

void ThermalVideoCamera::changeCameraData(int index, QString value)
{
    m_cameraData.at(index)->setParameterValue(value);
    emit cameraDataChanged();
}

void ThermalVideoCamera::setTemperatureRadius()
{
    m_temperatureRadius[0] = m_stream->getMinMax().x();
    m_temperatureRadius[4] = m_stream->getMinMax().y();
    auto diff = m_temperatureRadius[0] - m_temperatureRadius[4];
    m_temperatureRadius[1] = m_temperatureRadius[0] - 1*(diff)/4;
    m_temperatureRadius[2] = m_temperatureRadius[0] - 2*(diff)/4;
    m_temperatureRadius[3] = m_temperatureRadius[0] - 3*(diff)/4;

    emit temperatureRadiusChanged();
}

QList<float> ThermalVideoCamera::temperatureRadius()
{
    return m_temperatureRadius;
}

double ThermalVideoCamera::humidity()
{
    return m_humidity;
}
double ThermalVideoCamera::emissivity()
{
    return m_emissivity;
}
double ThermalVideoCamera::atmoTemp()
{
    return m_atmoTemp;
}
double ThermalVideoCamera::reflectTemp()
{
    return m_reflectTemp;
}
void ThermalVideoCamera::setHumidity(double newHumidity)
{
    if (m_camera && newHumidity >= 0 && newHumidity <= 1)
    {
        ThermalVideoCamera::changeCameraData(8, QString::number(newHumidity));
        m_humidity = newHumidity;
        m_camera->getSettings()->setHumidity(newHumidity);
        emit humidityChanged();
    }
}
void ThermalVideoCamera::setEmissivity(double newEmissivity)
{
    if (m_camera && newEmissivity >= 0 && newEmissivity <= 1)
    {
        ThermalVideoCamera::changeCameraData(7, QString::number(newEmissivity));
        m_emissivity = newEmissivity;
        m_camera->getSettings()->setEmissivity(newEmissivity);
        emit emissivityChanged();
    }
}
/* Altough you can theoretically set any atmospheric temperature, we will limit ourselves to a 0-255 value here*/
void ThermalVideoCamera::setAtmoTemp(double newAtmoTemp)
{
    if (m_camera && newAtmoTemp >= 0 && newAtmoTemp <= 255)
    {
        ThermalVideoCamera::changeCameraData(5, QString::number(newAtmoTemp));
        m_atmoTemp = newAtmoTemp;
        m_camera->getSettings()->setAtmospericTemperatureC(newAtmoTemp);
        emit atmoTempChanged();
    }
}
/* Altough you can theoretically set any reflected temperature, we will limit ourselves to a 0-255 value here */
void ThermalVideoCamera::setReflectTemp(double newReflectTemp)
{
    if (m_camera && newReflectTemp >= 0 && newReflectTemp < 256)
    {
        ThermalVideoCamera::changeCameraData(4, QString::number(newReflectTemp));
        m_reflectTemp = newReflectTemp;
        m_camera->getSettings()->setReflectedTemperatureC(newReflectTemp);
        emit reflectTempChanged();
    }
}

void ThermalVideoCamera::setHighRange() 
{ 
    m_camera->getSettings()->setRangeMode(CameraSerialSettings::RangeModes::High); 
} 
 
void ThermalVideoCamera::setLowRange() 
{ 
    m_camera->getSettings()->setRangeMode(CameraSerialSettings::RangeModes::Low); 
} 
 
void ThermalVideoCamera::setMiddleRange() 
{ 
    m_camera->getSettings()->setRangeMode(CameraSerialSettings::RangeModes::Middle); 
    m_camera->getSettings()->doFFC(); 
} 
