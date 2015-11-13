#include <iostream>
#include <math.h>

using namespace std;


#define m 4
#define OVERFLOWW 0
#define OK 1

template <class T>
struct Rec{
    pair<T,T> p1,p2;
    Rec(){
        p1.first = p2.first = 0;
        p1.second = p2.second = 0;
    }
    Rec(T x,T y){
        p1.first = p2.first = x;
        p1.second = p2.second = y;
    }
    Rec(T x1,T y1,T x2,T y2){
        p1.first = x1;
        p1.second = y1;
        p2.first = x2;
        p2.second = y2;
    }

    void print(){
        cout << " (" <<p1.first << "," << p1.second << ") (" << p2.first << "," << p2.second << ") " << endl ;
    }

    bool isPoint(){
        return (p1.first == p2.first && p1.second == p2.second);
    }
};

template <class T>
class Rtree {

    struct node {
        int count;
        Rec<T> data[m + 1];
        node * children[m + 1];
        node () {
            count = 0;
            for (int i = 0; i < m+1; i++)
                children [i] = NULL;
        }

        void insert_into (const Rec<T> &info) {
            this -> data[count] = info;
            this -> count++;
        }
    };
    //'--------------------------------------------------

    node * root;

public:

    Rtree () {
        root = new node();
    }

    void insert(const Rec<T> &info) {
        int ret = insert2 (root, info);
        if (ret == OVERFLOW){
            cout << "splitRoot" << endl;
            splitRoot();
        }
    }

    void print () {
            print2 (root, 0);
    }


private:


    bool isLeaf(node* parent){
        return (parent->children[0] == NULL);
    }

    int getArea(node* parent, Rec<T> rec){
        Rec<T> R;
        int base, altura;
        node *tmp = new node();
        for (int i = 0; i < parent->count; i++)
            tmp -> insert_into(parent->data[i]);
        tmp -> insert_into(rec);
        buildRec(tmp, R);

        base = R.p2.first - R.p1.first;
        altura = R.p2.second - R.p1.second;

        return base * altura;
    }

    int inMBR(node* parent, Rec<T> _rec){
        Rec<T> temp;
        int area, min_area = 999999;
        int pos = 0;

        for (int i=0; i < parent->count; i++){
            temp = parent->data[i];
            if ( (_rec.p1.first >= temp.p1.first) && (_rec.p2.first <= temp.p2.first) && (_rec.p1.second >= temp.p1.second) && (_rec.p2.second <= temp.p2.second) )
                return i;
        }//no esta dentro de ningun Rectangulo

        for (int i=0; i < parent->count; i++){
            area = getArea(parent->children[i], _rec);
            if (area < min_area){
                min_area = area;
                pos = i;
            }
        }
        return pos;
    }

    int insert2 (node* parent, const Rec<T> &info){
        int pos = 0;
        Rec<T> R;

        if ( isLeaf(parent) ) {
            parent->insert_into ( info );
        }
        else {
            pos = inMBR(parent, info);
            int ret = insert2 ( parent->children[pos], info );
            buildRec(parent->children[pos], R);
            parent->data[pos] = R;

            cout << "entro al else" << endl;
            if ( ret  == OVERFLOW ) {
                cout << "split" << endl;
                split (parent, pos);
            }
        }
        return (parent->count > m ) ? OVERFLOW : OK;
    }

    float calculoDistancia(Rec<T> p_ini, Rec<T> p_fin){
        if (p_ini.isPoint() && p_fin.isPoint()){
            return (sqrt( pow((p_fin.p1.first - p_ini.p1.first),2) + pow((p_fin.p1.second - p_ini.p1.second),2) ));
        }
    }

    void buildRec(node *parent, Rec<T> &rec){
        int min_x = 9999, min_y = 9999, max_x = 0, max_y = 0;

        for (int i = 0; i < parent->count; i++){
            if (parent->data[i].p1.first < min_x){
                min_x = parent->data[i].p1.first;
            }
            if (parent->data[i].p1.first > max_x){
                max_x = parent->data[i].p1.first;
            }
            if (parent->data[i].p1.second < min_y){
                min_y = parent->data[i].p1.second;
            }
            if (parent->data[i].p1.second > max_y){
                max_y = parent->data[i].p1.second;
            }
        }

        rec.p1.first = min_x;
        rec.p1.second = min_y;
        rec.p2.first = max_x;
        rec.p2.second = max_y;

    }

