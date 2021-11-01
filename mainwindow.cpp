#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/images/plode-small.png"));
    mkdir("Converted",0777);
    mkdir("simulationresults",0777);
    mkdir("DelayAndLogicResults",0777);
    mkdir("ComparisonFiles",0777);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_browseButton_clicked()
{
    CurrentCircuit::circ.submodules.clear();
    CurrentCircuit::circ.callOrder.clear();
    CurrentCircuit::circ.adjacencyList.clear();

    QPushButton *convertButton = findChild<QPushButton*>(QString("convertButton"));
    convertButton->setDisabled(false);

    QRadioButton *graphJoiningMethod = findChild<QRadioButton*>(QString("graphJoining"));
    graphJoiningMethod->setDisabled(false);

    QRadioButton *spiceSubcircuitMethod = findChild<QRadioButton*>(QString("spiceSubcircuiting"));
    spiceSubcircuitMethod->setDisabled(false);

    QString file_name = QFileDialog::getOpenFileName(this,"Choose file","C://","Verilog files (*.v)");

    if(!file_name.isEmpty()){
        ui->textEdit->setText(file_name);
    }
}

void MainWindow::on_convertButton_clicked()
{
    QPushButton *convertButton = findChild<QPushButton*>(QString("convertButton"));
    convertButton->setDisabled(true);

    auto start = std::chrono::high_resolution_clock::now();
    QRadioButton *graphJoiningMethod = findChild<QRadioButton*>(QString("graphJoining"));
    graphJoiningMethod->setDisabled(true);

    QRadioButton *spiceSubcircuitMethod = findChild<QRadioButton*>(QString("spiceSubcircuiting"));
    spiceSubcircuitMethod->setDisabled(true);

    if(CurrentCircuit::circ.elements.size()!=0){
        CurrentCircuit::circ.elements.clear();
        CurrentCircuit::circ.adjacencyList.clear();
    }

    if(ui->textEdit->toPlainText().toUtf8().constData()[0] == '\0'){
        std::cout << "File path is empty" << std::endl;
        QMessageBox::warning(this,"Warning!","A Verilog file must be selected");
        //return;
        convertButton->setDisabled(false);

    }
    else{


        std::cout << "FILE NAME:" << ui->textEdit->toPlainText().toUtf8().constData() << std::endl;
        CurrentCircuit::circ.fillFromVerilogFile(ui->textEdit->toPlainText().toUtf8().constData());
        char buffer[256];
        getcwd(buffer, 256 );
        std::string dirname{buffer};

        QTextEdit *libraryPathTextEdit = findChild<QTextEdit*>(QString("spiceLibraryDirPath"));
        std::string libraryPath = libraryPathTextEdit->toPlainText().toUtf8().constData();

        std::cout << "Library Path: "<< libraryPath << std::endl;

        if(libraryPath.empty()){
            std::cout << "LibraryPath is empty" << std::endl;
            QMessageBox::warning(this,"Warning!","Spice library folder must be selected");
            convertButton->setDisabled(false);

        }
        else{

            ui->textEdit_2->setText(QString::fromStdString(dirname + "/" + PLODELib::convertFromVerilogToSpice(CurrentCircuit::circ,libraryPath)));

            QPushButton * button = findChild<QPushButton*>(QString("runSimButton"));
            button->setEnabled(true);
        }
    }
    //To limit the height of the dropdown
    ui->outputDropdown->setStyleSheet("combobox-popup: 0;");

    for(size_t i = 0 ; i < CurrentCircuit::circ.outputs.size() ; i++){
        ui->outputDropdown->addItem(CurrentCircuit::circ.outputs.at(i).c_str());
    }
    auto stop = std::chrono::high_resolution_clock::now();
    auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "TIME INFO: Conversion took " << diff.count()/1000.0 << " seconds." << std::endl;
}

