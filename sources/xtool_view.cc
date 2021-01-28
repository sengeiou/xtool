#include <QFileDialog>
#include <QPushButton>
#include <QTextBrowser>
#include <QString>
#include <QToolBar>
#include <QSpinBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QAction>
#include <QLineEdit>
#include <QListWidget>
#include <QCheckBox>
#include <QSpacerItem>
#include <QString>

#include "xtool_view.h"
#include "xtool_model.h"
#include "ui_xtoolform.h"

XToolView::XToolView(QWidget *parent)
    : QMainWindow(parent),
      ui_(new Ui::MainWindow)
{
    ui_->setupUi(this);
    open_ = ui_->actionOpen;
    exit_ = ui_->actionExit;
    conn_ = ui_->actionConnect;
    about_ = ui_->actionAbout;
    transfer_ = ui_->actionSend_File;
    list_widget_ = ui_->listWidget;
    text_browser_ = ui_->textBrowser;
    item_text_browser_ = ui_->textBrowserItem;
    start_btn_ = ui_->pushButtonStart;
    skip_chkbox_ = ui_->checkBoxSkipBox;
    stop_btn_ = ui_->pushButtonStop;
    stop_btn_->setEnabled(false);

    QToolBar *toolbar = ui_->toolBar;
    toolbar_conn_ = toolbar->addAction(QIcon(":/images/connect.png"), "Connect");
    toolbar_disconn_ = toolbar->addAction(QIcon(":/images/disconnect.png"), "Disconnect");
    toolbar_setting_ = toolbar->addAction(QIcon(":/images/settings.png"), "Setting");
    toolbar_clear_ = toolbar->addAction(QIcon(":/images/clear.png"), "Clear");
    toolbar_disconn_->setEnabled(false);
    toolbar_conn_->setEnabled(true);

    //StatusBar
    QStatusBar *statusbar = ui_->statusBar;
    status_label_ = new QLabel("Disconnected");
    statusbar->addPermanentWidget(status_label_);

    //Set right-click menu for list-widget item
    list_widget_->setContextMenuPolicy(Qt::CustomContextMenu);
    setWindowIcon(QIcon(":/images/main_icon.png"));
    connect(list_widget_, &QListWidget::currentItemChanged,
            this, &XToolView::OnExecuteItemChanged);
}

XToolView::~XToolView()
{
    delete ui_;
}

bool XToolView::OpenFileSystem(QString *filename,
                               const QString &filter,
                               const QString &title)
{
    if (filename == nullptr)
        return false;

    QFileDialog fs(this);
    if (filename->isEmpty())
        fs.setDirectory(".");
    else
        fs.setDirectory(*filename);
    fs.setWindowTitle(title);
    fs.setNameFilter(filter);
    fs.setFileMode(QFileDialog::ExistingFiles);
    fs.setViewMode(QFileDialog::Detail);
    if (!fs.exec()) {
        filename->clear();
        return false;
    }
    QStringList list = fs.selectedFiles();
    if (list.isEmpty())
        return false;

    *filename = list.takeFirst();
    return true;
}

void XToolView::CreateListWidget(QListWidgetItem *item)
{
    item->setIcon(QIcon(":/images/bpoff.png"));
    list_widget_->addItem(item);
}

void XToolView::StartListProcess(QListWidgetItem *item)
{
    QBrush brush(QColor(255, 0, 255, 127));
    item->setBackground(brush);
    start_btn_->setEnabled(false);
    stop_btn_->setEnabled(true);
}

void XToolView::StopListProcess(QListWidgetItem *item)
{
    QBrush brush(QColor(255, 255, 255));
    item->setBackground(brush);
    stop_btn_->setEnabled(false);
    start_btn_->setEnabled(true);
}

void XToolView::SetSerialPortStatus(bool ready)
{
    if (ready) {
        toolbar_conn_->setEnabled(false);
        toolbar_disconn_->setEnabled(true);
    } else {
        toolbar_conn_->setEnabled(true);
        toolbar_disconn_->setEnabled(false);
    }
}

void XToolView::SetStatusBarText(const QString &text)
{
    status_label_->setText(text);
}

void XToolView::ClearLogText()
{
    text_browser_->clear();
}

void XToolView::ClearListWidget()
{
    list_widget_->clear();
}

QListWidgetItem *XToolView::current() const
{
    return list_widget_->currentItem();
}

QListWidgetItem *XToolView::next() const
{
    int next_row = list_widget_->currentRow() + 1;
    if (next_row < list_widget_->count()) {
        list_widget_->setCurrentRow(next_row);
        return list_widget_->item(next_row);
    }
    return nullptr;
}

QListWidgetItem *XToolView::item_at(const QPoint &point)
{
    return list_widget_->itemAt(point);
}

void XToolView::OnExecuteItemChanged(QListWidgetItem *curr,
                                     QListWidgetItem *prev)
{
    if (stop_btn_->isEnabled()) {
        if (prev) {
            QBrush prev_brush(QColor(255, 255, 255));
            prev->setBackground(prev_brush);
        }
        if (curr) {
            QBrush curr_brush(QColor(255, 0, 255, 127));
            curr->setBackground(curr_brush);
        }
    }
}
