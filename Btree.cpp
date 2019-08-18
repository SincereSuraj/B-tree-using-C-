
#include<string.h>
#include <iostream>

#define NUMBERLENGTH 10
using namespace std;

typedef struct userdata{
    char name[];
    char address[];
    char email[];
    char number[NUMBERLENGTH];
};

typedef struct node{
    node *pointerparent;
    node *pointer[6];
    userdata data[5];
    bool leafnode;
    int datanum;
};

node *basepointer = NULL;

node *AddNewNode(node *emptynode)
{
    emptynode = new node;
    emptynode->datanum = -1;
    for(int i = 0;i<6;i++){
        emptynode->pointer[i] = NULL;
    }
    if(basepointer == NULL){
        basepointer = emptynode;
    }
    emptynode->pointerparent = NULL;
    return emptynode;
}

node *Traverse(char *inputname,node *worknode)
{
    int i,looplength;
    if(worknode == NULL){
        cout<<"NULL pointer input to traversal";
        return NULL;
    }
    else{
        while(1){
            if(worknode->leafnode){
                cout<<"End of the b_tree reacherd"<<endl;
                return worknode;
            }
            looplength = (worknode->datanum);
            for(i = 0;i<=looplength;i++){
                if(strcmp(worknode->data[i].name,inputname)==0){
                    cout<<"Same name error"<<endl;
                    return worknode;
                }else if(strcmp(worknode->data[i].name,inputname)>0){
                    worknode = worknode->pointer[i];
                    break;
                }else if(i==looplength&&strcmp(worknode->data[i].name,inputname)<0){
                    worknode = worknode->pointer[i+1];
                    break;
                }
            }
        }
    }
}

void SplitChild(node *fullnode)
{
    cout<<"into split child:\n";
    int i;
    userdata middata;
    node *lownode,*highnode,*parentnode;
    lownode = AddNewNode(lownode);
    highnode = AddNewNode(highnode);
    middata = fullnode->data[2];
    if(fullnode->leafnode){
        lownode->leafnode = true;
        highnode->leafnode = true;
    }else{
        lownode->leafnode = false;
        highnode->leafnode = false;
    }
    for(i=0;i<2;i++){
        lownode->data[i] = fullnode->data[i];
        lownode->pointer[i] = fullnode->pointer[i];
        highnode->data[i] = fullnode->data[i+3];
        highnode->pointer[i] = fullnode->pointer[i+3];
    }
    lownode->pointer[2] = fullnode->pointer[2];
    highnode->pointer[2] = fullnode->pointer[5];
    lownode->datanum=1;
    highnode->datanum=1;
    parentnode = fullnode->pointerparent;

    delete fullnode;
    if(parentnode==NULL){
        parentnode = AddNewNode(parentnode);
        basepointer = parentnode;
        parentnode->leafnode = false;
        i = -1;
    }else{
        for(i = parentnode->datanum;i>=0;i--){
            if(strcmp(parentnode->data[i].name,middata.name)>0){
                parentnode->data[i+1]=parentnode->data[i];
                parentnode->pointer[i+2]= parentnode->pointer[i+1];
            }else if(strcmp(parentnode->data[i].name,middata.name)<0){
                break;
            }
        }
    }
    cout<<i<<" int in split\n";
    parentnode->data[1+i] = middata;
    parentnode->pointer[1+i] = lownode;
    parentnode->pointer[2+i] = highnode;
    lownode->pointerparent = parentnode;
    highnode->pointerparent = parentnode;
    for(i=0;i<3&&!lownode->leafnode&&!highnode->leafnode;i++){
        cout<<i<<" this is the valuee of pointer i "<<endl;
        lownode->pointer[i]->pointerparent = lownode;
        highnode->pointer[i]->pointerparent = highnode;
    }

    parentnode->datanum++;
    if(parentnode->datanum>=4){
        SplitChild(parentnode);
    }
}

