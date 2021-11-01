#include "circuit.h"

int Circuit::gen_temp_gate_count = 0;
int Circuit::gen_temp_wire_count = 0;
int Circuit::gen_submodule_temp_wire_count = 0;
int Circuit::gen_submodule_temp_gate_count = 0;
std::map<std::string, int> Circuit::submoduleCounts;

void Circuit::addEdge(CircuitElement & element1, CircuitElement & element2)
{
    if (std::find(elements.begin(), elements.end(), element1) == elements.end()) {
        elements.push_back(element1);
        adjacencyList.push_back(std::make_pair(element1, std::vector<CircuitElement>()));

    }

    if (std::find(elements.begin(), elements.end(), element2) == elements.end()) {
        elements.push_back(element2);
        adjacencyList.push_back(std::make_pair(element2, std::vector<CircuitElement>()));
    }

    for (size_t i = 0; i < adjacencyList.size(); i++) {
        if (adjacencyList[i].first == element1) {
            //if(std::find(adjacencyList[i].second.begin(), adjacencyList[i].second.end(), element2) == adjacencyList[i].second.end()) {
                adjacencyList[i].second.push_back(element2);
            //}
        }
    }
}

void Circuit::print()
{
    std::cout << "Name: " << circuitName << std::endl;

    std::map<CircuitElementType, std::string> mapOfGates = {
        {CircuitElementType::AND,"AND"},
        {CircuitElementType::OR,"OR"},
        {CircuitElementType::NAND,"NAND"},
        {CircuitElementType::NOR,"NOR"},
        {CircuitElementType::XOR,"XOR"},
        {CircuitElementType::XNOR,"XNOR"},
        {CircuitElementType::NOT,"NOT"},
        {CircuitElementType::BUF,"BUF"},
        {CircuitElementType::OAI3,"OAI3"},

    };

    for (auto& adj : adjacencyList){
        if (adj.first.elementType == CircuitElementType::INPUT) {
            std::cout << "INPUT " << adj.first.elementName << " is connected to: ";
            for (auto& connected : adj.second) {
                std::cout << connected.elementName << " ";

            }

            std::cout << std::endl;
        }
        else if (adj.first.elementType == CircuitElementType::OUTPUT) {
            std::cout << "OUTPUT " << adj.first.elementName << " is the result of: ";
            for (auto& a : adjacencyList) {
                for (auto& s : a.second) {
                    if (s.elementName == adj.first.elementName) {
                        std::cout << a.first.elementName << " " ;
                    }
                }
            }
            std::cout << std::endl;
        }
        else if (adj.first.elementType == CircuitElementType::WIRE) {
            std::cout << "WIRE " << adj.first.elementName << " is the result of: ";
            for (auto& a : adjacencyList) {
                for (auto& s : a.second) {
                    if (s.elementName == adj.first.elementName) {
                        std::cout << a.first.elementName << " ";
                    }
                }
            }

            std::cout << "also becomes an input to ";
            for (auto& connected : adj.second) {
                std::cout << connected.elementName << " ";
            }

            std::cout << std::endl;
        }
        else {
            std::cout << mapOfGates[adj.first.elementType] << " gate " << adj.first.elementName << " get inputs of: ";
            for (auto& a : adjacencyList) {
                for (auto& s : a.second) {
                    if (s.elementName == adj.first.elementName) {
                        std::cout << a.first.elementName << " ";
                    }
                }
            }

            std::cout << "and outputs: ";
            for (auto& a : adjacencyList) {
                if (a.first.elementName == adj.first.elementName) {
                    for (auto& s : a.second) {
                        std::cout << s.elementName << " ";
                    }
                }
            }

            std::cout << std::endl;
        }
    }

}

