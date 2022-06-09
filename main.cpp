#include <fstream>
#include <iostream>
#include <sstream>
#include <regex>
#include <chrono>
#include <algorithm>
#include <cctype>
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
    string word, temp;
    ifstream urls("urls.txt");
    vector<string> visiURL;
    while (getline(urls, line))
    {
        visiURL.push_back(line);
    }
    char dot = '.';
    size_t found;
    int lenght;
    while(ss >> word){
        found = word.find_last_of(dot);
        if(found != string::npos)
        {
            lenght = word.length() - found - 1;
            temp = word.substr(found + 1, lenght);
            for (auto it = visiURL.begin(); it != visiURL.end(); it++)
            {
                if(temp == *it) urlList.push_back(word);
            }
            
        }
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

void delMax(map<string, set<int>> wordsList){
    int max = 0;
    string maxW, maxWup;
    for(auto it = wordsList.begin(); it != wordsList.end(); it++)
    {
        if(it->second.size() > max) 
        {
            maxW = it->first;
            max = it->second.size();
        }
    }
    maxWup = maxW;
    maxWup[0] = toupper(maxWup[0]);
    cout << "Dazniausiai pasikartojantis zodis: " << maxWup << endl;
    string line;
    ifstream file("duom.txt");
    ofstream out("outputBeMaxElem.txt");
    while(getline(file, line))
    {
        line = regex_replace(line, regex(maxW), "");
        line = regex_replace(line, regex(maxWup), "");
        out << line << endl;
    }

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
    delMax(wordsList);
    
    printUrlAndWordsList(urlList, wordsList);
    

    // End timer
    auto end = sc.now();
    auto time_span = static_cast<chrono::duration<double>>(end - start);   // measure time span between start & end
    cout<<"Programos veikimo laikas: "<<time_span.count()<<"s";

    return 0;
}