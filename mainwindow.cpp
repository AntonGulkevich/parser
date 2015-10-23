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
    /*0xFA0BD8A5 flag*/

    appendToLog("Programm started");
}

void MainWindow::autoStart()
{
    if (!autoMode)
    {
        if(onOpenFileDialogFinished(pathToFile))
        {
            appendToLog("Single auto mode finished");
            saveLog();
            exit(1);
        }
        else
            appendToLog("Single auto mode stopped with error");
    }
    else
    {
        if(onOpenDirDialogFinished(pathToFolder)){
            appendToLog("Multi auto mode finished");
            saveLog();
            exit(1);
        }
        else
            appendToLog("Multi auto mode stopped with error");
    }
}

bool MainWindow::openFile(const QString &fileName)
{
    //stream binary read
    currentFile = new QFile (fileName);
    if (!currentFile->open(QIODevice::ReadOnly))
    {        
        currentFile->close();
        appendToLog("Unable to open file "+pathToFile);
        currentFile = NULL;
        return false;
    }
    appendToLog("File "+ pathToFile+ " opened");
    return true;
}

void MainWindow::convertFile(const QString &fileName){
    parseCurrentFile();
    QString zipFileName =fileName+".zip";
    //zip(fileName, zipFileName); //zip file using QT zip.WARNING: u can unzif file only in qt!
    if (createArchiveFile)
        srcToZip(fileName, zipFileName); //zip file using 7z.exe
}

void MainWindow::saveLog()
{
    QString saveWay= pathToFolder;

    logSaveToPrFolder!=0 ? saveWay="log.txt" : saveWay+="log.txt";

    QFile logFileName(saveWay);
    logFileName.open(QIODevice::Append | QIODevice::Text);
    QTextStream outStream(&logFileName);
    outStream <<"\r\n" << logEdit->toPlainText();

    logFileName.close();
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
                               "background-color: #800000;"
                               "color: white}"
                               "QPushButton:hover{"
                               "background-color: #B30000}");

    optionsButton = new QPushButton(this);
    optionsButton->setGeometry(MAXW-SMALLBUTTONSIZE*2-PADDING, PADDING*2 +SMALLBUTTONSIZE, SMALLBUTTONSIZE*2, SMALLBUTTONSIZE*2);
    optionsButton->setText("settings");
    optionsButton->setToolTip("settings");

    logButton = new QPushButton(this);
    logButton->setGeometry(MAXW-SMALLBUTTONSIZE*2-PADDING, PADDING*3 +SMALLBUTTONSIZE*3, SMALLBUTTONSIZE*2, SMALLBUTTONSIZE*2);
    logButton->setText("log");
    logButton->setToolTip("Show log");

    openDirDlg = new QFileDialog(this,tr("Open file"), pathToFolder);
    openFileDlg = new QFileDialog(this,tr("Open file"), pathToFolder, tr("bin files (*.bin)"));

    openDirDlg->setDirectory(pathToFolder);
    openFileDlg->setDirectory(pathToFolder);

    openDirDlg->setFileMode(QFileDialog::DirectoryOnly);

    logEdit = new QTextEdit(this);
    logEdit->setGeometry(PADDING, MAXH, MAXW-PADDING*2, MAXH-PADDING);
    logEdit->setReadOnly(true);
    logEdit->setVisible(false);
    logEdit->setStyleSheet("QTextEdit{"
                           "border-style:solid; "
                           "border-color:white;"
                           "border-width: 1px; "
                           "color:white;}");

    boxWidget = new QGroupBox(this);
    boxWidget->setGeometry(MAXW, PADDING, MAXW-PADDING, MAXH-PADDING*2);
    boxWidget->setVisible(false);
    boxWidget->setStyleSheet("QGroupBox{"
                             "border-style: solid; "
                             "border-color: white;"
                             "border-width: 1px; "
                             "color:white;"
                             "padding: 0px;"
                             "margin: 0px;}"
                             "QLabel{"
                             "border-style:none; "
                             "color:white; "
                             "margin:13px;}"
                             "QCheckBox{"
                             "border-style:none; "
                             "color:white; "
                             "margin:13px;}");

    dir7ZipL = new QLabel( "Path to 7 zip:", boxWidget);
    zipLE = new QLineEdit(pathTo7ZipExe,boxWidget);
    zipLE->setGeometry(150, PADDING, 245, PADDING*2 );

    zipCompressionLevelL = new QLabel ("Zip compression level 0-9:", boxWidget);
    zipCompressionLevelL->move(0, 30);

    zipCompressionLE = new QSpinBox(boxWidget);
    zipCompressionLE->setGeometry(150, 40, 245, PADDING*2 );
    zipCompressionLE->setValue(zipCompressionLevel);
    zipCompressionLE->setMinimum(0);
    zipCompressionLE->setMaximum(7);
    zipCompressionLE->setSingleStep(1);

    createArchive = new QCheckBox("Create archive file", boxWidget);
    createArchive->move(0, 60);
    createArchive->setCheckState(Qt::CheckState(createArchiveFile));

    saveLogToSameFolder = new QCheckBox("Save log to same dir", boxWidget);
    saveLogToSameFolder->move(0, 90);
    saveLogToSameFolder->setCheckState(Qt::CheckState(logSaveToPrFolder));

    connect(closeButton, SIGNAL(clicked(bool)), this, SLOT(onCloseButtonClicked()));
    connect(openDirButton, SIGNAL(clicked(bool)), openDirDlg, SLOT(open()));
    connect(openFileButton, SIGNAL(clicked(bool)), openFileDlg, SLOT(open()));
    connect(logButton, SIGNAL(clicked(bool)), this, SLOT(onOpenLogClicked()));
    connect(optionsButton, SIGNAL(clicked(bool)), this, SLOT(onOpenSetupClicked()));
    connect(openFileDlg, SIGNAL(fileSelected(QString)), this, SLOT(onOpenFileDialogFinished(QString)));
    connect(openDirDlg, SIGNAL(fileSelected(QString)), this, SLOT(onOpenDirDialogFinished(QString)));
    connect(zipLE, SIGNAL(textChanged(QString)), this, SLOT(onZipPathTextChanged(QString)));
    connect(zipCompressionLE, SIGNAL(valueChanged(int)), this, SLOT(onZipComprLevelChanged(int)));
    connect(createArchive, SIGNAL(stateChanged(int)), this, SLOT(onCreateArchiveStateChanged(int)));
    connect(saveLogToSameFolder, SIGNAL(stateChanged(int)), this, SLOT(onSaveLogSameDir(int)));
}

