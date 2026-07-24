#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QMutex>
#include <QThread>
#include <vector>
#include <QString>

extern "C" {
#include "v192_decoder.h"
}

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// 帧数据结构（用于线程间传递）
struct FrameData {
    uint32_t frame_id;
    double   timestamp;
    uint32_t count;
    bool     is_dual;
    std::vector<V192Point> points;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onStartOnlineClicked();
    void onStopOnlineClicked();
    void onOpenPcapClicked();
    void onClosePcapClicked();
    void onReadNextFrameClicked();
    void onUpdateUI();
    void onFrameReceived(const FrameData& frame);
    void onSaveFrameClicked();
    void onExportPCDClicked();
    void onExportPLYClicked();
    void onToggleAutoSaveClicked(bool checked);

private:
    Ui::MainWindow *ui;
    
    // V192 解码器句柄
    V192DecoderHandle m_decoder;
    
    // 定时器（用于更新UI）
    QTimer* m_updateTimer;
    
    // 线程安全的帧数据
    QMutex m_frameMutex;
    FrameData m_latestFrame;
    bool m_hasNewFrame;
    
    // 统计信息
    uint32_t m_frameCount;
    double m_lastTimestamp;
    
    // 文件保存相关
    QString m_outputDir;
    bool m_autoSave;
    
    // 初始化解码器
    bool initDecoder();
    
    // 更新统计信息显示
    void updateStatistics(const FrameData& frame);
    
    // 静态回调函数（C风格）
    static void frameCallback(const V192Frame* frame, void* user_data);
    
    // 文件保存功能
    QString createTimestampedDirectory();
    bool saveFrameToTxt(const FrameData& frame, const QString& filename);
    bool exportToPCD(const FrameData& frame, const QString& filename);
    bool exportToPLY(const FrameData& frame, const QString& filename);
};

#endif // MAINWINDOW_H