void MergeParent(node *criticalnode)
{
    int j,k;
    userdata emptydata;
    if(criticalnode == basepointer && basepointer->datanum==0){
        return;
    }
    node *parentnode = criticalnode->pointerparent;
    for(j = 0;j<=parentnode->datanum;j++){
        if(j==0&&(strcmp(parentnode->data[j].name,criticalnode->data[0].name)>0)){
            if((parentnode->pointer[j+1]->datanum)>1){
                criticalnode->data[++criticalnode->datanum] = parentnode->data[j];
                parentnode->data[j] = parentnode->pointer[j+1]->data[0];
                criticalnode->pointer[criticalnode->datanum+1] = parentnode->pointer[j+1]->pointer[0];
                for(k=0;k<parentnode->pointer[j]->datanum;k++){
                    parentnode->pointer[j]->data[k] = parentnode->pointer[j]->data[k+1];
                    parentnode->pointer[j]->pointer[k] = parentnode->pointer[j]->pointer[k+1];
                }
                parentnode->pointer[j]->pointer[k] = parentnode->pointer[j]->pointer[k+1];
                parentnode->pointer[j+1]->data[parentnode->pointer[j+1]->datanum] = emptydata;
                parentnode->pointer[j]->datanum--;
            }
            else{
                criticalnode->data[++criticalnode->datanum]=parentnode->data[j];
                for(k=0;k<=parentnode->pointer[j+1]->datanum;k++)
                {
                    criticalnode->data[++criticalnode->datanum] = parentnode->pointer[j+1]->data[k];
                    criticalnode->pointer[criticalnode->datanum] = parentnode->pointer[j+1]->pointer[k];
                }
                criticalnode->pointer[1+criticalnode->datanum] = parentnode->pointer[j+1]->pointer[k];
                delete parentnode->pointer[j+1];
                for(k = j;k<parentnode->datanum;k++)
                {
                    parentnode->data[k] = parentnode->data[k+1];
                    parentnode->pointer[k+1] = parentnode->pointer[k+2];
                }
                parentnode->data[parentnode->datanum] = emptydata;
                parentnode->pointer[parentnode->datanum+1] = NULL;
                parentnode->datanum--;
            }
            j = -1;
            break;
        }
        else if(j>0&&(strcmp(parentnode->data[j].name,criticalnode->data[0].name)>0)){
            if((parentnode->pointer[j-1]->datanum)>1){
                for(k=criticalnode->datanum;k>=0;k--){
                    criticalnode->data[k+1] = criticalnode->data[k];
                    criticalnode->pointer[k+2] = criticalnode->pointer[k+1];
                }
                criticalnode->pointer[k+2] = criticalnode->pointer[k+1];
                criticalnode->data[0] = parentnode->data[j-1];
                criticalnode->pointer[0] = parentnode->pointer[j-1]->pointer[parentnode->pointer[j-1]->datanum+1];
                parentnode->data[j-1] = parentnode->pointer[j-1]->data[parentnode->pointer[j-1]->datanum];
                parentnode->pointer[j-1]->data[parentnode->pointer[j-1]->datanum] = emptydata;
                parentnode->pointer[j-1]->pointer[parentnode->pointer[j-1]->datanum+1] = NULL;
                parentnode->pointer[j-1]->datanum--;
            }
            else if((parentnode->pointer[j+1]->datanum)>1){
                criticalnode->data[++criticalnode->datanum] = parentnode->data[j];
                parentnode->data[j] = parentnode->pointer[j+1]->data[0];
                criticalnode->pointer[criticalnode->datanum+1] = parentnode->pointer[j+1]->pointer[0];
                for(k = 0;k<parentnode->pointer[j]->datanum;k++){
                    parentnode->pointer[j]->data[k] = parentnode->pointer[j]->data[k+1];
                    parentnode->pointer[j]->pointer[k] = parentnode->pointer[j]->pointer[k+1];
                }
                parentnode->pointer[j]->pointer[k] = parentnode->pointer[j]->pointer[k+1];
                parentnode->pointer[j+1]->data[parentnode->pointer[j+1]->datanum] = emptydata;
                parentnode->pointer[j]->datanum--;
            }
            else{
                criticalnode->data[++criticalnode->datanum]=parentnode->data[j];
                for(k = 0;k<=parentnode->pointer[j+1]->datanum;k++)
                {
                    criticalnode->data[++criticalnode->datanum] = parentnode->pointer[j+1]->data[k];
                    criticalnode->pointer[++criticalnode->datanum] = parentnode->pointer[j+1]->pointer[k];
                }
                criticalnode->pointer[++criticalnode->datanum] = parentnode->pointer[j+1]->pointer[k];
                delete parentnode->pointer[j+1];
                for(k = j;k<parentnode->datanum;k++)
                {
                    parentnode->data[k] = parentnode->data[k+1];
                    parentnode->pointer[k+1] = parentnode->pointer[k+2];
                }
                parentnode->datanum--;

            }
            j = -1;
            break;
        }
    }
    if((j>parentnode->datanum)&&strcmp(parentnode->data[parentnode->datanum].name,criticalnode->data[0].name)<0){
        if((parentnode->pointer[j-1]->datanum)>1){
                for(k=criticalnode->datanum;k>=0;k--){
                    criticalnode->data[k+1] = criticalnode->data[k];
                    criticalnode->pointer[k+2] = criticalnode->pointer[k+1];
                }
                criticalnode->pointer[k+2] = criticalnode->pointer[k+1];
                criticalnode->data[0] = parentnode->data[j-1];
                criticalnode->pointer[0] = parentnode->pointer[j-1]->pointer[parentnode->pointer[j-1]->datanum+1];
                parentnode->data[j-1] = parentnode->pointer[j-1]->data[parentnode->pointer[j-1]->datanum];
                parentnode->pointer[j-1]->data[parentnode->pointer[j-1]->datanum] = emptydata;
                parentnode->pointer[j-1]->pointer[parentnode->pointer[j-1]->datanum+1] = NULL;
                parentnode->pointer[j-1]->datanum--;
            }
        else{
            parentnode->pointer[j-1]->data[++parentnode->pointer[j-1]->datanum] = parentnode->data[j-1];
            parentnode->pointer[j-1]->pointer[1+parentnode->pointer[j-1]->datanum] = criticalnode->pointer[0];
            for(k=0;k<=criticalnode->datanum;k++){
                parentnode->pointer[j-1]->data[++parentnode->pointer[j-1]->datanum] = criticalnode->data[k];
                parentnode->pointer[j-1]->pointer[1+parentnode->pointer[j-1]->datanum] = criticalnode->pointer[k+1];
            }
            parentnode->data[parentnode->datanum] = emptydata;
            parentnode->datanum--;
            delete criticalnode;
        }
    }
    if(parentnode->datanum<=1 && parentnode!=basepointer){
        MergeParent(parentnode);
    }else if(basepointer->datanum<0){
        basepointer = parentnode->pointer[0];
        basepointer->pointerparent = NULL;
        delete parentnode;
    }
}