void MainWindow::initDefVars()
{
    currentFile = NULL;
    initSettings();
}

bool MainWindow::saveToFile(const QString &fileName, const QString &src, const QByteArray &header)
{
    QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
    QByteArray encodedString = header + codec->fromUnicode(src);
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
    appendToLog("File " + fileName + " saved");
    return true;
}

void MainWindow::parseCurrentFile()
{
    appendToLog("Parsing started.");
    QDataStream in(currentFile);

    /*parsing header*/
    QByteArray header;
    header.resize(HEADER_SIZE);
    in.readRawData(header.data(), HEADER_SIZE);
    /*
    QVector <Fat> headerVector;
    for (int i = 0; (header[i] != 0xFF && i < 64); i+=4)
    {
        Fat f;
        f.address = (static_cast<uint>(header.at(i) & 0xFF) <<8)
                    + (static_cast<uint>(header.at(i+1)) <<16);
        f.size =  (static_cast<uint>(header.at(i+2)) & 0xFF)
                 + (static_cast<uint>(header.at(i+3))<<8);
        headerVector.push_back(f);

        qDebug()<<f.address<<f.size;
    }
    */
    /*end of parsing header*/

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
    QVector <Fat> headerVector;
    headerVector.clear();
    int address=HEADER_SIZE;
    for (int i=0; i<dataByParts.count();++i)
    {
        QString temp;
        temp+="GENERAL"+dataByParts[i];
        temp=temp.left(temp.lastIndexOf("\r\nEND\r\n")+7);
        temp.replace(QRegExp("[ ]{0,}(=)[ ]{0,}"), QString("="));
        temp.replace(QRegExp("[ ]{0,}(==)[ ]{0,}"), QString("=="));
        temp.replace(QRegExp("[ ]{0,}(>)[ ]{0,}"), QString(">"));
        temp.replace(QRegExp("[ ]{0,}(<)[ ]{0,}"), QString("<"));
        temp.replace(QRegExp("[ ]{0,}(\\&\\&)[ ]{0,}"), QString("&&"));
        temp.replace(QRegExp("[ ]{0,}(\\|\\|)[ ]{0,}"), QString("||"));
        temp.replace(QRegExp("[ ]{0,}(\\})[ ]{0,}"), QString("}"));
        temp.replace(QRegExp("[ ]{0,}(\\{)[ ]{0,}"), QString("{"));
        temp.remove('\t');
        //deleting comments
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
        temp.replace(QRegExp("(\r\n){2,}"),QString("\r\n"));
        temp.replace(QString("}\r\n"), QString("}"));
        temp.replace(QString("{\r\n"), QString("{"));
        temp.replace(QRegExp("[ ]{0,}(;\r\n)[ ]{0,}"), QString(";"));

        /*text part must be multiply of 256: fill 0xFF*/
        tempCount = temp.count();
        int bytesToFill;
        bytesToFill = HEADER_SIZE-tempCount%HEADER_SIZE;
        QByteArray fillStr;
        fillStr.resize(bytesToFill);
        fillStr.fill(0xFF);
        QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
        QString convertedFillStr;
        convertedFillStr = codec->toUnicode(fillStr);
        temp.append(convertedFillStr);
        /*end of fill*/

        /*calc header*/
        Fat tempFat;
        tempFat.address=address;
        tempFat.size=tempCount;
        address+=temp.size();
        headerVector.push_back(tempFat);
        /*end of cal header*/
        dataByParts[i]= temp;
    }

    /*create new header for converted file*/
    QByteArray headerNew;
    headerNew.clear();
    headerNew.resize(HEADER_SIZE-4);
    headerNew.fill(0xFF);
    qint32 flagForConvertedFile = 0xFA0BD8A5;
    QByteArray headerPart;
    for (QVector <Fat>::iterator it = headerVector.begin(); it!=headerVector.end(); ++it){
        headerPart.append((char)(((*it).address & (0xFF << 8)) >> 8));
        headerPart.append((char)(((*it).address & (0xFF << 16)) >> 16));

        headerPart.append((char)(((*it).size & (0xFF << 0)) >> 0));
        headerPart.append((char)(((*it).size & (0xFF << 8)) >> 8));
    }
    headerNew.replace(0,headerPart.size(), headerPart);
    for (uint i=0; i<sizeof(flagForConvertedFile);++i)
        headerNew.append((char)((flagForConvertedFile & (0xFF << (i*8))) >> (i*8))); //flag
    /* end of creating new header for converted file*/

    QString strAfterPars=dataByParts.join("");
    appendToLog("Parsing finished.");

    saveToFile(currentFile->fileName()+"_a", strAfterPars, headerNew);

    /*end of data parsing*/

    currentFile->close();
    currentFile= NULL;
}

