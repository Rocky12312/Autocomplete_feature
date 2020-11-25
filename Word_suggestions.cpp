#include<iostream>
#include<string>
#include<vector>
using namespace std;
#define ndchilds 26

struct trie_node{
	bool word_end;
	trie_node* child[ndchilds];
};

trie_node* creating_newnode(){
	trie_node*temp = new trie_node;
	temp->word_end = false;
	for(int i=0;i<ndchilds;i++){
		temp->child[i] = NULL;
	}
	return temp;
}

void populating_trie(trie_node*root,string s){
	trie_node*temp = root;
	for(int rlevel = 0;rlevel<s.length();rlevel++){
		int index = s[rlevel]-'a';
		if(!temp->child[index]){
			temp->child[index] = creating_newnode();
		}
		temp = temp->child[index];
	}
	temp->word_end = true;
}

bool search_string(trie_node*root,string s){
	trie_node*temp = root;
	for(int rlevel = 0;rlevel<s.length();rlevel++){
		int index = s[rlevel]-'a';
		if(!temp->child[index]){
			return false;
		}
		temp = temp->child[index];
	}
	if(temp != NULL && temp->word_end){
		return true;
	}
	return false;
}

bool is_leafnode(trie_node*root){
	for(int lf=0;lf<ndchilds;lf++){
		if(root->child[lf]){
			return 0;
		}
	}
	return 1;
}

void suggestions(trie_node*temp,string current_prefix){
	if(temp->word_end){
		cout<<current_prefix<<endl;
	}
	if(is_leafnode(temp)){
		return;
	}
	for(int level_temp = 0;level_temp<ndchilds;level_temp++){
		if(temp->child[level_temp]){
			current_prefix.push_back(level_temp+'a');
			suggestions(temp->child[level_temp],current_prefix);
		}
	}
}

int generating_suggestions(trie_node*root,string s){
	trie_node*temp = root;
	for(int level = 0;level<s.length();level++){
		int index = s[level]-'a';
		if(!temp->child[index]){
			return 0;
		}
		temp = temp->child[index];
	}
	bool is_not_prefix = (temp->word_end==true);//Will tell whether its a word in trie or just prefix of some word
	bool leafnode = is_leafnode(temp);//Tell whether it's a leaf node or not
	if(is_not_prefix && leafnode){
		cout<<s<<endl;
		return -1;
	}
	if(!leafnode){
		string current_prefix = s;
		suggestions(temp,current_prefix);
		return 1;
	}
	return 1;
}

int main(){
	int n;
	cout<<"Enter the number of words in dictionary manually"<<endl;
	cin>>n;
	vector<string> word_dictionary;
	string word;
	for(int i=0;i<n;i++){
		cin>>word;
		word_dictionary.push_back(word);
	}
	trie_node*root = creating_newnode();
	for(int i=0;i<word_dictionary.size();i++){
		populating_trie(root,word_dictionary[i]);
	}
	string s;
	cout<<"Enter the string for which you want to check the autosuggestion generated"<<endl;
	cin>>s;
	int fn = generating_suggestions(root,s);
	if(fn == -1 || fn == 0){//If our string is leafnode or if there is no such string in trie
		cout<<"Sorry no strings found with this particular prefix"<<endl;
	}
	return 0;
	
}
