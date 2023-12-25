#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <wx/scrolwin.h>
#include <wx/grid.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

#include "icons/open_icon.xpm"
#include "icons/remove_icon.xpm"
#include "icons/debug_icon.xpm"
#include "icons/save_icon.xpm"
#include "icons/save_as_icon.xpm"
#include "icons/clear_icon.xpm"
#include "icons/delete_icon.xpm"
#include "icons/add_icon.xpm"
#include "icons/find_icon.xpm"
#include "icons/clear_search_icon.xpm"

#define OPEN_BUTTON 2
#define REMOVE_BUTTON 3
#define SAVE_BUTTON 4
#define SAVE_AS_BUTTON 5
#define DEBUG_BUTTON 6
#define RADIOBOX 7
#define ADD_BUTTON 8
#define DELETE_BUTTON 9
#define CLEAR_BUTTON 10
#define UPDATE_BUTTON 11
#define SEARCH_BUTTON 12
#define CLEAR_SEACH_BUTTON 13
#define RADIOBOX 30

struct worker {
	string FIO;
	string Age;
	string Experience;
	string Profession;
	string Salary;

	worker(string fio = "", string age = "", string experience = "", string profession = "", string salary = "") {
		FIO = fio;
		Age = age;
		Experience = experience;
		Profession = profession;
		Salary = salary;
	}

	void print() {
		cout << this->FIO << endl;
		cout << this->Age << endl;
		cout << this->Experience << endl;
		cout << this->Profession << endl;
		cout << this->Salary << endl;
		cout << "" << endl;
	}

	bool check() {
		int count = 0;
		bool fio1 = true,
			fio2 = true,
			fio3 = true,
			age = true,
			exper = true,
			profes = true,
			salary = true,
			space = true;

		for (int i = 0; i < FIO.size(); i++) {
			if (count <= 2) {
				if (space) {
					if (!((FIO[i] >= 'А') && (FIO[i] <= 'Я')))
						fio1 = false;
					space = false;
				}
				else {
					if (((FIO[i] >= 'а') && (FIO[i] <= 'я')) || ((FIO[i] == ' ') || (FIO[i] == '-'))) {
						if (FIO[i] == ' ') {
							count++;
							space = true;
							if (count == 2) fio2 = true;
							else fio2 = false;
						}
					}
					else fio3 = false;
				}
			}
			else fio3 = false;
		}
		count = 0;

		if (!((Profession[0] >= 'А') && (Profession[0] <= 'Я')))
			profes = false;
		else {
			for (int i = 1; i < Profession.size(); i++) {
				if (((FIO[i] >= 'а') && (FIO[i] <= 'я')) || (FIO[i] == ' ')) {
					if (FIO[i] == ' ') {
						count++;
						if (count == 2) profes = false;
					}
				}
			}
		}

		if (Age.size() < 3) {
			for (int i = 0; i < Age.size(); i++) {
				if (!((Age[i] >= '0') && (Age[i] <= '9'))) {
					age = false;
				}
			}
		}

		if (Experience.size() < 3) {
			for (int i = 0; i < Age.size(); i++) {
				if (!((Age[i] >= '0') && (Age[i] <= '9'))) {
					age = false;
				}
			}
		}

		for (int i = 0; i < Salary.size(); i++) {
			if (!((Salary[i] >= '0') && (Salary[i] <= '9'))) {
				salary = false;
			}

			if ((fio1) && (fio3) && (fio2) && (age) && (exper) && (profes) && (salary))
				return true;
			else
				return false;
		}
	};

};

struct SPR {
	vector<worker> WorkerBuffer;
};

struct list :public SPR
{
	struct node {
		int index;
		node* pNext;
	};
	int size = 0;
	node* head;
	list() : head(nullptr) {}

	void add(node*& head, int index) {
		if (head == nullptr) {
			head = new node;
			head->index = index;
			head->pNext = nullptr;
		}
		else {
			node* current = head;
			while (current->pNext != nullptr) {
				current = current->pNext;
			}
			node* New = new node;
			New->index = index;
			New->pNext = nullptr;
			current->pNext = New;
		}
		size++;
	}

	string listprint() {
		string res = "";
		for (struct node* current = head; current; current = current->pNext)
			res += to_string(current->index) + " ";
		return res;
	}

	void print(vector<worker> WorkerBuffer) {
		for (struct node* current = head; current; current = current->pNext)
			WorkerBuffer[current->index].print();
	}

	void Delete(node*& head, int index) {
		if (head) {
			if (head->index == index) {
				node* deleting = head;
				head = head->pNext;
				delete deleting;
			}
			else {
				node* current = head;
				while ((current->pNext) && (current->pNext->index != index))current = current->pNext;
				if ((current->pNext) && (current->pNext->index == index)) {
					node* deleting = current->pNext;
					current->pNext = current->pNext->pNext;
					delete deleting;
				}
			}
			size--;
		}
	}


	void pop_front() {
		node* toDelite = head;
		head = head->pNext;
		delete toDelite;
	}
};

