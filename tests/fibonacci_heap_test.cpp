#include "gtest/gtest.h"
#include "../src/fibonacci_heap.h"
#include <algorithm>
#include <utility>
#include <chrono>
#include <random>
#include <string>
#include <vector>

using namespace std;

struct player{
    string name;
    int points;

    bool operator<(const player& other) const {
        return points < other.points;
    }
};

random_device rd;
uniform_int_distribution<unsigned int> distribution(0,100);
bernoulli_distribution bernoulli(0.5);

fibonacci_heap<unsigned int> random_fib(unsigned int size){
    fibonacci_heap<unsigned int> res;
    for (unsigned int i = 0; i < size; ++i) {
        unsigned int number = distribution(rd);
        res.insert(number);
    }
    return res;
}

TEST(utils_test, log2ul) {
    EXPECT_EQ(log2ul(0),0);
    EXPECT_EQ(log2ul(1),0);
    EXPECT_EQ(log2ul(2),1);
    EXPECT_EQ(log2ul(3),2);
    EXPECT_EQ(log2ul(4),2);
    EXPECT_EQ(log2ul(5),3);
    EXPECT_EQ(log2ul(6),3);
    EXPECT_EQ(log2ul(7),3);
    EXPECT_EQ(log2ul(8),3);
    EXPECT_EQ(log2ul(9),4);
    EXPECT_EQ(log2ul(4294967295),32);
    EXPECT_EQ(log2ul(9223372036854775807),63);
    EXPECT_EQ(log2ul(9223372036854775808ULL),63);
}

TEST(fibonacci_heap_test, constructor_empty) {
    fibonacci_heap<int> f1;
    fibonacci_heap<double> f2;
    fibonacci_heap<pair<int,unsigned int> > f3;
}

TEST(fibonacci_heap_test, copiar_empty) {
    fibonacci_heap<pair<int,double> > f;
    fibonacci_heap<pair<int,double> > f2;
    fibonacci_heap<pair<int,double> > f1(f);
    f2 = f;
    f = f1;
}

TEST(fibonacci_heap_test, clear_empty) {
    fibonacci_heap<pair<int,double> > f;
    f.clear();
}

TEST(fibonacci_heap_test, swap_empty) {
    fibonacci_heap<pair<int,double> > f1;
    fibonacci_heap<pair<int,double> > f2;
    f1.swap(f2);
}

TEST(fibonacci_heap_test, empty_empty) {
    fibonacci_heap<int> f1;
    fibonacci_heap<double> f2;
    fibonacci_heap<pair<int,unsigned int> > f3;
    EXPECT_TRUE(f1.empty());
    EXPECT_TRUE(f2.empty());
    EXPECT_TRUE(f3.empty());
}

TEST(fibonacci_heap_test, size_empty) {
    fibonacci_heap<int> f1;
    fibonacci_heap<double> f2;
    fibonacci_heap<pair<int,unsigned int> > f3;
    EXPECT_EQ(f1.size(),0);
    EXPECT_EQ(f2.size(),0);
    EXPECT_EQ(f3.size(),0);
}

TEST(fibonacci_heap_test, insert_one) {
    fibonacci_heap<int> f1;
    fibonacci_heap<double> f2;
    fibonacci_heap<pair<int,unsigned int> > f3;
    f1.insert(3);
    f2.insert(2.3);
    f3.insert({-2,1});
}

TEST(fibonacci_heap_test, minimum_one) {
    fibonacci_heap<int> f1;
    fibonacci_heap<double> f2;
    fibonacci_heap<pair<int,unsigned int> > f3;
    f1.insert(3);
    f2.insert(2.3);
    f3.insert({-2,1});
    EXPECT_EQ(f1.minimum(),3);
    EXPECT_DOUBLE_EQ(f2.minimum(),2.3);
    EXPECT_EQ(f3.minimum().first,-2);
    EXPECT_EQ(f3.minimum().second,1);
}

TEST(fibonacci_heap_test, extract_minimum_one) {
    fibonacci_heap<int> f1;
    fibonacci_heap<double> f2;
    fibonacci_heap<pair<int,unsigned int> > f3;
    f1.insert(3);
    f2.insert(2.3);
    f3.insert({-2,1});
    f1.extract_min();
    f2.extract_min();
    f3.extract_min();
}

