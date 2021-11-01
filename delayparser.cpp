#include"delayparser.h"

void DelayParser::ClearThemAll()
{
    delayMatrix.clear();
    logicMatrix.clear();

    inputTransitionTimes.clear();

    allInputTransitionTimes.clear();

    outputNames.clear();

    outputTransitionTimes.clear();

    outputTransitionValues.clear();
}

void DelayParser::CombineInputTransitionTimes(double transitionInterval)
{
    std::size_t inputs = inputTransitionTimes.size();

    double transitionMargin = MARGINRATIO * transitionInterval;

    //Holds all of the transitions even there are repetitions.
    //The repetitions will be canceled next
    std::vector<double> allTransitions;

    for(std::size_t i = 0; i < inputs; ++i)
    {
        allTransitions.insert(allTransitions.end(), inputTransitionTimes[i].begin(), inputTransitionTimes[i].end());
    }

    std::sort(allTransitions.begin(), allTransitions.end());

    if(allTransitions.size() < 1)
    {
        throw("Error: There is no transitions in any of the inputs!");
    }

    allInputTransitionTimes.push_back(allTransitions[0]);
    std::cout << "Next recorded transition:" << allTransitions[0] << std::endl;

    for(std::size_t i = 0; i < allTransitions.size() - 1; ++i)
    {
        if(allTransitions[i + 1] > allTransitions[i] + transitionMargin)
        {
            allInputTransitionTimes.push_back(allTransitions[i + 1]);
            std::cout << "Next recorded transition:" << allTransitions[i + 1] << std::endl;
        }
    }

}


std::vector<int> DelayParser::CompareActualWithExpected(std::string actualFile, std::string expectedFile, std::string resultFile)
{
    std::ifstream inActual;
    std::ifstream inExpected;
    std::string strActual;
    std::string strExpected;

    int errors = 0;


    std::ofstream outResult(resultFile);

    inActual.open(actualFile);

    if(!inActual.is_open())
    {
        throw std::invalid_argument("Actual Outputs File cannot be opened! Check the file path!");
    }

    inExpected.open(expectedFile);

    if(!inExpected.is_open())
    {
        throw std::invalid_argument("Expected Outputs File cannot be opened! Check the file path!");
    }

     //Read until the end of file
    int count = 0;
    int totalBitCount = 0;
    int totalWrongBitCount = 0;

    while(std::getline(inActual, strActual))
    {
        std::vector<int> wrongBits;
        std::getline(inExpected, strExpected);
        count++;
        //Windows type file
        strActual.erase(std::remove_if(strActual.begin(), strActual.end(), [](char chr) { return chr == '\r'; }), strActual.end());
        strExpected.erase(std::remove_if(strExpected.begin(), strExpected.end(), [](char chr) { return chr == '\r'; }), strExpected.end());

        if(strExpected.size() != strActual.size())
        {
            throw std::invalid_argument("The number of outputs mismatch between the expected and actual output file.");
        }

        totalBitCount += strExpected.size();

        for(std::size_t i = 0; i < strExpected.size(); ++i)
        {
            if(strActual[i] != strExpected[i] && (strExpected[i] != 'x' && strExpected[i] != 'X'))
            {
                totalWrongBitCount++;
                wrongBits.push_back(strExpected.size() - i - 1);
            }
        }

        if(wrongBits.size() != 0)
        {
            outResult << std::setw(10) << "Input Vector Index: " << count  << std::endl;
            outResult << std::setw(10) << "Result: " << strActual << std::endl;
            outResult << std::setw(10) << "Expected: " << strExpected << std::endl;
            outResult << std::setw(10) << "Wrong Bit Indices: ";

            for(auto& a : wrongBits)
            {
               outResult << a << " ";
            }

            outResult << std::endl << std::endl;

            errors++;
        }

    }
    outResult << std::setw(10) << "There are total " << totalBitCount << " outputs" << std::endl;
    outResult << std::setw(10) << "Among  them " << totalWrongBitCount << " outputs are wrong" << std::endl;
    outResult << std::setw(10) << "Accuracy Percentage = %" << 100 * ((totalBitCount - totalWrongBitCount)/(float)totalBitCount) << std::endl;

    inActual.close();
    inExpected.close();
    outResult.close();

    std::cout << "Info: Actual output is compared with the expected output." << std::endl;
    std::cout << "Info: There are " << errors << " differences written to " << resultFile << std::endl;

    std::vector<int> returnVec;
    returnVec.push_back(count);
    returnVec.push_back(errors);

    return returnVec;
}