struct AvlTree : public SPR {
	struct Node {
		string data;
		list spisok;
		Node* leftChild;
		Node* rightChild;
		int balanceFactor;

		Node(string _data) {
			data = _data;
			spisok.head = nullptr;
			leftChild = nullptr;
			rightChild = nullptr;
			balanceFactor = 0;
		}
	};

	bool  heightChanged;
	Node* root;
	AvlTree() : root(nullptr) {}

	void addNode(Node*& pointer, string val, int index) {
		Node* pointer1, * pointer2;

		if (pointer == nullptr) {
			heightChanged = true;
			pointer = new Node(val);
			pointer->spisok.add(pointer->spisok.head, index);
		}
		else if (pointer->data > val) {
			addNode(pointer->leftChild, val, index);
			if (heightChanged) {
				if (pointer->balanceFactor == 1) {
					pointer->balanceFactor = 0;
					heightChanged = false;
				}
				else if (pointer->balanceFactor == 0) {
					pointer->balanceFactor = -1;
				}
				else {
					pointer2 = new Node(val);
					pointer1 = new Node(val);
					pointer1 = pointer->leftChild;
					if (pointer1->balanceFactor == -1) {
						pointer->leftChild = pointer1->rightChild;
						pointer1->rightChild = pointer;
						pointer->balanceFactor = 0;
						pointer = pointer1;
					}
					else {
						pointer2 = pointer1->rightChild;
						pointer1->rightChild = pointer2->leftChild;
						pointer2->leftChild = pointer1;
						pointer->leftChild = pointer2->rightChild;
						pointer2->rightChild = pointer;
						if (pointer2->balanceFactor == -1) pointer->balanceFactor = 1; else pointer->balanceFactor = 0;
						if (pointer2->balanceFactor == 1) pointer1->balanceFactor = -1; else pointer1->balanceFactor = 0;
						pointer = pointer2;
					}
					pointer->balanceFactor = 0;
					heightChanged = false;
				}
			}
		}
		else if (pointer->data < val) {
			addNode(pointer->rightChild, val, index);
			if (heightChanged) {
				if (pointer->balanceFactor == -1) {
					pointer->balanceFactor = 0;
					heightChanged = false;
				}
				else if (pointer->balanceFactor == 0) {
					pointer->balanceFactor = 1;
				}
				else {
					pointer2 = new Node(val);
					pointer1 = new Node(val);
					pointer1 = pointer->rightChild;
					if (pointer1->balanceFactor == 1) {
						pointer->rightChild = pointer1->leftChild;
						pointer1->leftChild = pointer;
						pointer->balanceFactor = 0;
						pointer = pointer1;
					}
					else {
						pointer2 = pointer1->leftChild;
						pointer1->leftChild = pointer2->rightChild;
						pointer2->rightChild = pointer1;
						pointer->rightChild = pointer2->leftChild;
						pointer2->leftChild = pointer;
						if (pointer2->balanceFactor == 1) pointer->balanceFactor = -1; else pointer->balanceFactor = 0;
						if (pointer2->balanceFactor == -1) pointer1->balanceFactor = 1; else pointer1->balanceFactor = 0;
						pointer = pointer2;

					}
					pointer->balanceFactor = 0;
					heightChanged = false;
				}
			}

		}
		else {
			pointer->spisok.add(pointer->spisok.head, index);
			heightChanged = false;
		}
	}

	void print_Tree(Node* p, int level, string &ans) {
		if (p != nullptr) {
			print_Tree(p->rightChild, level + 1, ans);
			for (int i = 0; i < level; i++) ans += "   ";
			ans += p->data + "[ " + p->spisok.listprint() + "]" + '\n';
			print_Tree(p->leftChild, level + 1, ans);
		}
	}

	void _del_balanceLeft(Node*& node) {
		Node* pointer1, * pointer2;
		if (node->balanceFactor == -1) node->balanceFactor = 0;
		else if (node->balanceFactor == 0) {
			node->balanceFactor = 1;
			heightChanged = false;
		}
		else {
			pointer1 = new Node(node->data);
			pointer1 = node->rightChild;
			if (pointer1->balanceFactor >= 0) {
				node->rightChild = pointer1->leftChild;
				pointer1->leftChild = node;
				if (pointer1->balanceFactor == 0) {
					node->balanceFactor = 1;
					pointer1->balanceFactor = -1;
					heightChanged = false;
				}
				else {
					node->balanceFactor = 0;
					pointer1->balanceFactor = 0;
				}
				node = pointer1;
			}
			else {
				pointer2 = new Node(node->data);
				pointer2 = pointer1->leftChild;
				pointer1->leftChild = pointer2->rightChild;
				pointer2->rightChild = pointer1;
				node->rightChild = pointer2->leftChild;
				pointer2->leftChild = node;
				if (pointer2->balanceFactor == 1) {
					node->balanceFactor = -1;
				}
				else {
					node->balanceFactor = 0;
				}
				if (pointer2->balanceFactor == -1) {
					pointer1->balanceFactor = 1;
				}
				else {
					pointer1->balanceFactor = 0;
				}
				node = pointer2;
				pointer2->balanceFactor = 0;
			}
		}
	}

