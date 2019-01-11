#include <cstdio>
#include <cstdlib>
#include <vector>
#include <queue>
using namespace std;
int nVal;
int nPointer;
struct node {
    bool leaf;
    bool isRoot;
    node *par;
    vector<int>value;
    vector<node*>child;

    node *last;
};

node* getTargetNode(node *tNode, int val){
    if(tNode->leaf) return tNode;
    int i;
    for(i=0;i<tNode->value.size();i++){
        if(tNode->value[i]>val) break;
    }
    return getTargetNode(tNode->child[i],val);
}

node *Root=NULL;
node* getNewNode(bool isLeaf,bool isRoot){
    node* tmp = new node;
    tmp->isRoot = isRoot;
    tmp->leaf = isLeaf;
    tmp->last = NULL;
    return tmp;
}




void insertInParentNode(node *n, int kprime, node *nprime){
    if(n->isRoot){
        Root = getNewNode(false,true);
        n->isRoot=false;

        Root->child.push_back(n);
        Root->child.push_back(nprime);
        Root->value.push_back(kprime);
        n->par = Root;
        nprime->par = Root;
    }else{
        node *p = n->par;
        int i;
        for(i=0;i<p->value.size();i++){
            if(p->value[i]>kprime) break;
        }
        int tmpK;
        node *tmpP;

        for(int j = i; j<p->value.size(); j++){
            tmpK = p->value[j];
            tmpP = p->child[j+1];

            p->value[j] = kprime;
            p->child[j+1] = nprime;

            kprime = tmpK;
            nprime = tmpP;
        }
        p->value.push_back(kprime);
        p->child.push_back(nprime);
        nprime->par = p;


        //printf("\n\np->child size: %d\n\n",p->child.size());

        if(p->child.size()>nPointer){
            node *pprime = getNewNode(false,false);
            int nbytwoceil = (nPointer+1)/2;
            int kdoubleprime = p->value[nbytwoceil];
            for(i = nbytwoceil+1; i<p->value.size();i++){
                pprime->child.push_back(p->child[i]);
                p->child[i]->par = pprime;
                pprime->value.push_back(p->value[i]);
            }
            pprime->child.push_back(p->child[i]);
            p->child[i]->par = pprime;


            p->value.erase(p->value.begin()+nbytwoceil,p->value.end());
            p->child.erase(p->child.begin()+nbytwoceil+1,p->child.end());

            insertInParentNode(p,kdoubleprime,pprime);
        }
    }

}


void insertInLeafNode(node *leafNode, int k, node *p){
    int i;
    for(i=0;i<leafNode->value.size();i++){
        if(k<leafNode->value[i]) break;
    }
    int tmpK;
    node *tmpP;

    for(int j = i; j<leafNode->value.size(); j++){
        tmpP = leafNode->child[j];
        tmpK = leafNode->value[j];

        leafNode->child[j] = p;
        leafNode->value[j] = k;

        p = tmpP;
        k = tmpK;
    }
    leafNode->child.push_back(p);
    leafNode->value.push_back(k);

}


void insert2(int k, node *p){
    node *leafNode;
    if(Root==NULL){
        Root = getNewNode(true,true);
        leafNode = Root;
    }else leafNode = getTargetNode(Root,k);

    int keyValueCount = leafNode->value.size();
    if(keyValueCount<nVal) insertInLeafNode(leafNode,k,p);
    else{
        node* leafNode2 = getNewNode(true,false);
        insertInLeafNode(leafNode,k,p);
        leafNode2->last = leafNode->last;
        leafNode->last=leafNode2;
        int nbytwoceil = (nPointer+1)/2;

        for(int i = nbytwoceil; i<nPointer ; i++){
            leafNode2->child.push_back(leafNode->child[i]);
            leafNode2->value.push_back(leafNode->value[i]);
        }
        leafNode->value.erase(leafNode->value.begin()+nbytwoceil,leafNode->value.end());
        leafNode->child.erase(leafNode->child.begin()+nbytwoceil,leafNode->child.end());

        int kprime = leafNode2->value[0];
        insertInParentNode(leafNode,kprime,leafNode2);
    }
}

void valueOfNodeInBox(node* tNode){
    printf(" [");
    int i ;
     for(i=0; i<tNode->value.size()-1;i++){
        printf("%d|",tNode->value[i]);
    }
    if(tNode->value.size()>0) printf("%d]",tNode->value[i]);
    //printf(" ");
}

struct pNode{
    node *tNode;
    bool nl;
    pNode(node *Node, bool b){
        tNode = Node;
        nl = b;
    }
    pNode(){

    }
};

queue<pNode>q;
void bfsTraverse(node *tNode){

    q.push(pNode(tNode,true));
    while(!q.empty()){
        pNode p = q.front();
        //printf("  Got pNode ");
        node *temp = p.tNode;
        q.pop();
        valueOfNodeInBox(temp);
        //printf(" printed temp ");
        if(p.nl) printf("\n");
        int i;
        if(!temp->leaf){
        for(i=0;i<temp->child.size()-1;i++){
            q.push(pNode(temp->child[i],false));
        }
        //printf(" inserted second last child  ");
        if(p.nl) q.push(pNode(temp->child[i],true));
        else q.push(pNode(temp->child[i],false));
        //printf(" inserted  last child  ");
        }

    }
}

int main(){
    printf("How many values in each node?\n");
    FILE *fp;
    fp = fopen("input.txt","r");
    fscanf(fp,"%d",&nPointer);
    nVal = nPointer-1;


    printf("nPointer = %d\n",nPointer);
    int k;
    while(fscanf(fp,"%d",&k)!=EOF){
        printf("val: %d\n",k);
        insert2(k,NULL);
    }
    while(true){
        printf("Action: \npress 1 to insert\npress 2 to print in tree structure\npress 0 for exit\n");
        int choice;
        scanf("%d",&choice);
        if(choice==1){
            int value;
            scanf("%d",&value);
            insert2(value,NULL);
        }else if(choice==2){
            printf("\n\n\n");
            bfsTraverse(Root);
            printf("\n\n\n");
        }else if(choice==0) break;

    }
    fclose(fp);
    return 0;
}
