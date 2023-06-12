//The program automatically builds a tree and installation routes
#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
using namespace std;
//The structure of transitions
struct Transition
{
	unsigned int s1; //Past state
	unsigned int trin; //Input character
	unsigned int trout; //Output character
	unsigned int s2; //New state
};

//The tree
class Node
{
public:
	vector <unsigned int> node; //States
	vector <Node*>* successor; //Successor
	Node() { successor = new vector<Node*>(); }
	friend class FSM;	
};

class FSM
{
private:
	unsigned int s; //Number of states
	unsigned int i; //Number of input character
	unsigned int o; //Number of output character
	unsigned int tr; //Number of transitions
	Transition* Transit;
	Node* root;
public:	
	friend class Node;
	FSM()
	{
		s = 0;
		i = 0;
		o = 0;
		tr = 0;
		Transit = NULL;
		root = NULL;
	}
	FSM(string FSMpr);
	unsigned int gettr()
	{
		return tr;
	}
	unsigned int geti()
	{
		return i;
	}
	unsigned int geto()
	{
		return o;
	}
	unsigned int gets()
	{
		return s;
	}
	Transition  getTr1(unsigned int y)
	{
		return Transit[y];
	}

	Node* createTree(vector<vector<unsigned int>>&);
	Node* createTreeRecursion(vector<unsigned int>&, vector<vector<unsigned int>>&,
		vector<unsigned int>&, int);
	void printTree();
	void printNode(Node* node, int level);
	void setRoot(Node* rt) {root = rt;}
	void printProperties();
	void printHomingTraces(string, vector<vector<unsigned int>> );
};

Node* FSM::createTreeRecursion(vector<unsigned int>& states,
	vector<vector<unsigned int>>& ht, vector<unsigned int>& w_vec, int level)
{
	// create a new node and fill it with states
	Node* obj_node = new Node;
	obj_node->node = states;
	level++;
	// for each pair (input character, output character) of the input alphabet
	if (level == 100)
		return obj_node;
	
		for (unsigned int f_in = 0; f_in < i; f_in++) {
			for (unsigned int f_out = 0; f_out < o; f_out++) {
				// creating a new set of states that can be switched to by the current pair (input symbol, output symbol)
				vector<unsigned int> new_states;
				for (unsigned int m = 0; m < tr; m++) {
					for (unsigned int y = 0; y < states.size(); y++)
					{
						if (Transit[m].s1 == states[y] && Transit[m].trin == f_in && Transit[m].trout == f_out) {
							bool found = false;
							//looking for repetitions in a new set of states
							for (unsigned int n = 0; n < new_states.size(); n++) {
								if (new_states[n] == Transit[m].s2) {
									found = true;
									break;
								}
							}
							if (!found) {
								new_states.push_back(Transit[m].s2);
							}
						}
					}
				}
				// if the new set of states is not empty, create a new node for it and add it as a descendant of the current node
				if (new_states.size() > 1 && new_states != states) {
					// Creating a copy of w_vec to avoid changing the original at the following levels
					vector<unsigned int> new_w_vec = w_vec; 
					new_w_vec.push_back(f_in); //input
					new_w_vec.push_back(f_out);	//output
					//adding an element
					Node* new_node = createTreeRecursion(new_states, ht, new_w_vec, level);
					if (obj_node->successor == NULL) {
						obj_node->successor = new vector<Node*>;
					}
					obj_node->successor->push_back(new_node);

				}
				else
				{
					//saving sequences
					if (new_states.size() == 1)
					{
						vector<unsigned int> new_w_vec = w_vec;
						//we add characters and delete them, because at the last iteration it does not add
						new_w_vec.push_back(f_in); //input
						new_w_vec.push_back(f_out);
						//adding and removing a state for output
						new_w_vec.insert(new_w_vec.begin(),new_states[0]);
						ht.push_back(new_w_vec);
						//delete everything you added
						new_w_vec.erase(new_w_vec.begin());
						new_w_vec.pop_back();
						new_w_vec.pop_back();
					}
					Node* new_node = new Node; new_node->node = new_states;
					if (obj_node->successor == NULL) {
						obj_node->successor = new vector<Node*>;
					}
					obj_node->successor->push_back(new_node);

				}

			}
		}
		//deleting an element
		if (w_vec.size())
		{
			w_vec.pop_back();
			w_vec.pop_back();
		}
		level--;
	return obj_node;
}

