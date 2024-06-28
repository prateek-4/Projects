#include <iostream>
using namespace std;
#include <vector>
#define printvec(vec) do { \
    std::cout << "["; \
    for (size_t _i = 0; _i < (vec).size(); _i++) { \
        std::cout << (vec)[_i]; \
        if (_i < (vec).size() - 1) { \
            std::cout << ", "; \
        } \
    } \
    std::cout << "]" << std::endl; \
} while (false)
int search_free_slot(vector<int> & mem_loc){
    if(mem_loc[0]!=0)
    {   mem_loc.insert(mem_loc.begin(),0);
        return 0;}
    for(int i=0;i<mem_loc.size()-1;i++){
        if((mem_loc[i+1]-mem_loc[i])>=2)
            {mem_loc.insert(mem_loc.begin()+i+1,i+1);
            return i+1;
            }
    }
    int size=mem_loc.size();
    mem_loc.push_back(size);
    return size;
}
int main(){
    vector<int> mem_loc={1,5,6};
    cout<<search_free_slot(mem_loc)<<endl;
    printvec(mem_loc);
    cout<<search_free_slot(mem_loc)<<endl;
    printvec(mem_loc);
    cout<<search_free_slot(mem_loc)<<endl;
    printvec(mem_loc);
    cout<<search_free_slot(mem_loc)<<endl;
    printvec(mem_loc);
    cout<<search_free_slot(mem_loc)<<endl;
    printvec(mem_loc);
    cout<<search_free_slot(mem_loc)<<endl;
    printvec(mem_loc);
    cout<<search_free_slot(mem_loc)<<endl;
    printvec(mem_loc);
    cout<<search_free_slot(mem_loc)<<endl;
    printvec(mem_loc);
    return 0;
}