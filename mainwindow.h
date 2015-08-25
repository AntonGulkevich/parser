#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFrame>
#include <QPoint>
#include <QApplication>
#include <QDesktopWidget>
#include <QPushButton>
#include <QProgressBar>
#include <QFile>
#include <QFileDialog>
#include <QStandardPaths>
#include <QDebug>
#include <QAbstractAnimation>
#include <QString>
#include <QMouseEvent>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QStringList>
#include <QVector>
#include <QTextEdit>
#include <QDateTime>
#include <QRegExp>
#include <QProcess>
#include <QCryptographicHash>
#include <QTextStream>
#include <QDir>
#include <QDirIterator>
#include <QPropertyAnimation>
#include <QGroupBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QLabel>
#include <QSpinBox>

class MainWindow : public QFrame
{
    Q_OBJECT
private:
    QPoint mainWindownStartPoint;
    QSize mainWindowSize;
    QDateTime dateTime;

    QPushButton * openFileButton;
    QPushButton * openDirButton;
    QPushButton * closeButton;
    QPushButton * optionsButton;
    QPushButton * logButton;
    QTextEdit * logEdit;
    QGroupBox * boxWidget;
    QLineEdit * zipLE;
    QSpinBox  * zipCompressionLE;
    QCheckBox * createArchive;
    QCheckBox * saveLogToSameFolder;
    QLabel * dir7ZipL;
    QLabel * zipCompressionLevelL;


    QFileDialog * openDirDlg;
    QFileDialog * openFileDlg;

    QString pathToFolder;
    QString pathToFile;
    QStringList filesToConvert;
    QFile * currentFile;

    QPoint mpos;
    bool canMove;

    /*settings*/
    QString pathTo7ZipExe;
    int zipCompressionLevel; // must be 1-9
    bool autoMode;
    int logSaveToPrFolder;
    int createArchiveFile;

    /*end of settings*/

    void initDefaultStyle();
    void initContent();
    void initDefVars();
    bool saveToFile(const QString &fileName, const QString &src, const QByteArray &header);
    void parseCurrentFile();
    bool openFile(const QString &fileName);
    void convertFile(const QString &fileName);
    void saveLog();
    void listDirRec(QString directory);

public:
    struct Fat{
        uint address;
        uint size;
    };
    explicit MainWindow(QWidget *parent = 0);
    void autoStart();
    void zip (const QString &filename ,const QString &zipfilename);
    void unZip (const QString &zipfilename , const QString &filename);
    void srcToZip (const QString & filename , const QString & zipfilename);
    void appendToLog(const QString &text);
    void initSettings();
    void saveSettings();

    void setPathToFolder(const QString &folder);
    void setAutoMode(bool aMode);
    bool getMode();

    ~MainWindow();
private slots:
    void onCloseButtonClicked();
    bool onOpenFileDialogFinished(const QString &fileName);
    bool onOpenDirDialogFinished(const QString &dirName);
    void onOpenLogClicked();
    void onOpenSetupClicked();
    void onZipPathTextChanged(const QString &path);
    void onZipComprLevelChanged(int level);
    void onCreateArchiveStateChanged(int state);
    void onSaveLogSameDir(int state);
protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
};

#endif // MAINWINDOW_H
