#include "PLODELib.h"

int andcount = 0;
int orcount = 0;
int nandcount = 0;
int norcount = 0;
int xorcount = 0;
int xnorcount = 0;
int notcount = 0;
int bufcount = 0;
int oaicount = 0;
int dffcount = 0;

int subcircuitCount = 0;
double PLODELib::simulationDuration = 0;
double PLODELib::totalSimulationTime = 0;
std::map<std::string,std::vector<std::pair<double,double>>> PLODELib::inputsVoltageTimeList;
std::map<std::string,std::vector<std::pair<double,double>>> PLODELib::outputsVoltageTimeList;
DelayParser PLODELib::delayParser;

void addHeadersToSpiceFile(Circuit & circuit, std::string filename, std::string library_path){
    char buffer[256];
    getcwd(buffer, 256 );
    std::ofstream conversionFile;
    conversionFile.open(filename);
    conversionFile << circuit.circuitName << std::endl;

    for(const auto& entry : std::experimental::filesystem::directory_iterator(library_path)){
        std::cout << entry.path() << std::endl;
        conversionFile << ".include " << entry.path().string()<< std::endl;

    }

    conversionFile <<"Vx Vsup Vcc 0"<< std::endl;

    conversionFile << ".control" << std::endl;
    conversionFile << "set filetype = ascii" << std::endl;
    conversionFile << ".endc" << std::endl << std::endl;

    conversionFile.close();
}
void convertContents(Circuit & circuit, std::string filename, bool isSubcircuit){
    std::ofstream conversionFile;
    conversionFile.open(filename, std::ofstream::app);
    for (auto adj : circuit.adjacencyList){
            if(conversionFile.is_open()){
                std::vector<CircuitElement> inputs;
                std::vector<CircuitElement> outputs;
                std::string inputsString;
                std::string outputsString;
                for (auto a : circuit.adjacencyList) {
                    for (auto s : a.second) {
                        if (s.elementName == adj.first.elementName && (a.first.elementType == CircuitElementType::INPUT || a.first.elementType == CircuitElementType::WIRE)) {
                            inputs.push_back(a.first);
                        }
                    }
                }
                for (auto a : circuit.adjacencyList) {
                    if (a.first.elementName == adj.first.elementName) {
                        for (auto s : a.second) {
                            if(s.elementType == CircuitElementType::OUTPUT || s.elementType == CircuitElementType::WIRE){
                                outputs.push_back(s);
                            }
                        }
                    }
                }

                for(auto i: inputs){
                    inputsString += i.elementName + " ";
                }
                for(auto o: outputs){
                    outputsString += o.elementName + " ";
                }
                if(adj.first.elementType == CircuitElementType::AND){
                    std::string andname = "Xand" + std::to_string(andcount);
                    andcount++;
                    if(!isSubcircuit){
                        conversionFile << andname << " " << inputsString << "Vcc gnd " << outputsString;
                    }
                    else{
                        conversionFile << andname << " " << inputsString << "Vcc Vss " << outputsString;
                    }
                    switch (inputs.size()) {
                        case 2:
                            conversionFile << "and2" << std::endl;
                            break;
                        case 3: conversionFile << "and3" <<  std::endl;
                            break;
                        case 4: conversionFile << "and4" <<  std::endl;
                            break;
                        case 5: conversionFile << "and5" <<  std::endl;
                            break;
                        case 8: conversionFile << "and8" <<  std::endl;
                            break;
                        case 9: conversionFile << "and9" <<  std::endl;
                            break;
                        default:
                            break;
                    }
                }
                else if(adj.first.elementType == CircuitElementType::OR){
                    std::string orname = "Xor" + std::to_string(orcount);
                    orcount++;
                    if(!isSubcircuit){
                        conversionFile << orname << " " << inputsString << "Vcc gnd " << outputsString;
                    }
                    else{
                        conversionFile << orname << " " << inputsString << "Vcc gnd " << outputsString;
                    }

                    switch (inputs.size()) {
                        case 2: conversionFile << "or2" <<  std::endl;
                            break;
                        case 3: conversionFile << "or3" <<  std::endl;
                            break;
                        case 4: conversionFile << "or4" <<  std::endl;
                            break;
                        case 5: conversionFile << "or5" <<  std::endl;
                            break;
                        default:
                            break;
                    }
                }
                else if(adj.first.elementType == CircuitElementType::NAND){
                    std::string nandname = "Xnand" + std::to_string(nandcount);
                    nandcount++;
                    if(!isSubcircuit){
                        conversionFile << nandname << " " << inputsString << "Vcc gnd " << outputsString;
                    }
                    else{
                        conversionFile << nandname << " " << inputsString << "Vcc Vss " << outputsString;
                    }

                    switch (inputs.size()) {
                        case 2: conversionFile << "nand2" <<  std::endl;
                            break;
                        case 3: conversionFile << "nand3" <<  std::endl;
                            break;
                        case 4: conversionFile << "nand4" <<  std::endl;
                            break;
                        case 5: conversionFile << "nand5" <<  std::endl;
                            break;
                        case 8: conversionFile << "nand8" <<  std::endl;
                            break;
                        default:
                            break;
                    }
                }
                else if(adj.first.elementType == CircuitElementType::NOR){
                    std::string norname = "Xnor" + std::to_string(norcount);
                    norcount++;
                    if(!isSubcircuit){
                        conversionFile << norname << " " << inputsString << "Vcc gnd " << outputsString;
                    }
                    else{
                        conversionFile << norname << " " << inputsString << "Vcc Vss " << outputsString;
                    }
                    switch (inputs.size()) {
                        case 2: conversionFile << "nor2" <<  std::endl;
                            break;
                        case 3: conversionFile << "nor3" <<  std::endl;
                            break;
                        case 4: conversionFile << "nor4" <<  std::endl;
                            break;
                        case 8: conversionFile << "nor8" <<  std::endl;
                            break;
                        default:
                            break;
                    }
                }
                else if(adj.first.elementType == CircuitElementType::XOR){
                    std::string xorname = "Xxor" + std::to_string(xorcount);
                    xorcount++;
                    if(!isSubcircuit){
                        conversionFile << xorname << " " << inputsString << "Vcc gnd " << outputsString;
                    }
                    else{
                        conversionFile << xorname << " " << inputsString << "Vcc Vss " << outputsString;
                    }
                    switch (inputs.size()) {
                        case 2: conversionFile << "xor2" <<  std::endl;
                            break;
                        default:
                            break;
                    }
                }
                else if(adj.first.elementType == CircuitElementType::XNOR){
                    std::string xnorname = "Xxnor" + std::to_string(xnorcount);
                    xnorcount++;
                    if(!isSubcircuit){
                        conversionFile << xnorname << " " << inputsString << "Vcc gnd " << outputsString;
                    }
                    else{
                        conversionFile << xnorname << " " << inputsString << "Vcc Vss " << outputsString;
                    }
                    switch (inputs.size()) {
                        case 2: conversionFile << "xnor2" <<  std::endl;
                            break;
                        default:
                            break;
                    }
                }
                else if(adj.first.elementType == CircuitElementType::NOT){
                    std::string notname = "Xnot" + std::to_string(notcount);
                    notcount++;
                    if(!isSubcircuit){
                        conversionFile << notname << " " << inputsString << "Vcc gnd " << outputsString;
                    }
                    else{
                        conversionFile << notname << " " << inputsString << "Vcc Vss " << outputsString;
                    }
                    switch (inputs.size()) {
                        case 1: conversionFile << "not1" <<  std::endl;
                            break;
                        default:
                            break;
                    }
                }
                else if(adj.first.elementType == CircuitElementType::BUF){
                    std::string bufname = "Xbuf" + std::to_string(bufcount);
                    bufcount++;
                    if(!isSubcircuit){
                        conversionFile << bufname << " " << inputsString << "Vcc gnd " << outputsString;
                    }
                    else{
                        conversionFile << bufname << " " << inputsString << "Vcc Vss " << outputsString;
                    }
                    switch (inputs.size()) {
                        case 1: conversionFile << "buf1" <<  std::endl;
                            break;
                        default:
                            break;
                    }
                }
                else if(adj.first.elementType == CircuitElementType::OAI3){
                    std::string oainame = "Xoai" + std::to_string(oaicount);
                    oaicount++;
                    if(!isSubcircuit){
                        conversionFile << oainame << " " << inputsString << "Vcc gnd " << outputsString;
                    }
                    else{
                        conversionFile << oainame << " " << inputsString << "Vcc Vss " << outputsString;
                    }
                    switch (inputs.size()) {
                        case 3: conversionFile << "oai3" <<  std::endl;
                            break;
                        default:
                            break;
                    }
                }
                else if(adj.first.elementType == CircuitElementType::DFF){
                    std::string dffname = "Xdff" + std::to_string(dffcount);
                    dffcount++;
                    if(!isSubcircuit){
                        conversionFile << dffname << " " << inputs.at(0).elementName << " " << outputsString << inputs.at(1).elementName << " Vcc gnd DFF_X2" << std::endl;
                    }
                    else{
                        conversionFile << dffname << " " << outputsString << inputsString << "Vcc Vss DFF_X2" << std::endl;
                    }
                }
                else if(adj.first.elementType == CircuitElementType::SUBCIRCUIT){
                    std::string submoduleName = "Xsc" + std::to_string(subcircuitCount);
                    subcircuitCount++;
                    std::string inputLine = "";
                    std::string outputLine = "";

                    for(auto input : adj.first.getInputOrder()){
                        inputLine += input + " ";
                    }
                    for(auto output: adj.first.getOutputOrder()){
                        outputLine += output + " ";
                    }

                    if(!isSubcircuit){
                        conversionFile << submoduleName << " " << inputLine << "Vcc gnd " << outputLine;
                    }
                    else{
                        conversionFile << submoduleName << " " << inputLine << "Vcc Vss " << outputLine;
                    }
                    conversionFile << adj.first.subcircuitType << std::endl;
                }

            }
    }
    conversionFile.close();
}
std::string PLODELib::convertFromVerilogToSpice(Circuit & circuit, std::string library_path)
{
    std::string filename = "Converted/" + circuit.circuitName + ".sp";
    std::ofstream conversionFile;
    conversionFile.open(filename, std::ofstream::app);

    addHeadersToSpiceFile(circuit, filename, library_path);
    conversionFile << "******************* SUBCIRCUIT DEFINITIONS *******************" << std::endl;
    conversionFile.close();
    addSubcircuitsToSpiceFile(circuit, filename);
    conversionFile.open(filename, std::ofstream::app);
    conversionFile << "************************ MAIN CIRCUIT ************************" << std::endl;
    convertContents(circuit, filename, false);
    conversionFile.close();

    return filename;
}


