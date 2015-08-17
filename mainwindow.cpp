#include "mainwindow.h"

#define BUTTONSIZE 190
#define PADDING 10
#define SMALLBUTTONSIZE 30
#define MAXH 210
#define MAXW 480
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

    openDirButton = new QPushButton(this);
    openDirButton->setVisible(true);
    openDirButton->setGeometry(MAXH, PADDING, BUTTONSIZE, BUTTONSIZE);
    openDirButton->setText("Open Directory");

    closeButton = new QPushButton(this);
    closeButton->setVisible(true);
    closeButton->setGeometry(MAXW-SMALLBUTTONSIZE*2-PADDING, PADDING, SMALLBUTTONSIZE*2, SMALLBUTTONSIZE);
    closeButton->setText("close");

    optionsButton = new QPushButton(this);
    optionsButton->setVisible(true);
    optionsButton->setGeometry(MAXW-SMALLBUTTONSIZE*2-PADDING, PADDING*2 +SMALLBUTTONSIZE, SMALLBUTTONSIZE*2, SMALLBUTTONSIZE);
    optionsButton->setText("options");

    optionsButton = new QPushButton(this);
    optionsButton->setVisible(true);
    optionsButton->setGeometry(MAXW-SMALLBUTTONSIZE*2-PADDING, PADDING*3 +SMALLBUTTONSIZE*2, SMALLBUTTONSIZE*2, SMALLBUTTONSIZE*3+PADDING*2);
    optionsButton->setText("log");

    openDirDlg= new QFileDialog(this,tr("Open file"),
                                QStandardPaths::locate(QStandardPaths::DesktopLocation, QString(), QStandardPaths::LocateDirectory),
                                tr("bin files (*.bin)"));

    connect(closeButton, SIGNAL(clicked(bool)), this, SLOT(onCloseButtonClicked()));
    connect(openDirButton, SIGNAL(clicked(bool)), openDirDlg, SLOT(open()));

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