void MainWindow::on_runSimButton_clicked()
{
    //QComboBox *outputDropdown = findChild<QComboBox*>(QString("outputDropdown"));
    //outputDropdown->clear();
    auto start = std::chrono::high_resolution_clock::now(); //For simulation File extraction

    QPushButton * button = findChild<QPushButton*>(QString("runSimButton"));

    //button->setDisabled(true);

    std::vector<std::vector<int> > inputValues;
    QTextEdit *time = findChild<QTextEdit*>(QString("time"));

    if(time == NULL){
        return;
    }

    std::string timeString(time->toPlainText().toUtf8().constData());
    if(timeString[0] == '\0'){
        return;
    }

    double inputChangeTime= std::stod(timeString);
    QTextEdit *suppvoltage = findChild<QTextEdit*>(QString("supplyvoltage"));
    std::string suppVoltageString(suppvoltage->toPlainText().toUtf8().constData());
    double supplyvoltage;

    if(suppvoltage == NULL){
        return;
    }
    if(suppVoltageString[0] == '\0'){
        supplyvoltage = 0;
    }
    else{
        supplyvoltage = std::stod(suppVoltageString);
    }

    std::ifstream inputFile;
    QTextEdit * simulationInputFilePath = findChild<QTextEdit*>(QString("simulationInputFilePath"));
    inputFile.open(simulationInputFilePath->toPlainText().toUtf8().constData());

    if(!inputFile){

    }
    std::string inputLine;

    bool executeSimulation = true;

    for(std::size_t i = 0 ; i < CurrentCircuit::circ.inputs.size(); i++){

        std::vector<int> inputVect;

        getline(inputFile, inputLine);

        if (inputLine.back() == '\r') {
            inputLine.pop_back();
        }

        for(auto c : inputLine){
            if(c == '0'){
                inputVect.push_back(0);
            }
            else if(c == '1'){
                inputVect.push_back(1);
            }
            else{
                std::cout << "CHAR:" << c << std::endl;
                executeSimulation = false;
                break;
            }
        }
        if(!executeSimulation){
            QMessageBox::warning(this,"Warning!",QString("Input file must only contain 0s and 1s!"));
            break;
        }
        inputValues.push_back(inputVect);
    }


    std::cout << "Before writing simulation input values..." << std::endl;

    QList<QWidget*> simulationOptions = findChildren<QWidget*>(QString("simulationOptionInput"));
    std::vector<std::pair<std::string,std::string>> options;
    for(auto widget : simulationOptions){
        std::cout << "Reading option name..." << std::endl;

        std::string optionName = widget->findChild<QComboBox*>(QString("simulationOptionName"))->currentText().toUtf8().constData();
        std::cout << "Finished reading option name..." << std::endl;

        std::string optionValue;
        std::cout << "Checking option type..." << std::endl;

        if(Utils::isNumericSpiceOption(optionName)){
            std::cout << "Option is numeric spice option" << std::endl;

            optionValue = widget->findChild<QTextEdit*>(QString("simulationOptionValueNumeric"))->toPlainText().toLower().toUtf8().constData();
            std::cout << "Finished reading the value of the numeric value" << std::endl;

            if(!Utils::validNumberString(optionValue)){
                std::cout << optionValue << std::endl;
                std::string str = optionName + " has an invalid numeric value!";
                QMessageBox::warning(this,"Warning!",QString(str.c_str()));
                executeSimulation = false;
                options.clear();
                break;
            }
            else{
                std::pair<std::string,std::string> optionPair = std::make_pair(optionName,optionValue);
                options.push_back(optionPair);
            }

        }
        else if(Utils::isStringValueSpiceOption(optionName)){
            std::cout << "Option is string valued spice option" << std::endl;

            optionValue = widget->findChild<QTextEdit*>(QString("simulationOptionValueString"))->toPlainText().toLower().toUtf8().constData();
            std::cout << "Finished reading the value of the string value" << std::endl;

            if(!Utils::isValidStringValueForSpiceOption(optionName,optionValue)){
                std::cout << optionValue << std::endl;
                std::string str = optionName + " has an invalid value";
                QMessageBox::warning(this,"Warning!",QString(str.c_str()));
                executeSimulation = false;
                options.clear();
                break;
            }
            else{
                std::pair<std::string,std::string> optionPair = std::make_pair(optionName,optionValue);
                options.push_back(optionPair);
            }

        }

        else if(Utils::isBoolValueSpiceOption(optionName)){
            std::cout << "Option is bool valued spice option" << std::endl;

            optionValue = widget->findChild<QComboBox*>(QString("simulationOptionValueBool"))->currentText().toLower().toUtf8().constData();
            std::cout << "Finished reading the value of the bool value" << std::endl;

            if(optionValue == "yes"){
                std::pair<std::string,std::string> optionPair = std::make_pair(optionName,"");
                options.push_back(optionPair);
            }
        }



    }

    if(executeSimulation){
        std::string clockPeriod = findChild<QTextEdit*>(QString("clockPeriodTextEdit"))->toPlainText().toUtf8().constData();
        double clockPeriodVal;
        std::string clockName = findChild<QTextEdit*>(QString("clockNameTextEdit"))->toPlainText().toUtf8().constData();

        if(clockPeriod[0] == '\0'){
            clockPeriodVal = 0;
        }
        else{
            clockPeriodVal = std::stod(clockPeriod);
        }

        std::string newFilePath = PLODELib::addSimulationParameters(CurrentCircuit::circ,inputValues,inputChangeTime,supplyvoltage,options, clockPeriodVal, clockName);
        ui->textEdit_2->setText(newFilePath.c_str());

        std::string file_path(ui->textEdit_2->toPlainText().toUtf8());
        char cwd[1024];
        getcwd(cwd, 1024 );

        std::string sim_file_name = newFilePath.substr(newFilePath.find_last_of('/')+1,newFilePath.find('.')-newFilePath.find_last_of('/')-1);
        std::string dirname{cwd};

        ui->textEdit_3->setText(QString::fromStdString(dirname + "/simulationresults/" + sim_file_name + "_currentsimulation.txt" ));

        std::string voltageSimulationFileHeader = dirname + "/simulationresults/" + sim_file_name + "_voltagesimulation";

        QDialog * simulationInProgressDialog = new QDialog(this);
        QLabel * label = new QLabel;

        QHBoxLayout * layout = new QHBoxLayout;
        layout->addWidget(label);
        simulationInProgressDialog->setLayout(layout);
        simulationInProgressDialog->setWindowTitle("Simulation finished");

        setDisabled(true);

        auto stop = std::chrono::high_resolution_clock::now();
        auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
        std::cout << "TIME INFO: Simulation file construction took " << diff.count()/1000.0 << " seconds." << std::endl;


        QMessageBox * messageBox = new QMessageBox(this);
        messageBox->setText("Press OK to run the simulation");
        messageBox->setWindowTitle("Start simulation");
        messageBox->exec();

        setDisabled(false);

        std::string powerSimulationFile = ui->textEdit_3->toPlainText().toUtf8().constData();

        start = std::chrono::high_resolution_clock::now(); //For Spice simulation part

        PLODELib::runSpiceSimulation(file_path);

        stop = std::chrono::high_resolution_clock::now();
        diff = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
        std::cout << "TIME INFO: Spice Simulation took " << diff.count()/1000.0 << " seconds." << std::endl;

        QSound::play(":/images/finished.wav");
        label->setText("Simulation duration: " + QString(std::to_string((int) std::round(PLODELib::simulationDuration)).c_str()) + " s");

        simulationInProgressDialog->exec();

        button->setDisabled(false);

        //createVoltageGraphs();

        button = findChild<QPushButton*>(QString("powerSimButton"));
        button->setEnabled(true);

        button = findChild<QPushButton*>(QString("voltageSimButton"));
        button->setEnabled(true);

        start = std::chrono::high_resolution_clock::now();//For analysis part

        //TODO: Change this into a different button later on
        PLODELib::generateLogicAndDelayResults(CurrentCircuit::circ, supplyvoltage, inputChangeTime, voltageSimulationFileHeader, clockName);

        int posStart = voltageSimulationFileHeader.find(CurrentCircuit::circ.circuitName + "_");
        int posEnd = voltageSimulationFileHeader.find("_voltagesimulation");
        posStart += CurrentCircuit::circ.circuitName.length() + 1;

        std::string dateString = voltageSimulationFileHeader.substr(posStart,posEnd - posStart);

        std::string logicResFilename = dirname + "/DelayAndLogicResults/" + CurrentCircuit::circ.circuitName + "_" + dateString + "_logicFile.txt";
        ui->logicResultFilePath->setText(logicResFilename.c_str());

        stop = std::chrono::high_resolution_clock::now();
        diff = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
        std::cout << "TIME INFO: Analysis took " << diff.count()/1000.0 << " seconds." << std::endl;


    }
}