TEST(fibonacci_heap_test, delete_key_one) {
    fibonacci_heap<int> f1;
    fibonacci_heap<double> f2;
    fibonacci_heap<pair<int,unsigned int> > f3;
    fibonacci_heap<int>::handle h1 = f1.insert(3);
    fibonacci_heap<double>::handle h2 = f2.insert(2.3);
    fibonacci_heap<pair<int,unsigned int> >::handle h3 = f3.insert({-2,1});
    f1.delete_key(h1);
    f2.delete_key(h2);
    f3.delete_key(h3);
}

TEST(fibonacci_heap_test, decrease_key_one) {
    fibonacci_heap<int> f1;
    fibonacci_heap<double> f2;
    fibonacci_heap<pair<int,unsigned int> > f3;
    fibonacci_heap<int>::handle h1 = f1.insert(3);
    fibonacci_heap<double>::handle h2 = f2.insert(2.3);
    fibonacci_heap<pair<int,unsigned int> >::handle h3 = f3.insert({-2,1});
    f1.decrease_key(h1,-2);
    f2.decrease_key(h2,1.1);
    f3.decrease_key(h3,{-2,0});
}

TEST(fibonacci_heap_test, join_empty) {
    fibonacci_heap<pair<int,unsigned int> > f1;
    fibonacci_heap<pair<int,unsigned int> > f2;
    f1.join(f2);
}

TEST(fibonacci_heap_test, handle) {
    fibonacci_heap<pair<double,int> > f1;
    fibonacci_heap<pair<double,int> >::handle h1 = f1.insert({1.1,-1});
    fibonacci_heap<pair<double,int> >::handle h2 = f1.insert({2.2,-2});
    fibonacci_heap<pair<double,int> >::handle h3 = f1.insert({3.3,-3});
    fibonacci_heap<pair<double,int> >::handle h4(h1);
    EXPECT_DOUBLE_EQ(h1->first,1.1);
    EXPECT_DOUBLE_EQ(h2->first,2.2);
    EXPECT_DOUBLE_EQ(h3->first,3.3);
    EXPECT_EQ(h1,h4);
    EXPECT_NE(h1,h2);
    EXPECT_NE(h1,h3);
    EXPECT_NE(h3,h2);
    EXPECT_EQ(f1.minimum(),*h1);
    f1.extract_min();
    h1 = h2;
    h4 = h3;
    EXPECT_EQ(h1,h2);
    EXPECT_EQ(h4,h3);
    EXPECT_NE(h1,h4);
    EXPECT_EQ(h1->second,-2);
    EXPECT_EQ(h4->second,-3);
    EXPECT_EQ(h2->second,-2);
    EXPECT_EQ(h3->second,-3);
    EXPECT_EQ(f1.minimum(),*h1);
    EXPECT_EQ(*h2,*h1);
    EXPECT_EQ(*h3,*h4);
    fibonacci_heap<pair<double,int> > f2;
    h1 = f2.insert({4.7,3});
    f1.join(f2);
    EXPECT_NE(h1,h2);
    EXPECT_EQ(h4,h3);
    EXPECT_NE(h1,h4);
    EXPECT_EQ(h1->second,3);
    EXPECT_EQ(h4->second,-3);
    EXPECT_EQ(h2->second,-2);
    EXPECT_EQ(h3->second,-3);
    EXPECT_EQ(f1.minimum(),*h2);
    EXPECT_NE(*h2,*h1);
    EXPECT_EQ(*h3,*h4);
    f1.extract_min();
    h2 = h1;
    EXPECT_EQ(h1,h2);
    EXPECT_EQ(h4,h3);
    EXPECT_NE(h1,h4);
    EXPECT_EQ(h1->second,3);
    EXPECT_EQ(h4->second,-3);
    EXPECT_EQ(h2->second,3);
    EXPECT_EQ(h3->second,-3);
    EXPECT_EQ(f1.minimum(),*h3);
    EXPECT_EQ(*h2,*h1);
    EXPECT_EQ(*h3,*h4);
}

