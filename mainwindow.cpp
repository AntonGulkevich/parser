#include "mainwindow.h"

#define BUTTONSIZE 170
#define PADDING 10
#define SMALLBUTTONSIZE 30
#define MAXH 190
#define MAXW 440
#define HEADER_SIZE 256


MainWindow::MainWindow(QWidget *parent):QFrame(parent)
{
    initDefaultStyle();
    initDefVars();
    initContent();

    autoStart(pathToFolder+"commod.bin");


}

bool MainWindow::autoStart(const QString &fileName)
{
   onOpenFileDialogFinished(fileName);
   return true;
}

bool MainWindow::openFile(const QString &fileName)
{
    //stream binary read
    currentFile = new QFile (fileName);
    if (!currentFile->open(QIODevice::ReadOnly))
    {        
        currentFile->close();
        logEdit->append("Unable to open file.");
        currentFile = NULL;
        return false;
    }
    appendToLog("File "+ fileName+ " opened.");
    return true;
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
    openFileButton->setGeometry(PADDING, PADDING, BUTTONSIZE, BUTTONSIZE);
    openFileButton->setText("Convert File");
    openFileButton->setToolTip("Open single file to parse");

    openDirButton = new QPushButton(this);
    openDirButton->setGeometry(MAXH, PADDING, BUTTONSIZE, BUTTONSIZE);
    openDirButton->setText("Convert files\nin directory");
    openDirButton->setToolTip("Open all files in current folder");

    closeButton = new QPushButton(this);
    closeButton->setGeometry(MAXW-SMALLBUTTONSIZE*2-PADDING, PADDING, SMALLBUTTONSIZE*2, SMALLBUTTONSIZE);
    closeButton->setText("close");
    closeButton->setToolTip("Close programm");
    closeButton->setStyleSheet("QPushButton{"
                               "border-style: none;"
                               "border-width: 0px;"
                               "background-color: #800000;"
                               "color: white}"
                               "QPushButton:hover{"
                               "background-color: #B30000}");

    optionsButton = new QPushButton(this);
    optionsButton->setGeometry(MAXW-SMALLBUTTONSIZE*2-PADDING, PADDING*2 +SMALLBUTTONSIZE, SMALLBUTTONSIZE*2, SMALLBUTTONSIZE*2);
    optionsButton->setText("setup");
    optionsButton->setToolTip("setup");

    logButton = new QPushButton(this);
    logButton->setGeometry(MAXW-SMALLBUTTONSIZE*2-PADDING, PADDING*3 +SMALLBUTTONSIZE*3, SMALLBUTTONSIZE*2, SMALLBUTTONSIZE*2);
    logButton->setText("log");
    logButton->setToolTip("Show log");

    //pathToFolder = "//fs/Group Projects/UBS/База конфигураций";//
    pathToFolder = QStandardPaths::locate(QStandardPaths::DesktopLocation, QString(), QStandardPaths::LocateDirectory);

    openDirDlg = new QFileDialog(this,tr("Open file"), pathToFolder);
    openFileDlg = new QFileDialog(this,tr("Open file"), pathToFolder, tr("bin files (*.bin)"));

    openDirDlg->setFileMode(QFileDialog::DirectoryOnly);

    logEdit = new QTextEdit(this);
    logEdit->setGeometry(PADDING, MAXH, MAXW-PADDING*2, MAXH-PADDING);
    logEdit->setReadOnly(true);
    logEdit->setVisible(false);
    logEdit->setStyleSheet("border-style:solid; border-color:white;border-width:1px; color:white");
    appendToLog("Programm started");


    connect(closeButton, SIGNAL(clicked(bool)), this, SLOT(onCloseButtonClicked()));
    connect(openDirButton, SIGNAL(clicked(bool)), openDirDlg, SLOT(open()));
    connect (openFileButton, SIGNAL(clicked(bool)), openFileDlg, SLOT(open()));
    connect (logButton, SIGNAL(clicked(bool)), this, SLOT(onOpenLogClicked()));
    connect (openFileDlg, SIGNAL(fileSelected(QString)), this, SLOT(onOpenFileDialogFinished(QString)));
}

void MainWindow::initDefVars()
{
    currentFile= NULL;
}

