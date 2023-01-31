#include "TaskTable.h"
#include "Settings.h"
#include "utils.h"
#include <QAction>
#include <QtWidgets>
#include "DownloadTask.h"


static constexpr int MaxConcurrentTaskCount = 3;
static constexpr int SaveTasksInterval = 5000; // ms

static constexpr int DownRateTimerInterval = 500; // ms
static constexpr int DownRateWindowLength = 10;

TaskTableWidget::TaskTableWidget(QWidget *parent)
    : QTableWidget(parent)
{
    this->horizontalHeader()->hide();
    this->verticalHeader()->hide();
    setColumnCount(1);
    horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    setFocusPolicy(Qt::NoFocus);
    setStyleSheet( "QTableWidget {"
        "selection-background-color: rgb(227, 227, 229);"
    "}");

    startAllAct = new QAction(QString::fromLocal8Bit("ȫ����ʼ"));
    stopAllAct = new QAction(QString::fromLocal8Bit("ȫ����ͣ"));
    removeAllAct = new QAction(QString::fromLocal8Bit("ȫ��ɾ��"));
    connect(startAllAct, &QAction::triggered, this, &TaskTableWidget::startAll);
    connect(stopAllAct, &QAction::triggered, this, &TaskTableWidget::stopAll);
    connect(removeAllAct, &QAction::triggered, this, &TaskTableWidget::removeAll);

    saveTasksTimer = new QTimer(this);
    saveTasksTimer->setInterval(SaveTasksInterval);
    saveTasksTimer->setSingleShot(false);
    connect(saveTasksTimer, &QTimer::timeout, this, &TaskTableWidget::save);
}
void TaskTableWidget::setDirty()
{
    if (dirty) { return; }
    dirty = true;
    if (!saveTasksTimer->isActive()) {
        saveTasksTimer->start();
    }
}
void TaskTableWidget::save()
{
   
}

void TaskTableWidget::load()
{
    auto settings = Settings::inst();
    auto array = QJsonDocument::fromJson(settings->value("tasks").toByteArray()).array();
    QList<AbstractDownloadTask*> tasks;
    for (auto obj : array) {
        auto task = AbstractDownloadTask::fromJsonObj(obj.toObject());
        if (task != nullptr) {
            tasks.append(task);
        }
    }
    addTasks(tasks, false);
}

void TaskTableWidget::addTasks(const QList<AbstractDownloadTask*>& tasks, bool activate)
{
    auto shouldSetDirty = false;
    auto rowHt = TaskCellWidget::cellHeight();
    for (auto task : tasks) {
        auto cell = new TaskCellWidget(task);
    //    int idx = rowCount();
    //    insertRow(idx);
    //    setRowHeight(idx, rowHt);
    //    setCellWidget(idx, 0, cell);

    //    connect(cell, &TaskCellWidget::downloadStopped, this, &TaskTableWidget::onCellTaskStopped);
    //    connect(cell, &TaskCellWidget::downloadFinished, this, &TaskTableWidget::onCellTaskFinished);
    //    connect(cell, &TaskCellWidget::startBtnClicked, this, &TaskTableWidget::onCellStartBtnClicked);
    //    connect(cell, &TaskCellWidget::removeBtnClicked, this, &TaskTableWidget::onCellRemoveBtnClicked);

    //    if (activate) {
    //        if (activeTaskCnt < MaxConcurrentTaskCount) {
    //            activeTaskCnt++;
    //            shouldSetDirty = true;
    //            cell->startDownload();
    //        }
    //        else {
    //            cell->setWaitState();
    //        }
    //    }
    }

    if (shouldSetDirty) {
        setDirty();
    }
}


void TaskTableWidget::stopAll()
{
   
}

void TaskTableWidget::startAll()
{
    auto shouldSetDirty = false;
    
      
}

void TaskTableWidget::removeAll()
{
    auto rowCnt = rowCount();
   
  
}

int TaskCellWidget::cellHeight()
{
    auto lineSpacing = QFontMetrics(QApplication::font()).lineSpacing();
    auto style = QApplication::style();
    auto layoutTopMargin = style->pixelMetric(QStyle::PM_LayoutTopMargin);
    auto layoutBtmMargin = style->pixelMetric(QStyle::PM_LayoutBottomMargin);
    auto layoutSpacing = style->pixelMetric(QStyle::PM_LayoutVerticalSpacing);
    return lineSpacing * 2 + layoutTopMargin + layoutSpacing + layoutBtmMargin + 2;
}

static void flattenPushButton(QPushButton* btn)
{
    btn->setCursor(Qt::PointingHandCursor);
    btn->setFlat(true);
    btn->setStyleSheet("QPushButton:pressed{border:none; }");
}