void MainWindow::appendToLog(const QString &text)
{
    dateTime = QDateTime::currentDateTime();
    logEdit->append(dateTime.toString("dd.MM.yyyy hh:mm:ss ")+ text);
}

void MainWindow::initSettings()
{
    QFile file(QStandardPaths::locate(QStandardPaths::DocumentsLocation, QString(), QStandardPaths::LocateDirectory)+"obfuscationSet.txt");
    if (file.open(QIODevice::ReadOnly)){
        QDataStream in(&file);
        in  >>pathTo7ZipExe
                >>zipCompressionLevel
                >>logSaveToPrFolder
                >>createArchiveFile
                >>pathToFolder;
    }
    else{
        zipCompressionLevel = 7;
        //pathToFolder = "//fs/Group Projects/UBS/База конфигураций";//
        pathToFolder = QStandardPaths::locate(QStandardPaths::DesktopLocation, QString(), QStandardPaths::LocateDirectory);
        pathToFile = "commod.bin";
        logSaveToPrFolder = true;
        autoMode = false;
        createArchiveFile = true;
    }
    file.close();
}

void MainWindow::saveSettings()
{
    QFile file(QStandardPaths::locate(QStandardPaths::DocumentsLocation, QString(), QStandardPaths::LocateDirectory)+"obfuscationSet.txt");
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);
    out <<pathTo7ZipExe
        <<zipCompressionLevel
        <<logSaveToPrFolder
        <<createArchiveFile
        <<pathToFolder;
    file.close();
}

MainWindow::~MainWindow()
{
    delete openDirButton;
    delete openFileButton;
    delete openDirDlg;
    delete openFileDlg;
    delete zipCompressionLevelL;
    delete zipLE;
    delete logEdit;
    delete saveLogToSameFolder;
}

void MainWindow::onCloseButtonClicked()
{
    saveLog();
    exit(0);
}

bool MainWindow::onOpenFileDialogFinished(const QString &fileName)
{
    bool ok = openFile(fileName);
    if (ok)
        convertFile(fileName);
    return ok;
}

