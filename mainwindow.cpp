#include "mainwindow.h"

#define BUTTONSIZE 170
#define PADDING 10
#define SMALLBUTTONSIZE 30
#define MAXH 190
#define MAXW 440



MainWindow::MainWindow(QWidget *parent)
    : QFrame(parent)
{
    initDefaultStyle();

    initContent();

}

void MainWindow::initDefaultStyle(){
    QDesktopWidget *d = QApplication::desktop();
    int desctopW = d->width();
    int desctopH = d->height();
    delete d;

    mainWindowSize.setHeight(MAXH);
    mainWindowSize.setWidth(MAXW);

    mainWindownStartPoint.setY(desctopH/2 - mainWindowSize.height()/2);
    mainWindownStartPoint.setX(desctopW/2 - mainWindowSize.width()/2);

    setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    setWindowFlags(Qt::FramelessWindowHint);
    resize(mainWindowSize);
    move(mainWindownStartPoint);

    QString buttonQss;
    QString frameQss;
    frameQss = "QFrame{background-color:#035280;"
               "border-style: none;}";
    buttonQss = "QPushButton{"
                "border-style: none;"
                "border-width: 0px;"
                "background-color: #022f49;"
                "color: white}"
                "QPushButton:hover{"
                "background-color: #023D5E}";

    setStyleSheet(buttonQss+frameQss);
}

void MainWindow::initContent(){

    openFileButton = new QPushButton(this);
    openFileButton->setVisible(true);
    openFileButton->setGeometry(PADDING, PADDING, BUTTONSIZE, BUTTONSIZE);
    openFileButton->setText("Open File");
    openFileButton->setToolTip("Open single file to parse");

    openDirButton = new QPushButton(this);
    openDirButton->setVisible(true);
    openDirButton->setGeometry(MAXH, PADDING, BUTTONSIZE, BUTTONSIZE);
    openDirButton->setText("Open Directory");
    openDirButton->setToolTip("Open all files in current folder");

    closeButton = new QPushButton(this);
    closeButton->setVisible(true);
    closeButton->setGeometry(MAXW-SMALLBUTTONSIZE*2-PADDING, PADDING, SMALLBUTTONSIZE*2, SMALLBUTTONSIZE);
    closeButton->setText("close");
    closeButton->setToolTip("Close programm");
    closeButton->setStyleSheet("QPushButton{"
                               "border-style: none;"
                               "border-width: 0px;"
                               "background-color: #800000;"
                               "color: white}"
                               "QPushButton:hover{"
                               "background-color: #990000}");

    optionsButton = new QPushButton(this);
    optionsButton->setVisible(true);
    optionsButton->setGeometry(MAXW-SMALLBUTTONSIZE*2-PADDING, PADDING*2 +SMALLBUTTONSIZE, SMALLBUTTONSIZE*2, SMALLBUTTONSIZE*2);
    optionsButton->setText("options");
    optionsButton->setToolTip("Setup");

    optionsButton = new QPushButton(this);
    optionsButton->setVisible(true);
    optionsButton->setGeometry(MAXW-SMALLBUTTONSIZE*2-PADDING, PADDING*3 +SMALLBUTTONSIZE*3, SMALLBUTTONSIZE*2, SMALLBUTTONSIZE*2);
    optionsButton->setText("log");
    optionsButton->setToolTip("Show log");

    pathToFolder = "//fs/Group Projects/UBS/База конфигураций";//QStandardPaths::locate(QStandardPaths::DesktopLocation, QString(), QStandardPaths::LocateDirectory)

    openDirDlg = new QFileDialog(this,tr("Open file"), pathToFolder);
    openFileDlg = new QFileDialog(this,tr("Open file"), pathToFolder, tr("bin files (*.bin)"));

    openDirDlg->setFileMode(QFileDialog::DirectoryOnly);

    connect(closeButton, SIGNAL(clicked(bool)), this, SLOT(onCloseButtonClicked()));
    connect(openDirButton, SIGNAL(clicked(bool)), openDirDlg, SLOT(open()));
    connect (openFileButton, SIGNAL(clicked(bool)), openFileDlg, SLOT(open()));

}

MainWindow::~MainWindow()
{
    delete openDirButton;
    delete openFileButton;
    delete openDirDlg;
}

void MainWindow::onCloseButtonClicked()
{
    exit(0);
}

bool MainWindow::onOpenFileButtonClicked()
{

    return true;
}

bool MainWindow::onOpenDirButtonClicked()
{
    return true;
}