TaskCellWidget::TaskCellWidget(AbstractDownloadTask* task, QWidget* parent)
    : QWidget(parent), task(task)
{
    auto fm = fontMetrics();
    auto lineSpacing = fontMetrics().lineSpacing();
    auto mainLayout = new QHBoxLayout(this);

    iconButton = new QPushButton;
    iconButton->setFixedSize(32, 32);
    iconButton->setIconSize(QSize(32, 32));
    iconButton->setToolTip("��");
    if (qobject_cast<ComicDownloadTask*>(task)) {
       iconButton->setIcon(QIcon(":/icons/manga.svg"));
    }
    else {
        iconButton->setIcon(QIcon(":/icons/video.svg"));
    }
    flattenPushButton(iconButton);
    mainLayout->addWidget(iconButton);

    auto leftVLayout = new QVBoxLayout;
    titleLabel = new ElidedTextLabel;
    titleLabel->setText(task->getTitle());
    // titleLabel->setHintWidthToString("ħ����Ůӣ Clear Cardƪ ��01�� Сӣ��͸������");
    auto layoutUnderTitle = new QHBoxLayout;
    qnDescLabel = new QLabel;
    qnDescLabel->setEnabled(false); // set gray color
    progressLabel = new QLabel;
    progressLabel->setEnabled(false); // set gray color
    layoutUnderTitle->addWidget(qnDescLabel, 1);
    layoutUnderTitle->addWidget(progressLabel, 2);
    leftVLayout->addWidget(titleLabel);
    leftVLayout->addLayout(layoutUnderTitle);
    mainLayout->addLayout(leftVLayout, 1);

    auto centerVLayout = new QVBoxLayout;
    auto centerWidgetsSize = QSize(fm.horizontalAdvance("00:00:00++++123.45KB/s"), lineSpacing);
    progressBar = new QProgressBar;
    progressBar->setFixedSize(centerWidgetsSize);
    progressBar->setAlignment(Qt::AlignCenter);
    centerVLayout->addWidget(progressBar);

    statusStackedWidget = new QStackedWidget;
    statusStackedWidget->setFixedSize(centerWidgetsSize);
    statusTextLabel = new ElidedTextLabel("��ͣ��...");
    statusTextLabel->setEnabled(false); // set gray color as default color
    timeLeftLabel = new QLabel;
    timeLeftLabel->setEnabled(false); // set gray color
    downRateLabel = new QLabel;
    downRateLabel->setToolTip("�����ٶ�");
    downRateLabel->setEnabled(false); // set gray color
    downloadStatsWidget = new QWidget;
    auto hLayout = new QHBoxLayout(downloadStatsWidget);
    hLayout->setContentsMargins(0, 0, 0, 0);
    hLayout->addWidget(statusTextLabel);
    hLayout->addWidget(timeLeftLabel);
    hLayout->addStretch(1);
    hLayout->addWidget(downRateLabel);
    statusStackedWidget->addWidget(statusTextLabel);
    statusStackedWidget->addWidget(downloadStatsWidget);
    centerVLayout->addWidget(statusStackedWidget);
    mainLayout->addLayout(centerVLayout);
    mainLayout->addSpacing(20);

    startStopButton = new QPushButton;
    flattenPushButton(startStopButton);
    removeButton = new QPushButton;
    removeButton->setToolTip("ɾ��");
    removeButton->setIcon(QIcon(":/icons/remove.svg"));
    flattenPushButton(removeButton);
    mainLayout->addWidget(startStopButton);
    mainLayout->addWidget(removeButton);

    updateStartStopBtn();
    //initProgressWidgets();
    //qnDescLabel->setText(task->getQnDescription());

    //if (qobject_cast<LiveDownloadTask*>(task)) {
    //    timeLeftLabel->setToolTip("������ʱ��");
    //}
    //else {
    //    timeLeftLabel->setToolTip("ʣ��ʱ��");
    //}

    //downRateTimer = new QTimer(this);
    //downRateTimer->setInterval(DownRateTimerInterval);
    //downRateTimer->setSingleShot(false);
    //downRateWindow.reserve(DownRateWindowLength);

    //connect(iconButton, &QAbstractButton::clicked, this, &TaskCellWidget::open);

    //connect(task, &AbstractDownloadTask::errorOccurred, this, &TaskCellWidget::onErrorOccurred);

    //connect(task, &AbstractDownloadTask::getUrlInfoFinished, this, [this] {
    //    initProgressWidgets();
    //    // titleLabel->setText(this->task->getTitle());
    //    qnDescLabel->setText(this->task->getQnDescription());
    //    startCalcDownRate();
    //    });

    //connect(task, &AbstractDownloadTask::downloadFinished, this, &TaskCellWidget::onFinished);

    //connect(startStopButton, &QAbstractButton::clicked, this, &TaskCellWidget::startStopBtnClicked);

    //connect(removeButton, &QAbstractButton::clicked, this, [this] {
    //    remove();
    //    emit removeBtnClicked();
    //    });

    //connect(downRateTimer, &QTimer::timeout, this, &TaskCellWidget::updateDownloadStats);
}


void TaskCellWidget::updateStartStopBtn()
{
    if (state == State::Waiting || state == State::Downloading) {
        startStopButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
        startStopButton->setToolTip("��ͣ");
    }
    else {
        startStopButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        startStopButton->setToolTip("��ʼ");
    }
}