void MainWindow::on_powerSimButton_clicked()
{
    std::ifstream sim_file;
    std::string line;
    std::vector<std::string> file_content;

    std::string file_name(ui->textEdit_3->toPlainText().toUtf8().constData());

    if(file_name[0] == '\0'){
        return;
    }

    QTextEdit *suppvoltage = findChild<QTextEdit*>(QString("supplyvoltage"));
    double supplyvoltage;
    if(suppvoltage->toPlainText().toUtf8().constData()[0] ==  '\0'){
        supplyvoltage = 0;
    }
    else{
        std::string suppVoltageString(suppvoltage->toPlainText().toUtf8().constData());
        supplyvoltage = std::stod(suppVoltageString);
    }

    QChartView* power_chart = PLODELib::createPowerGraph(file_name,supplyvoltage);

    power_chart->show();

}

void MainWindow::on_voltageSimButton_clicked()
{

    std::string outputName = ui->outputDropdown->currentText().toUtf8().constData();

    //Because Spice only prints in lowercase file names.
    std::string outputNameLower = outputName;
    std::transform(outputNameLower.begin(), outputNameLower.end(), outputNameLower.begin(), ::tolower);

    std::string file_name(ui->textEdit_3->toPlainText().toUtf8().constData());
    std::string voltage_file_name(file_name);
    voltage_file_name = voltage_file_name.substr(0, voltage_file_name.find_last_of('_'));
    voltage_file_name += "_voltagesimulation_" + outputNameLower + ".txt";

    std::cout << voltage_file_name << std::endl;

    if(file_name[0] == '\0'){
        return;
    }
    std::string timeString(ui->time->toPlainText().toUtf8().constData());
    if(timeString[0] == '\0'){
        return;
    }

    double inputChangeTime= std::stod(timeString);
    QChartView* voltage_chart = PLODELib::createVoltageGraph(voltage_file_name,outputName,inputChangeTime);
    voltage_chart->setWindowTitle(outputName.c_str());
    //voltage_chart->setMinimumSize(QSize(1680,900));
    //voltage_chart->show();

    std::string delay_file_name(file_name);
    delay_file_name= delay_file_name.substr(0, delay_file_name.find_last_of('_'));
    delay_file_name += "_delayFile.csv";
    delay_file_name = std::regex_replace(delay_file_name, std::regex("simulationresults"), "DelayAndLogicResults");

    QChartView* delay_chart = PLODELib::createDelayGraph(delay_file_name,outputName);
    //delay_chart->setMinimumSize(QSize(1680,900));
    //delay_chart->show();

    QWidget * graphDialog_v = new QWidget();
    graphDialog_v->setWindowTitle("Voltage-time Plot");
    QWidget * graphDialog_d = new QWidget();
    graphDialog_d->setWindowTitle("Delay-time Plot");

    QHBoxLayout * layout_v = new QHBoxLayout();
    layout_v->addWidget(voltage_chart);
    QHBoxLayout * layout_d = new QHBoxLayout();
    layout_d->addWidget(delay_chart);

    graphDialog_v->setLayout(layout_v);
    graphDialog_v->setMinimumSize(800,400);
    graphDialog_v->show();

    graphDialog_d->setLayout(layout_d);
    graphDialog_d->setMinimumSize(800,400);
    graphDialog_d->show();
}