void DelayParser::ExtractTransitionTimes(std::string fileLocation, double transitionVoltage, bool isInput)
{
    std::ifstream instream;             //The file descriptor
    std::string nodeName;               //The name of the processed node
    std::string str;                    //Will be iterated per line in the file
    std::vector<double> times;          //Holds the time per record
    std::vector<double> voltages;       //Holds the voltage per record

    instream.open(fileLocation);

    if(!instream.is_open())
    {
        throw std::invalid_argument("File cannot be opened! Check the file path!");
    }


    std::size_t parOpenPos = fileLocation.find_last_of('_');
    std::size_t parClosePos = fileLocation.find_first_of('.');
    nodeName = fileLocation.substr(parOpenPos + 1, parClosePos - parOpenPos - 1);

    //If it is output node insert  the name of the node
    if(!isInput)
    {
        outputNames.push_back(nodeName);
    }

    //Read until the end of file
    while(std::getline(instream, str))
    {
        //Get the records from the file. It must start with a digit
        if(str[0] >= '0' && str[0] <= '9' )
        {
            double time;
            double voltage;
            std::size_t spaceLoc1;
            std::size_t spaceLoc2;

            //Read three lines containing one index and time and voltage and space;
            spaceLoc1 = str.find('\t');
            std::string substr1 = str.substr(spaceLoc1 + 1);
            spaceLoc2 = substr1.find('\t');
            std::string strTime = substr1.substr(0, spaceLoc2);
            std::string strVoltage = substr1.substr(spaceLoc2 + 1);

            //std::cout << spaceLoc << std::endl;
            time = std::stod(strTime);
            times.push_back(time);
            voltage = std::stod(strVoltage);
            voltages.push_back(voltage);
        }
     }


    if(isInput)
    {
        std::vector<double> tempVec = ReturnTransitionTimesAndValues(times, voltages, transitionVoltage, isInput);
        tempVec.push_back(times.back());
        inputTransitionTimes.push_back(tempVec);

    }
    else
    {
        outputTransitionTimes.push_back(ReturnTransitionTimesAndValues(times, voltages, transitionVoltage, isInput));

    }

    //std::cout << "Info: Spice file is parsed..." << std::endl;
    //std::cout << "Info: The transition times array is filled..." << std::endl;
    //std::cout << "Info: You can add a new output node." << std::endl;
    //std::cout << "Attention: Be sure to add all outputs in your design." << std::endl;
    //std::cout << "++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;

    instream.close();
}


