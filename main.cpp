#include <fstream>
#include <iostream>
#include <sstream>
#include <regex>
#include <chrono>
#include <algorithm>

#include <set>
#include <map>
#include <stack>
#include <queue>

using namespace std;

void removeUnnecessaryElements(string &line){
    //Removing symbols
    set<char> symbols {'!','@','#','$','%','^','&','*','(',')','-','_','=','+','{','}','[',']','\\','|',':',';','\'','"','<','>',',','.','/','?','1','2','3','4','5','6','7','8','9','0'};
    for(char symbol : symbols){
        line.erase(std::remove(line.begin(), line.end(), symbol), line.end());
    }

    //Turning all letters into lower case
    for(int i=0; i<line.length(); i++){
        line[i] = tolower(line[i]);
    }
}

void findUrls(string line, vector<string> &urlList){
    stringstream ss(line);
    string word;

    const regex pattern("(((http|https)://)?www\\.)?[a-zA-Z0-9@:%._\\+~#?&//=]{2,256}\\.[a-z]{2,6}\\b([-a-zA-Z0-9@:%._\\+~#?&//=]*)");

    while(ss >> word){
        if(regex_match(word, pattern)) urlList.push_back(word);
    }
}

void addWordsToWordsList(string line, map<string, set<int>> &wordsList, int lineNr){
    stringstream ss(line);
    string word;

    while(ss >> word){
        auto match = wordsList.find(word);

        if(match == wordsList.end()){
            //Jei dar nera map
            set<int> emptySet;
            emptySet.insert(lineNr);
            wordsList.insert(pair <string, set<int>> (word, emptySet));
        }
        else match->second.insert(lineNr);
    }
}

void printUrlAndWordsList(vector<string> urlList, map<string, set<int>> wordsList){
    ofstream out("output.txt");

    // Printing url
    if(urlList.size() >= 1){
        out << "Url's:" << endl;
        for(int i=0; i<urlList.size(); i++){
            out << urlList[i] << endl;
        }
        out << "---------------------------------------------------" << endl;
    }
    
    // Printing repetitive words from wordsList
    for(auto it = wordsList.begin(); it != wordsList.end(); it++){
        if(it->second.size() > 1) {
            out << "'" << it->first << "' pasikartoja " << it->second.size() << " kartus, eilutese: ";
            for(int i : it->second){
                out << i << " ";
            }
            out << endl;
        }
        
    }
}

string getFileName(){
    string fileName;

    while(true){
        try
        {
            cout << "Iveskite failo pavadinima: ";
            cin >> fileName;

            // Try opening file, if failed, throw an exception
            ifstream in(fileName);
            if(in.fail()) throw 1;
            
            return fileName;
        }
        catch(int errorID)
        {
            cout << "Failas pavadinimu '" << fileName << "' buvo nerastas, arba jo atidaryti nepavyko" << endl;
        }
    }
    
    return fileName;
}

int main(){
    ifstream file(getFileName());

    // Start timer
    chrono::steady_clock sc;
    auto start = sc.now();
    cout << "Programa pradeta" << endl;
    
    // Variable initialisation
    map<string, set<int>> wordsList;
    vector<string> urlList;
    string line;
    int lineNr = 1;

    // Perform actions with every line of text from text file
    while(getline(file, line)){
        findUrls(line, urlList);

        removeUnnecessaryElements(line);

        addWordsToWordsList(line, wordsList, lineNr);
        
        lineNr++;
    }

    printUrlAndWordsList(urlList, wordsList);

    // End timer
    auto end = sc.now();
    auto time_span = static_cast<chrono::duration<double>>(end - start);   // measure time span between start & end
    cout<<"Programos veikimo laikas: "<<time_span.count()<<"s";

    return 0;
}