void PLODELib::addSubcircuitsToSpiceFile(Circuit & circuit, std::string filename){
    if(circuit.submoduleConnectionMethod == Circuit::SubmoduleConnectionMethod::spiceSubcircuitMethod){
        std::ofstream conversionFile;
        conversionFile.open(filename, std::ofstream::app);
        for(auto subcircuit: circuit.submodules){
            std::string subsktString = ".subckt " + subcircuit.circuitName + " ";
            for(auto input: subcircuit.inputs){
                subsktString += input + " ";
            }
            subsktString += "Vcc Vss ";
            for(auto output: subcircuit.outputs){
                subsktString += output + " ";
            }
            conversionFile << subsktString << std::endl;
            convertContents(subcircuit, filename, true);
            conversionFile << ".ends" << std::endl;
        }
    }

}
QChartView* PLODELib::createPowerGraph(std::string file_name, double supplyvoltage){
    std::ifstream sim_file;
    std::string line;
    std::vector<std::string> file_content;

    sim_file.open(file_name);

    if(!sim_file.is_open()){
        throw std::invalid_argument("Error: createPowerGraph: wrong file given");
    }

    QSplineSeries* power_spline = new QSplineSeries();

    std::vector<std::pair<double, double>> timeVoltagePairs;

    //Read until the end of file
    while (std::getline(sim_file, line)) {

      //Get the records from the file. It must start with a digit
        if(line[0] >= '0' && line[0] <= '9' )
        {
            std::size_t spaceLoc1;
            std::size_t spaceLoc2;

            //Read three lines containing one index and time and voltage and space;
            spaceLoc1 = line.find('\t');
            std::string substr1 = line.substr(spaceLoc1 + 1);
            spaceLoc2 = substr1.find('\t');
            std::string strTime = substr1.substr(0, spaceLoc2);
            std::string strCurrent = substr1.substr(spaceLoc2 + 1);

            //std::cout << spaceLoc << std::endl;
            power_spline->append(stod(strTime)*1000000000 ,stod(strCurrent)*supplyvoltage);

        }
    }

    QChart* power_chart = new QChart();
    power_chart->legend()->hide();
    power_chart->addSeries(power_spline);
    std::string chartName = "Power Simulation Results";
    power_chart->setTitle(chartName.c_str());
    //power_chart->createDefaultAxes();
    power_chart->setObjectName("powerchart");

    QValueAxis *axisX = new QValueAxis();
    axisX->setTitleText("Time");
    power_chart->addAxis(axisX, Qt::AlignBottom);
    power_spline->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("P(t)");
    //axisX->setMin(0);
    power_chart->addAxis(axisY, Qt::AlignLeft);
    power_spline->attachAxis(axisY);


    QPen pen(QRgb(0xe74221));
    pen.setWidth(4);

    power_spline->setPen(pen);

    QFont font;
    font.setPixelSize(30);
    font.setBold(true);
    power_chart->setFont(font);
    power_chart->setTitleFont(font);
    axisX->setTitleFont(font);
    axisY->setTitleFont(font);

    QChartView* power_chart_view = new QChartView(power_chart);

    power_chart_view->setRenderHint(QPainter::Antialiasing);
    power_chart_view->resize(800,400);

    return power_chart_view;
}

