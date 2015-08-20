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
    QFile *currentFile;
    int zipCompressionLevel; // must be 1-9

    QPoint mpos;
    bool canMove;

    void initDefaultStyle();
    void initContent();
    void initDefVars();
    bool saveToFile(const QString &fileName, const QString &src);
    void parseCurrentFile();
    void appendToLog(const QString &text);
    bool openFile(const QString &fileName);



public:
    struct Fat{
        uint address;
        uint size;
    };
    explicit MainWindow(QWidget *parent = 0);
    void autoStart(const QString & fileName);
    void zip (QString filename , QString zipfilename);
    void unZip (QString zipfilename , QString filename);
    void srcToZip (const QString & filename , const QString & zipfilename);

    ~MainWindow();
private slots:
    void onCloseButtonClicked();
    bool onOpenFileButtonClicked();
    bool onOpenDirButtonClicked();
    bool onOpenFileDialogFinished(QString fileName);
    void onOpenLogClicked();
protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
};

#endif // MAINWINDOW_H