bool Circuit::fillFromVerilogFile(std::string fileName)
{
    auto start = std::chrono::steady_clock::now();

    inputs.clear();
    outputs.clear();
    std::ifstream inFile;
    inFile.open(fileName);
    if (!inFile) {
        std::cout << fileName << std::endl;
        std::cerr << "Unable to open file"<<std::endl;
        return false;
    }

    std::string line;
    std::map<std::string, CircuitElementType> isInputOutputOrWire;

    std::vector<std::string> fileLines = Utils::getLinesFromVerilogFile(fileName);
    for (auto line : fileLines) {
        // Handle Windows / DOS style line endings.
        if (line.back() == '\r') {
            line.pop_back();
        }

        if(line.length() == 0){
            continue;
        }
        if(line.rfind("//", 0) == 0){
            continue;
        }

        line.erase(std::remove_if(line.begin(), line.end(), [](char chr) { return chr == ' (' || chr == ' )' || chr == ';'; }), line.end());

        std::replace(line.begin(), line.end(), ',', ' ');
        std::replace(line.begin(), line.end(), '(', ' ');
        std::replace(line.begin(), line.end(), ')', ' ');

        std::istringstream stm(line);
        std::string word;
        std::vector<std::string> tokens;

        while (stm >> word) {
            tokens.push_back(word);
        }

        if (tokens.front() == "module") {
            circuitName = tokens.at(1);
            for(size_t i = 2; i < tokens.size(); i++){
                callOrder.push_back(tokens.at(i));
            }
        }

        else if (tokens.front() == "input") {
            for (size_t i = 1; i < tokens.size(); i++) {
                isInputOutputOrWire[tokens.at(i)] = CircuitElementType::INPUT;
                inputs.push_back(tokens.at(i));
            }
            sortInputsWithCallOrder();
        }
        else if (tokens.front() == "output") {
            for (size_t i = 1; i < tokens.size(); i++) {
                isInputOutputOrWire[tokens.at(i)] = CircuitElementType::OUTPUT;
                outputs.push_back(tokens.at(i));
            }
            sortOutputsWithCallOrder();
        }
        else if (tokens.front() == "wire") {
            for (size_t i = 1; i < tokens.size(); i++) {
                isInputOutputOrWire[tokens.at(i)] = CircuitElementType::WIRE;
            }
        }

        else if(tokens.front() == "'include"){
            std::string moduleFilename = "";
            for(size_t i = 1; i < tokens.size(); i++){
                moduleFilename.append(tokens.at(i));

                if(i != tokens.size() - 1){
                    moduleFilename.append(" ");
                }
            }

            moduleFilename.erase(std::remove(moduleFilename.begin(), moduleFilename.end(), '"'), moduleFilename.end());

            if(moduleFilename.find('/') == std::string::npos){
                std::string parentDirectoryPath = fileName;
                int endIndex = parentDirectoryPath.find_last_of('/');
                moduleFilename = parentDirectoryPath.substr(0,endIndex) + "/" + moduleFilename;
            }
            Circuit submodule;
            submodule.fillFromVerilogFile(moduleFilename);
            submodules.push_back(submodule);

        }
        else if(tokens.front() == "dff"){
            CircuitElement gateNode{ tokens.at(1), CircuitElementType::DFF};
            CircuitElement outputNode{ tokens.at(3), isInputOutputOrWire[tokens.at(3)] };
            addEdge(gateNode, outputNode);

            for (size_t i = 2; i < 5; i+=2) {
                CircuitElement inputNode{ tokens.at(i) , isInputOutputOrWire[tokens.at(i)] };
                addEdge(inputNode, gateNode);
            }
        }
        else if (mapOfNodeTypes.find(tokens.front()) != mapOfNodeTypes.end()) {
            CircuitElement gateNode{ tokens.at(1), mapOfNodeTypes[tokens.front()]};

            //This might not be the case for all the cell libraries, the gate submodules might have different input/output ordering
            //such as the case in the dff. A way to understand has to be added.
            //Also we are getting a cell library but we do not actually understand the contents of it, we just think it is like the Lib45NanGate
            //library which is also a problem. We need to find a way to fix that
            CircuitElement outputNode{ tokens.at(2), isInputOutputOrWire[tokens.at(2)] };
            addEdge(gateNode, outputNode);

            for (size_t i = 3; i < tokens.size(); i++) {
                CircuitElement inputNode{ tokens.at(i) , isInputOutputOrWire[tokens.at(i)] };
                addEdge(inputNode, gateNode);
            }
        }

        else{
            if(tokens.front()!= "endmodule"){
                bool exists = false;
                for(Circuit& c: submodules){
                    if(c.circuitName == tokens.front()){
                        exists = true;
                    }
                }

                Circuit submodule;

                if(exists){
                    for(auto & s: submodules){
                        if(s.circuitName == tokens.front()){
                            submodule = s;
                        }
                    }
                }
                else{
                    std::string parentDirectoryPath = fileName;
                    int endIndex = parentDirectoryPath.find_last_of('/');
                    std::string moduleFilename = tokens.front() + ".v";
                    moduleFilename = parentDirectoryPath.substr(0,endIndex) + "/" + moduleFilename;

                    submodule.submoduleConnectionMethod = this->submoduleConnectionMethod;

                    submodule.fillFromVerilogFile(moduleFilename);
                    for(const Circuit& module: submodule.submodules){
                        submodules.push_back(module);
                    }
                    submodules.push_back(submodule);

                }
                Circuit copyOfSubmodule = submodule;

                if(submoduleConnectionMethod == SubmoduleConnectionMethod::graphJoiningMethod){
                    std::vector <std::string> parameterList;

                    for(size_t i = 2; i < tokens.size(); i++){
                        parameterList.push_back(tokens.at(i));
                    }

                    connectSubmoduleByGraphJoining(&copyOfSubmodule, parameterList, isInputOutputOrWire);
                }
                else{
                    CircuitElement subcircuitNode{ tokens.at(1), CircuitElementType::SUBCIRCUIT,tokens.at(0)};
                    for(size_t i = 2; i < tokens.size(); i++){
                       if(std::find(submodule.inputs.begin(), submodule.inputs.end(), submodule.callOrder.at(i-2)) != submodule.inputs.end()){
                           subcircuitNode.addToInputOrder(tokens.at(i));
                       }
                       else if(std::find(submodule.outputs.begin(), submodule.outputs.end(), submodule.callOrder.at(i-2)) != submodule.outputs.end()){
                           subcircuitNode.addToOutputOrder(tokens.at(i));
                       }
                   }


                    for(size_t i = 2; i < tokens.size(); i++){
                       if(std::find(submodule.inputs.begin(), submodule.inputs.end(), submodule.callOrder.at(i-2)) != submodule.inputs.end()){
                           std::string inputToken = tokens.at(i);
                           CircuitElement inputNode{ inputToken , isInputOutputOrWire[inputToken] };
                           addEdge(inputNode, subcircuitNode);
                       }
                       else if(std::find(submodule.outputs.begin(), submodule.outputs.end(), submodule.callOrder.at(i-2)) != submodule.outputs.end()){
                           std::string outputToken = tokens.at(i);
                           CircuitElement outputNode{ outputToken , isInputOutputOrWire[outputToken] };
                           addEdge(outputNode, subcircuitNode);
                       }
                   }
                }
            }
        }

    }
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_time = end-start;
    std::cout << circuitName << " - CONVERSION TIME : " << elapsed_time.count() << std::endl;


    return true;


}
Circuit::Circuit (const Circuit & circuit){
    for(auto& input: circuit.inputs){
        inputs.push_back(input);
    }

    for(auto& output: circuit.outputs){
        outputs.push_back(output);
    }

    for(auto& adjListElement: circuit.adjacencyList){
        adjacencyList.push_back(adjListElement);
    }

    for(auto& element : circuit.elements){
        elements.push_back(element);
    }

    for(auto& submodule : circuit.submodules){
        submodules.push_back(submodule);
    }

    for(auto& order : circuit.callOrder){
        callOrder.push_back(order);
    }

    circuitName = circuit.circuitName;
}


