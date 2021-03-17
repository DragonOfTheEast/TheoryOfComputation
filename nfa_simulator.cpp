/**
 * This file contains the simulation of an nfa with epsilon closure
 */
#include <iostream>
#include <set>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <stack>

using namespace std;

/**
 * This class is representation of a state in the NFA/DFA
 */
class State{
public:
    set<int> states;
    bool start = false;
    bool final = false;
    unordered_map<string, set<int>> relationships;

    /**
     * this function resets the parameters of a state
     */
    void clear(){
        states = set<int>();
        start = final = false;
        relationships = unordered_map<string, set<int>>();
    }
};

/**
 * This function converts and NFA to DFA
 * @param NFA a non deterministic finite automata
 * @return a deterministic automata
 */
vector<State> NFA_to_DFA(vector<State>& NFA){
    vector<State> table;
    set <int> start = NFA[0].relationships["eps"];
    set <set<int>> found_states;
    found_states.insert(start);
    stack<set<int>> stack1;
    stack1.push(start);
    State temp;
    temp.start = true;

    //building the dfa
    while(!stack1.empty()){
        auto u = stack1.top(); stack1.pop();
        temp.states = u;

        for(auto i:u){
            for (auto j : NFA[i].relationships){
                set<int> temp_set;
                if (j.first != "eps") {
                    for(auto k : j.second){

                        for(auto l: NFA[k].relationships["eps"]) {

                            temp_set.insert(l);
                        }
                    }
                    for (auto h : temp_set){
                        temp.relationships[j.first].insert(h);
                    }

                    if(found_states.find(temp.relationships[j.first]) == found_states.end()){
                        found_states.insert(temp.relationships[j.first]);
                        stack1.push(temp.relationships[j.first]);
                    }
                }
            }
        }
        table.emplace_back(temp);
        temp.clear();
    }
    return table;

}
/**
 * This function gets the epsilon closure of a state
 * @param NFA a non determistic finite automata
 * @param i the state
 * @return the epsilon closure of the state
 */
set<int> e_closure(vector<State>&NFA, int i){
    vector<bool> states(NFA.size(), false);
    stack<int> stack1;
    for(auto val : NFA[i].relationships["eps"]){
        states[val] = true;
        stack1.push(val);
    }
    while(!stack1.empty()){
        int u = stack1.top(); stack1.pop();
        for(auto i : NFA[u].relationships["eps"]){
            if(!states[i]){
                states[i] = true;
                stack1.push(i);
            }
        }
    }
    set<int> ans;
    for(int i = 0; i < NFA.size(); i++){
        if (states[i])ans.insert(i);
    }
    return ans;
}


/**
 * This function simulates a DFA
 * @param DFA a determinisitc finite automata
 * @param input the string using to simulate the DFA
 * @param final_states the accepted states
 * @return if it is an accepted string or not
 */
bool DFA_simulation(vector<State> DFA, string input, set<int> final_states){

    //if empty string just check the intersection of start state and final states
    if(input == ""){
        vector<int> ans;
        set_intersection(DFA[0].states.begin(), DFA[0].states.end(), final_states.begin(),final_states.end(),
                         back_inserter(ans));
        return !ans.empty();
    }
    int curr = 0;
    for(char i : input){
            string temp = "";
            temp += i;
            if(DFA[curr].relationships.find(temp) == DFA[curr].relationships.end() ){
                return false;
            }
            set<int> next = DFA[curr].relationships[temp];

            for(int state = 0; state < DFA.size(); state++){
                if(DFA[state].states!= next){
                    continue;
                }
                curr = state;
            }
    }

    set<int> poss_ans = DFA[curr].states;
    vector<int> ans;
    set_intersection(poss_ans.begin(), poss_ans.end(), final_states.begin(),final_states.end(),
            back_inserter(ans));
    return !ans.empty();
}
int main() {
    //todo: remove freopen
    freopen("file2","r",stdin);
    int states; int transitions;
    set<string> transition_options;
    set<int> final_states;
    int a, c;
    string delta;
    int num_final_states;

    cin >> states;
    cin >> transitions;
    vector<State> NFA(states);

    for(int i = 0; i < states; i++){
        NFA[i].relationships["eps"].insert(i);
        NFA[i].states.insert(i);
    }
    for(int i = 0; i < transitions; i++){
        cin >> a >> delta >> c;

        NFA[a].relationships[delta].insert(c);
        NFA[a].states.insert(a);

        if(a == 0) NFA[a].start = true;
        transition_options.insert(delta);
    }

    cin >> num_final_states;
    for(int i = 0 ; i < num_final_states; i++){
        cin >> a;

        final_states.insert(a);
        final_states.insert(a);
        NFA[a].final = true;
    }

    for(int i = 0; i < states; i++) {
        NFA[i].relationships["eps"] = e_closure(NFA, i);
    }
    auto DFA = NFA_to_DFA(NFA);

    int num;
//    cin >> num;
//    string in;
//    vector<string> input;
//    cin.ignore();
//    for(int i = 0; i <num; i++ ){
//        getline(cin, in);
//        input.push_back(in);
//    }
//
//    for (string i: input){
//        bool res = DFA_simulation(DFA, i,final_states);
//
//        if(res){
//            cout << "accept\n";
//        }else{
//            cout << "reject\n";
//        }
//    }
    return 0;
}