Node* FSM::createTree(vector<vector<unsigned int>> &ht)
{
	vector<unsigned int> states;
	//the resulting sequence
	vector<unsigned int> w_vec;
	int level = 0;
	for (unsigned int j = 0; j < s; j++) 
		states.push_back(j);
	return createTreeRecursion(states, ht, w_vec, level);
}

FSM::FSM(string FSMpr)
{
	ifstream fin;
	fin.open(FSMpr);

	if (!fin.is_open())
		cout << "File opening error";
	else
		cout << "File is open" << "\n";
	fin >> s;
	fin >> i;
	fin >> o;
	fin >> tr;
	Transit = new Transition[gettr()];
	while (!fin.eof())
	{
		for (unsigned int i = 0;i < tr;i++)
		{
			fin >> Transit[i].s1;
			fin >> Transit[i].trin;
			fin >> Transit[i].s2;
			fin >> Transit[i].trout;
			
		}
	}

	fin.close();
	
}

void FSM::printProperties()
{
	cout << s << " Number of states"<<"\n";
	cout << i << " Number of input" << "\n";
	cout << o << " Number of output" << "\n";
	cout << tr << " Number of transition" << "\n";
	for (unsigned int i = 0;i < tr;i++)
	{
		cout << Transit[i].s1 << " ";
		cout << Transit[i].trin << " ";
		cout << Transit[i].trout << " ";
		cout << Transit[i].s2 << "\n";
	}
}

void FSM::printNode(Node* node, int level)
{
	// Output margins for the current nesting level
	for (int i = 0; i < level; i++) {
		cout << "  ";
	}
	// Output the status of the current node
	cout << "State(s): ";
	for (unsigned int i = 0; i < node->node.size(); i++) {
		cout << node->node[i] << " ";
	}
	cout << endl;
	// Recursively traversing the descendants of the current node
	if (node->successor != NULL) {
		for (unsigned int i = 0; i < node->successor->size(); i++) {
			printNode((*node->successor)[i], level + 1);
		}
	}
}

void FSM::printTree()
{
	if (root == NULL) {
		cout << "Tree is empty" << endl;
		return;
	}
	cout << "Homing tree:" << endl;
	// Recursively output the tree
	this->printNode(root, 0);
}

void FSM::printHomingTraces(string FSMht, vector<vector<unsigned int>> ht)
{
	ofstream fout;
	fout.open(FSMht);
	for (unsigned int s = 0;s < this->gets();s++)
	{
		for (unsigned int i = 0; i < ht.size(); i++)
		{
			unsigned int j = 0;
			if (ht[i][j] == s)
			{
				fout << ht[i][j] << " - ";
				j++;
				int slash = 0;
				for (;j < ht[i].size();j++)
				{
					slash++;
					fout << ht[i][j];
					if (slash < 2)
						fout << "/";
					else
					{
						slash = 0;fout << " ";
					}

				}
				fout << '\n';
			}
		}
	}
}

int main()
{
	string FSMpr = "Properties.txt";
	FSM obj(FSMpr);
	//output of FSM parameters to the console
	//obj.printProperties();
	//vector of all sequences
	vector<vector<unsigned int>> ht;

	Node* root = obj.createTree(ht);
	
	obj.setRoot(root);
	obj.printTree();

	string FSMht = "Homing traces.txt";
	obj.printHomingTraces(FSMht, ht);
}