void MainWindow::on_graphJoining_toggled(bool checked)
{
    if(checked){
        CurrentCircuit::circ.setSubmoduleConnectionMethod(Circuit::SubmoduleConnectionMethod::graphJoiningMethod);
    }
}

void MainWindow::on_spiceSubcircuiting_toggled(bool checked)
{
    if(checked){
        CurrentCircuit::circ.setSubmoduleConnectionMethod(Circuit::SubmoduleConnectionMethod::spiceSubcircuitMethod);
    }
}


void MainWindow::on_spiceLibBrowseButton_clicked()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setOption(QFileDialog::ShowDirsOnly,true);

    QString file_name = dialog.getExistingDirectory(this,"Choose Library","C://");


    if(!file_name.isEmpty()){
        QTextEdit *libraryPathTextEdit = findChild<QTextEdit*>(QString("spiceLibraryDirPath"));
        libraryPathTextEdit->setText(file_name);
    }

}

void addDropdownItems(QComboBox & combobox){
    QStringList * list = new QStringList();
    list->push_back("ACCT");
    list->push_back("NOACCT");
    list->push_back("NOINIT");
    list->push_back("LIST");
    list->push_back("NOMOD");
    list->push_back("NOPAGE");
    list->push_back("NODE");
    list->push_back("NOREFVALUE");
    list->push_back("OPTS");
    list->push_back("SEED");
    list->push_back("SEEDINFO");
    list->push_back("TEMP");
    list->push_back("TNOM");
    list->push_back("WARN");
    list->push_back("MAXWARNS");
    list->push_back("SAVECURRENTS");
    list->push_back("ABSTOL");
    list->push_back("GMIN");
    list->push_back("GMINSTEPS");
    list->push_back("ITL1");
    list->push_back("ITL2");
    list->push_back("KEEPOPINFO");
    list->push_back("NOOPITER");
    list->push_back("PIVREL");
    list->push_back("PIVTOL");
    list->push_back("RELTOL");
    list->push_back("RSHUNT");
    list->push_back("VNTOL");
    list->push_back("PIVTOL");
    list->push_back("AUTOSTOP");
    list->push_back("CHGTOL");
    list->push_back("CONVSTEP");
    list->push_back("CONVABSSTEP");
    list->push_back("INTERP");
    list->push_back("ITL3");
    list->push_back("ITL4");
    list->push_back("ITL5");
    list->push_back("ITL6");
    list->push_back("MAXEVITER");
    list->push_back("MAXOPALTER");
    list->push_back("MAXORD");
    list->push_back("METHOD");
    list->push_back("NOOPALTER");
    list->push_back("RAMPTIME");
    list->push_back("SRCSTEPS");
    list->push_back("TRTOL");
    list->push_back("XMU");
    list->sort();
    combobox.addItems(*list);

}
void MainWindow::on_addOptionButton_clicked()
{
    QVBoxLayout *simulationParameters = findChild<QVBoxLayout*>(QString("simulationOptionValues"));

    QHBoxLayout *inputLayout = new QHBoxLayout();

    QWidget * containerWidget = new QWidget();
    containerWidget->setObjectName("simulationOptionInput");
    containerWidget->setLayout(inputLayout);

    QLabel *optionNameLabel = new QLabel(containerWidget);
    //QTextEdit *optionNameValue = new QTextEdit(this);
    QComboBox *optionNameValue = new QComboBox(containerWidget);
    optionNameValue->setEditable(true);
    optionNameValue->lineEdit()->setPlaceholderText(QString("Select..."));
    addDropdownItems(*optionNameValue);
    optionNameValue->setFixedHeight(31);
    connect(optionNameValue,SIGNAL(currentIndexChanged(QString)),this,SLOT(optionValueAreaCreation(QString)));
    optionNameValue->setCurrentIndex(-1);

    optionNameLabel->setText("Name:");
    //optionNameLabel->setMinimumWidth(40);

    optionNameValue->setObjectName("simulationOptionName");


    inputLayout->addWidget(optionNameLabel);
    inputLayout->addWidget(optionNameValue);


    simulationParameters->addWidget(containerWidget);
}
void MainWindow::deleteOptionButton(){
    std::cout << QObject::sender()->parent()->metaObject()->className() << std::endl;
    delete QObject::sender()->parent();
}



