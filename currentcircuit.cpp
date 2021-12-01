#include "currentcircuit.h"

#include <stack>
#include <vector>

Circuit CurrentCircuit::circ;

CurrentCircuit::CurrentCircuit()
{

}


void topological_sort_helper(std::string element_name, bool visited[], std::stack<int>& stack) {
    Circuit& circ = CurrentCircuit::circ;
    auto& adj = circ.adjacencyList;
    bool found_element = false;
    int vertex;

    for(int i = 0; i < circ.elements.size(); i++) {
        if(circ.elements[i].elementName == element_name) {
            vertex = i;
            found_element = true;
            break;
        }
    }

    if(!found_element) {
        printf("Element name was incorrect!\n");
        return;
    }

    if(visited[vertex]) {
        return;
    }
    
    // Mark the current node as visited.
    visited[vertex] = true;

    auto& list = adj[vertex].second;
    for(int i = 0; i < list.size(); i++) {
        auto& curr = list[i];
        topological_sort_helper(curr.elementName, visited, stack);
    }

    stack.push(vertex);
}

void CurrentCircuit::topological_sort() {
    Circuit& circ = CurrentCircuit::circ;
    auto& adj = circ.adjacencyList;
    std::stack<int> stack;
    int vertex_count = circ.elements.size();
    bool* visited = new bool[vertex_count];
    for (int i = 0; i < vertex_count; i++)
        visited[i] = false;
    
    for (int i = 0; i < vertex_count; i++)
        if (visited[i] == false)
            topological_sort_helper(circ.elements[i].elementName, visited, stack);
    
    std::vector<std::pair<std::string, int>> top_list {};

    while (stack.empty() == false) {
        int vertex = stack.top();
        auto& element = circ.elements[vertex];
        std::vector<CircuitElement> predecessor_elements;

        for(auto& pair : adj) {
            auto& second = pair.second;
            for(auto& connection : second) {
                if(connection.elementName == element.elementName) {
                    predecessor_elements.push_back(pair.first);
                }
            }
        }
        
        int max_arrival = 0;
        for(auto& predecessor : predecessor_elements) {
            int arrival_time = 0;
            for(int i = 0; i < circ.elements.size(); i++) {
                if(circ.elements[i].elementName == predecessor.elementName) {
                    arrival_time = circ.elements[i].arrivalTime;
                    break;
                }
            }
            if(arrival_time > max_arrival) {
                max_arrival = arrival_time;
            }
        }

        element.arrivalTime += max_arrival + element.delay;

        // if(element.isGate()) {
            top_list.push_back({element.elementName, element.arrivalTime});
        // }
        // printf("%s(d:%d)(a:%d)\n", element.elementName.c_str(), element.delay, element.arrivalTime);
        stack.pop();
    }

    printf("Topological order: ");
    for(auto& element : top_list) {
        printf("%s[%d], ", element.first.c_str(), element.second);
    }
    printf("\n");

    printf("Critical path:\n");
    int slowest_output_index = 0;
    for(int i = 0, current_max = 0; i < top_list.size(); i++) {
        auto& element = top_list[i];
        if(element.second > current_max) {
            current_max = element.second;
            slowest_output_index = i;
        }
    }
    printf("%s[%d]\n", top_list[slowest_output_index].first.c_str(), top_list[slowest_output_index].second);
    while(true) {
        std::vector<CircuitElement> predecessor_elements;
        // get pred elements
        for(auto& pair : adj) {
            auto& second = pair.second;
            for(auto& connection : second) {
                if(connection.elementName == top_list[slowest_output_index].first) {
                    predecessor_elements.push_back(pair.first);
                }
            }
        }
        if(predecessor_elements.empty()) {
            break;
        }
        // hack for dirty ugly datastructure to get arrival times..
        for(auto& pred : predecessor_elements) {
            for(auto& el : circ.elements) {
                if(el.elementName == pred.elementName) {
                    pred.arrivalTime = el.arrivalTime;
                    break;
                }
            }
        }
        // determine slowest pred
        int slowest_pred_index = 0;
        int slowest_pred_arrival_time = 0;
        for(int i = 0; i < predecessor_elements.size(); i++) {
            auto& pred = predecessor_elements[i];
            if(pred.arrivalTime > slowest_pred_arrival_time) {
                slowest_pred_arrival_time = pred.arrivalTime;
                slowest_pred_index = i;
            }
        }
        // find slowest pred in topological sorted list
        CircuitElement& slowest_pred = predecessor_elements[slowest_pred_index];
        int found_index = -1;
        for(int i = 0; i < top_list.size(); i++) {
            if(top_list[i].first == slowest_pred.elementName) {
                found_index = i;
                break; 
            }
        }
        printf("%s[%d] \n", slowest_pred.elementName.c_str(), slowest_pred.arrivalTime);
        slowest_output_index = found_index;
    }
    // printf("DEBUG slowest output index %d\n", slowest_output_index);
    printf("\n");
}