	void _del_balanceRight(Node*& node) {
		Node* pointer1, * pointer2;
		if (node->balanceFactor == 1) node->balanceFactor = 0;
		else if (node->balanceFactor == 0) {
			node->balanceFactor = -1;
			heightChanged = false;
		}
		else {
			pointer1 = new Node(node->data);
			pointer1 = node->leftChild;
			if (pointer1->balanceFactor <= 0) {
				node->leftChild = pointer1->rightChild;
				pointer1->rightChild = node;
				if (pointer1->balanceFactor == 0) {
					node->balanceFactor = -1;
					pointer1->balanceFactor = 1;
					heightChanged = false;
				}
				else {
					node->balanceFactor = 0;
					pointer1->balanceFactor = 0;
				}
				node = pointer1;
			}
			else {
				pointer2 = new Node(node->data);
				pointer2 = pointer1->rightChild;
				pointer1->rightChild = pointer2->leftChild;
				pointer2->leftChild = pointer1;
				node->leftChild = pointer2->rightChild;
				pointer2->rightChild = node;
				if (pointer2->balanceFactor == -1) node->balanceFactor = 1; else node->balanceFactor = 0;
				if (pointer2->balanceFactor == 1) pointer1->balanceFactor = -1; else pointer1->balanceFactor = 0;
				node = pointer2;
				pointer2->balanceFactor = 0;
			}
		}
	}

	void _delWhenTwoChild(Node*& node, Node*& delNode) {
		if (node->rightChild != nullptr) {
			_delWhenTwoChild(node->rightChild, delNode);
			if (heightChanged) {
				_del_balanceRight(node);
			}
		}
		else {
			delNode->data = node->data;
			delNode->spisok = node->spisok;
			delNode = node;
			node = node->leftChild;
			heightChanged = true;
		}
	}

	void delNode(Node*& pointer, string val, int index) {
		Node* temp = nullptr;
		if (pointer == nullptr);
		else if (pointer->data > val) {
			delNode(pointer->leftChild, val, index);
			if (heightChanged)
				_del_balanceLeft(pointer);
		}
		else if (pointer->data < val) {
			delNode(pointer->rightChild, val, index);
			if (heightChanged)
				_del_balanceRight(pointer);
		}
		else {
			if (pointer->spisok.size > 1)
				pointer->spisok.Delete(pointer->spisok.head, index);
			else {
				temp = pointer;
				if (temp->rightChild == nullptr) {
					pointer = temp->leftChild;
					heightChanged = true;
					delete temp;
					temp = nullptr;
				}
				else if (temp->leftChild == nullptr) {

					pointer = temp->rightChild;
					heightChanged = true;
					delete temp;
					temp = nullptr;
				}
				else {
					_delWhenTwoChild(temp->leftChild, temp);
					if (heightChanged) _del_balanceLeft(pointer);
				}
			}
		}
	}

	void free_tree(Node* aBranch) {
		if (!aBranch) return;
		free_tree(aBranch->leftChild);
		free_tree(aBranch->rightChild);
		delete aBranch;
		return;
	}

	void Search(Node* root, string val, vector<worker> WorkerBuffer, list* &ind) {
		if (!root)
			return;
		else if (val < root->data)
			return Search(root->leftChild, val, WorkerBuffer, ind);
		else if (val > root->data)
			return Search(root->rightChild, val, WorkerBuffer, ind);
		else if (val == root->data) {
			root->spisok.print(WorkerBuffer);
			ind = &(root->spisok);
		}
	}

	int search(Node* root, string val, vector<worker> WorkerBuffer) {
		if (!root)
			return -1;
		else if (val < root->data)
			return search(root->leftChild, val, WorkerBuffer);
		else if (val > root->data)
			return search(root->rightChild, val, WorkerBuffer);
		else if (val == root->data)
			return 1;
	}

};

class HTworker :public SPR
{
private:
	int* table;
	string* data_table;
	char* status;
	int size;
	int k;


	int Nod(int size) {
		bool not_swapped = true;
		int n = 1;
		while (not_swapped) {
			n += 1;
			if ((size % n)) {
				not_swapped = false;
			}
		}
		return n;
	}

public:
	int count = 0;

	HTworker(int newSize) {
		size = newSize;
		table = new int[size];
		data_table = new string[size];
		status = new char[size];
		k = Nod(size);
		for (int i = 0; i < size; i++)
			status[i] = 0;
		for (int i = 0; i < size; i++)
			table[i] = -1;
	}

	int firstHf(string data, int k) {
		string a = "";
		int sum = 0;
		for (int i = 1; i < data.size() + 1; i++) {
			a = a + to_string(data[i - 1] * -1);
			if (i % k == 0) {
				sum += stoi(a);
				a = "";
			}
		}
		if (!(a == "")) sum += stoi(a);
		return sum % this->size;
	}

