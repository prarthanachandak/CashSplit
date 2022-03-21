#include <bits/stdc++.h>
using namespace std;

vector<string> people;
map<string, float> cash;

//calculate net amount by adding credits and substracting debts

void netAmount(string debiter, string crediter, float amount){
		
		if(cash.find(debiter) == cash.end()){
			cash[debiter] = 0 - amount;
		}
		else{
			cash[debiter] -= amount;
		}

		if(cash.find(crediter) == cash.end()){
			cash[crediter] = amount;
		}
		else{
			cash[crediter] += amount;
		}

}

//add crediters and debiters (all people) to a common list

void peopleList(string debiter, string crediter){

		int flagc=0, flagd=0;

		for(int i=0; i<people.size(); i++){
			if(people[i]==crediter){
				flagc=1;
				break;
			}
		}

		if(flagc!=1){
			people.push_back(crediter);
		}

		for(int i=0; i<people.size(); i++){
			if(people[i]==debiter){
				flagd=1;
				break;
			}
		}

		if(flagd!=1){
			people.push_back(debiter);
		}
}

//input all the transactions

void Transactions(){
	cout<<"Enter transactions: "<<endl;
	char s;
	do{

		string crediter, debiter;
		float amount;
		cin>>debiter;
		cin>>crediter;
		cin>>amount;

		peopleList(debiter, crediter);
		netAmount(debiter, crediter, amount);	

		cout<<"Do you want to continue? If Yes, enter Y, else N"<<endl;
		cin>>s;

	}
	while(s!='N' && s!='n');
}

//input the common group transactions

void groupTransactions(){

	int g;
	cout<<"Enter the number of group transactions: \n";
	cin>>g;

	for(int i=0; i<g; i++){
		cout<<"Enter details of group transaction "<<i+1<<":\n";
		string crediter;
		float amount;
		int n;

		cout<<"Enter Crediter's name\n";
		cin>>crediter;
		cout<<"Enter the amount paid by "<<crediter<<":\n";
		cin>>amount;

		cout<<"Enter the number of people in the contribution:\n";
		cin>>n;

		cash[crediter]+=amount;

		float each = amount/n;

		for(int i=0; i<n; i++){
			string name;
			cin>>name;
			cash[name] -= each;
		}

	}
}

//find the person who is the maximum crediter

pair<string, float> maximumCrediter()
{
    
    pair<string, float> personWithMaxCredit
        = make_pair("xyz", 0);
  
    map<string, float>::iterator currentEntry;
    for (currentEntry = cash.begin();
         currentEntry != cash.end();
         ++currentEntry) {
  

        if (currentEntry->second
            > personWithMaxCredit.second) {
  
            personWithMaxCredit
                = make_pair(
                    currentEntry->first,
                    currentEntry->second);
        }
    }
  
    return personWithMaxCredit;
}

//find the person who has to pay maximum debts

pair<string, float> maximumDebiter()
{
  
    auto it = cash.begin();

    pair<string, float> personWithMaxDebt
        = make_pair(it->first, it->second);
  
    map<string, float>::iterator currentEntry;
    for (currentEntry = cash.begin();
         currentEntry != cash.end();
         ++currentEntry) {
  
        if (currentEntry->second
            < personWithMaxDebt.second) {
  
            personWithMaxDebt
                = make_pair(
                    currentEntry->first,
                    currentEntry->second);
        }
    }
  
    return personWithMaxDebt;
}

//actual logic to minimize the cash flow and store it as adjacency list reprsentation of graph
vector<pair<int, float> >* minimizeCashFlow(vector<pair<int, float> > finalFlow[]){

	while(!cash.empty()){

		string Pc, Pd;
		float maxCredit, maxDebit;

		pair<string, float> credit = maximumCrediter();
		Pc = credit.first;
		maxCredit = credit.second;

		pair<string, float> debit = maximumDebiter();
		Pd = debit.first;
		maxDebit = 0 - debit.second;

		float x = min(maxDebit, maxCredit);

		cash[Pd] +=x ;
		cash[Pc] -= x;

		vector<string>::iterator itd = find(people.begin(), people.end(), Pd);
		vector<string>::iterator itc = find(people.begin(), people.end(), Pc);

		int dd = distance(people.begin(), itd);
		int dc = distance(people.begin(), itc);

		pair<int, float> entry = make_pair(dc, x);
		finalFlow[dd].push_back(entry);

		if(x==maxCredit && x==maxDebit){
			cash.erase(Pc);
			cash.erase(Pd);
		}
		else{
			if(x==maxCredit){
				cash.erase(Pc);
			}
			else{
				cash.erase(Pd);
			}
		}

	}
	return finalFlow;
}

//print the final flow

void printFinalFlow(vector<pair<int, float> > finalFlow[], int n){
	int v;
	float w;

	for(int u=0; u<n; u++){
		if(finalFlow[u].empty()){
			continue;
		}
		else{
			cout<<people[u]<<" -> ";

			for(auto it = finalFlow[u].begin(); it!=finalFlow[u].end(); it++){
				v = it->first;
				w = it->second;

				cout<<"("<<people[v]<<","<<it->second<<")"<<"\t";
			}
			cout<<"\n";


		}
	}
}

int main(){

	int n;
	cout<<"Enter number of people: "<<endl;
	cin>>n;

	vector<pair<int, float> > finalFlow[n];
	vector<pair<int, float>> *p;


	Transactions();
	groupTransactions();

	p = minimizeCashFlow(finalFlow);
	printFinalFlow(p, n);
	
}

/*Input
Enter number of people: 
3
Enter transactions: 
p1 p2 1000
Do you want to continue? If Yes, enter Y, else N
y
p2 p3 5000
Do you want to continue? If Yes, enter Y, else N
y
p1 p3 2000
Do you want to continue? If Yes, enter Y, else N
n
Enter the number of group transactions: 
2
Enter details of group transaction 1:
Enter Crediter's name
p1
Enter the amount paid by p1:
5000
Enter the number of people in the contribution:
2  
p1
p2
Enter details of group transaction 2:
Enter Crediter's name
p3
Enter the amount paid by p3:
2000
Enter the number of people in the contribution:
2
p3 
p1
p2 -> (p3,6500)	
p1 -> (p3,1500)	*/