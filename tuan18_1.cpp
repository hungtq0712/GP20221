#include<iostream>
#include <fstream>
#include<vector>
#include<string.h>
#include<map>
#include <queue>
#include<math.h>
#include <tuple>
#include "ComparisonTimeSpace.h"
using namespace std;
typedef std::tuple<double, double> Point;
typedef std::tuple<Point, Point, double> Shape;
typedef std::tuple<int, double> Index;
typedef std::tuple<std::string, int, Shape, std::vector<Index>, double> TimeSpace;


template<typename T>
std::vector<T> create_copy(std::vector<T> const &vec)
{
    std::vector<T> v(vec);
    return v;
}

double splitNumber(char* shape,int &i){
    double x1=0;
    bool lock=1;
    int sign=1;
    int k=10;
    if(i>strlen(shape))
        return 0.0;
    if(shape[i]=='-'){
        sign=-1;
        i++;
    }
    while(shape[i]!=',' && shape[i]!=' ' && shape[i]!='\0' && shape[i]!='_'){
        //cout<<shape[i]<<endl;
        if(shape[i]=='.'){
            lock=0;
            i++;
            continue;
        }
        if(lock==1)
            x1=x1*10+(double)(shape[i]-'0')*sign;
        else{
            x1=x1+(double)(shape[i]-'0')/k*sign;
            k=k*10;
        }
        i++;
    }
    i++;
    return x1;
}

bool isValid(char *namefile, double x){
    ifstream inFile;
    inFile.open(namefile);
    string line;
    char *line1;
    map < pair<string,int>,bool>Exit;
    while(getline(inFile, line)){
        line1=(char*)line.c_str();
        int i=0;
        //cout<<line<<"\n";
        char *nameEJ= new char[10];
        for(i=0;i<strlen(line1);i++)
            if(line1[i]==' '){
                nameEJ[i]='\0';
                i++;
                break;
            }
            else
                nameEJ[i]=line1[i];
        if(i>=strlen(line1))
            return false;
        int id=(int)splitNumber(line1,i);
        if(i>=strlen(line1))
            return false;
        double x1,x2,y1,y2;
        x1=splitNumber(line1,i);
        if(i>=strlen(line1))
            return false;
        y1=splitNumber(line1,i);
        if(i>=strlen(line1))
            return false;
        x2=splitNumber(line1,i);
        if(i>=strlen(line1))
            return false;
        y2=splitNumber(line1,i);
        if(sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2))-x>0.0001){
            cout<<sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2))-x<<"\n";
            cout<<x1<<" "<<y1<<"  "<<x2<<" "<<y2<<"\n";
            return false;
        }
        //cout<<pair<char*,int> (nameEJ,id).first;
        //cout<<nameEJ<<" "<<id<<"\n";
        if(Exit.find(pair<string,int>(nameEJ,id))==Exit.end()){
            Exit.insert(pair<pair<string,int>,bool> ( pair<string,int> (nameEJ,id),true));
            //std::cout << "mymap.size() is " << Exit.size() << '\n';
        }
        else{
            cout<<line<<"\n";
            return false;
        }

    }
    return true;
}
bool checkValid(std::vector<TimeSpace> *all, double x, double epsilon) {
    vector<TimeSpace>::iterator it;
    double L;
    for ( it = all->begin() ; it != all->end(); it++){
        L=get<2>(get<2>(*it));
        if(fabs(L-x)/x>epsilon)
            return false;
    }

    return true;
}
void assignNeighbors(std::vector<TimeSpace> *all, int index) {
    vector<TimeSpace>::iterator it;
    int index_neighbor=0;
    for (it = all->begin() ; it != all->end(); it++,index_neighbor++){
        if(get<1>(get<2>(all->at(index)))==get<0>(get<2>(*it))){
            (get<3>(all->at(index))).push_back(tuple<int, double> (index_neighbor,0.0));
        }
    }
}
bool checkValidation(vector<vector<TimeSpace> > *graph, double V, double H, double dt){
    int N=ceil(H/dt);
    long numNodes=0;
    long numEdges=0;
    cout<<graph->size()<<"\n";
    for(vector<vector<TimeSpace> > :: iterator it=graph->begin();it!=graph->end();it++)
        numNodes+=(*it).size();
    cout<<"numnode="<<numNodes<<"\n";
    if(numNodes%(N+1)!=0){
            cout<<"exit 1\n";
        return false;
    }
    for(vector<vector<TimeSpace> > :: iterator it=graph->begin();it!=graph->end();it++)
        for(vector<TimeSpace>:: iterator it1=(*it).begin();it1!=(*it).end();it1++)
            numEdges+=get<3>(*it1).size();
    if(numEdges<=N*numNodes){
        cout<<"exit 2\n";
        cout<<numEdges<<" "<<(N+1)*numNodes<<"\n";
        return false;
    }
    //int N=ceil()
    return true;
}
//3a
vector<vector<TimeSpace> >* updateEdge(string name, double length, double H, double dt, vector<vector<TimeSpace> > *graph){
    int N=ceil(H/dt);
    for(int i=0;i<=N;i++){
        vector<TimeSpace> temp = graph->at(i);
        for(int j=0;j<temp.size();j++){
            if(get<0>(temp.at(j))==name){
                vector<Index> indices = get<3>(temp.at(j));
                int S = indices.size() - 1;
                vector<Index> Neighbors;
                copy(indices.begin(), indices.end(), back_inserter(Neighbors));
                Neighbors.erase (Neighbors.begin()+j);
                vector<int> SubNeighbors{};
                vector<int> Remove{};

                for(int k=0;k<=S;k++){
                    int e=get<0>(indices.at(k));
                    if(k!=j){
                        if(get<1>(indices.at(k))>i*dt){
                            if(i+length<=N){
                                get<1>(indices.at(k))=(i+length)*dt;
                                SubNeighbors.push_back(e);
                            }
                            else{
                                Remove.push_back(e);
                            }
                        }
                    }
                    for(vector<Index>::iterator it=Neighbors.begin();it!=Neighbors.end();it++){
                        int find_e=0;
                        for(vector<int>:: iterator it1=SubNeighbors.begin();it1!=SubNeighbors.end();it1++)
                            if(*it1==get<0>(*it)){
                                find_e=1;
                                break;
                            }
                        if(find_e==0 && i+length<=N){
                            indices.push_back(make_tuple(e, (i+length)*dt));
                        }
                    }
                    int index_remove=0;
                    for(vector<int>:: iterator it=Remove.begin();it!=Remove.end();it++){
                        indices.erase(indices.begin()+(*it)-index_remove);
                        index_remove++;
                    }

                }
                get<3>(temp.at(j))=indices;
            }
        }
    }
    return graph;
}
//3b
int find(std::string name, vector<TimeSpace> v){
    int index=0;
    for(vector<TimeSpace>:: iterator it=v.begin();it!=v.end();it++,index++)
        if(get<0>(*it)==name)
            return index;
    return -1;
}
vector<pair<int, int> > getPossibleDestinations(Objective *obj, std::vector<std::vector<TimeSpace>> *graph, double dt, double H, double initTime){
     std::vector<std::pair<int, int> > result;
     priority_queue <ComparisonTimeSpace*,  vector<ComparisonTimeSpace*>, cmp> TSqueue;
     int index = find(obj->name, graph->at(0));
     int Init =  ceil(initTime/dt);
     int N=ceil(H/dt);
     for(int i=0;i<=N;i++){
          TSqueue.push(new ComparisonTimeSpace(i, index,i*dt-initTime,obj,dt));
     }
     while(!TSqueue.empty()){
        ComparisonTimeSpace *c=TSqueue.top();
        TSqueue.pop();
        result.push_back(make_pair(c->getFirstIndex(),c->getSecondIndex()));
     }


    return result;
}