void DelayParser::FillLogicDelayMatrices()
{

    std::size_t transitions = allInputTransitionTimes.size();
    std::size_t outputs = outputNames.size();

    //This loop will compute the delay per transition per output
    //The for loops below detects the delay for the ith ouput at jth transition
    //If output does not change for the current transition, the delay is set to 0.
    //It also considers the glitches
    for(size_t i = 0; i < outputs; ++i)
    {
        //First get the transition times for the corresponding output to a vector
        std::vector<double> transitionTimes = outputTransitionTimes[i];
        std::vector<bool> transitionValues = outputTransitionValues[i];
        size_t k = 0;

        double currentTransitionTime;
        double nextTransitionTime;
        std::vector<double> perOutputDelays;
        std::vector<bool> perOutputValues;

        //Add the first logic value corresponding to first input before transitions
        //perOutputValues.push_back(transitionValues[0]);

        //For each transition compute the delay and logic value
        for(size_t j = 0; j < transitions - 1; ++j)
        {
            currentTransitionTime = allInputTransitionTimes[j];
            nextTransitionTime = allInputTransitionTimes[j + 1];

            //Find the maximum output transition time smaller than the next input transition
            while(k < transitionTimes.size() && transitionTimes[k] < nextTransitionTime)
            {
                ++k;
            }

            //If even the first output transition is bigger than the nextTransitionTime
            //then there is no transition at the output for this input transition
            //then set logic-value to the very first value of that output and
            //set the delşay to 0
            //ICCD
            /*
            if(k == 0)
            {
                perOutputValues.push_back(transitionValues[0]);
                perOutputDelays.push_back(0);
            }
            else*/
            //{
                //The logic value of the output belongs to the last transition prior to kth
                //(Changed for ICCD) Instead of k - 1 k is used as index because the first value in transitionValues array is first value of the output
                perOutputValues.push_back(transitionValues[k - 1]);

                if(transitionTimes[k - 1] < nextTransitionTime && transitionTimes[k - 1] > currentTransitionTime)
                {
                    perOutputDelays.push_back(transitionTimes[k - 1] - currentTransitionTime);
                }
                else
                {
                    perOutputDelays.push_back(0);
                }
            //}
        }


        delayMatrix.push_back(perOutputDelays);
        logicMatrix.push_back(perOutputValues);
    }

    std::cout << "Info: Logic and Delay Matrices are filled with delay and logic value for each output & transition.." << std::endl;
    //std::cout << "**********************************************************************" << std::endl;

}

std::vector<double> DelayParser::GetAllInputTransitionTimes()
{
    return allInputTransitionTimes;
}

std::vector<std::string> DelayParser::GetOutputNames()
{
    return outputNames;
}

std::vector<double> DelayParser::ReturnTransitionTimesAndValues(const std::vector<double>& times, const std::vector<double>& voltages, double transitionVoltage, bool isInput)
{
    std::size_t records = voltages.size();

    std::vector<double> transitionTimes;
    std::vector<bool> transitionValues;

    //if the difference is decreasing this is made true;
    //bool decreasing = false;

    //Shows the difference from transitionVoltage
    double voltageDifference1;
    double voltageDifference2;


    if(!isInput)
    {
        transitionValues.push_back((voltages[0] > transitionVoltage) ? true : false);
    }

    //ICCD
    //First transition is time 0.
    transitionTimes.push_back(0);

    for(size_t i = 0; i < records - 1; ++i)
    {
        voltageDifference1 = voltages[i] - transitionVoltage;
        voltageDifference2 = voltages[i + 1] - transitionVoltage;

        //Understand if the voltages passed through the transitionVoltage
        //Then store the transition time computed by interpolation
        if(std::signbit(voltageDifference1) != std::signbit(voltageDifference2))
        {
            //Perform linear interpolation to find exact transition time
            double interTime = times[i + 1] - times[i];
            double interVoltage = std::abs(voltages[i + 1] - voltages[i]);
            transitionTimes.push_back((std::abs(voltageDifference1) / interVoltage) * interTime + times[i]);

            //Below if is for recording the value of the output after that transition
            if(!isInput)
            {
                if(voltages[i + 1] != transitionVoltage)
                {
                    transitionValues.push_back((voltages[i + 1] > transitionVoltage) ? true : false);
                }
                //if coincidentally they became equal test the next voltage in time
                else
                {
                    transitionValues.push_back((voltages[i + 2]  > transitionVoltage) ? true : false);
                }
            }
            //std::cout << "Transition Time: " << transitionTimes[transitionTimes.size() - 1] << std::endl;
        }

    }

    //Add the transition values for that particular output to the output values 2d array
    if(!isInput)
    {
        outputTransitionValues.push_back(transitionValues);
    }

    return transitionTimes;

}