QChartView* PLODELib::createVoltageGraph(std::string file_name, std::string nodeName, double inputChangeInterval){
    QLineSeries* voltage_series = new QLineSeries();

    QChart* voltage_chart = new QChart();

    std::ifstream sim_file;
    std::string line;
    std::vector<std::string> file_content;


    std::cout << "CREATE VOLTAGE GRAPH: " << file_name << std::endl;
    sim_file.open(file_name);

    if(!sim_file.is_open()){
        throw std::invalid_argument("Error: createVoltageGraph: wrong file given");
    }


    std::vector<std::pair<double, double>> timeVoltagePairs;

    //Read until the end of file
    while (std::getline(sim_file, line)) {

      //Get the records from the file. It must start with a digit
        if(line[0] >= '0' && line[0] <= '9' )
        {
            std::size_t spaceLoc1;
            std::size_t spaceLoc2;

            //Read three lines containing one index and time and voltage and space;
            spaceLoc1 = line.find('\t');
            std::string substr1 = line.substr(spaceLoc1 + 1);
            spaceLoc2 = substr1.find('\t');
            std::string strTime = substr1.substr(0, spaceLoc2);
            std::string strVoltage = substr1.substr(spaceLoc2 + 1);

            //std::cout << spaceLoc << std::endl;
            timeVoltagePairs.push_back(std::make_pair(stod(strTime)*1000000000 ,stod(strVoltage)));
            voltage_series->append(stod(strTime)*1000000000 ,stod(strVoltage));
        }
    }

    outputsVoltageTimeList[nodeName] = timeVoltagePairs;

    //voltage_chart->legend()->hide();
    voltage_chart->addSeries(voltage_series);
    std::string chartName = "Voltage - Time Graph for " + nodeName;
    voltage_chart->setTitle(chartName.c_str());
    voltage_chart->setAnimationOptions(QChart::SeriesAnimations);
    voltage_chart->legend()->hide();

    QValueAxis *axisX = new QValueAxis();
    axisX->setTitleText("Time (ns)");
    std::cout << "TOTAL SIMULATION TIME:" << totalSimulationTime << " - INPUT CHANGE INTERVAL:" << inputChangeInterval << std::endl;
    int tickCount = std::round((totalSimulationTime)/inputChangeInterval + 1);
    axisX->setTickCount(tickCount);
    voltage_chart->addAxis(axisX, Qt::AlignBottom);
    voltage_series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Voltage (V)");
    //axisX->setMin(0);
    voltage_chart->addAxis(axisY, Qt::AlignLeft);
    voltage_series->attachAxis(axisY);

    // Customize chart title
    QPen pen(QRgb(0x39c11e));
    pen.setWidth(4);

    QPen gridPen(QRgb(0x174b60));
    gridPen.setWidth(2);
    gridPen.setStyle(Qt::DashLine);

    QFont labelFont;
    labelFont.setPixelSize(18);

    QFont font;
    font.setPixelSize(30);
    font.setBold(true);
    voltage_series->setPen(pen);
    voltage_chart->setFont(font);
    voltage_chart->setTitleFont(font);

    axisX->setLabelsFont(labelFont);
    axisX->setTitleFont(font);
    axisX->setGridLineVisible(true);
    axisX->setGridLinePen(gridPen);

    axisY->setLabelsFont(labelFont);
    axisY->setTitleFont(font);

    QChartView* voltage_chart_view = new QChartView(voltage_chart);
    voltage_chart_view->setRenderHint(QPainter::Antialiasing);

    return voltage_chart_view;
}