void clearOptionValueArea(QComboBox * combobox){
    QPushButton * button = combobox->parentWidget()->findChild<QPushButton *>("simulationOptionDeleteButton");
    if(button != NULL){
        delete button;
    }
    QTextEdit * valueArea = combobox->parentWidget()->findChild<QTextEdit *>("simulationOptionValueNumeric");
    if(valueArea != NULL){
        delete valueArea;
    }

    QTextEdit * valueAreaString = combobox->parentWidget()->findChild<QTextEdit *>("simulationOptionValueString");
    if(valueAreaString != NULL){
        delete valueAreaString;
    }

    QComboBox * valueAreaBool = combobox->parentWidget()->findChild<QComboBox *>("simulationOptionValueBool");
    if(valueAreaBool != NULL){
        delete valueAreaBool;
    }

    QLabel * valueLabel = combobox->parentWidget()->findChild<QLabel *>("simulationOptionValueLabel");
    if(valueLabel != NULL){
        delete valueLabel;
    }
}
void MainWindow::optionValueAreaCreation(){

    QComboBox *combobox = qobject_cast<QComboBox *>(QObject::sender());

    if(combobox && combobox->currentIndex() != -1){
        clearOptionValueArea(combobox);

        std::string currentText = combobox->currentText().toUpper().toStdString();
        if(Utils::isNumericSpiceOption(currentText)){
            QLabel *optionValueLabel = new QLabel(this);
            QTextEdit *optionValue = new QTextEdit(this);

            optionValueLabel->setText("Value:");
            optionValueLabel->setObjectName("simulationOptionValueLabel");

            optionValue->setFixedWidth(200);
            optionValue->setFixedHeight(31);
            optionValue->setObjectName("simulationOptionValueNumeric");

            combobox->parentWidget()->layout()->addWidget(optionValueLabel);
            combobox->parentWidget()->layout()->addWidget(optionValue);
        }
        else if(Utils::isStringValueSpiceOption(currentText)){
            QLabel *optionValueLabel = new QLabel(this);
            QTextEdit *optionValue = new QTextEdit(this);

            optionValueLabel->setText("Value:");
            optionValueLabel->setObjectName("simulationOptionValueLabel");

            optionValue->setFixedWidth(200);
            optionValue->setFixedHeight(31);
            optionValue->setObjectName("simulationOptionValueString");

            combobox->parentWidget()->layout()->addWidget(optionValueLabel);
            combobox->parentWidget()->layout()->addWidget(optionValue);
        }
        else if(Utils::isBoolValueSpiceOption(currentText)){

            QLabel *optionValueLabel = new QLabel(this);
            QComboBox *optionValue = new QComboBox(this);
            optionValue->addItem("Yes");
            optionValue->addItem("No");
            optionValue->setFixedWidth(200);
            optionValue->setFixedHeight(31);

            optionValueLabel->setText("Value:");
            optionValueLabel->setObjectName("simulationOptionValueLabel");


            optionValue->setObjectName("simulationOptionValueBool");

            combobox->parentWidget()->layout()->addWidget(optionValueLabel);
            combobox->parentWidget()->layout()->addWidget(optionValue);
        }

        QPushButton* deleteButton = new QPushButton();
        deleteButton->setText(QString("Delete"));
        deleteButton->setObjectName("simulationOptionDeleteButton");
        connect(deleteButton,SIGNAL(clicked()),this,SLOT(deleteOptionButton()));
        combobox->parentWidget()->layout()->addWidget(deleteButton);

    }

}