void InsertData(userdata input)
{
    int i;
    node *worknode;
    if(basepointer == NULL){
        worknode = AddNewNode(worknode);
        worknode->data[0] = input;
        worknode->pointerparent = NULL;
        worknode->leafnode = true;
        worknode->datanum++;
    }else{
        worknode = Traverse(input.name,basepointer);
        for(i = worknode->datanum;i>=0;i--){
                cout<<"\nHere is it :"<<i<<"\n";
            if(strcmp(worknode->data[i].name,input.name)==0){
                cout<<"same name error"<<endl;
                return;
            }else if(strcmp(worknode->data[i].name,input.name)>0){
                worknode->data[i+1]=worknode->data[i];
                continue;
            }else if(strcmp(worknode->data[i].name,input.name)<0){
                break;
            }
        }
        cout<<"\nHere is it :"<<i<<"\n";
        worknode->data[i+1] = input;
        worknode->datanum++;

        if(worknode->datanum>=4){
            SplitChild(worknode);
        }
    }
}

userdata SearchData(char *name)
{
    int i,looplength;
    node *worknode = basepointer;
    while(1){
        if(worknode==NULL){
            cout<<"Data not found"<<endl;
            userdata emptydata;
            return emptydata;
        }else{
            int looplength = (worknode->datanum);
            for(i = 0;i<=looplength;i++){
                cout<<worknode->data[i].name<< "  ";
                if(strcmp(worknode->data[i].name,name)==0){
                    cout<<"match found"<<endl;
                    return worknode->data[i];
                }else if(strcmp(worknode->data[i].name,name)>0){
                    worknode = worknode->pointer[i];
                    break;
                }else if(i==looplength&&strcmp(worknode->data[i].name,name)<0){
                    worknode = worknode->pointer[i+1];
                }
            }
        }
    }
}

