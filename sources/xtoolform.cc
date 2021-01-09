#include <QtUiTools>
#include <QSpinBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QFile>
#include <QAction>
#include <QPushButton>
#include <QLineEdit>
#include <QFileDialog>
#include <QListWidget>
#include <QTextBrowser>

#include "xtoolform.h"
#include "serialform.h"
#include "transferform.h"
#include "xmlparse.h"
#include "xmlwidget.h"

#include "serial.h"


XToolForm::XToolForm(QWidget *parent)
    : QMainWindow(parent), serial_(nullptr)
{
    QUiLoader loader;

    QFile file(":/forms/xtoolform.ui");
    if (!file.open(QFile::ReadOnly)) {
        //logger()->debug(tr("Failed to open UI file."));
        return;
    }

    QWidget *widget = loader.load(&file, this);
    file.close();
    if (widget == nullptr)
        return;

    //File menu
    open_ = findChild<QAction *>("actionOpen");
    exit_ = findChild<QAction *>("actionExit");

    //Port menu
    conn_ = findChild<QAction *>("actionConnect");

    //About
    about_ = findChild<QAction *>("actionAbout");

    //File transmit group
    transfer_ = findChild<QAction *>("actionSend_File");

    list_widget_ = findChild<QListWidget *>("listWidget");
    text_browser_ = findChild<QTextBrowser *>("textBrowser");

    //Connect events
    connect(exit_, &QAction::triggered, this, &QMainWindow::close);
    connect(conn_, &QAction::triggered, this, &XToolForm::OnActionConnect);
    connect(transfer_, &QAction::triggered, this, &XToolForm::OnActionTransfer);
    connect(open_, &QAction::triggered, this, &XToolForm::OnOpenFile);
    connect(list_widget_, &QListWidget::itemClicked, this, &XToolForm::OnListActived);

    widget->setWindowFlags(Qt::SubWindow);
    setWindowTitle(tr("xtool"));
    //setCentralWidget(widget);
}

XToolForm::~XToolForm()
{
    if (serial_)
        delete serial_;
    if (xml_)
        delete xml_;
}

void XToolForm::OnActionConnect()
{
    SerialForm *form = new SerialForm();
    form->SetMasterForm(this);
    form->showNormal();
}

void XToolForm::OnActionTransfer()
{
    TransferForm *form = new TransferForm();
    form->showNormal();
}

void XToolForm::OnOpenFile()
{
    QFileDialog fdlg(this);
    fdlg.setWindowTitle("Configure File");
    if (filename_.isEmpty())
        fdlg.setDirectory(".");
    else
        fdlg.setDirectory(filename_);

    //Filter file name
    fdlg.setNameFilter("File(*.xml)");
    fdlg.setFileMode(QFileDialog::ExistingFiles);
    fdlg.setViewMode(QFileDialog::Detail);

    if (fdlg.exec()) {
        QStringList list = fdlg.selectedFiles();
        if (!list.isEmpty()) {
            filename_ = list.takeFirst();
            xml_ = new XmlParse(filename_);
            if (xml_->ParseFile()) {
                CreateWidgetList();
                return;
            }
            delete xml_;
            xml_ = nullptr;
        }
    }
}

void XToolForm::CreateWidgetList()
{
    if (!list_widget_)
        return;

    for (XmlDataNode *node : xml_->root()) {
        if (node) {
            XmlWidgetItem *item = new XmlWidgetItem(node);
            list_widget_->addItem(item);
        }
    }
}

void XToolForm::OnListActived(QListWidgetItem *item)
{
    XmlWidgetItem *xml_widget = static_cast<XmlWidgetItem *>(item);

    xml_widget->minor();
    text_browser_->clear();
    text_browser_->append("Major:" + xml_widget->major());
    text_browser_->append("Timeout:" + xml_widget->timeout());
    for (XmlDataNode::MinorItem *minor : xml_widget->minor()) {
        if (minor) {
           text_browser_->append("Minor:" + minor->minor);
           text_browser_->append("Length:" + minor->length);
           QString data("Data：");
           if (!minor->data.isEmpty())
               data.append(minor->data);
           text_browser_->append(data);
           text_browser_->append("Expect:" + minor->expect);
        }
    }
}
