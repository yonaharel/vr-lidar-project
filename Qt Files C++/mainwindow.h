#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QProcess>
#include <QDebug>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

using namespace::std;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_finishButton_clicked();

    void on_browseItemsButton_clicked();

    void on_loadExisting_clicked();

    void on_nameTextField_textChanged(const QString &arg1);

private:
    QString openFileDialog(QString title, QString fileType);
    void openUnrealEngine();
    void writeIntoPythonFile(QString, QString, QString);

private:
    Ui::MainWindow *ui;
    QString currentFile = "";
    QString itemsDirPath = "";
    QString newLevelName = "";
    bool isCreatingNew = false;
    QString pythonFilePath = "D:/vr project/new_vr/Content/Python/init_unreal.py";
};
#endif // MAINWINDOW_H