void DeleteData(char *inputname){
    int i,j,looplength,k;
    node *parentnode,*worknode = basepointer;
    userdata emptydata;
    if(worknode == NULL){
        cout<<"NULL pointer input to Deletion";
        return;
    }else{
        while(1){
            if(worknode->leafnode){
                cout<<"End of the b_tree reacherd"<<endl;
                //deletion from leaf node work here
                for(i = 0;i<=worknode->datanum;i++){
                    if(strcmp(worknode->data[i].name,inputname)==0){
                        switch(worknode->datanum){
                            case 2:
                                do{
                                    worknode->data[i] = worknode->data[i+1];
                                    i++;
                                }while(i<worknode->datanum);
                                worknode->data[i] = emptydata;
                                worknode->datanum--;
                                return;
                                break;
                            case 1:
                                cout<<"parent node: "<<worknode->pointerparent<<"\nwoeknode"<<worknode<<endl;
                                parentnode = worknode->pointerparent;
                                looplength = parentnode->datanum;
                                for(j = 0;j<=looplength;j++){
                                    cout<<"B-tree"<<parentnode->data[j].name<<" Data "<<worknode->data[i].name<<endl;
                                    if(j==0&&(strcmp(parentnode->data[j].name,worknode->data[i].name)>0)){
                                        if((parentnode->pointer[j+1]->datanum)>1){
                                            for(int k=i;k<worknode->datanum;k++){
                                                worknode->data[k] = worknode->data[k+1];
                                            }
                                            worknode->data[worknode->datanum] = parentnode->data[j];
                                            parentnode->data[j] = parentnode->pointer[j+1]->data[0];
                                            for(k = 0;k<parentnode->pointer[j+1]->datanum;k++){
                                                parentnode->pointer[j+1]->data[k] = parentnode->pointer[j+1]->data[k+1];
                                            }
                                            parentnode->pointer[j+1]->data[parentnode->pointer[j+1]->datanum] = emptydata;
                                            parentnode->pointer[j+1]->datanum--;
                                        }
                                        else{
                                            for(int k = i;k<worknode->datanum;k++){
                                                worknode->data[k] = worknode->data[k+1];
                                            }
                                            worknode->data[worknode->datanum] = emptydata;
                                            worknode->datanum--;
                                            //works for this case only..
                                            for(k = parentnode->pointer[j+1]->datanum;k>=0;k--){
                                                parentnode->pointer[j+1]->data[k+2] = parentnode->pointer[j+1]->data[k];
                                            }
                                            for(k=0;k<=worknode->datanum;k++){
                                                parentnode->pointer[j+1]->data[k] = worknode->data[k];
                                            }
                                            delete worknode;
                                            parentnode->pointer[j+1]->data[k] = parentnode->data[j];
                                            parentnode->pointer[j] = parentnode->pointer[j+1];
                                            for(k=j;k<parentnode->datanum;k++){
                                                parentnode->data[j] = parentnode->data[j+1];
                                                parentnode->pointer[j+1] = parentnode->pointer[j+2];
                                            }
                                            parentnode->data[parentnode->datanum] = emptydata;
                                            parentnode->datanum--;
                                            if(parentnode->datanum<0){
                                                basepointer = parentnode->pointer[j+1];
                                                parentnode->pointer[j+1]->pointerparent = NULL;
                                                delete parentnode;
                                                break;
                                            }
                                        }
                                        j = 0;
                                        break;
                                    }
                                    else if(j>0&&(strcmp(parentnode->data[j].name,worknode->data[i].name)>0)){
                                        if((parentnode->pointer[j-1]->datanum)>1){
                                            for(int k=i;k>0;k--){
                                                worknode->data[k] = worknode->data[k-1];
                                            }
                                            worknode->data[0] = parentnode->data[j-1];
                                            parentnode->data[j-1] = parentnode->pointer[j-1]->data[parentnode->pointer[j-1]->datanum];
                                            parentnode->pointer[j-1]->data[parentnode->pointer[j-1]->datanum] = emptydata;
                                            parentnode->pointer[j-1]->datanum--;
                                        }
                                        else if((parentnode->pointer[j+1]->datanum)>1){
                                            for(int k=i;k<worknode->datanum;k++){
                                                worknode->data[k] = worknode->data[k+1];
                                            }
                                            worknode->data[worknode->datanum] = parentnode->data[j];
                                            parentnode->data[j] = parentnode->pointer[j+1]->data[0];
                                            for(k=0;k<parentnode->pointer[j+1]->datanum;k++){
                                                parentnode->pointer[j+1]->data[k] = parentnode->pointer[j+1]->data[k+1];
                                            }
                                            parentnode->pointer[j+1]->data[parentnode->pointer[j+1]->datanum] = emptydata;
                                            parentnode->pointer[j+1]->datanum--;
                                        }
                                        else{
                                            for(k = i;k<worknode->datanum;k++){
                                                worknode->data[k] = worknode->data[k+1];
                                            }
                                            worknode->datanum--;
                                            parentnode->pointer[j-1]->data[++parentnode->pointer[j-1]->datanum] = parentnode->data[j-1];
                                            for(k=0;k<=worknode->datanum;k++){
                                                parentnode->pointer[j-1]->data[++parentnode->pointer[j-1]->datanum] = worknode->data[k];
                                            }
                                            delete worknode;
                                            for(k=j;k<=parentnode->datanum;k++){
                                                parentnode->data[k-1] = parentnode->data[k];
                                                parentnode->pointer[k] = parentnode->pointer[k+1];
                                            }

                                            parentnode->data[parentnode->datanum] = emptydata;
                                            parentnode->pointer[parentnode->datanum+1] = NULL;
                                            parentnode->datanum--;
                                            if(parentnode->datanum<0){
                                                basepointer = parentnode->pointer[j-1];
                                                parentnode->pointer[j-1]->pointerparent = NULL;
                                                delete parentnode;
                                            }

                                        }
                                        j = 0;
                                        break;
                                    }
                                }
                                if((j>looplength)&&strcmp(parentnode->data[parentnode->datanum].name,worknode->data[i].name)<0){
                                    if((parentnode->pointer[j]->datanum)>1){
                                        for(int k=i;k>0;k--){
                                            worknode->data[k] = worknode->data[k-1];
                                        }
                                        worknode->data[0] = parentnode->data[j-1];
                                        parentnode->data[j-1] = parentnode->pointer[j-1]->data[parentnode->pointer[j-1]->datanum];
                                        parentnode->pointer[j-1]->data[parentnode->pointer[j-1]->datanum] = emptydata;
                                        parentnode->pointer[j-1]->datanum--;
                                    }
                                    else{
                                        for(k = i;k<worknode->datanum;k++){
                                            worknode->data[k] = worknode->data[k+1];
                                        }
                                        worknode->datanum--;
                                        parentnode->pointer[j-1]->data[++parentnode->pointer[j-1]->datanum] = parentnode->data[j-1];
                                        for(k=0;k<=worknode->datanum;k++){
                                            parentnode->pointer[j-1]->data[++parentnode->pointer[j-1]->datanum] = worknode->data[k];
                                        }
                                        delete worknode;
                                        for(k=j;k<=parentnode->datanum;k++){
                                            parentnode->data[k-1] = parentnode->data[k];
                                            parentnode->pointer[k] = parentnode->pointer[k+1];
                                        }

                                        parentnode->data[parentnode->datanum] = emptydata;
                                        parentnode->pointer[parentnode->datanum+1] = NULL;
                                        parentnode->datanum--;
                                        if(parentnode->datanum<0){
                                            basepointer = parentnode->pointer[j-1];
                                            parentnode->pointer[j-1]->pointerparent = NULL;
                                            delete parentnode;
                                            break;
                                        }

                                    }
                                }
                                if(parentnode->datanum<1){
                                    MergeParent(parentnode);
                                }
                                return;
                                break;
                            default:
                                cout<<"there is an error in b tree";
                        }
                    }
                }
                cout<<"\nData match not found!";
                return;
            }
            looplength = (worknode->datanum);
            for(i = 0;i<=looplength;i++){
                if(strcmp(worknode->data[i].name,inputname)==0){
                    //deletion from internal nodes
                    node *left,*right;
                    left = worknode->pointer[i];
                    right = worknode->pointer[i+1];
                    while(!(left->leafnode&&right->leafnode)){
                        left = left->pointer[left->datanum+1];
                        right = right->pointer[0];
                    }
                    while(left!=worknode&&right!=worknode){
                        if(left->datanum <= 1 && right->datanum<=1){
                            for(j = 0;j<=right->datanum;j++){
                                left->data[++left->datanum] = right->data[j];
                                left->pointer[left->datanum+1] = right->pointer[j+1];
                            }
               //           left->pointer[left->datanum+1] = right->pointer[j];
                            right = right->pointerparent;
                            left = left->pointerparent;
                            delete right->pointer[0];
                            continue;
                        }
                        else if(left->datanum>1){
                            worknode->data[i] = left->data[left->datanum];
                            left->data[left->datanum] = emptydata;
                            right->pointer[0] = left->pointer[left->datanum+1];
                            left->pointer[left->datanum+1] = NULL;
                            left->datanum--;
                        }
                        else if(right->datanum>1){
                            worknode->data[i] = right->data[0];
                            for(j = 0;j<right->datanum;j++){
                                right->data[j] = right->data[j+1];
                                right->pointer[j] = right->pointer[j+1];
                            }
                            right->pointer[j] = right->pointer[j+1];
                            right->datanum--;
                        }
                        return;
                    }
                    if((right==worknode)&&(left==worknode)){
                        for(j=i;j<worknode->datanum;j++){
                            worknode->data[j] = worknode->data[j+1];
                            worknode->pointer[j+1] = worknode->pointer[j+2];
                        }
                        worknode->pointer[worknode->datanum+1] = NULL;
                        worknode->data[worknode->datanum] = emptydata;
                        worknode->datanum--;
                        if(worknode->datanum<0){
                            basepointer = worknode->pointer[0];
                            worknode->pointer[0]->pointerparent = NULL;
                            delete worknode;
                        }
                        if(worknode->datanum<1){
                            MergeParent(worknode);
                        }
                        return;
                    }
                }else if(strcmp(worknode->data[i].name,inputname)>0){
                    worknode = worknode->pointer[i];
                    break;
                }else if(i==looplength&&strcmp(worknode->data[i].name,inputname)<0){
                    worknode = worknode->pointer[i+1];
                    break;
                }
            }
        }
    }
}

void DisplayData(node *traversal)
{
    int i;
    for(i = 0;i<=traversal->datanum;i++){
        if(!traversal->leafnode){
            DisplayData(traversal->pointer[i]);
        }
        cout<<traversal->data[i].name<< " in node "<<traversal<<"\n";
    }
    if(!traversal->leafnode){
        DisplayData(traversal->pointer[i]);
    }
}
// 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18
void Editdata(char *searchname,userdata info)
{
    DeleteData(searchname);
    InsertData(info);
}
