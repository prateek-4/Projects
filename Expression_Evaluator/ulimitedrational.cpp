//**********************************************************************
// UnlimitedRational
// -----------------
// This is a simple implementation that works but does not reduce the
// faction to its base form.
//**********************************************************************

#include "ulimitedrational.h"
using namespace std;
//helper functions
UnlimitedInt * UNIT = new UnlimitedInt(0);
static bool ui_iszero(UnlimitedInt* ui) {
    int* pui = ui->get_array();
    for (int i = 0; i < ui->get_size(); i++)
        if (pui[i] != 0)
            return false;
    return true;
}
UnlimitedInt* help_gcd(UnlimitedInt* p , UnlimitedInt* q){

    if(ui_iszero(q))
        return p;
    
    //temp2->get_sign()=2;
    UnlimitedInt* temp=UnlimitedInt::mod(p,q);
    return help_gcd(q,temp);
}
UnlimitedInt* get_gcd(UnlimitedInt* p,UnlimitedInt*q){
    if(p->get_sign()==-1||q->get_sign()==-1){
        UnlimitedInt* temp1=new UnlimitedInt(p->get_array(),1,1,p->get_size());
        UnlimitedInt* temp2=new UnlimitedInt(q->get_array(),1,1,q->get_size());
        return help_gcd(temp1,temp2);
    }
    return help_gcd(p,q);
}
void reduce_int(UnlimitedInt* &p, UnlimitedInt* &q){
    int sgn= (p->get_sign())*(q->get_sign());
    UnlimitedInt* temp=get_gcd(p,q);
    UnlimitedInt* temp1=new UnlimitedInt(p->get_array(),1,1,p->get_size());
        UnlimitedInt* temp2=new UnlimitedInt(q->get_array(),1,1,q->get_size());
        p=temp1;
        q=temp2;
    //cout<<temp->get_sign()<<" is the sign"<<endl;
    p=UnlimitedInt::div(p,temp);
    //cout<<"p is now reduced to :"<<p->to_string()<<endl;
    q=UnlimitedInt::div(q,temp);
        temp1=new UnlimitedInt(p->get_array(),1,sgn,p->get_size());
        temp2=new UnlimitedInt(q->get_array(),1,1,q->get_size());
        p=temp1;
        q=temp2;

}
//class functions
UnlimitedRational::UnlimitedRational() {
    p = new UnlimitedInt();
    q = new UnlimitedInt();
}
UnlimitedRational::UnlimitedRational(UnlimitedInt* num, UnlimitedInt* den) {
    p = new UnlimitedInt(num->get_array(), num->get_capacity(), num->get_sign(), num->get_size());
    q = new UnlimitedInt(den->get_array(), den->get_capacity(), den->get_sign(), den->get_size());
}

UnlimitedRational::~UnlimitedRational() {
    delete p;
    delete q;
}

UnlimitedInt* UnlimitedRational::get_p() {
    return p;
}

UnlimitedInt* UnlimitedRational::get_q() {
    return q;
}

string UnlimitedRational::get_p_str() {
    return p->to_string();
}

string UnlimitedRational::get_q_str() {
    return q->to_string();
}
    
string UnlimitedRational::get_frac_str() {
    return p->to_string() + "/" + q->to_string();
}

// Rational arithmetic operations
UnlimitedRational* UnlimitedRational::add(UnlimitedRational* r1, UnlimitedRational* r2) {
    UnlimitedInt* ui1 = UnlimitedInt::mul(r1->get_p(), r2->get_q());
    UnlimitedInt* ui2 = UnlimitedInt::mul(r2->get_p(), r1->get_q());
    UnlimitedInt* num = UnlimitedInt::add(ui1, ui2);
    UnlimitedInt* den = UnlimitedInt::mul(r1->get_q(), r2->get_q());
     if(ui_iszero(den))
         return new UnlimitedRational(UNIT,UNIT);
    // reduce_int(num,den);
    UnlimitedRational* r3 = new UnlimitedRational(num, den);

    delete ui1;
    delete ui2;
    delete num;
    delete den;

    return r3;
}

UnlimitedRational* UnlimitedRational::sub(UnlimitedRational* r1, UnlimitedRational* r2) {
    UnlimitedInt* ui1 = UnlimitedInt::mul(r1->get_p(), r2->get_q());
    UnlimitedInt* ui2 = UnlimitedInt::mul(r2->get_p(), r1->get_q());
    UnlimitedInt* num = UnlimitedInt::sub(ui1, ui2);
    UnlimitedInt* den = UnlimitedInt::mul(r1->get_q(), r2->get_q());
     if(ui_iszero(den))
         return new UnlimitedRational(UNIT,UNIT);
    // reduce_int(num,den);
    UnlimitedRational* r3 = new UnlimitedRational(num, den);

    delete ui1;
    delete ui2;
    delete num;
    delete den;

    return r3;
}

UnlimitedRational* UnlimitedRational::mul(UnlimitedRational* r1, UnlimitedRational* r2) {
    UnlimitedInt* num = UnlimitedInt::mul(r1->get_p(), r2->get_p());
    UnlimitedInt* den = UnlimitedInt::mul(r2->get_q(), r1->get_q());
     if(ui_iszero(den))
        return new UnlimitedRational(UNIT,UNIT);
    // reduce_int(num,den);
    UnlimitedRational* r3 = new UnlimitedRational(num, den);

    delete num;
    delete den;

    return r3;
}

UnlimitedRational* UnlimitedRational::div(UnlimitedRational* r1, UnlimitedRational* r2) {
    UnlimitedInt* num = UnlimitedInt::mul(r1->get_p(), r2->get_q());
    UnlimitedInt* den = UnlimitedInt::mul(r1->get_q(), r2->get_p());
     if(ui_iszero(den))
        return new UnlimitedRational(UNIT,UNIT);
    // reduce_int(num,den);
    UnlimitedRational* r3 = new UnlimitedRational(num, den);

    delete num;
    delete den;

    return r3;
}
