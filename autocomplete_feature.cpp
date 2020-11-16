//Creating an autocomplete feature out of curiosity
#include<iostream>
#include<string>
#include<queue>
using namespace std;

//Structure of a trie node
struct trienode{
	bool is_word;
	trienode* child[26];
};

//Creating the new trienode
trienode* create_new_node(){
	//Creating a new trienode
	trienode*temp = new trienode;
	temp->is_word = false;
	//Setting all the child pointers to null
	for(int i=0;i<26;i++){
		temp->child[i] = NULL;
	}

	return temp;
}

//This will help in inserting the words into the trie
void insert_into_trie(trienode* root,string key){
	trienode* temp = root;
	for(int j=0;j<key.length();j++){
		int idx = key[j]-'a';
		if(temp->child[idx] == NULL){
			temp->child[idx] = create_new_node();
		}
		temp = temp->child[idx];
	}

	temp->is_word = true;
}

bool searching_key(trienode* root,string key){

	if(root == NULL){
		return false;
	}

	trienode*temp = root;
	for(int j=0;j<key.length();j++){
		int idx = key[j]-'a';
		if(temp->child[idx] == NULL){
			return false;
		}
		temp = temp->child[idx];
	}

	return temp->is_word;
}

trienode* delete_helper(trienode*root,string key,int idx){
	//If trie is empty
	if(root == NULL){
		return root;
	}

	bool is_empty = true;
	for(int i=0;i<26;i++){
		if(root->child[i] != NULL){
			is_empty = false;
			break;
		}
	}

	if(idx == key.length()){

        if(is_empty==false){
            //When the key is prefix of other key i
            //In this case set is_word of current root equals to false
            root->is_word = false;
        }else{
            //When the key is not a prefix of any other key 
            //In this case delete this node
            delete(root);
            root = NULL;
        }
        
        return root;
    }

    int pos = key[idx]-'a';
    root->child[pos] = delete_helper(root->child[pos],key,idx+1);
    
    //Now what we gonna do is check is there is there any child of root which is not null
    //If there is any then we will do nothing and return root(cond 1)
    //Else if all the child of root are null then delete the root and return(cond 2)
    is_empty = true;
    for(int j=0;j<26;j++){
        if(root->child[j] != NULL){
            is_empty = false;
            break;
        }
    }
    
    if(is_empty && root->is_word == false ){
        delete(root);
        root = NULL;
    }
    
    return root;
}

trienode* deleting_key(trienode*root,string key){
	//If trie is empty return root
	if(root == NULL){
		return root;
	}
	//If the key to be deleted is an empty string simply return root
	if(key == ""){
		return root;
	}
	//Deleting key using helper function(it will return the modified trie)
	root = delete_helper(root,key,0);

	return root;
}

void query_responses(trienode*root,string key,vector<string>& autocomplete_responses){
	if(root == NULL){
		return;
	}

	trienode*temp = root;

	for(int j=0;j<key.length();j++){
		int idx = key[j]-'a';

		if(temp->child[idx] == NULL){
			return;
		}

		temp = temp->child[idx];
	}

	//Now as the key(prefix) exist next we have traverse down the trie to search for every node which has is_word true
	//Performing BFS
	queue<pair<trienode*,string>> q;
	q.push(make_pair(temp,key));
	string potential_autocomplete_candidate;

	while(q.empty() == false){

		temp = q.front().first;
		potential_autocomplete_candidate = q.front().second;
		//If there exist a word at current index(means a word end at current index)
		//So push the word into the vector which will be outputted later
		if(temp->is_word){
			autocomplete_responses.push_back(potential_autocomplete_candidate);
		}
		q.pop();
		//If a child[i] is not null push it into the queue(as it can contain potential candidate for the autocomplete response)
		for(int j=0;j<26;j++){
			if(temp->child[j] != NULL){
				q.push(make_pair(temp->child[j],potential_autocomplete_candidate+char(j+'a')));
			}
		}
	}
}

//This function will generate the autocompleted responses for the queries
void generated_autocompleted_response(trienode*root,string key){
	//If our key is empty then we dont have to generate the response and simply return 
	if(key.length() == 0){
		return;
	}

	//Else if we have a valid key then we will have to generate the responses(autocomplete responses for the queries)
	vector<string> autocomplete_responses;
	query_responses(root,key,autocomplete_responses);

	if(autocomplete_responses.size() == 0){
		cout<<"Sorry we don't have any responses may be first you populate the dictionary with some of you search"<<endl;
		return;
	}

	//If we have responses means our generated_responses vector is no empty
	cout<<"There are almost "<<autocomplete_responses.size()<<" suggestions for your prefix based upon your populating of dictionary"<<endl;
	for(int j=0;j<autocomplete_responses.size();j++){
		cout<<autocomplete_responses[j]<<" ";
	}
	cout<<endl;
	return;
}



int main(){
	int n;//Size of the dictionary from where we will search
	cout<<"Enter the size of the dictionary."<<endl;
	cin>>n;

    //The dictonary will contain the words entered by you which will be used for providing autocompleted(auto suggestion) responses for the queries 
	vector<string> word_dictionary;
	string key;
	//Populating the dictionary(It a manual population but you can also integrate it with your service)
	for(int i=0;i<n;i++){
		cin>>key;
		word_dictionary.push_back(key);
	}

    //Creating the root of the trie
	trienode*root = create_new_node();

	//Inserting the words from the dictionary to the trie
	for(int i=0;i<word_dictionary.size();i++){
		insert_into_trie(root,word_dictionary[i]);
	}

	//Now as we have our autocomplete engine ready we can test it with giving some input query and generating autocompleted response
	cout<<"Enter the key for which you want to have an autocompleted response"<<endl;
	cin>>key;
	//Function generateda_autocomplete_response will provide us the autocompleted responses to the queries
	generated_autocompleted_response(root,key);
	return 0;
}