bool Circuit::doesElementWithNameExist(std::string name) {
    for(auto & mainCircuitElement : adjacencyList){
        if(name == mainCircuitElement.first.elementName){
            return true;
        }
    }

    return false;
}

void Circuit::sortOutputsWithCallOrder(){
    std::vector<std::pair<std::string,int>> outputCallOrder;

    for(size_t i = 0; i < outputs.size(); i++){
        for(size_t j = 0; j < callOrder.size(); j++){
            if(outputs.at(i) == callOrder.at(j)){
                outputCallOrder.push_back(std::make_pair(outputs.at(i), j));
            }
        }
    }

    sort(outputCallOrder.begin(), outputCallOrder.end(), [=](std::pair<std::string, int>& a, std::pair<std::string, int>& b)
    {
        return a.second < b.second;
    });

    outputs.clear();
    for(size_t i = 0; i < outputCallOrder.size(); i++){
        outputs.push_back(outputCallOrder.at(i).first);
    }
}
void Circuit::sortInputsWithCallOrder(){
    std::vector<std::pair<std::string,int>> inputCallOrder;
    for(size_t i = 0; i < inputs.size(); i++){
        for(size_t j = 0; j < callOrder.size(); j++){
            if(inputs.at(i) == callOrder.at(j)){
                inputCallOrder.push_back(std::make_pair(inputs.at(i), j));
            }
        }
    }

    sort(inputCallOrder.begin(), inputCallOrder.end(), [=](std::pair<std::string, int>& a, std::pair<std::string, int>& b)
    {
        return a.second < b.second;
    });

    inputs.clear();
    for(size_t i = 0; i < inputCallOrder.size(); i++){
        inputs.push_back(inputCallOrder.at(i).first);
    }
}