	int secondHf(int hash, int step, int k) {
		return (hash + k * step) % size;
	}

	void addToTable(string data, vector<worker> WorkerBuffer, int index) {
		int hash = firstHf(data, k);

		if (!(status[hash] == 1)) {
			table[hash] = index;
			data_table[hash] = data;
			status[hash] = 1;
			count++;
		}
		else
			collisionAddToTable(hash, data, WorkerBuffer, index);
	}

	void collisionAddToTable(int hash, string data, vector<worker> WorkerBuffer, int index) {
		int step = k;
		int secondHash = secondHf(hash, step, k);
		while ((status[secondHash] == 1) && (step / 5 <= size)) {
			if ((WorkerBuffer[table[secondHash]].Profession == data) && (status[secondHash] == 1)) {
				break;
			}
			step = step + k;
			secondHash = secondHf(hash, step, k);
		}
		if (!(status[secondHash] == 1)) {
			table[secondHash] = index;
			data_table[secondHash] = data;
			status[secondHash] = 1;
			count++;
		}
	}

	void Udalenie(string data, vector<worker> WorkerBuffer) {
		int hash = firstHf(data, k);
		if ((WorkerBuffer[table[hash]].Profession == data) && status[hash] == 1) {
			status[hash] = 2;
			count--;
		}
		else collisionUdal(hash, data, WorkerBuffer);
	}

	void collisionUdal(int hash, string data, vector<worker> WorkerBuffer) {
		int step = k;
		int secondHash = secondHf(hash, step, k);
		while (!((WorkerBuffer[table[secondHash]].Profession == data) && status[secondHash] == 1) && status[secondHash] != 0 && (step / 5 <= size)) {
			step = step + k;
			secondHash = secondHf(hash, step, k);
		}
		if ((WorkerBuffer[table[secondHash]].Profession == data) && status[secondHash] == 1) {
			status[secondHash] = 2;
			count--;
		}
		else cout << " Не могу удалить " << endl;
	}

	void Pechat(string &ans) {
		int i = 0;
		while (i < size) {
			if (status[i] == 1)
				ans += to_string(i) + " == " + data_table[i] + '(' + to_string(table[i]) + ')' + '\n';
			i = i + 1;
		}
	}

	int poisk(string val, vector<worker> WorkerBuffer) {
		int hash = firstHf(val, k);
		if (!(table[hash] > WorkerBuffer.size()))
			if ((WorkerBuffer[table[hash]].Profession == val) && status[hash] == 1)
				return table[hash];
			else return collisionPoisk(hash, val, WorkerBuffer);

		else
			return -1;
	}

	int collisionPoisk(int hash, string val, vector<worker> WorkerBuffer) {
		int step = 0;
		int secondHash = hash;
		while ((status[secondHash] != 0) && (step / 5 <= size)) {
			if (status[secondHash] == 1) {
				if (WorkerBuffer[table[secondHash]].Profession == val) {
					return table[secondHash];
				}
			}
			step = step + k;
			secondHash = secondHf(hash, step, k);
		}
		return -1;
	}
};

struct SprWorker : public SPR {
	AvlTree treeF;
	AvlTree treeA;
	AvlTree treeEx;
	AvlTree treeSal;
	int HTsize;
	HTworker* HT;

	SprWorker(int size = 20) {
		HTsize = size;
		HT = new HTworker(HTsize);
	}

	bool PreSeach(worker W, int index) {
		if (((W.FIO == WorkerBuffer[index].FIO) || (W.FIO == "")) && ((W.Age == WorkerBuffer[index].Age) || (W.Age == "")) && ((W.Experience == WorkerBuffer[index].Experience) || (W.Experience == "")) && ((W.Profession == WorkerBuffer[index].Profession) || (W.Profession == "")) && ((W.Salary == WorkerBuffer[index].Salary) || (W.Salary == "")))
			return true;
		else return false;
	}

	int Search(string val) {
		int index = HT->poisk(val, WorkerBuffer);
		if (!(index == -1)) {
			return index;
		}
		else return -1;
	}

	int Search_Profession(string val) {
		int index = Search(val);
		if (index != -1)
			this->WorkerBuffer[index].print();
		return index;
	}

	list* Search_FIO(string val) {
		list* ind = nullptr;
		treeF.Search(treeF.root, val, WorkerBuffer, ind);
		return ind;
	}

	list* Search_Age(string val) {
		list* ind = nullptr;
		treeA.Search(treeA.root, val, WorkerBuffer, ind);
		return ind;
	}

	list* Search_Experience(string val) {
		list* ind = nullptr;
		treeEx.Search(treeEx.root, val, WorkerBuffer, ind);
		return ind;
	}

	list* Search_Salary(string val) {
		list* ind = nullptr;
		treeSal.Search(treeSal.root, val, WorkerBuffer, ind);
		return ind;
	}

	void Search_Oll(worker W) {
		int index = Search(W.Profession);
		if ((index != -1) && (PreSeach(W, index)))
			this->WorkerBuffer[index].print();
	}