TEST(fibonacci_heap_test, join_handle) {
    fibonacci_heap<pair<double,int> > f1;
    fibonacci_heap<pair<double,int> > f2;
    fibonacci_heap<pair<double,int> >::handle h1 = f1.insert({1.1,-1});
    fibonacci_heap<pair<double,int> >::handle h2 = f2.insert({1.1,-1});
    fibonacci_heap<pair<double,int> >::handle h3 = f1.insert({3.3,-3});
    EXPECT_DOUBLE_EQ(h1->first,1.1);
    EXPECT_DOUBLE_EQ(h2->first,1.1);
    EXPECT_DOUBLE_EQ(h3->first,3.3);
    EXPECT_NE(h2,h1);
    EXPECT_NE(h1,h3);
    EXPECT_NE(h3,h2);
    EXPECT_EQ(f1.minimum(),*h1);
    EXPECT_EQ(f2.minimum(),*h2);
    f1.join(f2);
    EXPECT_DOUBLE_EQ(h1->first,1.1);
    EXPECT_DOUBLE_EQ(h2->first,1.1);
    EXPECT_DOUBLE_EQ(h3->first,3.3);
    EXPECT_NE(h2,h1);
    EXPECT_NE(h1,h3);
    EXPECT_NE(h3,h2);
    EXPECT_EQ(f1.minimum(),*h1);
    EXPECT_EQ(f1.minimum(),*h2);
}

TEST(fibonacci_heap_test, tipo_raro) {
    fibonacci_heap<player> f1;
    fibonacci_heap<player> f2;
    EXPECT_TRUE(f1.empty());
    EXPECT_TRUE(f2.empty());
    EXPECT_EQ(f1.size(),0);
    EXPECT_EQ(f2.size(),0);
    vector<fibonacci_heap<player>::handle> handles;
    handles.push_back(f1.insert({"FACUNDO",7}));
    handles.push_back(f1.insert({"NICOLAS",24}));
    handles.push_back(f1.insert({"PABLO",17}));
    handles.push_back(f1.insert({"MATIAS",23}));
    handles.push_back(f1.insert({"MIGUEL",26}));
    handles.push_back(f1.insert({"AGUSTIN",46}));
    handles.push_back(f1.insert({"GONZALO",30}));
    handles.push_back(f1.insert({"ENZO",35}));
    handles.push_back(f1.insert({"JUAN",18}));
    handles.push_back(f1.insert({"MIKE",21}));
    handles.push_back(f1.insert({"ANGEL",39}));
    handles.push_back(f1.insert({"RICARDO",52}));
    handles.push_back(f1.insert({"LUCAS",2}));
    handles.push_back(f2.insert({"RODOLFO",38}));
    handles.push_back(f2.insert({"RAUL",41}));
    handles.push_back(f2.insert({"CIRO",1}));
    EXPECT_FALSE(f1.empty());
    EXPECT_FALSE(f2.empty());
    EXPECT_EQ(f1.size(),13);
    EXPECT_EQ(f2.size(),3);
    EXPECT_EQ(f1.minimum().name,handles[12]->name);
    EXPECT_EQ(f2.minimum().name,handles.back()->name);
    f1.extract_min();
    f2.extract_min();
    EXPECT_FALSE(f1.empty());
    EXPECT_FALSE(f2.empty());
    EXPECT_EQ(f1.size(),12);
    EXPECT_EQ(f2.size(),2);
    EXPECT_EQ(f1.minimum().name,handles[0]->name);
    EXPECT_EQ(f2.minimum().name,"RODOLFO");
    f1.join(f2);
    EXPECT_FALSE(f1.empty());
    EXPECT_TRUE(f2.empty());
    EXPECT_EQ(f1.size(),14);
    EXPECT_EQ(f2.size(),0);
    EXPECT_EQ(f1.minimum().name,handles[0]->name);
    f1.extract_min();
    EXPECT_FALSE(f1.empty());
    EXPECT_EQ(f1.size(),13);
    EXPECT_EQ(f1.minimum().name,"PABLO");
    f1.decrease_key(handles[11],{"RICARDO",17});
    EXPECT_FALSE(f1.empty());
    EXPECT_EQ(f1.size(),13);
    EXPECT_EQ(f1.minimum().name,"PABLO");
    EXPECT_EQ(handles[11]->points,17);
    f1.extract_min();
    EXPECT_FALSE(f1.empty());
    EXPECT_EQ(f1.size(),12);
    EXPECT_EQ(f1.minimum().name,"RICARDO");
    f1.delete_key(handles[6]);
    EXPECT_FALSE(f1.empty());
    EXPECT_EQ(f1.size(),11);
    EXPECT_EQ(f1.minimum().name,"RICARDO");
    f1.decrease_key(handles[5],{"AGUSTIN",15});
    EXPECT_FALSE(f1.empty());
    EXPECT_EQ(f1.size(),11);
    EXPECT_EQ(f1.minimum().name,"AGUSTIN");
    EXPECT_EQ(handles[5]->points,15);
    f1.decrease_key(handles[13],{"RODOLFO",18});
    EXPECT_FALSE(f1.empty());
    EXPECT_EQ(f1.size(),11);
    EXPECT_EQ(f1.minimum().name,"AGUSTIN");
    EXPECT_EQ(handles[13]->points,18);
    f1.decrease_key(handles[8],{"JUAN",14});
    EXPECT_FALSE(f1.empty());
    EXPECT_EQ(f1.size(),11);
    EXPECT_EQ(f1.minimum().name,"JUAN");
    EXPECT_EQ(handles[8]->points,14);
    f1.extract_min();
    EXPECT_FALSE(f1.empty());
    EXPECT_EQ(f1.size(),10);
    EXPECT_EQ(f1.minimum().name,"AGUSTIN");
    f1.delete_key(handles[11]);
    EXPECT_FALSE(f1.empty());
    EXPECT_EQ(f1.size(),9);
    EXPECT_EQ(f1.minimum().name,"AGUSTIN");
    f1.clear();
    EXPECT_TRUE(f1.empty());
    EXPECT_EQ(f1.size(),0);
}

