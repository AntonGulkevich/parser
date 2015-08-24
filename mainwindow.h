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

class MainWindow : public QFrame
{
    Q_OBJECT
private:
    QPoint mainWindownStartPoint;
    QSize mainWindowSize;
    QDateTime dateTime;

    QPushButton *openFileButton;
    QPushButton *openDirButton;
    QPushButton *closeButton;
    QPushButton *optionsButton;
    QPushButton *logButton;
    QTextEdit * logEdit;

    QFileDialog *openDirDlg;
    QFileDialog *openFileDlg;

    QString pathToFolder;
    QString pathToFile;
    QStringList filesToConvert;
    QFile *currentFile;

    QPoint mpos;
    bool canMove;

    /*settings*/
    QString pathTo7ZipExe;
    int zipCompressionLevel; // must be 1-9
    bool autoMode;
    bool logSaveToPrFolder;

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
    void zip (QString filename , QString zipfilename);
    void unZip (QString zipfilename , QString filename);
    void srcToZip (const QString & filename , const QString & zipfilename);
    void appendToLog(const QString &text);

    void setPathToFolder(const QString &folder);
    void setAutoMode(bool aMode);
    bool getMode();

    ~MainWindow();
private slots:
    void onCloseButtonClicked();
    bool onOpenFileDialogFinished(QString fileName);
    bool onOpenDirDialogFinished(QString dirName);
    void onOpenLogClicked();
protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
};

#endif // MAINWINDOW_H