std::string PLODELib::createFilename(std::string main_circuit_file_name){
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];
    time (&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer,sizeof(buffer),"%d-%m-%Y_%H-%M-%S",timeinfo);
    std::string str(buffer);
    std::string file_name = main_circuit_file_name + "_" + str + + ".sp";
    return file_name;
}

void PLODELib::runSpiceSimulation(std::string file_path){
    char cwd[1024];
    getcwd(cwd, 1024 );
    int ff, p[2];

    auto start = std::chrono::steady_clock::now();

    if(pipe(p) != 0){
       std::exit(EXIT_FAILURE);
    }

    if ((ff = fork()) == -1 )
    {
        fprintf(stderr, "Fork failed\n");
        std::exit(EXIT_FAILURE);
    }

    if(ff == 0){
        ::close(0);
        ::close(p[1]);
        if ( dup2(p[0], 0) == -1 )
        {
          fprintf(stderr, "dup2(0) failed\n");
          std::exit(EXIT_FAILURE);
        }
        if( execlp("ngspice", "ngspice", "-b", "-r", "outputFile.raw", file_path.c_str(), NULL) == -1)
        {
               fprintf(stderr, "Exec failed\n");
               std::exit(EXIT_FAILURE);
        }
    }
    ::close(p[0]);

    int status;
    waitpid(ff, &status, 0);
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_time = end-start;
    PLODELib::simulationDuration = elapsed_time.count();
    std::cout << "Spice successfully finished in " << elapsed_time.count() <<  " s" << std::endl;
    ::close(p[1]);
}

