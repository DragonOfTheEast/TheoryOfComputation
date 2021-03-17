#include <iostream>
#include <vector>
#include <bits/stdc++.h>
#include <set>
#include <map>
#include <unordered_set>
class rep{
public:
    char ch;
    bool state;

    rep(){
        this->ch = 'c';
        this->state= false;
    }
    rep(char ch, bool state){
        this->ch  = ch;
        this->state = state;
    }
    bool operator ==(const rep &other) const {
        return (this->ch == other.ch && this->state == other.state);
    }

    bool operator !=(const rep &other) const {
        return !(this->ch == other.ch && this->state == other.state);
    }
    bool operator <(rep const &other) const {
        if(this->state < other.state)
            return this->state < other.state;
        else if(this->ch <= other.ch){
            return true;
        }
        return false;
    }
};
int main() {
    //todo:take off
    //std::freopen("file.txt", "r", stdin);
    int n;
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cin >> n;
    std::vector<std::vector<rep>> values;
    std::string x, y, z;
    std::unordered_set<char> my_set;
    std::vector<rep> temp;

    for (int i = 0; i < n; i++) {
        std::cin >> x >> y >> z;
        rep a;
        if (x.length() == 1) {
            a.ch = x[0];
            a.state = true;
            temp.push_back(a);
        }else {
            a.ch = x[1];
            a.state = false;
            temp.push_back(a);
        }

        if (y.length() == 1) {
            a.ch = y[0];
            a.state = true;
            temp.push_back(a);
        }else {
            a.ch = y[1];
            a.state = false;
            temp.push_back(a);
        }

        if (z.length() == 1) {
            a.ch = z[0];
            a.state = true;
            temp.push_back(a);
        }else {
            a.ch = z[1];
            a.state = false;
            temp.push_back(a);
        }
        values.push_back(temp);
        temp.clear();
    }
    int max = pow(2, 26);
    bool current_state = false;
    char variable_for_clause;
    char clause_is_satisfied;
    char mask;
    char x_or;
    char assigned = '\0';
    for (int bin = 0; bin < max; bin++) {
        for (int i = 0; i < n; i++) {
            assigned = '\0';
            current_state = false;
            mask = '\0';
            x_or = '\0';
            for (int count = 0 ; count < 3; count++){
                if(values[i][count].state){
                    mask |= 1 << count;
                }
                else{
                        mask |= 1 << count;
                        x_or |= 1 << count;
                }
                if ((bin & (1 << (values[i][count].ch - 97)))){
                    assigned |= 1 << count;
                }
            }
            if(mask == '\0')break;
            variable_for_clause = assigned & mask;
            clause_is_satisfied = variable_for_clause ^ x_or;

            if (clause_is_satisfied == '\0') { break;}
            else current_state = true;
        }
        if (current_state) {
            std::cout << "yes";
            return 0;
        }
    }
    std::cout << "no";
    return 0;
}
