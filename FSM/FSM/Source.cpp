#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
using namespace std;
//структура переходов
struct Transition
{
	unsigned int s1; //прошлое состо€ние
	unsigned int trin; //входной символ
	unsigned int trout; //выходной символ
	unsigned int s2; //новое состо€ние
};

//дерево
class Node
{
public:
	vector <unsigned int> node; //состо€ни€?
	vector <Node*>* successor; //приемник
	Node() { successor = new vector<Node*>(); }
	friend class FSM;	
	void printNode(Node* node, int level);

};

class FSM
{
private:
	unsigned int s; //кол-во состо€ний
	unsigned int i; //кол-во входов
	unsigned int o; //кол-во выходов
	unsigned int tr; //кол-во переходов(дуг)
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
		return Tr1[y];
	}

	Node* createTree(vector<vector<unsigned int>>&);
	Node* createTreeRecursion(vector<unsigned int>&, vector<vector<unsigned int>>&, vector<unsigned int>&, int level );
	void printTree();
	void setRoot(Node* rt)
	{
		root = rt;
	}
	void printProperties();
};

Node* FSM::createTreeRecursion(vector<unsigned int>& states,
	vector<vector<unsigned int>>& ht, vector<unsigned int>& w_vec, int level)
{
	// создаем новый узел и заполн€ем его состо€ни€ми
	Node* node = new Node;
	node->node = states;
	level++;

	// дл€ каждой пары (входной символ, выходной символ) входного алфавита
	if (level == 10)
		return node;
		for (unsigned int j = 0; j < i; j++) {
			for (unsigned int k = 0; k < o; k++) {
				// создаем новое множество состо€ний, в которое можно перейти по текущей паре (входной символ, выходной символ)
				vector<unsigned int> new_states;
				for (unsigned int m = 0; m < tr; m++) {
					for (unsigned int y = 0; y < states.size(); y++) {
						if (Tr1[m].s1 == states[y] && Tr1[m].trin == j && Tr1[m].trout == k) {
							bool found = false;
							//ищет повторени€ в новом множестве состо€ний
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
				// если новое множество состо€ний не пустое, создаем дл€ него новый узел и добавл€ем его в качестве потомка текущего узла
				if (new_states.size() > 1 && new_states != states) {
					w_vec.push_back(j); //input
					w_vec.push_back(k);	//output
					//добавление элемента
					Node* new_node = createTreeRecursion(new_states, ht, w_vec, level);
					if (node->successor == NULL) {
						node->successor = new vector<Node*>;
					}
					node->successor->push_back(new_node);

				}
				else
				{
					//сохранение послед-ти
					if (new_states.size() == 1)
					{
						//добавл€ем символы и удал€ем, т.к. на последней итерации не добавл€ет
						w_vec.push_back(j); //input
						w_vec.push_back(k);
						//добавление и удаление состо€ни€ дл€ вывода
						w_vec.insert(w_vec.begin(),new_states[0]);
						ht.push_back(w_vec);
						//удалить все, что добавили
						w_vec.erase(w_vec.begin());
						w_vec.pop_back();
						w_vec.pop_back();
					}
					Node* new_node = new Node; new_node->node = new_states;
					if (node->successor == NULL) {
						node->successor = new vector<Node*>;
					}
					node->successor->push_back(new_node);

				}

			}
		}
		//удаление элемента
		if (w_vec.size())
		{
			w_vec.pop_back();
			w_vec.pop_back();
		}
		level--;
	return node;
}

Node* FSM::createTree(vector<vector<unsigned int>> &ht)
{
	vector<unsigned int> states;
	//получивща€с€ последовательность
	vector<unsigned int> w_vec;
	int level = 0;
	for (unsigned int j = 0; j < s; j++) {
		states.push_back(j);
	}
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

void FSM::printProperties()
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
	// ¬ыводим отступы дл€ текущего уровн€ вложенности
	for (int i = 0; i < level; i++) {
		cout << "  ";
	}
	// ¬ыводим состо€ни€ текущего узла
	cout << "State(s): ";
	for (unsigned int i = 0; i < node->node.size(); i++) {
		cout << node->node[i] << " ";
	}
	cout << endl;
	// –екурсивно обходим потомков текущего узла
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
	cout << "Homing tree(s):" << endl;
	// –екурсивно выводим дерево(а)
	printNode(root, 0);
}


int main()
{
	string FSMpr = "Properties.txt";
	FSM obj(FSMpr);
	//вывод параметров FSM
	//obj.printProperties();
	//вектор всех последовательностей
	vector<vector<unsigned int>> ht;

	Node* root = obj.createTree(ht);
	
	obj.setRoot(root);
	obj.printTree();

	cout << '\n';
	for (unsigned int s = 0;s < obj.gets();s++)
	{
		for (unsigned int i = 0; i < ht.size(); i++)
		{
			unsigned int j = 0;
			if (ht[i][j] == s)
			{
				cout << ht[i][j] << " - ";
				j++;
				int slash = 0;
				for (;j < ht[i].size();j++)
				{
					slash++;
					cout << ht[i][j];
					if (slash < 2)
						cout << "/";
					else
					{
						slash = 0;cout << " ";
					}

				}
				cout << '\n';
			}
		}
	}
}