std::string PLODELib::addSimulationParameters(Circuit circuit, std::vector<std::vector<int> > inputValues,
                                               double inputChangeTime,
                                               double supplyvoltage,
                                               std::vector<std::pair<std::string,std::string>> options,
                                               double clockPeriod,
                                               std::string clockName){

    std::cout << "CLOCK NAME:" << clockName << " CLOCK PERIOD:" << clockPeriod << std::endl;
    // To avoid comma or dot confusion when writing floating point numbers,
    // we set an ngspice compatible locale here.
    std::setlocale(LC_NUMERIC, "en_US.UTF-8");

    std::vector<std::string> outputNodeNames = circuit.outputs;
    std::vector<std::string> inputNodeNames = circuit.inputs;

    char cwd[1024];
    getcwd(cwd, 1024 );
    std::string curr_work_dir{cwd};
    std::string filename = curr_work_dir + "/Converted/" + circuit.circuitName;
    std::string new_file_name = createFilename(filename);
    std::ifstream  main_circuit_file(filename + ".sp", std::ios::binary);
    std::ofstream  conversionFile(new_file_name,   std::ios::binary);

    conversionFile << main_circuit_file.rdbuf();
    main_circuit_file.close();
    conversionFile.close();

    conversionFile.open(new_file_name,std::fstream::app);

    int count = 0;
    for(auto i : circuit.outputs){
        conversionFile << "C"+std::to_string(count)+" "+i+" 0 20f"<< std::endl;
        count++;
    }
    conversionFile << "VCC Vsup 0 DC=" + std::to_string(supplyvoltage) << std::endl;
    count =0;
    for(std::size_t i=0; i< circuit.inputs.size();i++){
        std::string inputName = "circinput" + std::to_string(i);

        std::vector<int> inValues = inputValues.at(i);

        if(inValues.size() == 0)
        {
            std::string inputString = "Vp"+std::to_string(count)+" "+circuit.inputs.at(i) +" 0 DC=0";
            conversionFile << inputString << std::endl;
            count++;
        }
        else
        {
            if(circuit.inputs.at(i) == clockName){
                std::string inputString = "Vp"+std::to_string(count)+" "+ clockName + " 0 0 PWL(";
                int clockChangeCount = inputChangeTime*inputValues.at(0).size() / (clockPeriod/2);
                double timeval = 0;

                for(int i = 0; i < clockChangeCount; i++){

                    inputString += std::to_string(timeval) + "N " + std::to_string(((i+1)%2!=0)*supplyvoltage) + "V ";
                    timeval += clockPeriod/2;
                    timeval-=0.01;
                    inputString += std::to_string(timeval) + "N " + std::to_string(((i+1)%2!=0)*supplyvoltage) + "V ";
                    timeval+=0.01;
                }
                inputString +=")";
                conversionFile << inputString << std::endl;
                count++;
            }
            else{
                std::string inputString = "Vp"+std::to_string(count)+" "+circuit.inputs.at(i) + " 0 0 PWL(";
                double timeval = 0;
                for(auto in : inValues){
                    inputString += std::to_string(timeval) + "N " + std::to_string(in*supplyvoltage) + "V ";
                    timeval += inputChangeTime;
                    timeval-=0.01;
                    inputString += std::to_string(timeval) + "N " + std::to_string(in*supplyvoltage) + "V ";
                    timeval+=0.01;
                }
                inputString +=")";
                conversionFile << inputString << std::endl;
                count++;
            }

        }

    }

    simulationDuration = inputChangeTime*inputValues.at(0).size();
    totalSimulationTime = inputChangeTime*inputValues.at(0).size();

    std::string optionLine = ".OPTIONS ";
    for(auto pair: options){
        if(pair.second != ""){
            optionLine += pair.first + "=" + pair.second + " ";
        }
        else{
            optionLine += pair.first + " ";
        }
    }
    conversionFile << optionLine << std::endl;

    //Write control and print lines
    //*************************START***********************************
    conversionFile << ".control" << std::endl;
    conversionFile <<"tran 0.01NS " << std::to_string(simulationDuration) << "NS" << std::endl;

    std::string file_path(new_file_name);
    std::string sim_file_name = new_file_name.substr(new_file_name.find_last_of('/')+1,new_file_name.find('.')-new_file_name.find_last_of('/')-1);
    std::string voltage_sim_file_name = "./simulationresults/" + sim_file_name + "_voltagesimulation";
    std::string power_sim_file_name = "./simulationresults/" + sim_file_name + "_currentsimulation.txt";

    char msg3[1024] = "print ";
    char msg4[1024] = "print ";
    char msg5[1024] = "save ";

    //Write the current print line
    strcat(msg5,"vx#branch");
    strcat(msg3,"vx#branch");
    strcat(msg3, (" > " + power_sim_file_name).c_str());
    strcat(msg5, "\n");
    strcat(msg3, "\n");
    conversionFile << msg5;
    conversionFile << msg3;

    strcpy(msg5, "save ");

    //write output simulation files individually*/

    for(std::size_t i = 0 ; i < outputNodeNames.size() ; i++ ){

        std::string voltage_node = "V(" + outputNodeNames.at(i) + ")";
        strcat(msg5,voltage_node.c_str());
        strcat(msg4,voltage_node.c_str());

        std::string outputNodeName = outputNodeNames.at(i);
        std::transform(outputNodeName.begin(), outputNodeName.end(), outputNodeName.begin(), ::tolower);

        strcat(msg4,(" > " + voltage_sim_file_name + "_" + outputNodeName + ".txt ").c_str());
        strcat(msg5,"\n");
        strcat(msg4,"\n");
        conversionFile << msg5;
        conversionFile << msg4;
        strcpy(msg4, "print ");
        strcpy(msg5, "save ");
    }

    //write input simulation files individually
    strcpy(msg4, "print ");

    for(std::size_t i = 0 ; i < inputNodeNames.size() ; i++ ){
        std::string voltage_node = "V(" + inputNodeNames.at(i) + ")";
        strcat(msg5,voltage_node.c_str());
        strcat(msg4,voltage_node.c_str());

        std::string inputNodeName = inputNodeNames.at(i);
        std::transform(inputNodeName.begin(), inputNodeName.end(), inputNodeName.begin(), ::tolower);

        strcat(msg4,(" > " + voltage_sim_file_name + "_input_" + inputNodeName + ".txt ").c_str());
        strcat(msg5,"\n");
        strcat(msg4,"\n");
        conversionFile << msg5;
        conversionFile << msg4;
        strcpy(msg4, "print ");
        strcpy(msg5, "save ");
    }

    //*************************END***********************************
    conversionFile << ".endc" << std::endl;
    conversionFile << ".END" << std::endl;
    conversionFile.close();

    return new_file_name;
}