	int Add(worker W) {
		if (W.check()) {
			int index = Search(W.Profession);
			if (index == -1) {
				if (HTsize > HT->count) {
					WorkerBuffer.push_back(W);
					treeF.addNode(treeF.root, W.FIO, WorkerBuffer.size() - 1);
					treeA.addNode(treeA.root, W.Age, WorkerBuffer.size() - 1);
					treeEx.addNode(treeEx.root, W.Experience, WorkerBuffer.size() - 1);
					treeSal.addNode(treeSal.root, W.Salary, WorkerBuffer.size() - 1);
					HT->addToTable(W.Profession, WorkerBuffer, WorkerBuffer.size() - 1);
					return 0;
				}
				else {
					cout << " Хеш-таблица заполнена " << endl;
					return 1;
				}
			}
			else {
				cout << " Данные не уникальны " << endl;
				return 2;
			}
		}
		else {
			cout << " Данные некорректны" << endl;
			return 3;
		}
	}

	int Delete(worker W) {
		int index = Search(W.Profession);

		if ((index > -1) && (PreSeach(W, index))) {
			treeF.delNode(treeF.root, W.FIO, index);
			treeA.delNode(treeA.root, W.Age, index);
			treeEx.delNode(treeEx.root, W.Experience, index);
			treeSal.delNode(treeSal.root, W.Salary, index);

			HT->Udalenie(W.Profession, WorkerBuffer);

			WorkerBuffer[index].FIO = "";
			WorkerBuffer[index].Age = "";
			WorkerBuffer[index].Experience = "";
			WorkerBuffer[index].Profession = "";
			WorkerBuffer[index].Salary = "";
			cout << " Удалено" << endl;
			return 0;
		}
		else {
			cout << " Такого элемента нет " << endl;
			return 1;
		}
	}

	void Reading(string path) {
		setlocale(LC_ALL, "ru");
		fstream in;
		in.open(path);
		string buffer;
		if (in.is_open()) {
			while (getline(in, buffer)) {
				stringstream l(buffer);
				string F, I, O, Age, Exp, Salary = "", S1, S2 = "", S3 = "", S4 = "", prof;
				l >> F >> I >> O >> Age >> Exp >> S1 >> S2 >> S3 >> S4 >> Salary;
				string FIO = F + ' ' + I + ' ' + O;

				if (Salary == "") {
					if ((S2 != "") && (S3 == "")) {
						prof = S1;
						Salary = S2;
					}
					if ((S3 != "") && (S4 == "")) {
						prof = S1 + ' ' + S2;
						Salary = S3;
					}
					if ((S4 != "") && (Salary == "")) {
						prof = S1 + ' ' + S2 + ' ' + S3;
						Salary = S4;
					}
				}
				else {
					prof = S1 + ' ' + S2 + ' ' + S3 + ' ' + S4;
					Salary = Salary;
				}
				worker w(FIO, Age, Exp, prof, Salary);
				Add(w);
			}
		}
		in.close();
	}

	void Write(string path) {
		ofstream in;
		in.open(path);
		if (in.is_open())
		{
			for (int i = 0; i < WorkerBuffer.size(); i++) {
				worker W = WorkerBuffer[i];
				if (!(W.FIO == ""))
					in << W.FIO << " " << W.Age << " " << W.Experience << " " << W.Profession << " " << W.Salary << endl;
			}
		}
		in.close();
	}

};

//==========================================UI==========================================//

class wxMyGrid : public wxGrid
{
public:
	wxMyGrid(wxWindow* parent) : wxGrid(parent, wxID_ANY) {
	};
	
	void clearGrid() {
		if (this->GetNumberRows() >= 1)this->DeleteRows(0, this->GetNumberRows());
	};

	void update(vector<worker> lst) {
		if (this->GetNumberRows() >= 1)this->DeleteRows(0, this->GetNumberRows());
		this->AppendRows(lst.size());
		this->ClearGrid();
		int j = 0;
		for (auto val : lst) {
			this->SetCellValue(wxGridCellCoords(j, 0), val.FIO);
			this->SetCellValue(wxGridCellCoords(j, 1), val.Age);
			this->SetCellValue(wxGridCellCoords(j, 2), val.Experience);
			this->SetCellValue(wxGridCellCoords(j, 3), val.Profession);
			this->SetCellValue(wxGridCellCoords(j, 4), val.Salary);
			j++;
		}
		this->SetColLabelValue(0, "ФИО");
		this->SetColLabelValue(1, "Возраст");
		this->SetColLabelValue(2, "Опыт\nработы");
		this->SetColLabelValue(3, "Специализация");
		this->SetColLabelValue(4, "Заработная\nплата");
	}

	void setColumnSize(unsigned int size) {
		int colWidth = (size - wxSystemSettings::GetMetric(wxSYS_VSCROLL_X) - this->GetRowLabelSize()) / 5;
		for (int i = 0; i < 5; i++) this->SetColSize(i, colWidth);
	};
protected:
	wxSize DoGetBestSize() const override {
		return GetParent()->GetClientSize();
	};
};

