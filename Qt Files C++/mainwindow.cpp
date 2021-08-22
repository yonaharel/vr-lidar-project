#include "mainwindow.h"
#include "ui_mainwindow.h"


//using namespace::std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->centralwidget);
    ui->messageLabel->setText(QString());
    ui->lineEdit->setReadOnly(true);
    ui->directoryTextField->setReadOnly(true);
}

MainWindow::~MainWindow()
{
    if (isCreatingNew){
        writeIntoPythonFile(pythonFilePath, currentFile, "{fbx_file_name}");
        if (!itemsDirPath.isEmpty()){
            writeIntoPythonFile(pythonFilePath, itemsDirPath, "{directory_path}");
        }
    }
    if (!newLevelName.isEmpty()){
        writeIntoPythonFile(pythonFilePath, newLevelName, "{new_level_name}");
     } else {
        writeIntoPythonFile(pythonFilePath, newLevelName, "NO_NAME");
    }
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
    QString message = "Please select the project 'new_vr' once Unreal Engine starts";
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
    }
    writeIntoPythonFile(pythonFilePath, "{new_level_name}", newLevelName);
    openUnrealEngine();
}


void MainWindow::openUnrealEngine(){
    QProcess *process = new QProcess(this);
    QString unrealProjectPath = "D:/UE_4.26/Engine/Binaries/Win64/UE4Editor.exe";
    process->start(unrealProjectPath, QStringList());
}


void MainWindow::on_browseItemsButton_clicked()
{
    itemsDirPath = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    "/home",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    ui->directoryTextField->setText(itemsDirPath);

    qDebug() << itemsDirPath;
    //writeIntoPythonFile(pythonFilePath, "{directory_path}", dir);
}

QString MainWindow::openFileDialog(QString title, QString fileType){
    return "";
}

void MainWindow::writeIntoPythonFile(QString filePath, QString searchString, QString valueToReplace) {

    string path = filePath.toStdString();
    ifstream inFile(path);
    std::ostringstream text;
    text << inFile.rdbuf();
    std::string str = text.str();
    std::string str_search = searchString.toStdString();
    std::string str_replace = valueToReplace.toStdString();
    size_t pos = str.find(str_search);
    str.replace(pos,  std::string(str_search).length(), str_replace);
    inFile.close();

    std::ofstream outFile(path);
    outFile << str;

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