void Circuit::connectSubmoduleByGraphJoining(Circuit * submodule, std::vector<std::string> parameters, std::map<std::string, CircuitElementType> isInputOutputOrWire){
    if (submoduleCounts.find(submodule->circuitName) != submoduleCounts.end()){
        submoduleCounts[submodule->circuitName]++;
    }

    else{
        submoduleCounts.insert(std::make_pair(submodule->circuitName, 0));
    }

    int currentSubmoduleCount = submoduleCounts[submodule->circuitName];

    // Rename non input/output elements.
    for (auto& element : submodule->elements){
        if (element.elementType != INPUT && element.elementType != OUTPUT){
            if (element.elementType == WIRE){
                std::string generatedWireName = "SM_" + submodule->circuitName + "_" + std::to_string(currentSubmoduleCount)+ "_WIRE_" + std::to_string(gen_submodule_temp_wire_count);
                submodule->changeElementName(element.elementName, generatedWireName);
                gen_submodule_temp_wire_count++;
            }
            else{
                std::string generatedGateName = "SM_" + submodule->circuitName + "_" + std::to_string(currentSubmoduleCount)+ "_GATE_" + std::to_string(gen_submodule_temp_gate_count);
                submodule->changeElementName(element.elementName, generatedGateName);
                gen_submodule_temp_gate_count++;
            }
        }
    }

    // Find corresponding input / outputs from parent to child.
    for (unsigned long i = 0; i < submodule->callOrder.size(); i++){
        submodule->changeElementName(submodule->callOrder[i], parameters[i]);

        // Also, change submodule input/output's type to corresponding element type in parent.
        submodule->changeElementType(parameters[i], isInputOutputOrWire[parameters[i]]);
    }

    // Merge subcircuit to this circuit.
    connectOtherCircuit(submodule);
}

void Circuit::changeElementName(std::string previousName, std::string newName){
    // Rename in elements list.
    for (auto& element : elements){
        if (element.elementName == previousName){
            element.elementName = newName;
        }
    }

    // Rename in adjacency list.
    for (auto& node : adjacencyList){
        for (auto& connectedNode : node.second){
            if (connectedNode.elementName == previousName){
                connectedNode.elementName = newName;
            }
        }

        if (node.first.elementName == previousName){
            node.first.elementName = newName;
        }
    }

    // Rename in inputs list (if it's an input).
    for (auto& input: inputs){
        if (input == previousName){
            input = newName;
        }
    }

    // Rename in outputs list (if it's an output).
    for (auto& output: outputs){
        if (output == previousName){
            output = newName;
        }
    }

    // Rename in call order (if it's an in/output).
    for (auto& callOrderElement : callOrder){
        if (callOrderElement == previousName){
            callOrderElement = newName;
        }
    }
}

void Circuit::changeElementType(std::string elementName, CircuitElementType newType){
    // Change type in elements list.
    for (auto& element : elements){
        if (element.elementName == elementName){
            element.elementType = newType;
        }
    }

    // Change type in adjacency list.
    for (auto& node :adjacencyList){
        for (auto& connectedNode : node.second){
            if (connectedNode.elementName == elementName){
                connectedNode.elementType = newType;
            }
        }

        if (node.first.elementName == elementName){
            node.first.elementType = newType;
        }
    }
}

void Circuit::connectOtherCircuit(Circuit * circuit){

    // For each element of the given (sub)circuit, find corresponding element in this circuit.
    // If there are no matches, create a new element.
    for (auto& node : circuit->adjacencyList){

        // Search for a node with same name.
        // If it's found, append adjacents of connected node to this circuit.
        bool nodeWithSameNameFound = false;
        for (auto& myNode : this->adjacencyList){

            if (node.first == myNode.first){

                nodeWithSameNameFound = true;

                for (auto& adjacentNode : node.second){
                    myNode.second.push_back(adjacentNode);
                }
            }
        }

        // If this circuit element is not in my circuit, crete a new element in my circuit.
        if (!nodeWithSameNameFound){
            this->adjacencyList.push_back(std::make_pair(node.first, std::vector<CircuitElement>()));

            for (auto& adjacentNode : node.second){

                // Get latest added element (the one we created 2 lines above)
                // And push all adjacent nodes to it, simply making a copy from given circuit to my circuit.
                this->adjacencyList.back().second.push_back(adjacentNode);
            }
        }
    }

    // Also add elements to element list if their name is unique.
    for (auto& element : circuit->elements){
        bool elementWithSameNameFound = false;

        for (auto& myElement : this->elements){

            if (element.elementName == myElement.elementName){
                elementWithSameNameFound = true;
            }
        }

        if (!elementWithSameNameFound){
            this->elements.push_back(element);
        }
    }
}
