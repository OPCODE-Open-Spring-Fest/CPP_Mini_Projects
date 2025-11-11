#include<bits/stdc++.h>
using namespace std;

struct Node {
    char ch;
    int freq;
    Node* left;
    Node* right;

    Node(char c, int f, Node* l = nullptr, Node* r = nullptr)
        : ch(c), freq(f), left(l), right(r) {}
};
struct Compare {
    bool operator()(Node* a, Node* b) {
        return a->freq > b->freq;
    }
};
unordered_map<char,string> encodings;

void prefix(Node* root,string s){
    if(root->ch!='*'){
        encodings[root->ch]=s;
        return;
    }
    if(root->left !=NULL)prefix(root->left,s+'0');
    if(root->right !=NULL)prefix(root->right, s+'1');
}

int main(){
    ifstream file("input.txt");
    if(!file.is_open()){
        cout<<"file could not be opened"<<endl;
        return 1;
    }
    unordered_map<char,int> hashmap;
    char ch;
    while(file.get(ch)){
        hashmap[ch]++;
    }
    file.close();
    priority_queue<Node*, vector<Node*>, Compare> pq;
    string orignal="";


    for(auto it=hashmap.begin();it!=hashmap.end();it++){
        char c=it->first;
        orignal+=c;
        int f=it->second;
        cout<<c<<" : "<<f<<endl;

        Node* a = new Node(c, f, NULL, NULL);
        pq.push(a);
    }
    
    while (pq.size() > 1) {
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();

        Node* parent = new Node('*', left->freq + right->freq, left, right);
        pq.push(parent);
    }
    Node* root=pq.top();
    prefix(root,"");

    for(auto it=encodings.begin();it!=encodings.end();it++){
        cout<< it->first<< " : "<<it->second<<endl;
    }

    string bitstream="";
    for(char c:orignal){
        bitstream+=encodings[c];
    }
    vector<unsigned char> bytes;
    unsigned char current=0;
    int count=0;

    for(char bit: bitstream){
        current=current>>1|bit-'0';
        count++;

        if(count==8){
            bytes.push_back(current);
            current=0;
            count=0;
        }
    }
    if(count > 0) {
        current <<= (8 - count);
        bytes.push_back(current);
    }

    ofstream out("output.bin",ios::binary);
    for(unsigned char b:bytes){
        out.write((char*)&b,1);

    }
    out.close();


}