void MainWindow::on_browseSimulationInputButton_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(this,"Choose file","C://","Text files (*.txt)");

    if(!file_name.isEmpty()){
        ui->simulationInputFilePath->setText(file_name);
    }
}

void MainWindow::on_expectedResultFileBrowse_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(this,"Choose file","C://","Text files (*.txt)");

    if(!file_name.isEmpty()){
        ui->expectedResultFilePath->setText(file_name);
    }
}

void MainWindow::on_compareResultsButton_clicked()
{
    char cwd[1024];
    getcwd(cwd, 1024 );
    std::string dirname{cwd};

    std::string expectedResultFilePath = ui->expectedResultFilePath->toPlainText().toUtf8().constData();
    std::string actualResultFilePath =  ui->plodeResultsFilePath->toPlainText().toUtf8().constData();

    if(actualResultFilePath.size() == 0)
    {
       actualResultFilePath =  ui->logicResultFilePath->toPlainText().toUtf8().constData();
    }

    std::string comparisonResultFilePath = dirname + "/ComparisonFiles/" + CurrentCircuit::circ.circuitName + "_resultComparisonFile.txt";
    std::vector<int> values = PLODELib::getDelayParser().CompareActualWithExpected(actualResultFilePath, expectedResultFilePath, comparisonResultFilePath);
    int totalValues = values[0];
    int wrongValues = values[1];

    ui->wrongResultValue->setText(std::to_string(wrongValues).c_str());
    ui->wrongResultValue->setReadOnly(true);
    ui->correctResultValue->setText(std::to_string(totalValues - wrongValues).c_str());
    ui->correctResultValue->setReadOnly(true);
    ui->totalResultValue->setText(std::to_string(totalValues).c_str());
    ui->totalResultValue->setReadOnly(true);

    ui->resultDifferenceFilePath->setText(comparisonResultFilePath.c_str());

}