class InputFrame : public wxFrame
{
public:
	InputFrame(const wxString& title, char type, SprWorker*& wr, wxMyGrid*& grid) : wxFrame(nullptr, wxID_ANY, title) {
		this->type = type;
		this->wr = wr;
		this->grid = grid;
		wxPanel* panel = new wxPanel(this);
		wxButton* btn;
		std::string lables[7]{ "ФИО", "Возраст","Опыт\nработы", "Специализация", "Заработная\nплата" };
		for (int i = 0; i < 5; i++)
		{
			new wxStaticText(panel, wxID_ANY, lables[i], wxPoint(10, 20 + 30 * i));
			inputFields[i] = new wxTextCtrl(panel, 100 + i, wxEmptyString, wxPoint(150, 20 + 30 * i), wxSize(230, 20));
		}
		if (type == 0) {
			btn = new wxButton(panel, 120, "Добавить", wxPoint(160, 180), wxSize(80, 20));
			btn->Bind(wxEVT_BUTTON, &InputFrame::checkData, this);
		}
		else if (type == 1) {
			btn = new wxButton(panel, 120, "Удалить", wxPoint(160, 180), wxSize(80, 20));
			btn->Bind(wxEVT_BUTTON, &InputFrame::delData, this);
		}
		else {
			wxArrayString choices;
			choices.Add("по ФИО");
			choices.Add("по Возрасту");
			choices.Add("по Опыту работы");
			choices.Add("по Специализации");
			choices.Add("по Заработной плате");
			this->choice = new wxChoice(panel, 220, wxPoint(30, 180), wxSize(120, 20), choices);
			this->choice->Select(0);
			btn = new wxButton(panel, 120, "Поиск", wxPoint(290, 180), wxSize(80, 20));
			btn->Bind(wxEVT_BUTTON, &InputFrame::searchData, this);
		}		
	}

private:
	char type;
	wxMyGrid* grid;
	wxTextCtrl* inputFields[5];
	wxChoice* choice;
	wxChoice* orderInput[2];
	SprWorker* wr;
	void checkData(wxCommandEvent& evt) {
		worker new_wr(string(inputFields[0]->GetValue().mb_str()), string(inputFields[1]->GetValue().mb_str()), string(inputFields[2]->GetValue().mb_str()), string(inputFields[3]->GetValue().mb_str()), string(inputFields[4]->GetValue().mb_str()));
		int code = wr->Add(new_wr);
		if(code == 1)wxMessageBox("Хеш-таблица заполнена");
		if (code == 2)wxMessageBox("Данные не уникальны");
		if (code == 3)wxMessageBox("Данные некорректны");
		this->grid->update(wr->WorkerBuffer);
		this->Close();
	};
	void delData(wxCommandEvent& evt) {
		worker new_wr(string(inputFields[0]->GetValue().mb_str()), string(inputFields[3]->GetValue().mb_str()), string(inputFields[4]->GetValue().mb_str()), string(inputFields[5]->GetValue().mb_str()), string(inputFields[6]->GetValue().mb_str()));
		int code = wr->Delete(new_wr);
		if (code == 1)wxMessageBox("Такого элемента нет");
		this->grid->update(wr->WorkerBuffer);
		this->Close();
	};
	void searchData(wxCommandEvent& evt) {
		if (choice->GetSelection() == 0) {
			list* ind = wr->Search_FIO(string(inputFields[0]->GetValue().mb_str()));
			if (ind != nullptr) {
				string ans = "совпадения найдены по индексам: ";
				for (struct list::node* current = ind->head; current; current = current->pNext) {
					ans += to_string(current->index+1)+ ' ';
				}
				wxMessageBox(ans);
			}
			else {
				wxMessageBox("Нет совпадений");
			}
		}
		else if (choice->GetSelection() == 1) {
			list* ind = wr->Search_Age(string(inputFields[1]->GetValue().mb_str()));
			if (ind != nullptr) {
				string ans = "совпадения найдены по индексам: ";
				for (struct list::node* current = ind->head; current; current = current->pNext) {
					ans += to_string(current->index + 1) + ' ';
				}
				wxMessageBox(ans);
			}
			else {
				wxMessageBox("Нет совпадений");
			}
		}
		else if (choice->GetSelection() == 2) {
			list* ind = wr->Search_Experience(string(inputFields[2]->GetValue().mb_str()));
			if (ind != nullptr) {
				string ans = "совпадения найдены по индексам: ";
				for (struct list::node* current = ind->head; current; current = current->pNext) {
					ans += to_string(current->index + 1) + ' ';
				}
				wxMessageBox(ans);
			}
			else {
				wxMessageBox("Нет совпадений");
			}
		}
		else if (choice->GetSelection() == 3) {
			int ind = wr->Search_Profession(string(inputFields[3]->GetValue().mb_str()));
			if (ind != -1) {
				string ans = "совпадения найдены по индексу: ";
				ans += to_string(ind + 1) + ' ';
				wxMessageBox(ans);
			}
			else {
				wxMessageBox("Нет совпадений");
			}
		}
		else if (choice->GetSelection() == 4) {
			list* ind = wr->Search_Salary(string(inputFields[4]->GetValue().mb_str()));
			if (ind != nullptr) {
				string ans = "совпадения найдены по индексам: ";
				for (struct list::node* current = ind->head; current; current = current->pNext) {
					ans += to_string(current->index + 1) + ' ';
				}
				wxMessageBox(ans);
			}
			else {
				wxMessageBox("Нет совпадений");
			}
		}
		this->Close();
	};
};

