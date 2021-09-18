#include "mainwindow.h"
#include "ui_mainwindow.h"


//using namespace::std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
//    QResource::registerResource("/path/to/myresource.rcc");

    ui->setupUi(this);
    this->setCentralWidget(ui->centralwidget);
    ui->messageLabel->setText(QString());
    ui->lineEdit->setReadOnly(true);
    ui->directoryTextField->setReadOnly(true);
    ui->finishButton->setText("Click here to launch Unreal Engine ");
}

MainWindow::~MainWindow()
{
    if (QFile::exists(unrealPythonFilePath)) {
        QFile file (unrealPythonFilePath);
           file.remove();
    }

    if (isCreatingNew){
        writeIntoPythonFile(pythonFilePath, currentFile, "{fbx_file_name}");
        if (!itemsDirPath.isEmpty()){
            writeIntoPythonFile(pythonFilePath, itemsDirPath, "{directory_path}");
        }
    }
    if (!newLevelName.isEmpty()){
        writeIntoPythonFile(pythonFilePath, newLevelName, "{new_level_name}");
     }
    resetOutputFile();
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    currentFile.clear();
    ui->lineEdit->setText(QString());
    QString s1 =
        QFileDialog::getOpenFileName(this, "Choose a Mesh file", "VR Mesh Model Object",
            "Supported Mesh Files (*.fbx)");
    currentFile = s1;
    ui->lineEdit->setText(currentFile);
    qDebug() << currentFile;

}


void MainWindow::on_pushButton_2_clicked()
{

}

void MainWindow::on_finishButton_clicked()
{
    QString message = "Please select the project '" + projectName + "' once Unreal Engine starts";
    if (unrealPath.isEmpty()) {
        ui->messageLabel->setText("Choose UE4 .exe path first!");
        return;
    }
    if (currentFile.isEmpty()){
        ui->messageLabel->setText("You have to select an FBX mesh first for the map");
        return;
    }
    if (newLevelName.isEmpty()){
        ui->messageLabel->setText("Please name the new map first!");
        return;
    }
    ui->messageLabel->setText(message);

    isCreatingNew = true;
    writeIntoPythonFile(pythonFilePath,"{fbx_file_name}", currentFile);
    if (!itemsDirPath.isEmpty()){
        writeIntoPythonFile(pythonFilePath, "{directory_path}", itemsDirPath);
    } else {
        writeIntoPythonFile(pythonFilePath, "{directory_path}", "NOT_SELECTED");
    }
    writeIntoPythonFile(pythonFilePath, "{new_level_name}", newLevelName);
    openUnrealEngine();
}


void MainWindow::openUnrealEngine(){

    QFile file(unrealPythonFilePath);
    if (file.exists()){
        file.remove();
    }
    createOutputFile();

    QProcess *process = new QProcess(this);
     process->start(unrealPath, QStringList());
}


void MainWindow::on_browseItemsButton_clicked()
{
    itemsDirPath = QFileDialog::getExistingDirectory(this, tr("Open Items Directory"),
                                                    "/C",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);

    ui->directoryTextField->setText(itemsDirPath);

    qDebug() << itemsDirPath;
    qDebug() << itemsDirPath.isEmpty();
    //writeIntoPythonFile(pythonFilePath, "{directory_path}", dir);
}

QString MainWindow::openFileDialog(QString title, QString fileType){
    return "";
}

void MainWindow::writeIntoPythonFile(QString filePath, QString searchString, QString valueToReplace) {

    QTextCodec *codec = QTextCodec::codecForLocale();

    if (fileText.isEmpty()){
        // Read file
        QFile file(filePath);
        if (!file.open(QFile::ReadOnly)) {
            qDebug() << "Error opening for read: " << file.errorString();
            return;
        }

        fileText = codec->toUnicode(file.readAll());
        file.close();
    }
    fileText.replace(searchString, valueToReplace);
}

void MainWindow::createOutputFile(){
    QTextCodec *codec = QTextCodec::codecForLocale();
    QFile outputFile(unrealPythonFilePath);
    if (!outputFile.open(QFile::WriteOnly)) {
        qDebug() << "Error opening for write: " << outputFile.errorString();
        return;
    }
    outputFile.write(codec->fromUnicode(fileText));
    outputFile.close();

}

void MainWindow::resetOutputFile()
{
    QTextCodec *codec = QTextCodec::codecForLocale();


    // Read file
    QFile file(pythonFilePath);
    if (!file.open(QFile::ReadOnly)) {
        qDebug() << "Error opening for read: " << file.errorString();
        return;
    }

    fileText = codec->toUnicode(file.readAll());
    file.close();
    QFile outputFile(unrealPythonFilePath);
    if (!outputFile.open(QFile::WriteOnly)) {
        qDebug() << "Error opening for write: " << outputFile.errorString();
        return;
    }
    outputFile.write(codec->fromUnicode(fileText));
    outputFile.close();
}

void MainWindow::on_loadExisting_clicked()
{
    writeIntoPythonFile(pythonFilePath,"{new_level_name}", "NO_NAME");
    openUnrealEngine();
}


void MainWindow::on_nameTextField_textChanged(const QString &arg1)
{
    newLevelName =  arg1;
}


void MainWindow::on_actionSelect_UE4_Path_triggered()
{
    QString path =
        QFileDialog::getOpenFileName(this, "Choose UE4 executable", "UE4Editor.exe",
            "Supported EXE Files (*.exe)");
    unrealPath = path;
}


void MainWindow::on_actionChoose_Project_Path_triggered()
{
    QString path = QFileDialog::getExistingDirectory(this, tr("Open Project Directory"),
                                                    "/C",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    unrealPythonFilePath = path + "/Content/Python/init_unreal.py";
    projectName = path.section('/', -1);
    qDebug() << "File exists = " << QFile::exists(unrealPythonFilePath);

}
