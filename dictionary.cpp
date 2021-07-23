#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <conio.h>
#define ALPHABETS 26
using namespace std;
class Trie_Node
{
	string meaning;
	Trie_Node *child [ALPHABETS];
	friend class Dictionary;

public:
	Trie_Node();
};

class Dictionary
{
	Trie_Node *root;
public:
	Dictionary();
	bool isEmpty(Trie_Node *);
	Trie_Node *removestr(Trie_Node *, string, int);
	Trie_Node *clearDict(Trie_Node *);
	void removestr(string);
	void insertstr(string, string);
	void searchstr(string);
	void autoComp(Trie_Node *, string, vector<string> &);
	~Dictionary();
};

Dictionary::Dictionary()
{
	root = new Trie_Node();
}
Trie_Node::Trie_Node() : meaning("")
{
	for (int i = 0; i < ALPHABETS; i++)
		child[i] = NULL;
}
void Dictionary::insertstr(string word, string meaning)
{
	int i, ctr;
	Trie_Node *temp = root;
	for (i = 0; i < word.length(); i++)
	{
		ctr = word[i] - 'a';
		if (temp->child[ctr] == NULL)
			temp->child[ctr] = new Trie_Node();
		temp = temp->child[ctr];
	}
	temp->meaning = meaning;
}
void Dictionary::autoComp(Trie_Node *root, string word, vector<string> &similarWords)
{
	
	Trie_Node *temp = root;
	if (root->meaning != "")
	{
		similarWords.push_back(word);
	}
	for (int i = 0; i < ALPHABETS; i++)
	{
		if (temp->child[i])
			autoComp(temp->child[i], word + (char)(i + 'a'), similarWords);
	}
}
void Dictionary::searchstr(string word)
{
		int i, ctr;
	Trie_Node *temp = root;
	vector<string> similarWords;
	for (i = 0; i < word.length(); i++)
	{
		ctr = word[i] - 'a';
		if (temp->child[ctr] == NULL)
		{
			autoComp(temp, word.substr(0, i), similarWords);
			break;
		}
		temp = temp->child[ctr];
	}
	if (temp->meaning == "")
		autoComp(temp, word, similarWords);

	if (temp->meaning != "")
		cout << "\n\tThe word \"" << word << "\" found with meaning \"" << temp->meaning << "\"";
	else
	{
		cout << "\n\tThe word \"" << word << "\" not found";
		if (similarWords.size() == 0)
			return;
		cout << "\n\tDid you mean? ";
		for (i = 0; i < similarWords.size() && i < 3; i++)
		{
			cout << similarWords[i];
			if (!(i == similarWords.size() - 1 || i == 2))
				cout << " or ";
		}
	}
}
bool Dictionary::isEmpty(Trie_Node *temp)
{
	int i;
	for (i = 0; i < ALPHABETS; i++)
		if (temp->child[i])
			return 0;
	return 1;
}
Trie_Node *Dictionary::removestr(Trie_Node *temp, string word, int ptr = 0)
{
	if (ptr == word.size())
	{
		temp->meaning = "";
		if (isEmpty(temp))
		{
			delete (temp);
			temp = NULL;
		}
		return temp;
	}
	int ctr = word[ptr] - 'a';
	temp->child[ctr] = removestr(temp->child[ctr], word, ptr + 1);
	if (isEmpty(temp) && temp->meaning == "")
	{
		delete temp;
		temp = NULL;
	}
	return temp;
}

void Dictionary::removestr(string word)
{
	removestr(root, word);
	cout << "\n\tThe word \"" << word << " has been successfully removed. ";
}

Trie_Node *Dictionary::clearDict(Trie_Node *temp)
{
	for (int i = 0; i < ALPHABETS; i++)
	{
		if (temp->child[i])
			temp->child[i] = clearDict(temp->child[i]);
	}
	delete temp;
	temp = NULL;
	return temp;
}
Dictionary ::~Dictionary()
{
	clearDict(root);
}

int main()
{
	Dictionary ob;
	string word, meaning;
	ifstream fin;
	int n, ch;
	char c;
	cout << "\n\tDICTIONARY";
	cout << "\n\t1.EXTRACTING DATA FROM FILE";
	cout << "\n\t2.ADD WORDS/MEANING";
	cout << "\n\tEnter your choice: ";
	cin >> ch;
	switch (ch)
	{
	case 1:
		fin.open("Dictionary.txt");
		while (!fin.eof())
		{
			fin >> word;
			fin >> meaning;
			ob.insertstr(word, meaning);
		}
		fin.close();
		break;
	case 2:
		ofstream fout; 
		ifstream fin;
		fin.open("Dictionary.txt");
		fout.open("Dictionary.txt", ios::app); 
		if (fin.is_open())
		{
			string word_add;
			cout<<"Enter Word";
			cin >> word_add;
            cout<<"Enter Meaning";
			string meaning_add;
			cin >> meaning_add;
			fout << "\n"<< word_add << " " << meaning_add;
		} 
		cout << "\n Data has been appended to file";
		fin.close();
		fout.close();
		fin.open("Dictionary.txt");
		while (!fin.eof())
		{
			fin >> word;
			fin >> meaning;
			ob.insertstr(word, meaning);
		}
		fin.close(); // Closing the file
		break;
	}
	cout << "\n\tAdding words to the dictionary...";
	cout << "\n\tAll the words have been successfully added to the dictionary.";
	do
	{
		system("cls");
		cout << "\n\t1.Search Word";
		cout << "\n\t2.Remove Word";
		cout << "\n\tEnter your choice: ";
		cin >> ch;
		cout << "\n\tENTER WORD: ";
		cin >> word;

		switch (ch)
		{
		case 1:
			ob.searchstr(word);
			break;
		case 2:
			ob.removestr(word);
			break;
		}
		cout << "\n\n\twant to continue (enter 'y' or 'Y') ";
		cin >> c;
	} while (c == 'y' || c == 'Y');
	return 0;
}
