#include <bits/stdc++.h>

using namespace std;

string Dataset_file_name ;

// utility functions
vector<string> process_attributes(FILE *f);
vector<string> process_data_types(FILE *f);

int main(int argc, char *argv[]){
    string file_name = argv[1];

    // opening new dataset.arff file
    char arff_file_name[100]={'@'};
    char relation_name[100];
    int i = 0;
    while (i<sizeof(argv[1])){
        if (argv[1][i] == '.') break;
        relation_name[i] = argv[1][i];
        arff_file_name[i++] = argv[1][i];  
    }
    relation_name[i] = '\0';
    Dataset_file_name = relation_name;
    for (auto x: ".arff\0") arff_file_name[i++] = x;
    FILE *arff = fopen(arff_file_name, "w");
    
    // PROCESSING FILE:
    FILE *f = fopen(argv[1] , "r");

    // Finding list of attributes:
    vector<string> attributes = process_attributes(f);
    // Finding data types of the attributes:
    vector<string> data_types = process_data_types(f);
    fclose(f);

    // put @RELATION 
    fputs("@RELATION ", arff);
    fputs(relation_name, arff);
    fputs("\n\n", arff);

    // put @ATTRIBUTE
    for (int i = 0 ;i<attributes.size() ;i++){
        fputs("@ATTRIBUTE ", arff);
        for (auto c: attributes[i]){
            putc(c, arff);
        }
        putc(' ', arff);
        for (auto c: data_types[i]){
            putc(c, arff);
        }
        putc('\n', arff);
    } 

    //removing header line
    f = fopen(argv[1], "r");
    char s[1000];
    fgets(s , 1000, f);

    // put @DATA
    putc('\n', arff);
    fputs("@DATA\n",arff);
    while (fgets(s, 1000, f)){
        fputs(s, arff);
    }

    fclose(f);
    fclose(arff);

    return 0;
}

vector<string> process_attributes(FILE *f){
    int i = 0;
    char c;
    vector<string> attributes ;
    while ((c=getc(f))!='\n' && c!=EOF){
        if (c == ',') continue;
        else if (c == '"'){
            string h;
            while ((c=getc(f))!='"') {
                h+=c;
            }
            attributes.push_back(h);
        }
        else {
            string h;
            while ((c=getc(f))!=',') {
                h+=c;
            }
            attributes.push_back(h);
        }
    }
    return attributes;
}

vector<string> process_data_types(FILE *f){
    vector<string> types;
    char c;
    int ind = 0;
    map<int, set<string>> m;
    while ((c=getc(f))!='\n') { 
        if (c == ',') continue;
        char s[100];
        int i = 0;
        s[i++]=c;
        while ((c=getc(f))!=',' and c!='\n'){
            s[i++]=c;
        }
        s[i++] = '\0';
        try{
            float f = stof(s);
            types.push_back("NUMERIC");
        }
        catch (exception e){
            m[ind].insert(s);
            types.push_back("STRING");
        }
        ind++;
        if (c == '\n') break;
    }
    int total_data = 1;
    while (c!=EOF){
        c = '1';
        ind = 0;
        total_data++;
        while ((c=getc(f))!='\n') {
            if (c == EOF) goto next_step;
            if (c == ',') continue;
            char s[100];
            int i = 0;
            s[i++]=c;
            while ((c=getc(f))!=',' and c!='\n' and c!=EOF){
                s[i++]=c;
            }
            s[i++] = '\0';
            try{
                float f = stof(s);
            }
            catch (exception e){
                m[ind].insert(s);
            }
            // cout << s << endl;
            ind++;
            if (c == EOF) goto next_step;
            if (c == '\n') break;
        }
    }
    next_step:
    for (auto x : m ){
        if (x.first <= total_data/2){
            string tmp = "{";
            for (auto y : x.second){
                tmp += Dataset_file_name+ "-" + y;
                if (y!=*(--x.second.end()))
                    tmp+=',';
            }
            tmp+='}';
            types[x.first] = tmp;
        }
        
    }
    return types;
}