void MainWindow::on_resultDifferenceFileOpen_clicked()
{
    std::string filePath = "file://";
    filePath += ui->resultDifferenceFilePath->toPlainText().toUtf8().constData();
    QDesktopServices::openUrl(QUrl(filePath.c_str()));
}

void MainWindow::on_convertedFileOpen_clicked()
{
    std::string filePath = "file://";
    filePath += ui->textEdit_2->toPlainText().toUtf8().constData();
    QDesktopServices::openUrl(QUrl(filePath.c_str()));
}

void MainWindow::on_currentSimFileOpen_clicked()
{
    std::string filePath = "file://";
    filePath += ui->textEdit_3->toPlainText().toUtf8().constData();
    QDesktopServices::openUrl(QUrl(filePath.c_str()));
}

void MainWindow::on_logicResultFileOpen_clicked()
{
    std::string filePath = "file://";
    filePath += ui->logicResultFilePath->toPlainText().toUtf8().constData();
    QDesktopServices::openUrl(QUrl(filePath.c_str()));
}

void MainWindow::on_textEdit_textChanged()
{
    ui->outputDropdown->clear();
}

void MainWindow::on_generateButton_clicked()
{
    QFileDialog dialog(this);

    QString fileName = dialog.getSaveFileName(this,
            tr("Save Output Hex File"), "",
            tr("Output Hex File (*.txt);;All Files (*)"));

    if(fileName.isEmpty()){
        std::cout << "Error: No file name is given." << std::endl;
        fileName = "./OutputHex.txt";
    }

    int startBit = ui->startBit->toPlainText().toInt();
    int stopBit = ui->stopBit->toPlainText().toInt();

    PLODELib::getDelayParser().WriteParticularOutputToHexFile(fileName.toStdString(), startBit, stopBit);

}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}


void MainWindow::on_savePathVerilogButton_clicked()
{
    std::string pathStr = ui->pathText->toPlainText().toStdString();
    std::istringstream stm(pathStr);
    std::string word;
    std::vector<std::string> pathNodes;

    while (stm >> word)
    {
        pathNodes.push_back(word);
    }

    QFileDialog dialog(this);

    QString fileName = dialog.getSaveFileName(this,
            tr("Save Path Verilog File"), "",
            tr("Path Verilog File (*.v);;All Files (*)"));

    if(fileName.isEmpty()){
        throw std::invalid_argument("Error: No file name is given to save path verilog.");
    }

    SinglePath sPath(pathNodes);
    sPath.ConvertToSinglePathVerilog(ui->sourceVerilog->toPlainText().toUtf8().constData(),fileName.toStdString());

}

void MainWindow::on_openVerilog_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(this,"Choose file","C://","Verilog files (*.v)");

    if(!file_name.isEmpty()){
        ui->sourceVerilog->setText(file_name);
    }
}

void MainWindow::on_plodeResultFileBrowse_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(this,"Choose file","C://","Text files (*.txt)");

    if(!file_name.isEmpty()){
        ui->plodeResultsFilePath->setText(file_name);
    }
}