TEST(fibonacci_heap_test, copiar_y_borrar) {
    fibonacci_heap<int> f1;
    fibonacci_heap<int> f2;
    vector<fibonacci_heap<int>::handle> handles;
    handles.push_back(f1.insert(-100));
    handles.push_back(f1.insert(-50));
    handles.push_back(f1.insert(-101));
    f1.extract_min();
    f2 = f1;
    EXPECT_EQ(f1.size(),2);
    EXPECT_EQ(f2.size(),2);
    f1.delete_key(handles[1]);
    EXPECT_EQ(f1.size(),1);
    EXPECT_EQ(f2.size(),2);
    f1.decrease_key(handles[0],-200);
    EXPECT_EQ(f1.minimum(),-200);
    EXPECT_EQ(f2.minimum(),-100);
}

TEST(fibonacci_heap_test, arbol_palito) {
    fibonacci_heap<int> f1;
    fibonacci_heap<int> f2;
    vector<fibonacci_heap<int>::handle> handles;
    handles.push_back(f1.insert(-100));
    handles.push_back(f1.insert(-50));
    handles.push_back(f1.insert(-101));
    f1.extract_min();
    handles.back() = f2.insert(10);
    handles.push_back(f2.insert(15));
    handles.push_back(f2.insert(5));
    f2.extract_min();
    f1.join(f2);
    handles.back() = f1.insert(-101);
    f1.extract_min();
    f1.delete_key(handles[1]);
    handles.back() = f2.insert(20);
    handles.push_back(f2.insert(30));
    handles.push_back(f2.insert(40));
    handles.push_back(f2.insert(50));
    handles.push_back(f2.insert(1));
    f2.extract_min();
    f2.delete_key(handles[5]);
    f1.join(f2);
    handles.back() = f1.insert(-101);
    f1.extract_min();
    f1.delete_key(handles[2]);
    f1.delete_key(handles[3]);
    handles.back() = f2.insert(20);
    handles.push_back(f2.insert(30));
    handles.push_back(f2.insert(40));
    handles.push_back(f2.insert(50));
    handles.push_back(f2.insert(60));
    handles.push_back(f2.insert(70));
    handles.push_back(f2.insert(80));
    handles.push_back(f2.insert(90));
    handles.push_back(f2.insert(1));
    f2.extract_min();
    f2.delete_key(handles[9]);
    f2.delete_key(handles[10]);
    f2.delete_key(handles[11]);
    f2.delete_key(handles[13]);
    f1.join(f2);
    handles.back() = f1.insert(-101);
    f1.extract_min();
    f1.delete_key(handles[7]);
    f1.delete_key(handles[6]);
    f1.delete_key(handles[4]);
    EXPECT_FALSE(f1.empty());
    EXPECT_EQ(f1.size(),5);
    EXPECT_EQ(-100,f1.minimum());
    EXPECT_TRUE(f2.empty());
    EXPECT_EQ(f2.size(),0);
}

