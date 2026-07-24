#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_decoder(nullptr)
    , m_updateTimer(nullptr)
    , m_hasNewFrame(false)
    , m_frameCount(0)
    , m_lastTimestamp(0.0)
    , m_autoSave(false)
{
    ui->setupUi(this);
    
    // 初始化解码器
    if (!initDecoder()) {
        QMessageBox::critical(this, "错误", "无法初始化V192解码器");
        return;
    }
    
    // 创建更新定时器
    m_updateTimer = new QTimer(this);
    connect(m_updateTimer, &QTimer::timeout, this, &MainWindow::onUpdateUI);
    m_updateTimer->start(100); // 每100ms更新一次UI
    
    // 连接信号槽
    connect(ui->btnStartOnline, &QPushButton::clicked, this, &MainWindow::onStartOnlineClicked);
    connect(ui->btnStopOnline, &QPushButton::clicked, this, &MainWindow::onStopOnlineClicked);
    connect(ui->btnOpenPcap, &QPushButton::clicked, this, &MainWindow::onOpenPcapClicked);
    connect(ui->btnClosePcap, &QPushButton::clicked, this, &MainWindow::onClosePcapClicked);
    connect(ui->btnReadNextFrame, &QPushButton::clicked, this, &MainWindow::onReadNextFrameClicked);
    connect(ui->btnSaveFrame, &QPushButton::clicked, this, &MainWindow::onSaveFrameClicked);
    connect(ui->btnExportPCD, &QPushButton::clicked, this, &MainWindow::onExportPCDClicked);
    connect(ui->btnExportPLY, &QPushButton::clicked, this, &MainWindow::onExportPLYClicked);
    connect(ui->chkAutoSave, &QCheckBox::toggled, this, &MainWindow::onToggleAutoSaveClicked);
    
    // 初始状态
    ui->btnStopOnline->setEnabled(false);
    ui->btnClosePcap->setEnabled(false);
    ui->btnReadNextFrame->setEnabled(false);
    
    // 设置默认端口
    ui->spinPort->setValue(8899);
    
    // 设置距离范围
    ui->spinMinDist->setValue(0.1);
    ui->spinMaxDist->setValue(300.0);
}

MainWindow::~MainWindow()
{
    if (m_decoder) {
        v192_stop(m_decoder);
        v192_close_pcap(m_decoder);
        v192_destroy(m_decoder);
    }
    delete ui;
}

bool MainWindow::initDecoder()
{
    // 创建解码器（使用默认校准参数）
    m_decoder = v192_create(nullptr);
    if (!m_decoder) {
        return false;
    }
    
    // 设置距离范围
    v192_set_distance_range(m_decoder, 0.1f, 300.0f);
    
    // 注册回调函数
    v192_set_frame_callback(m_decoder, MainWindow::frameCallback, this);
    
    return true;
}

void MainWindow::frameCallback(const V192Frame* frame, void* user_data)
{
    MainWindow* window = static_cast<MainWindow*>(user_data);
    if (!window) return;
    
    // 复制帧数据
    FrameData fd;
    fd.frame_id = frame->frame_id;
    fd.timestamp = frame->timestamp;
    fd.count = frame->count;
    fd.is_dual = frame->is_dual != 0;
    fd.points.assign(frame->points, frame->points + frame->count);
    
    // 线程安全地存储最新帧
    {
        QMutexLocker locker(&window->m_frameMutex);
        window->m_latestFrame = std::move(fd);
        window->m_hasNewFrame = true;
    }
    
    // 通过信号槽通知主线程（如果需要立即更新）
    // QMetaObject::invokeMethod(window, "onFrameReceived", Qt::QueuedConnection, Q_ARG(FrameData, fd));
}

void MainWindow::onStartOnlineClicked()
{
    uint16_t port = static_cast<uint16_t>(ui->spinPort->value());
    
    // 更新距离范围
    float minDist = static_cast<float>(ui->spinMinDist->value());
    float maxDist = static_cast<float>(ui->spinMaxDist->value());
    v192_set_distance_range(m_decoder, minDist, maxDist);
    
    // 启动在线接收
    if (v192_start_online(m_decoder, port) != 0) {
        QMessageBox::critical(this, "错误", QString("无法启动UDP接收，端口: %1").arg(port));
        return;
    }
    
    ui->btnStartOnline->setEnabled(false);
    ui->btnStopOnline->setEnabled(true);
    ui->btnOpenPcap->setEnabled(false);
    ui->statusbar->showMessage(QString("正在接收UDP数据，端口: %1").arg(port));
    
    // 重置统计
    m_frameCount = 0;
    m_lastTimestamp = 0.0;
}