void PLODELib::generateLogicAndDelayResults(Circuit circuit, double supplyVoltage, double inputChangeTime, std::string voltageSimulationFileHeader, std::string clockName){
    std::string voltageFileName;
    //DelayParser dp = getDelayParser();

    delayParser.ClearThemAll();


    for(std::size_t i = 0; i < circuit.inputs.size(); i++){
        if(circuit.inputs.at(i) != clockName){
            std::transform(circuit.inputs.at(i).begin(), circuit.inputs.at(i).end(), circuit.inputs.at(i).begin(), ::tolower);
            voltageFileName = voltageSimulationFileHeader + "_input_" + circuit.inputs.at(i) + ".txt";
            delayParser.ExtractTransitionTimes(voltageFileName,supplyVoltage/2.0, true);
        }
    }

    delayParser.CombineInputTransitionTimes(inputChangeTime*1e-9);


    for(std::size_t i = 0; i < circuit.outputs.size(); i++){
        std::transform(circuit.outputs.at(i).begin(), circuit.outputs.at(i).end(), circuit.outputs.at(i).begin(), ::tolower);
        voltageFileName = voltageSimulationFileHeader + "_" + circuit.outputs.at(i) + ".txt";
        //strcat(voltageFileName,(voltageSimulationFileHeader + "_" + outputs.at(i) + ".txt ").c_str());
        delayParser.ExtractTransitionTimes(voltageFileName,supplyVoltage/2.0,false);
        voltageFileName = "";
    }


    int posStart = voltageSimulationFileHeader.find(circuit.circuitName + "_");
    int posEnd = voltageSimulationFileHeader.find("_voltagesimulation");
    posStart += circuit.circuitName.length() + 1;

    std::string dateString = voltageSimulationFileHeader.substr(posStart,posEnd - posStart);

    std::cout << posStart << " - " << posEnd << std::endl;
    std::cout << "*******" << dateString << "********" << std::endl;

    delayParser.FillLogicDelayMatrices();


    //Write Delay and logic values to CSV (Excel) file
    delayParser.WriteDelayToFile("DelayAndLogicResults/" + circuit.circuitName + "_" + dateString + "_delayFile.csv", ",");

    delayParser.WriteLogicToFile("DelayAndLogicResults/" + circuit.circuitName + "_" + dateString + "_logicFile.csv", ",");

    //Write direct results with no space in between to text
    delayParser.WriteLogicToFile("DelayAndLogicResults/" + circuit.circuitName + "_" + dateString + "_logicFile.txt", "");

}