TEST(fibonacci_heap_test, cascading_cut) {
    fibonacci_heap<int> f;
    vector<fibonacci_heap<int>::handle> handles;
    for (int i = 65; i > 0; --i) {
        handles.push_back(f.insert(i));
    }
    EXPECT_FALSE(f.empty());
    EXPECT_EQ(f.size(),65);
    EXPECT_EQ(f.minimum(),1);
    f.extract_min();
    EXPECT_FALSE(f.empty());
    EXPECT_EQ(f.size(),64);
    EXPECT_EQ(f.minimum(),2);
    for (int j = 0; j < 64; ++j) {
        EXPECT_EQ(*handles[j],65-j);
    }
    f.delete_key(handles[0]);
    f.delete_key(handles[2]);
    f.delete_key(handles[6]);
    f.delete_key(handles[14]);
    f.delete_key(handles[30]);
    f.delete_key(handles[1]);
    EXPECT_FALSE(f.empty());
    EXPECT_EQ(f.size(),58);
    EXPECT_EQ(f.minimum(),2);
}

TEST(fibonacci_heap_test, move_operator) {
    unsigned int n1 = distribution(rd);
    unsigned int n2 = distribution(rd);
    fibonacci_heap<unsigned int> f1(random_fib(n1));
    EXPECT_FALSE(f1.empty());
    EXPECT_EQ(f1.size(),n1);
    fibonacci_heap<unsigned int> f2 = f1;
    f2 = random_fib(n2);
    EXPECT_FALSE(f2.empty());
    EXPECT_EQ(f2.size(),n2);
}

TEST(fibonacci_heap_test, copy_a_lot) {
    unsigned int n1 = distribution(rd);
    unsigned int n2 = distribution(rd);
    fibonacci_heap<unsigned int> f1(random_fib(n1));
    for (unsigned int i = 0; i < n2; ++i) {
        unsigned int n3 = distribution(rd) + 1;
        fibonacci_heap<unsigned int> f2(random_fib(n3));
        unsigned int m = f2.minimum();
        f1 = f2;
        EXPECT_EQ(f1.empty(),f2.empty());
        EXPECT_EQ(f1.size(),f2.size());
        EXPECT_EQ(f1.minimum(),f2.minimum());
        EXPECT_FALSE(f1.empty());
        EXPECT_EQ(f1.size(),n3);
        EXPECT_EQ(f1.minimum(),m);
    }
}

TEST(fibonacci_heap_test, heapsort){
    unsigned int n = distribution(rd);
    vector<unsigned int> v;
    fibonacci_heap<unsigned int> f;
    for (unsigned int i = 0; i < n; ++i) {
        unsigned int number = distribution(rd);
        v.push_back(number);
        f.insert(number);
    }
    sort(v.begin(),v.end());
    vector<unsigned int> res;

    while(!f.empty()) {
        res.push_back(f.minimum());
        f.extract_min();
    }
    EXPECT_EQ(res,v);
}

TEST(fibonacci_heap_test, random_insert_extract){
    unsigned int insert_amount = distribution(rd);
    unsigned int extract_min_amount = min(distribution(rd),insert_amount);
    fibonacci_heap<unsigned int> f;
    unsigned int size = 0;
    unsigned int minimum = 101;
    while(insert_amount && extract_min_amount){
        if(extract_min_amount && size && bernoulli(rd)){
            f.extract_min();
            --size;
            --extract_min_amount;
            if(size){
                minimum = f.minimum();
            }else{
                minimum = 1000;
            }
        }else if(insert_amount){
            unsigned int n = distribution(rd);
            minimum = min(minimum,n);
            f.insert(n);
            ++size;
            --insert_amount;
        }
        EXPECT_EQ(f.empty(),size == 0);
        EXPECT_EQ(f.size(),size);
        if(!f.empty()){
            EXPECT_EQ(f.minimum(),minimum);
        }
    }
}

