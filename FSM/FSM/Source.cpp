#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
using namespace std;
//��������� ���������
struct Transition
{
	unsigned int s1; //������� ���������
	unsigned int trin; //������� ������
	unsigned int trout; //�������� ������
	unsigned int s2; //����� ���������
};

//������
class Node
{
public:
	vector <unsigned int> node; //���������?
	vector <Node*>* successor; //��������
	Node() { successor = new vector<Node*>(); }
	friend class FSM;	
	void printNode(Node* node, int level);

};

class FSM
{
private:
	unsigned int s; //���-�� ���������
	unsigned int i; //���-�� ������
	unsigned int o; //���-�� �������
	unsigned int tr; //���-�� ���������(���)
	Transition* Tr1;
	Node* root;
public:	
	friend class Node;
	FSM()
	{
		s = 0;
		i = 0;
		o = 0;
		tr = 0;
		Tr1 = NULL;
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
	Transition  getTr1(unsigned int y)
	{
		return Tr1[y];
	}

	Node* createTree(vector<vector<unsigned int>>&);
	Node* createTreeRek(vector<unsigned int>&, vector<vector<unsigned int>>&, vector<unsigned int>& );
	void printnd();
	void setRoot(Node* rt)
	{
		root = rt;
	}
	void print();
	void derive_homing_sequences();
	void print_states();
	void printHT(Node*root,unsigned int state);
};
void FSM::printHT(Node*root, unsigned int state) {
	for (unsigned int i = 0; i < root->node.size(); i++) {
		if (root->node[i] == state) {
			cout << root->node[i] << " = ";
			for (unsigned int j = 0; j < root->successor->size(); j++) {
				unsigned int next_state = root->successor->at(j)->node[i];
				for (unsigned int k = 0; k < root->successor->at(j)->node.size(); k++) {
					if (root->successor->at(j)->node[k] == next_state) {
						cout << k << "/" << getTr1(j).trout << ", ";
						break;
					}
				}
			}
			cout << endl;
			return;
		}
	}
}
void FSM::print_states() {
	for (unsigned int i = 0; i < root->node.size(); i++) {
		printHT(root,root->node[i]);
	}
}
Node* FSM::createTreeRek(vector<unsigned int>& states, vector<vector<unsigned int>>&ht, vector<unsigned int>& w_vec)
{
	// ������� ����� ���� � ��������� ��� �����������
	Node* node = new Node;
	node->node = states;

	// ��� ������ ���� (������� ������, �������� ������) �������� ��������
	for (unsigned int j = 0; j < i; j++) {
		for (unsigned int k = 0; k < o; k++) {
			// ������� ����� ��������� ���������, � ������� ����� ������� �� ������� ���� (������� ������, �������� ������)
			vector<unsigned int> new_states;
			for (unsigned int m = 0; m < tr; m++) {
				for (unsigned int y = 0; y < states.size(); y++) {
					if (Tr1[m].s1 == states[y] && Tr1[m].trin == j && Tr1[m].trout == k) {
						bool found = false;
						//���� ���������� � ����� ��������� ���������
						for (unsigned int n = 0; n < new_states.size(); n++) {
							if (new_states[n] == Tr1[m].s2) {
								found = true;
								break;
							}
						}
						if (!found) {
							new_states.push_back(Tr1[m].s2);
						}
					}
				}
			}
			// ���� ����� ��������� ��������� �� ������, ������� ��� ���� ����� ���� � ��������� ��� � �������� ������� �������� ����
			if (new_states.size() > 1&&new_states!=states) {
				//���������� ��������
				w_vec.push_back(j); //input
				w_vec.push_back(k);	//output
				Node* new_node = createTreeRek(new_states, ht,w_vec);
				if (node->successor == NULL) {
					node->successor = new vector<Node*>;
				}
				node->successor->push_back(new_node);

			}
			else
			{
				//���������� ������-��
				if (new_states.size()==1)
				{
					//��������
					w_vec.push_back(j); //input
					w_vec.push_back(k);
					//w_vec.push_back(new_states[0]);
					ht.push_back(w_vec);
					//�������
					w_vec.pop_back();
					w_vec.pop_back();
				}
				Node* new_node=new Node; new_node->node = new_states;
				if (node->successor == NULL) {
					node->successor = new vector<Node*>;
				}
				node->successor->push_back(new_node);

			}

		}
	}
	//�������� ��������
	if (w_vec.size())
	{
		w_vec.pop_back();
		w_vec.pop_back();
	}

	return node;
}

Node* FSM::createTree(vector<vector<unsigned int>> &ht)
{
	vector<unsigned int> states;
	vector<unsigned int> w_vec;
	for (unsigned int j = 0; j < s; j++) {
		states.push_back(j);
	}
	return createTreeRek(states, ht,w_vec);
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
	Tr1 = new Transition[gettr()];
	while (!fin.eof())
	{
		for (unsigned int i = 0;i < tr;i++)
		{
			fin >> Tr1[i].s1;
			fin >> Tr1[i].trin;
			fin >> Tr1[i].s2;
			fin >> Tr1[i].trout;
			
		}
	}

	fin.close();
	
}

void FSM::print()
{
	cout << s << " Number of states"<<"\n";
	cout << i << " Number of input" << "\n";
	cout << o << " Number of output" << "\n";
	cout << tr << " Number of transition" << "\n";
	for (unsigned int i = 0;i < tr;i++)
	{
		cout << Tr1[i].s1 << " ";
		cout << Tr1[i].trin << " ";
		cout << Tr1[i].trout << " ";
		cout << Tr1[i].s2 << "\n";
	}
}

void printNode(Node* node, int level)
{
	// ������� ������� ��� �������� ������ �����������
	for (int i = 0; i < level; i++) {
		cout << "  ";
	}
	// ������� ��������� �������� ����
	cout << "State(s): ";
	for (unsigned int i = 0; i < node->node.size(); i++) {
		cout << node->node[i] << " ";
	}
	cout << endl;
	// ���������� ������� �������� �������� ����
	if (node->successor != NULL) {
		for (unsigned int i = 0; i < node->successor->size(); i++) {
			printNode((*node->successor)[i], level + 1);
		}
	}
}

void FSM::printnd()
{
	if (root == NULL) {
		cout << "Tree is empty" << endl;
		return;
	}
	cout << "Homing tree(s):" << endl;
	// ���������� ������� ������(�)
	printNode(root, 0);
}


int main()
{
	string FSMpr = "Properties.txt";
	FSM obj(FSMpr);
	//obj.print();
	vector<vector<unsigned int>> ht;

	Node* root = obj.createTree(ht);
	
	obj.setRoot(root);
	obj.printnd();

	cout << '\n';

	for (int i = 0;i < ht.size();i++)
	{
		for (int j = 0;j < ht[i].size();j++)
		{
			cout << ht[i][j]<<"/";
		}
		cout << '\n';
	}
	//obj.print_states();
}