bool MainWindow::onOpenDirDialogFinished(const QString &dirName)
{
    filesToConvert.clear();
    listDirRec(dirName);
    QString fileName;
    for (int i=0; i<filesToConvert.count();++i){
        fileName=filesToConvert[i];
        if (openFile(fileName))
            convertFile(fileName);
        else
            return false;
    }
    setPathToFolder(fileName);
    return true;
}

void MainWindow::onOpenLogClicked()
{
    QPropertyAnimation *animation;
    animation = new QPropertyAnimation(this, "geometry");
    animation->setDuration(300);
    animation->setEasingCurve(QEasingCurve::InCubic);
    QRect gm = this->geometry();
    logEdit->setVisible(!logEdit->isVisible());
    logEdit->setGeometry(PADDING, MAXH, gm.width()-PADDING*2, gm.height()-PADDING);
    if (logEdit->isVisible()){
        animation->setStartValue(gm);
        gm.setHeight(MAXH*2);
        animation->setEndValue(gm);
    }
    else{
        animation->setStartValue(gm);
        gm.setHeight(MAXH);
        animation->setEndValue(gm);
    }
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void MainWindow::onOpenSetupClicked()
{
    QPropertyAnimation *animation;
    animation = new QPropertyAnimation(this, "geometry");
    animation->setDuration(300);
    animation->setEasingCurve(QEasingCurve::InCubic);
    QRect gm = this->geometry();
    boxWidget->setVisible(!boxWidget->isVisible());
    if (boxWidget->isVisible()){
        animation->setStartValue(gm);
        gm.setWidth(MAXW*2);
        animation->setEndValue(gm);
        optionsButton->setText("Save \nsettings");
    }
    else{
        animation->setStartValue(gm);
        gm.setWidth(MAXW);
        animation->setEndValue(gm);
        optionsButton->setText("Settings");
        saveSettings();
    }
    animation->start(QAbstractAnimation::DeleteWhenStopped);
    logEdit->setGeometry(PADDING, MAXH, gm.width()-PADDING*2, logEdit->geometry().height());
}

void MainWindow::onZipPathTextChanged(const QString &path)
{
    pathTo7ZipExe = path;
}

void MainWindow::onZipComprLevelChanged(int level)
{
    zipCompressionLevel = level;
}

void MainWindow::onCreateArchiveStateChanged(int state)
{
    createArchiveFile = state;
}

void MainWindow::onSaveLogSameDir(int state)
{
    logSaveToPrFolder = state;
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
void MainWindow::zip (const QString &filename ,const QString &zipfilename){

    QFile infile(filename);
    QFile outfile(zipfilename);
    infile.open(QIODevice::ReadOnly);
    outfile.open(QIODevice::WriteOnly);
    QByteArray uncompressedData = infile.readAll();
    QByteArray compressedData = qCompress(uncompressedData,zipCompressionLevel);
    outfile.write(compressedData);
    infile.close();
    outfile.close();
}

void MainWindow::unZip (const QString &zipfilename , const QString &filename){
    QFile infile(zipfilename);
    QFile outfile(filename);
    infile.open(QIODevice::ReadOnly);
    outfile.open(QIODevice::WriteOnly);
    QByteArray uncompressedData = infile.readAll();
    QByteArray compressedData = qUncompress(uncompressedData);
    outfile.write(compressedData);
    infile.close();
    outfile.close();
}
void MainWindow::srcToZip (const QString & filename , const QString & zipfilename)
{
    QProcess toZip;
    QStringList list;
    list << "a" << "-tzip" << "-mx"+QString::number(zipCompressionLevel, 10) << zipfilename << filename;
    toZip.start(pathTo7ZipExe, list );
    toZip.waitForFinished(30000);
    appendToLog("File "+ filename + " archived in zip.");
    return;
}

void MainWindow::setPathToFolder(const QString &folder)
{
    pathToFolder = folder;
    pathToFile = folder + "commod.bin";
    openDirDlg->setDirectory(folder);
    openFileDlg->setDirectory(folder);
}

void MainWindow::setAutoMode(bool aMode)
{
    autoMode = aMode;
}

bool MainWindow::getMode()
{
    return autoMode;
}
void MainWindow::listDirRec(QString directory)
{
    QDirIterator iterator (directory, QDir::Files | QDir::NoSymLinks, QDirIterator::Subdirectories);
    while(iterator.hasNext())
    {
        iterator.next();
        QString tmp(iterator.fileInfo().absoluteFilePath());
        if (tmp.endsWith("commod.bin")){
            filesToConvert<<tmp;
        }
    }
}