class DebugFrame : public wxFrame
{
public:
	DebugFrame(const wxString& title, SprWorker*& wr) : wxFrame(nullptr, wxID_ANY, title) {
		this->wr = wr;
		wxScrolledWindow* mainPanel = new wxScrolledWindow(this);
		wxSizer* mainPanelSizer = new wxBoxSizer(wxVERTICAL);
		//Grid
		this->text = new wxStaticText(mainPanel, wxID_ANY, "~\n~\n~\n~\n~\n~\n~\n~\n~\n~\n~\n~\n~\n~\n~\n~\n~\n~\n~\n~\n~\n~\n~\n~\n~\n~\n~\n~\n~\n~\n~\n~\n~\n~\n~\n~\n~\n~\n~\n~\n~\n~\n~\n~\n~\n~\n~\n~\n");

		//RadioBox
		wxArrayString choices;
		choices.Add("Дерево ФИО");choices.Add("Дерево Возраста");choices.Add("Дерево Стажа");choices.Add("Дерево Заработной платы");choices.Add("Хеш Таблица");


		this->radioBox = new wxRadioBox(mainPanel, RADIOBOX, "Справочник", wxDefaultPosition, wxDefaultSize, choices);

		radioBox->SetSelection(2);

		mainPanelSizer->Add(this->radioBox, wxSizerFlags().Expand().Border());
		mainPanelSizer->Add(this->text, wxSizerFlags(1).Expand().Border());

		mainPanel->SetSizer(mainPanelSizer);
		mainPanel->FitInside();
		CreateStatusBar();
	};

private:
	SprWorker* wr;
	wxStaticText* text;
	wxRadioBox* radioBox;
	void PrintStruct(wxCommandEvent& evt) {
		string ans;
		switch (this->radioBox->GetSelection()) {
		case 0:wr->treeF.print_Tree(wr->treeF.root, 1, ans);break;
		case 1:wr->treeA.print_Tree(wr->treeA.root, 1, ans);break;
		case 2:wr->treeEx.print_Tree(wr->treeEx.root, 1, ans);break;
		case 3:wr->treeSal.print_Tree(wr->treeSal.root, 1, ans);break;
		case 4:wr->HT->Pechat(ans);break;
		};
		this->text->SetLabelText(ans);
	};
	wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(DebugFrame, wxFrame)
	EVT_RADIOBOX(RADIOBOX, DebugFrame::PrintStruct)
wxEND_EVENT_TABLE()

class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString& title, int size) : wxFrame(nullptr, wxID_ANY, title) {
		//Main Panel
		this->wr = new SprWorker(size);
		this->toolBar = CreateToolBar(wxTB_TEXT);
		this->toolBar->AddTool(OPEN_BUTTON, "открыть из файла", wxBitmap(open_icon), "Открыть сохранение");
		this->toolBar->AddTool(SAVE_BUTTON, "сохранить", wxBitmap(save_icon), "Сохранить");
		this->toolBar->AddTool(DEBUG_BUTTON, "окно отладки", wxBitmap(debug_icon), "Окно Отладки");
		this->toolBar->AddTool(ADD_BUTTON, "добавить в справочник", wxBitmap(add_icon), "Добавляет информацию в текущий справочник");
		this->toolBar->AddTool(DELETE_BUTTON, "удалить из справочника", wxBitmap(delete_icon), "Удаляет информацию из текущего справочника");
		this->toolBar->AddTool(SEARCH_BUTTON, "окно поиска", wxBitmap(find_icon), "Ищет в текущем справочнике");
		this->toolBar->Realize();
		this->mainPanel = new wxScrolledWindow(this);
		wxSizer* mainPanelSizer = new wxBoxSizer(wxVERTICAL);
		this->grid = new wxMyGrid(mainPanel);
		this->grid->CreateGrid(0, 5);
		this->grid->EnableEditing(false);
		this->grid->Refresh();
		this->grid->update(this->wr->WorkerBuffer);
		mainPanelSizer->Add(grid, wxSizerFlags(1).Expand().Border());
		this->mainPanel->SetSizer(mainPanelSizer);
		this->mainPanel->FitInside();
		this->mainPanel->SetScrollRate(16, 16);

		CreateStatusBar();

	}

	void resizeGrid() {
		this->grid->setColumnSize(mainPanel->GetClientSize().GetWidth());
	};

