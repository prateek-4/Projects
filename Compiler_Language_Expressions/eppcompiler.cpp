/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "eppcompiler.h"
#include <iostream>
using namespace std;
//......................................
// Helper functions



int search_free_slot(MinHeap &least_mem_loc){
    int idx=least_mem_loc.get_min();
    least_mem_loc.pop();
    return idx;
}
vector<string> gen_targ(ExprTreeNode* root,Parser& targ){
    vector<string> commands;
    if(root==NULL)
        return commands;
    //recursive step
    vector<string> left_commands=gen_targ(root->left,targ);
    vector<string> right_commands=gen_targ(root->right,targ);
    //join the commands
    commands.insert(commands.end(), right_commands.begin(), right_commands.end());
    commands.insert(commands.end(), left_commands.begin(), left_commands.end());

    //handling the current node

    if(root->type=="VAR"){
        //cout<<"VAR type"<<endl;
        int idx=targ.symtable->search(root->id);
        //cout<<"var is "<<root->id<<endl;
        commands.push_back("PUSH mem["+to_string(idx)+"]");
    }
    if(root->type=="VAL"){
        //cout<<"VAL type"<<endl;
        commands.push_back("PUSH "+to_string(root->num));
    }
    if(root->type=="ADD"){
        //cout<<"ADD type"<<endl;
        commands.push_back("ADD");        
    }
    if(root->type=="MUL"){
        //cout<<"MUL type"<<endl;
        commands.push_back("MUL");
    }
    if(root->type=="SUB"){
        //cout<<"SUB type"<<endl;
        commands.push_back("SUB");
    }
    if(root->type=="DIV"){
        //cout<<"DIV type"<<endl;
        commands.push_back("DIV");
    }
    //ok so processed each of the types of nodes
    return commands;

}



//.................................
// Class functions



EPPCompiler::EPPCompiler(){
    memory_size=0;
    output_file="";
}

EPPCompiler::EPPCompiler(string out_file,int mem_limit){
    memory_size=mem_limit;
    this->output_file=out_file;
    for (int i=0;i<mem_limit;i++){//it will store the indexes available to use
        least_mem_loc.push_heap(i);
    }
}

void EPPCompiler::compile(vector<vector<string>> code){
    ofstream out(output_file, ios::out);//overwrite the file if already set of expression is present fromt he previous file
    out.close();
    vector<vector<string>> commands;
    //cout<<"now opening the compiler"<<endl;
    int count=0;
     for(auto i: code){
        //cout<<"now parsing : "<<count<<endl;
        targ.parse(i);
        //cout<<"success parsing : "<<count<<endl;
        count++;
        
        ExprTreeNode* curr=targ.expr_trees.back();
        
            if(curr->left->type=="VAR"){//if its a var type assign a memory address
            //cout<<"now searching for free slot : "<<count<<endl;;
            int idx=search_free_slot(least_mem_loc);
            //cout<<"success finding free slot as : "<<idx<<" and var found is: "<<curr->left->id<<endl;
            targ.symtable->assign_address(curr->left->id,idx);
        }
        if(curr->left->type=="DEL"){
         ;//do nothing here in generate targ commands it is handled   
        }
        commands.push_back(generate_targ_commands());
    }
    for(auto i: commands)
        {write_to_file(i);
        ofstream out(output_file,ios::app);
        out<<"\n";
        out.close();
        }
}

vector<string> EPPCompiler::generate_targ_commands(){
    //cout<<"now generating targ commands"<<endl;
    ExprTreeNode* curr= targ.expr_trees.back();
    vector<string> commands;
    if(curr->left->type=="DEL"){//first handle del type
            int idx=targ.symtable->search(curr->right->id);
            least_mem_loc.push_heap(idx);
            targ.symtable->remove(curr->right->id);
            commands.push_back("DEL = mem["+to_string(idx)+"]");
        return commands;//further steps are unneccesary
    }

    //we will use recursion to generate the commands
    //for the left node of the ass type node we will handle it at last
    commands=gen_targ(curr->right,targ);
    //now we have to handle the left node
    if(curr->left->type=="VAR"){
        ////cout<<"generating command for var type"<<endl;
        int idx=targ.symtable->search(curr->left->id);
        commands.push_back("mem["+to_string(idx)+"] = POP");
        //cout<<"POP TYPE COMMAND FOR VAR"<<endl;
        //cout<<"success generating command for var type"<<endl;
        return commands;
    }
    if(curr->left->type=="RET"){
        //cout<<"generating command for ret type"<<endl;
        commands.push_back("RET = POP");
        //cout<<"POP TYPE COMMAND FOR RET"<<endl;
        //cout<<"success generating command for ret type"<<endl;
        return commands;   
    }
    return commands;


}

void EPPCompiler::write_to_file(vector<string> commands){
    
   //cout<<"new commands"<<endl;
    ofstream out(output_file,ios::app);
    for (auto i: commands)
        {out<<i<<endl;
        // cout<<i<<endl;
        }


    out.close();
    //cout<<"success writing to file"<<endl;
}

EPPCompiler::~EPPCompiler(){
  
}