    void splitRoot() {

        int i, j, pos1, pos2;
        float dis_mayor = 0.0, dis = 0.0, dis1 = 0.0, dis2 = 0.0;

        node *child = root ;
        node *node1 = new node();
        node *node2 = new node();

        for ( i = 0; i < m + 1; i++ ) {
            for (j = i+1; j < m+1; j++){
                dis = calculoDistancia(child->data[i], child->data[j]);
                if (dis_mayor < dis){
                    dis_mayor = dis;
                    pos1 = i;
                    pos2 = j;
                }
            }
        }

        node1->insert_into(child->data[pos1]);
        node2->insert_into(child->data[pos2]);

        for (i = 0; i < m+1; i++){
            if (pos1 != i && pos2 != i){
                dis1 = calculoDistancia(child->data[pos1], child->data[i]);
                dis2 = calculoDistancia(child->data[pos2], child->data[i]);
                if (dis1 < dis2){
                    node1->insert_into(child->data[i]);
                }else{
                    node2->insert_into(child->data[i]);
                }
            }
        }

        Rec<T> R1, R2;

        buildRec(node1, R1);
        buildRec(node2, R2);

        node *parent = new node();
        parent ->insert_into(R1);
        parent ->insert_into(R2);

        parent->children[0] = node1;
        parent->children[1] = node2;

        root = parent;

        // delete child;
    }

    void split (node *parent ,  int pos) {

        int i, j, pos1, pos2;
        float dis_mayor = 0.0, dis = 0.0, dis1 = 0.0, dis2 = 0.0;

        node *child = parent->children[pos] ;
        node *node1 = new node();
        node *node2 = new node();

        for ( i = 0; i < m + 1; i++ ) {
            for (j = i+1; j < m+1; j++){
                dis = calculoDistancia(child->data[i], child->data[j]);
                if (dis_mayor < dis){
                    dis_mayor = dis;
                    pos1 = i;
                    pos2 = j;
                }
            }
        }

        node1->insert_into(child->data[pos1]);
        node2->insert_into(child->data[pos2]);

        for (i = 0; i < m+1; i++){
            if (pos1 != i && pos2 != i){
                dis1 = calculoDistancia(child->data[pos1], child->data[i]);
                dis2 = calculoDistancia(child->data[pos2], child->data[i]);
                if (dis1 < dis2){
                    node1->insert_into(child->data[i]);
                }else{
                    node2->insert_into(child->data[i]);
                }
            }
        }

        Rec<T> R1, R2;

        buildRec(node1, R1);
        buildRec(node2, R2);

        parent->data[pos] = R1;
        parent -> insert_into(R2);

        //delete parent->children[pos];
        parent -> children[pos] = node1;
        parent -> children[parent->count - 1] = node2;

        //delete child;
    }
    void print2(node* ptr, int level ) {
        if (ptr) {

            int i;
            for (i = ptr->count - 1; i >= 0; i--) {
               print2(ptr->children[i+1], level + 1);

                for (int k = 0; k  < level; k++)
                    cout << "   ";
                ptr->data [i].print();
            }
            print2( ptr->children[i+1], level + 1);

        }
    }

};

int main()
{
    Rtree<int> rt;
    Rec<int> a(1,3);
    Rec<int> b(2,5);
    Rec<int> c(5,9);
    Rec<int> d(8,2);
    Rec<int> e(11,1);
    Rec<int> f(4,6);
    Rec<int> g(3,4);
    Rec<int> h(6,6);
    Rec<int> i(9,3);
    Rec<int> j(6,1);
    Rec<int> k(10,2);
    Rec<int> l(2,9);
    Rec<int> n(3,8);

    rt.insert(a);
    rt.insert(b);
    rt.insert(c);
    rt.insert(d);
    rt.insert(e);
    rt.insert(f);
    rt.insert(g);
    rt.insert(h);
    rt.insert(i);
    rt.insert(j);
    rt.insert(k);
    rt.insert(l);
    //rt.insert(n);
    rt.print();

    return 0;
}