bool MainWindow::saveToFile(const QString &fileName, const QString &src)
{
    QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
    QByteArray encodedString = codec->fromUnicode(src);

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly))
    {
        appendToLog("Unable to save file"+ fileName);
        file.close();
        return false;
    }
    QDataStream out(&file);
    out.writeRawData(encodedString.data(), encodedString.size());
    file.close();
    appendToLog("File" + fileName + " saved");
    return true;

}

void MainWindow::parseCurrentFile()
{
    appendToLog("Parsing started.");
    /*parsing header to vector*/
    QDataStream in(currentFile);

    QByteArray header;
    header.resize(HEADER_SIZE);
    in.readRawData(header.data(), HEADER_SIZE);
    QVector <Fat> headerVector;
    for (int i = 0; (header[i] != 0xFF && i < 64); i+=4)
    {
        Fat f;
        f.address = (static_cast<qint32>(header.at(i) & 0xFF) <<8)
                    + (static_cast<qint32>(header.at(i+1)) <<16);
        f.size =  (static_cast<qint32>(header.at(i+2)) & 0xFF)
                 + (static_cast<qint32>(header.at(i+3))<<8);
        headerVector.push_back(f);
    }
    /*end of parding header*/

    /*parsing data*/
    QByteArray dataToParse;
    int dataSize;
    dataSize = currentFile->size()-HEADER_SIZE;
    dataToParse.resize(dataSize);
    in.readRawData(dataToParse.data(), dataSize);

    QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
    QString textData = codec->toUnicode(dataToParse);
    QStringList dataByParts = textData.split("GENERAL");
    dataByParts.removeFirst();//the firts part is always zero

    for (int i=0; i<dataByParts.count();++i)
    {
        /*kav names*/
        QString temp(dataByParts[i]);
        temp.simplified();
        temp=temp.left(temp.lastIndexOf("\r\nEND\r\n")+7);
        //temp+="\r\nEND\r\n";
        temp.replace(QString(" = "), QString("="));
        int charsToDel=0;
        int start =0;
        int tempCount=temp.size()-1;
        for (int j=0;j<tempCount;++j)
        {
            if (temp[j]=='/'&&temp[j+1]=='/')
            {
                start=j;
                charsToDel=0;
                while (temp[j++]!='\r')
                    charsToDel++;

                temp.remove(start, charsToDel);
                j-=charsToDel;
                tempCount=tempCount-charsToDel;
            }
        }
        tempCount=temp.count()-1;
        for (int j=0;j<tempCount;++j)
        {
            if (temp[j]==' '&&temp[j+1]==' ')
            {
                start=j;
                charsToDel=0;
                while (temp[j++]==' ')
                    charsToDel++;

                temp.remove(start, charsToDel);
                j-=charsToDel;
                tempCount=tempCount-charsToDel;
            }

        }
        temp.simplified();
        dataByParts[i]= temp;
    }

    QString teststr=dataByParts.join("\r\nGENERAL\r\n");
    saveToFile(currentFile->fileName()+"t", teststr);

    /*end of data parsing*/

    /*text part must be multiply of 256*/

    currentFile->close();
    currentFile= NULL;

}

void MainWindow::appendToLog(const QString &text)
{
    dateTime = QDateTime::currentDateTime();
    logEdit->append(dateTime.toString("dd.MM.yyyy hh:mm:ss ")+ text);
}

MainWindow::~MainWindow()
{
    delete openDirButton;
    delete openFileButton;
    delete openDirDlg;
    delete openFileDlg;
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

void MainWindow::onOpenFileDialogFinished(QString fileName)
{
    openFile(fileName);
    parseCurrentFile();
}

void MainWindow::onOpenLogClicked()
{
    logEdit->setVisible(!logEdit->isVisible());
    if (logEdit->isVisible()){
        mainWindowSize.setHeight(MAXH*2);
    }
    else{
        mainWindowSize.setHeight(MAXH);
    }
    resize(mainWindowSize);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        canMove = true;
        mpos = event->pos();
    }
}
void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if((event->buttons() & Qt::LeftButton) && canMove)
    {
        move(event->globalPos()-mpos);
    }
}
void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && canMove)
    {
        canMove = false;
    }
}