void MainWindow::onStopOnlineClicked()
{
    v192_stop(m_decoder);
    
    ui->btnStartOnline->setEnabled(true);
    ui->btnStopOnline->setEnabled(false);
    ui->btnOpenPcap->setEnabled(true);
    ui->statusbar->showMessage("已停止接收");
}

void MainWindow::onOpenPcapClicked()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "选择PCAP文件",
        "",
        "PCAP Files (*.pcap *.pcapng);;All Files (*.*)"
    );
    
    if (fileName.isEmpty()) {
        return;
    }
    
    // 更新距离范围
    float minDist = static_cast<float>(ui->spinMinDist->value());
    float maxDist = static_cast<float>(ui->spinMaxDist->value());
    v192_set_distance_range(m_decoder, minDist, maxDist);
    
    // 打开PCAP文件
    if (v192_open_pcap(m_decoder, fileName.toUtf8().constData()) != 0) {
        QMessageBox::critical(this, "错误", "无法打开PCAP文件");
        return;
    }
    
    ui->btnOpenPcap->setEnabled(false);
    ui->btnClosePcap->setEnabled(true);
    ui->btnReadNextFrame->setEnabled(true);
    ui->btnStartOnline->setEnabled(false);
    ui->statusbar->showMessage(QString("已打开: %1").arg(fileName));
    
    // 重置统计
    m_frameCount = 0;
    m_lastTimestamp = 0.0;
}

void MainWindow::onClosePcapClicked()
{
    v192_close_pcap(m_decoder);
    
    ui->btnOpenPcap->setEnabled(true);
    ui->btnClosePcap->setEnabled(false);
    ui->btnReadNextFrame->setEnabled(false);
    ui->btnStartOnline->setEnabled(true);
    ui->statusbar->showMessage("已关闭PCAP文件");
}

void MainWindow::onReadNextFrameClicked()
{
    int result = v192_read_next_frame(m_decoder);
    
    if (result == 1) {
        // 文件结束
        QMessageBox::information(this, "信息", "已到达文件末尾");
        onClosePcapClicked();
    } else if (result == -1) {
        // 错误
        QMessageBox::critical(this, "错误", "读取帧时发生错误");
    }
    // result == 0 表示成功读取一帧，回调函数会被调用
}

void MainWindow::onUpdateUI()
{
    // 检查是否有新帧
    FrameData frame;
    bool hasFrame = false;
    
    {
        QMutexLocker locker(&m_frameMutex);
        if (m_hasNewFrame) {
            frame = m_latestFrame;
            hasFrame = true;
            m_hasNewFrame = false;
        }
    }
    
    if (hasFrame) {
        updateStatistics(frame);
        
        // 自动保存功能
        if (m_autoSave && !m_outputDir.isEmpty()) {
            QString filename = QString("%1/frame_%2.txt")
                .arg(m_outputDir)
                .arg(frame.frame_id, 5, 10, QChar('0'));
            saveFrameToTxt(frame, filename);
        }
    }
}

void MainWindow::updateStatistics(const FrameData& frame)
{
    m_frameCount++;
    
    // 计算帧率
    double fps = 0.0;
    if (m_lastTimestamp > 0.0) {
        double dt = frame.timestamp - m_lastTimestamp;
        if (dt > 0.0) {
            fps = 1.0 / dt;
        }
    }
    m_lastTimestamp = frame.timestamp;
    
    // 更新基本信息
    ui->lblFrameId->setText(QString::number(frame.frame_id));
    ui->lblFrameCount->setText(QString::number(m_frameCount));
    ui->lblPointCount->setText(QString::number(frame.count));
    ui->lblFPS->setText(QString::number(fps, 'f', 2));
    ui->lblDualEcho->setText(frame.is_dual ? "是" : "否");
    
    // 统计有效点数和距离范围
    uint32_t validPoints = 0;
    float minDist = 1000.0f;
    float maxDist = 0.0f;
    
    for (const auto& p : frame.points) {
        if (p.distance > 0.01f) {
            validPoints++;
            minDist = std::min(minDist, p.distance);
            maxDist = std::max(maxDist, p.distance);
        }
    }
    
    ui->lblValidPoints->setText(QString::number(validPoints));
    
    if (validPoints > 0) {
        ui->lblDistRange->setText(QString("%1 - %2 m")
            .arg(minDist, 0, 'f', 2)
            .arg(maxDist, 0, 'f', 2));
    } else {
        ui->lblDistRange->setText("N/A");
    }
    
    // 显示前几个有效点的详细信息
    QString pointDetails;
    int displayCount = 0;
    for (uint32_t i = 0; i < frame.count && displayCount < 10; ++i) {
        const V192Point& p = frame.points[i];
        if (p.distance < 0.01f) continue;
        
        pointDetails += QString("Ring %1: dist=%2m, az=%3°, el=%4°, xyz=(%5, %6, %7), refl=%8\n")
            .arg(p.ring, 3)
            .arg(p.distance, 0, 'f', 2)
            .arg(p.azimuth, 0, 'f', 1)
            .arg(p.elevation, 0, 'f', 1)
            .arg(p.x, 0, 'f', 2)
            .arg(p.y, 0, 'f', 2)
            .arg(p.z, 0, 'f', 2)
            .arg(static_cast<int>(p.reflectivity));
        
        displayCount++;
    }
    
    ui->textPointDetails->setPlainText(pointDetails);
}