QChartView * PLODELib::createDelayGraph(std::string file_name, std::string nodename){
    QBarSeries * delay_series = new QBarSeries();
    QChart * delay_chart = new QChart();
    std::string originalNodename = nodename;

    std::for_each(nodename.begin(), nodename.end(), [](char & c){
        c = ::tolower(c);
    });
    std::cout << "FILE NAME: " << file_name << std::endl;
    std::ifstream sim_file;
    sim_file.open(file_name);

    std::string line;
    size_t pos;
    std::string token;
    while(getline(sim_file,line)){
        std::cout << "LINE: "<< line << std::endl;
        pos = line.find(',');
        token = line.substr(0, pos);

        if(token == nodename){
            line.erase(0, pos + 1);
            break;
        }
    }

    int count = 1;
    QStringList categories;
    QBarSet *set0 = new QBarSet("Delay");
    //delay_series->setPointsVisible(true);
    delay_series->setVisible(true);
    delay_series->setLabelsAngle(-90);
    delay_series->setLabelsPosition(QAbstractBarSeries::LabelsInsideBase);
    delay_series->setLabelsPrecision(3);
    delay_series->setLabelsVisible(true);
    delay_series->setLabelsFormat("@value");
    //delay_series->setLabelsClipping(false);
    double maxDelay = 0;
    while ((pos = line.find(',')) != std::string::npos) {
        std::string token = line.substr(0, pos);
        //delay_series->append(count, std::stod(token)*1e9);
        std::cout << "TOKEN: " << token << std::endl;
        *set0 << std::stod(token) * 1e9;
        if(std::stod(token) * 1e9 > maxDelay)
        {
            maxDelay = std::stod(token) * 1e9;
        }
        line.erase(0, pos + 1);
        QString qstr = QString::fromStdString("Tr" + std::to_string(count-1)); //ICCD
        categories << qstr;
        count++;

    }

    delay_series->append(set0);
    delay_chart->legend()->hide();
    delay_chart->addSeries(delay_series);
    delay_chart->setAnimationOptions(QChart::SeriesAnimations);

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->setTitleText("Transition Count");
    axisX->append(categories);
    //axisX->setMin(0);
    //axisX->setLabelFormat("%d");
    delay_chart->addAxis(axisX, Qt::AlignBottom);
    delay_series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Delay (ns)");
    axisY->setRange(0, maxDelay * 1.1);
    axisY->setTickCount(12);
    delay_chart->addAxis(axisY, Qt::AlignLeft);
    delay_series->attachAxis(axisY);

    // Customize chart title
    QPen pen(QRgb(0x39c11e));
    pen.setWidth(4);

    QPen gridPen(QRgb(0x174b60));
    gridPen.setWidth(2);
    gridPen.setStyle(Qt::DashLine);

    QFont labelFont;
    labelFont.setPixelSize(18);

    QFont font;
    font.setPixelSize(30);
    font.setBold(true);
    delay_chart->setFont(font);
    delay_chart->setTitleFont(font);

    axisY->setLabelsFont(labelFont);
    axisY->setTitleFont(font);
    axisY->setGridLineVisible(true);
    axisY->setGridLinePen(gridPen);
    axisX->setTitleFont(font);
    axisX->setLabelsFont(labelFont);

    set0->setColor(QRgb(0x1ca9de));
    set0->setLabelColor(QRgb(0x251128));
    set0->setLabelFont(labelFont);


    delay_chart->axes(Qt::Horizontal).at(0)->setMin(0);
    //delay_chart->axes(Qt::Horizontal).at(0)->setGridLinePen(Qt::DashLine);

    std::string chartName = "Delay - Transition Graph for " + originalNodename;
    delay_chart->setTitle(chartName.c_str());

    QChartView* delay_chart_view = new QChartView(delay_chart);
    delay_chart_view->setRenderHint(QPainter::Antialiasing);
    return delay_chart_view;
}