void DelayParser::WriteDelayAndValuesToConsole()
{
    std::size_t outputs = delayMatrix.size();
    std::size_t transitions = delayMatrix[0].size();

    std::cout << std::endl;
    std::cout << "******DISPLAYING DELAY & LOGIC VALUES FOR OUTPUTS******" << std::endl;
    std::cout << "Delay of Each Output (Rows) at Each Transition (Columns):" << std::endl;
    std::cout.precision(17);
    //To see the delay contents for different outputs and transitions
    for(size_t i = 0; i < outputs; ++i)
    {
        std::cout <<outputNames[i] << ": ";
        for(size_t j = 0; j < transitions; ++j)
        {
            std::cout << "tr[" << j + 1 << "] = "  << std::setw(25) << delayMatrix[i][j] << "   ";
        }
        std::cout << std::endl;

    }

    std::cout << std::endl;
    std::cout << "Logic Value of Each Output (Columns) for Each Transition (Rows):" << std::endl;

    //First write down output labels
    for(size_t i = 0; i < outputs; ++i)
    {
        std::cout << outputNames[i] << "    ";
    }
    std::cout << std::endl;

    //To see the logic values for different outputs and transitions
    for(size_t j = 0; j < transitions + 1; ++j)
    {

        for(size_t i = 0; i < outputs; ++i)
        {
            std::cout << logicMatrix[i][j] << "     ";
        }
        std::cout << std::endl;
    }
    std::cout << "********************END OF DISPLAY*********************" << std::endl;
    std::cout << std::endl;

}

void DelayParser::WriteDelayToFile(std::string delayFileLoc, std::string delimiter)
{
    std::size_t outputs = delayMatrix.size();
    std::size_t transitions = delayMatrix[0].size();

    // Create and open a text file
    std::ofstream delayFile(delayFileLoc);

    double worstDelay = 0;
    int worstDelayOutput = 0;
    int worstDelayTransition = 0;

    delayFile.precision(17);

    //To see the delay contents for different outputs and transitions
    for(size_t i = 0; i < outputs; ++i)
    {
        delayFile << outputNames[i] << delimiter;
        for(size_t j = 0; j < transitions; ++j)
        {
            if(worstDelay < delayMatrix[i][j])
            {
                worstDelay = delayMatrix[i][j];
                worstDelayOutput = i;
                worstDelayTransition = j;
            }
            delayFile << delayMatrix[i][j] << delimiter;
        }
        delayFile << std::endl;
    }

    delayFile << "Worst Case Delay," << outputNames[worstDelayOutput] << ",transition[" << worstDelayTransition + 1 << "]," << worstDelay << std::endl;

    // Close the file
    delayFile.close();
    std::cout << "Info: Delay data for each output and each transition is written to " << delayFileLoc << std::endl;

}

void DelayParser::WriteLogicToFile(std::string logicFileLoc, std::string delimiter)
{

    std::size_t outputs = delayMatrix.size();
    std::size_t transitions = delayMatrix[0].size();

    // Create and open a text file
    std::ofstream logicFile(logicFileLoc);

    for(size_t j = 0; j < transitions; ++j) //ICCD
    {

        for(size_t i = 0; i < outputs; ++i)
        {
            logicFile << logicMatrix[i][j] << delimiter;
        }
        logicFile << std::endl;
    }

    logicFile.close();
    std::cout << "Info: Logic data for each output and each transition is written to " << logicFileLoc << std::endl;

}



void DelayParser::WriteParticularOutputToHexFile(std::string hexFileLoc, int start, int end)
{
    std::size_t outputs = delayMatrix.size();
    std::size_t transitions = delayMatrix[0].size();

    if(start < 0 || (size_t)end > outputs - 1)
    {
        throw("Error: The computed output indices are out of bounds...");
    }

    // Create and open a text file
    std::ofstream hexFile(hexFileLoc);

    for(size_t j = 0; j < transitions + 1; ++j)
    {
        std::string str = "";
        for(int i = start; i < end + 1; ++i)
        {
            if(logicMatrix[i][j])
            {
                str = str + "1";
            }
            else
            {
                str = str + "0";
            }

        }
        hexFile << std::setfill('0') << std::setw(std::ceil((end - start + 1)/4.0)) << std::hex << std::uppercase << std::stoi(str, 0, 2) << std::endl;
    }

    hexFile.close();
    std::cout << "Info: Hex data for output[" << start << ":" << end <<"] is written to " << hexFileLoc << std::endl;
}