void MainWindow::onFrameReceived(const FrameData& frame)
{
    // 这个槽函数可以用于需要立即响应的场景
    // 当前实现使用定时器轮询，所以这里暂时不用
    Q_UNUSED(frame);
}

// ====================================================================
//  文件保存功能实现
// ====================================================================

QString MainWindow::createTimestampedDirectory()
{
    // 获取当前时间
    QDateTime now = QDateTime::currentDateTime();
    QString dirName = now.toString("yyyyMMdd_HHmmss");
    
    // 创建目录
    QDir dir;
    if (!dir.mkpath(dirName)) {
        qWarning() << "Failed to create directory:" << dirName;
        return QString();
    }
    
    return dirName;
}

bool MainWindow::saveFrameToTxt(const FrameData& frame, const QString& filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Failed to open file for writing:" << filename;
        return false;
    }
    
    QTextStream out(&file);
    
    // 写入头部信息
    out << "# frame_id: " << frame.frame_id
        << "  timestamp: " << QString::number(frame.timestamp, 'f', 6)
        << "  total_points: " << frame.count << "\n";
    out << "# ring  dist(m)  az(deg)  el(deg)  x  y  z  refl\n";
    
    // 写入点数据
    for (uint32_t i = 0; i < frame.count; ++i) {
        const V192Point& p = frame.points[i];
        if (p.distance < 0.01f) continue;
        
        out << p.ring << "  "
            << QString::number(p.distance, 'f', 4) << "  "
            << QString::number(p.azimuth, 'f', 4) << "  "
            << QString::number(p.elevation, 'f', 4) << "  "
            << QString::number(p.x, 'f', 4) << "  "
            << QString::number(p.y, 'f', 4) << "  "
            << QString::number(p.z, 'f', 4) << "  "
            << p.reflectivity << "\n";
    }
    
    file.close();
    return true;
}

bool MainWindow::exportToPCD(const FrameData& frame, const QString& filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Failed to open file for writing:" << filename;
        return false;
    }
    
    QTextStream out(&file);
    
    // 统计有效点数
    uint32_t validPoints = 0;
    for (uint32_t i = 0; i < frame.count; ++i) {
        if (frame.points[i].distance > 0.01f) {
            validPoints++;
        }
    }
    
    // PCD文件头
    out << "# .PCD v0.7 - Point Cloud Data file format\n";
    out << "VERSION 0.7\n";
    out << "FIELDS x y z intensity\n";
    out << "SIZE 4 4 4 4\n";
    out << "TYPE F F F U\n";
    out << "COUNT 1 1 1 1\n";
    out << "WIDTH " << validPoints << "\n";
    out << "HEIGHT 1\n";
    out << "VIEWPOINT 0 0 0 1 0 0 0\n";
    out << "POINTS " << validPoints << "\n";
    out << "DATA ascii\n";
    
    // 写入点数据
    for (uint32_t i = 0; i < frame.count; ++i) {
        const V192Point& p = frame.points[i];
        if (p.distance < 0.01f) continue;
        
        out << QString::number(p.x, 'f', 6) << " "
            << QString::number(p.y, 'f', 6) << " "
            << QString::number(p.z, 'f', 6) << " "
            << p.reflectivity << "\n";
    }
    
    file.close();
    return true;
}

