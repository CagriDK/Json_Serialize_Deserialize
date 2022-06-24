#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QShortcut>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/img/json.png"));
    ui->tabWidget->setCurrentIndex(0);
/*Alternative ShortCut Key //If Uncommented *overloaded the shortcut, so dont uncomment this shorcut already in use at QAction from actionSave_File*/

//    QShortcut  *keyCtrlS=new QShortcut(this);
//    keyCtrlS->setKey(Qt::CTRL + Qt::Key_S);
//    connect(keyCtrlS, &QShortcut::activated, this, &MainWindow::Json_Read);

    ui->comboBox->setCurrentIndex(1);
    connect(ui->comboBox,&QComboBox::currentTextChanged,this,&MainWindow::Source_Field_Items);
    ui->comboBox->setCurrentIndex(0);
    connect(ui->actionFile_Explorer,&QAction::triggered,this,&MainWindow::Open_File_Explorer);
    connect(ui->actionSave_File,&QAction::triggered,this,&MainWindow::Json_Read);
    connect(ui->pushButton_2,&QPushButton::clicked,this,&MainWindow::Json_Read);

    connect(ui->checkBox,&QCheckBox::toggled,ui->checkBox_2,&QCheckBox::setDisabled);
    connect(ui->checkBox,&QCheckBox::toggled,ui->serial_tab,&QWidget::setDisabled);
    connect(ui->checkBox,&QCheckBox::toggled,ui->lineEdit_3,&QLineEdit::clear);
    connect(ui->checkBox,&QCheckBox::toggled,ui->lineEdit_4,&QLineEdit::clear);

    connect(ui->checkBox_2,&QCheckBox::toggled,ui->checkBox,&QCheckBox::setDisabled);
    connect(ui->checkBox_2,&QCheckBox::toggled,ui->udp_tab,&QWidget::setDisabled);
    connect(ui->checkBox_2,&QCheckBox::toggled,ui->lineEdit_2,&QLineEdit::clear);
}

MainWindow::~MainWindow()
{
    delete ui;
}



