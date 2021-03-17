#include <iostream>
#include <set>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <stack>
#include <unordered_set>

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
 * This function gets the new index of a state
 * @param key the current state
 * @param mapping the mapping of all the current states
 * @param final_states the set of final states
 * @return new index
 */
int find_and_final_state(set<int> &key, vector<set<int>>& mapping, set<int> &final_states){
  auto it = find(mapping.begin(), mapping.end(), key);
  int ret = distance(mapping.begin(), it);
  return ret;
}

/**
 * This fucntion converts a DFA to the complement of the DFA
 * @param DFA current DFA
 * @param mapping mapping of all the current states
 * @param final_states set of all final states
 * @param ret_final set of new final states
 * @param complement_transition number of transitions
 * @return complement of DFA
 */
vector<State> complementDFA(vector<State> &DFA, vector<set<int>>& mapping, set<int> &final_states,
        vector<int>& ret_final, int& complement_transition){

    vector<State> ret(mapping.size());
    for(auto &a : DFA){
        State state;
        for(auto &var : a.relationships){
            state.relationships[var.first].insert(find_and_final_state(var.second, mapping, final_states));
            complement_transition++;
        }
        vector<int> ans;
        set_intersection(a.states.begin(), a.states.end(), final_states.begin(),final_states.end(),
                         back_inserter(ans));
        if(ans.empty())state.final = true;
        auto temp = find_and_final_state(a.states, mapping, final_states);
        ret[temp] = state;
    }

    return ret;
}

/**
 * This function adds a sink state
 * @param dfa dfa to be added sink state
 * @param transition_options the set of alphabet
 * @param complement_transitions the number of transitions
 */
void add_sink_state(vector<State> &dfa, set<string> &transition_options, int& complement_transitions){
    int index = dfa.size();

    for(auto &i : transition_options){
        for (auto &j : dfa){
            if(j.relationships.find(i)==j.relationships.end()){
                j.relationships[i].insert(index);
                complement_transitions++;
            }
        }
    }
    State state;
    state.final = true;

    for(auto i : transition_options){
        state.relationships[i].insert(index);
        complement_transitions++;
    }

    dfa.push_back(state);
}
int main() {
    //todo: remove freopen
    //freopen("file2","r",stdin);
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

        if(delta != "eps"){
            transition_options.insert(delta);
        }


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
    vector<set<int>> mapping ;
    for (auto dfa : DFA){
        mapping.push_back(dfa.states);
    }



    vector<int> complement_final_states;
    int complement_transitions = 0;
    auto ans = complementDFA(DFA,mapping,final_states, complement_final_states,complement_transitions);

    if(mapping.size()*transition_options.size() != complement_transitions)add_sink_state(ans, transition_options, complement_transitions);

    cout << ans.size() << "\n" << complement_transitions << "\n";

    for(int i = 0; i <ans.size(); i++){
        if(ans[i].final)complement_final_states.push_back(i);
        for(auto &print : ans[i].relationships){
            cout << i << " " <<  print.first << " ";
            for(int j : print.second){
                cout << j << '\n';
            }
        }
    }

    cout << complement_final_states.size() << '\n';
    for (auto u : complement_final_states){
        cout << u << '\n';
    }

    return 0;
}