private:
	wxToolBar* toolBar;
	wxRadioBox* radioBox;
	wxMyGrid* grid;
	wxScrolledWindow* mainPanel;
	SprWorker* wr;
	void OpenData(wxCommandEvent& evt) {
		this->wr->Reading("./save.txt");
		this->grid->update(this->wr->WorkerBuffer);
	};
	void RemoveData(wxCommandEvent& evt);
	void SaveData(wxCommandEvent& evt) {
		this->wr->Write("./save.txt");
	};
	void ClearData(wxCommandEvent& evt);
	void DebugWin(wxCommandEvent& evt) {
		DebugFrame* debugFrame = new DebugFrame("Debug", this->wr);
		debugFrame->SetMinSize(wxSize(1000, 1000));
		debugFrame->SetMaxSize(wxSize(1000, 1000));
		debugFrame->Show();
	};
	void AddData(wxCommandEvent& evt) {
		InputFrame* inputFrame = new InputFrame("Input", 0, this->wr, this->grid);
		inputFrame->SetParent(this);
		inputFrame->SetMinSize(wxSize(400, 250));
		inputFrame->SetMaxSize(wxSize(400, 250));
		inputFrame->Show();
	};
	void DeleteData(wxCommandEvent& evt) {
		InputFrame* inputFrame = new InputFrame("Input", 1, this->wr, this->grid);
		inputFrame->SetParent(this);
		inputFrame->SetMinSize(wxSize(400, 250));
		inputFrame->SetMaxSize(wxSize(400, 250));
		inputFrame->Show();
	};
	void SearchData(wxCommandEvent& evt) {
		InputFrame* inputFrame = new InputFrame("Input", 2, this->wr, this->grid);
		inputFrame->SetParent(this);
		inputFrame->SetMinSize(wxSize(400, 250));
		inputFrame->SetMaxSize(wxSize(400, 250));
		inputFrame->Show();
	};
	wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_TOOL(OPEN_BUTTON, MainFrame::OpenData)
	EVT_TOOL(SAVE_BUTTON, MainFrame::SaveData)
	EVT_TOOL(DEBUG_BUTTON, MainFrame::DebugWin)
	EVT_TOOL(ADD_BUTTON, MainFrame::AddData)
	EVT_TOOL(DELETE_BUTTON, MainFrame::DeleteData)
	EVT_TOOL(SEARCH_BUTTON, MainFrame::SearchData)
wxEND_EVENT_TABLE()


class startFrame : public wxFrame
{
	public:
		startFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
			this->SetMinSize(wxSize(250, 150));
			this->SetMaxSize(wxSize(250, 150));
			wxPanel* panel = new wxPanel(this);
			wxButton* btn;
			new wxStaticText(panel, wxID_ANY, "размер ХТ", wxPoint(20, 20));
			spn1 = new wxSpinCtrl(panel, wxID_ANY, "размер ХТ", wxPoint(180, 20), wxDefaultSize, wxSP_ARROW_KEYS, 10, 100);
			spn1->SetValue(10);
			btn = new wxButton(panel, 333, "Подтвердить", wxPoint(80, 70), wxSize(90, 20));
			btn->Bind(wxEVT_BUTTON, &startFrame::OK, this);
		};

	private:
		int* size;
		int* k;
		wxSpinCtrl* spn1, * spn2;
		void OK(wxCommandEvent& evt) {
			MainFrame* mainFrame = new MainFrame("Справочник", this->spn1->GetValue());
			mainFrame->Center();
			mainFrame->Maximize();
			mainFrame->Show();
			mainFrame->resizeGrid();
			this->Close();
		};
};

class Cursach : public wxApp
{
public:
	bool OnInit() {
		(new startFrame("конфиг ХТ"))->Show();

		return true;
	}
};

wxIMPLEMENT_APP(Cursach);


/*
int main()
{
	setlocale(LC_ALL, "ru");
	SprWorker wr(10);

	// создание данных о работнике
	worker w1("Уваров Даниил Александрович", "20", "0", "Дворник", "2500");
	worker w2("Петров Анатолий Викторович", "40", "9", "Охранник", "3000");
	worker w3("Иванов Иван Иванович", "30", "15", "Охраник", "3500");
	worker w4("Иванов Иван Иванович", "40", "9", "Охранник", "3000");

	//// добавление данных 
	wr.Add(w1);
	wr.Add(w2);
	wr.Add(w3);

	//// выыод деревьев
	wr.treeF.print_Tree(wr.treeF.root, 1);
	wr.treeA.print_Tree(wr.treeA.root, 1);
	wr.treeEx.print_Tree(wr.treeEx.root, 1);
	wr.treeSal.print_Tree(wr.treeSal.root, 1);

	//// вывод хт
	wr.HT->Pechat();

	//// поиски
	wr.Search_Profession("Дворник");
	wr.Search_Profession("Охранник");
	wr.Search_Oll(w3);

	//// удаление
	wr.Delete(w1);
	wr.Delete(w3);
	wr.Delete(w4);

	wr.HT->Pechat();

};
*/