int main(){
    cout<<"Day la chuong trinh thuc hien bai 2 tai link:https://docs.google.com/document/d/1snBJqZHk2xKfge8OAGWh05AgDuYDsExLnakwHrhpXcw/edit\n";
    char *namefile="AllParts.txt";
    vector<TimeSpace> *init = new vector<TimeSpace>;
    ifstream inFile;
    inFile.open(namefile);
    string line;
    char *line1;
    while(getline(inFile, line)){
        line1=(char*)line.c_str();
        int i=0;
        char *nameEJ= new char[10];
        for(i=0;i<strlen(line1);i++)
            if(line1[i]==' '){
                nameEJ[i]='\0';
                i++;
                break;
            }
            else
                nameEJ[i]=line1[i];
        int id=(int)splitNumber(line1,i);
        double x1,x2,y1,y2;
        x1=splitNumber(line1,i);
        y1=splitNumber(line1,i);
        x2=splitNumber(line1,i);
        y2=splitNumber(line1,i);
        Point p1=tuple<double, double> (x1,y1);
        Point p2=tuple<double, double> (x2,y2);
        double L=sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
        Shape s=tuple<Point, Point, double>(p1,p2,L);
        std::vector<Index> myvector{};
        TimeSpace times=tuple<string, int, Shape, vector<Index>, double> (nameEJ,id,s,myvector,0.0);
        init->push_back(times);
    }

    //cout<<checkValid(init,1.41,0.001);
    //cau d
    //giá trị thực H và giá trị thực dt, một vector std::vector<TimeSpace> init, vận tốc V
    vector<TimeSpace>::iterator it;
    int index=0;

    for (it = init->begin() ; it != init->end(); it++,index++)
        assignNeighbors(init,index);
    double H=3,dt=1;
    double V=1;
    int N=ceil(H/dt);
    std::vector<std::vector<TimeSpace> > graph;
    for(int i=0;i<=N;i++){
        std::vector<TimeSpace> temp;
        copy(init->begin(), init->end(), back_inserter(temp));
        //cout<<temp.size()<<"\n";
        for(int j=0;j<temp.size();j++){
            vector<Index> indices = get<3>(temp.at(j));
            int S=indices.size()-1;
            for(int k=0;k<=S;k++){
                indices.at(k) = make_tuple(get<0>(indices.at(k)), i*dt);
            }
            double x=get<2>(get<2>(temp.at(j)));
            int min1=ceil(x/V);
             for(int k=0;k<=S;k++){
                 if(i+min1<=N)
                    indices.push_back(make_tuple(get<0>(indices.at(k)),(i+min1)*dt));
             }
             if((i + 1) <= N)
                indices.push_back(make_tuple(j, (i+1)*dt));
             get<4> (temp.at(j))= i*dt;
             get<3>(temp.at(j))=indices;
        }
        //temp.update(indices);
        //get<3>(temp.at(j))=indices;
        graph.push_back(temp);

    }
    //cout<<"Hop le:"<<isValid("AllParts.txt",1.41);
    cout<<checkValidation(&graph,1,3,1.0);

}