bool MainWindow::exportToPLY(const FrameData& frame, const QString& filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Failed to open file for writing:" << filename;
        return false;
    }
    
    QTextStream out(&file);
    
    // 统计有效点数
    uint32_t validPoints = 0;
    for (uint32_t i = 0; i < frame.count; ++i) {
        if (frame.points[i].distance > 0.01f) {
            validPoints++;
        }
    }
    
    // PLY文件头
    out << "ply\n";
    out << "format ascii 1.0\n";
    out << "comment V192 Point Cloud Data\n";
    out << "element vertex " << validPoints << "\n";
    out << "property float x\n";
    out << "property float y\n";
    out << "property float z\n";
    out << "property uchar intensity\n";
    out << "end_header\n";
    
    // 写入点数据
    for (uint32_t i = 0; i < frame.count; ++i) {
        const V192Point& p = frame.points[i];
        if (p.distance < 0.01f) continue;
        
        out << QString::number(p.x, 'f', 6) << " "
            << QString::number(p.y, 'f', 6) << " "
            << QString::number(p.z, 'f', 6) << " "
            << p.reflectivity << "\n";
    }
    
    file.close();
    return true;
}

void MainWindow::onSaveFrameClicked()
{
    QMutexLocker locker(&m_frameMutex);
    if (!m_hasNewFrame) {
        QMessageBox::information(this, "提示", "没有可用的帧数据");
        return;
    }
    
    // 创建时间戳文件夹（如果还没有）
    if (m_outputDir.isEmpty()) {
        m_outputDir = createTimestampedDirectory();
        if (m_outputDir.isEmpty()) {
            QMessageBox::critical(this, "错误", "无法创建输出目录");
            return;
        }
    }
    
    // 保存当前帧
    QString filename = QString("%1/frame_%2.txt")
        .arg(m_outputDir)
        .arg(m_latestFrame.frame_id, 5, 10, QChar('0'));
    
    if (saveFrameToTxt(m_latestFrame, filename)) {
        ui->statusbar->showMessage(QString("已保存: %1").arg(filename), 3000);
    } else {
        QMessageBox::critical(this, "错误", "保存文件失败");
    }
}

void MainWindow::onExportPCDClicked()
{
    QMutexLocker locker(&m_frameMutex);
    if (!m_hasNewFrame) {
        QMessageBox::information(this, "提示", "没有可用的帧数据");
        return;
    }
    
    QString filename = QFileDialog::getSaveFileName(
        this,
        "导出为PCD文件",
        QString("frame_%1.pcd").arg(m_latestFrame.frame_id, 5, 10, QChar('0')),
        "PCD Files (*.pcd);;All Files (*.*)"
    );
    
    if (filename.isEmpty()) {
        return;
    }
    
    if (exportToPCD(m_latestFrame, filename)) {
        QMessageBox::information(this, "成功", "PCD文件导出成功");
    } else {
        QMessageBox::critical(this, "错误", "导出PCD文件失败");
    }
}

void MainWindow::onExportPLYClicked()
{
    QMutexLocker locker(&m_frameMutex);
    if (!m_hasNewFrame) {
        QMessageBox::information(this, "提示", "没有可用的帧数据");
        return;
    }
    
    QString filename = QFileDialog::getSaveFileName(
        this,
        "导出为PLY文件",
        QString("frame_%1.ply").arg(m_latestFrame.frame_id, 5, 10, QChar('0')),
        "PLY Files (*.ply);;All Files (*.*)"
    );
    
    if (filename.isEmpty()) {
        return;
    }
    
    if (exportToPLY(m_latestFrame, filename)) {
        QMessageBox::information(this, "成功", "PLY文件导出成功");
    } else {
        QMessageBox::critical(this, "错误", "导出PLY文件失败");
    }
}

void MainWindow::onToggleAutoSaveClicked(bool checked)
{
    m_autoSave = checked;
    
    if (m_autoSave) {
        // 创建时间戳文件夹
        m_outputDir = createTimestampedDirectory();
        if (m_outputDir.isEmpty()) {
            QMessageBox::critical(this, "错误", "无法创建输出目录");
            ui->chkAutoSave->setChecked(false);
            m_autoSave = false;
            return;
        }
        ui->statusbar->showMessage(QString("自动保存已启用，输出目录: %1").arg(m_outputDir));
    } else {
        ui->statusbar->showMessage("自动保存已禁用");
    }
}