TEST(fibonacci_heap_test, random_join){
    fibonacci_heap<unsigned int> f1;
    unsigned int size = 1;
    unsigned int minimum = 101;
    f1.insert(101);
    unsigned int join_amount = distribution(rd);
    while(join_amount){
        unsigned int n = distribution(rd);
        fibonacci_heap<unsigned int> f2 = random_fib(n);
        if(bernoulli(rd)){
            f2.insert(0);
            f2.extract_min();
        }
        if(!f2.empty()){
            minimum = min(minimum,f2.minimum());
        }
        f1.join(f2);
        size += n;
        --join_amount;
        EXPECT_TRUE(f2.empty());
        EXPECT_FALSE(f1.empty());
        EXPECT_EQ(f1.size(),size);
        EXPECT_EQ(f1.minimum(),minimum);
    }
    f1.extract_min();
    EXPECT_EQ(f1.size(),size-1);
}

TEST(fibonacci_heap_test, random_all){
    unsigned int insert_amount = distribution(rd);
    unsigned int extract_min_amount = min(distribution(rd),insert_amount);
    unsigned int delete_amount = min(distribution(rd),insert_amount-extract_min_amount);
    unsigned int decrease_amount = distribution(rd);
    unsigned int swap_amount = distribution(rd);
    fibonacci_heap<int> f1;
    fibonacci_heap<int> f2;
    unsigned int size1 = 1;
    int min1 = 101 + distribution(rd);
    fibonacci_heap<int>::handle h1 = f1.insert(min1);
    bool handleValido1 = true;
    unsigned int size2 = 1;
    int min2 = 101 + distribution(rd);
    fibonacci_heap<int>::handle h2 = f2.insert(min2);
    bool handleValido2 = true;
    while(insert_amount || extract_min_amount || delete_amount || decrease_amount || swap_amount){
        if(insert_amount && bernoulli(rd)){
            int n = distribution(rd);
            h1 = f1.insert(n);
            min1 = min(min1,n);
            ++size1;
            handleValido1 = true;
            --insert_amount;
        }else if(extract_min_amount && size1 && bernoulli(rd)){
            f1.extract_min();
            --size1;
            handleValido1 = false;
            --extract_min_amount;
            if(size1){
                min1 = f1.minimum();
            }else{
                min1 = 1000;
            }
        }else if(delete_amount && handleValido1 && bernoulli(rd)){
            f1.delete_key(h1);
            --size1;
            handleValido1 = false;
            --delete_amount;
            if(size1){
                min1 = f1.minimum();
            }else{
                min1 = 1000;
            }
        }else if(decrease_amount && handleValido1 && bernoulli(rd)){
            f1.decrease_key(h1,*h1 - 1);
            min1 = min(min1,*h1);
            --decrease_amount;
        }else if(swap_amount && bernoulli(rd)){
            f1.swap(f2);
            swap(size1,size2);
            swap(min1,min2);
            swap(h1,h2);
            swap(handleValido1,handleValido2);
            --swap_amount;
        }else{
            int n = distribution(rd);
            h1 = f1.insert(n);
            min1 = min(min1,n);
            ++size1;
            handleValido1 = true;
        }
        EXPECT_EQ(f1.empty(),size1 == 0);
        EXPECT_EQ(f1.size(),size1);
        if(handleValido1){
            EXPECT_EQ(f1.minimum(),min1);
        }
        EXPECT_EQ(f2.empty(),size2 == 0);
        EXPECT_EQ(f2.size(),size2);
        if(handleValido2){
            EXPECT_EQ(f2.minimum(),min2);
        }
    }
    f1.join(f2);
    EXPECT_FALSE(f1.empty());
    EXPECT_EQ(f1.size(),size1+size2);
    EXPECT_EQ(f1.minimum(),min(min1,min2));
    EXPECT_TRUE(f2.empty());
    EXPECT_EQ(f2.size(),0);
    f1.clear();
    f2.clear();
    EXPECT_TRUE(f1.empty());
    EXPECT_EQ(f1.size(),0);
    EXPECT_TRUE(f2.empty());
    EXPECT_EQ(f2.size(),0);
}