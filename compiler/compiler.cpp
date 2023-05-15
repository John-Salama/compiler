#include <iostream>
#include <string>
#include <regex>
#include <vector>
#include <stdio.h>

using namespace std;

/* *******************************Functions Headers******************************************* */
string translate(string code);
string extractRest(string cppCode, int restOfTheCodeStartindex);
string formatter(string code);
/* ************************************************************************** */

/* ******************************* HELPERS ******************************************* */

void error(string message) {
    cout << message << endl;
    exit(EXIT_SUCCESS);
}

vector<size_t> extractCondition(string cppStatment){
int count = 0;
    if(cppStatment.find("(") != string::npos){
        count++;
    }
    size_t bodyStartIndex = cppStatment.find("(") + 1;
    for(int i = bodyStartIndex; i < cppStatment.size(); i++){
        if(cppStatment[i] == '('){
            count++;
        }
        else if(cppStatment[i] == ')'){
            count--;
        }
        if(count == 0){
            return {bodyStartIndex, (size_t)i};
        }
    }
    return {};
}

vector<string> extractCode(string cppCode , size_t conditionEndIndex){
    int count = 0;   
    if(cppCode.substr(conditionEndIndex + 1, 1) != "{"){
        if(cppCode.find(";")==string::npos)
            error("statment without body");
        return {cppCode.substr(conditionEndIndex + 1, cppCode.find(";") - conditionEndIndex)+"\n", extractRest(cppCode, cppCode.find(";") + 1)};
    }
    if(cppCode.find("{") != string::npos){
        count++;
    }
    size_t body_start_index = cppCode.find("{") + 1;
    for(int i = body_start_index; i < cppCode.size(); i++){
        if(cppCode[i] == '{'){
            count++;
        }
        else if(cppCode[i] == '}'){
            count--;
        }
        if(count == 0){
            return {cppCode.substr(body_start_index, i - body_start_index)+"\n",extractRest(cppCode, i+1)};
        }
    }
    return {};
}

string extractRest(string cppCode, int restOfTheCodeStartindex){
    return cppCode.substr(restOfTheCodeStartindex);
}

bool checkCondition(string condition){
    regex conditionRegex("^\\s*(\\()?[\\w\\d\\+\\-\\*/%<>=!&|()\\s]+(\\))?(\\s*(&&|\\|\\|)\\s*(\\()?[\\w\\d\\+\\-\\*/%<>=!&|()\\s]+(\\))?)?\\s*$");
    return !regex_match(condition, conditionRegex);
}

bool checkVariableName(string variableName){
    regex variableRegex("^\\s*[a-zA-Z_]+[a-zA-Z0-9_]*\\s*$");
    return !regex_match(variableName, variableRegex);
}

bool checkValueExpression(string valueExpression){
    regex valueRegex("^\\s*[\\w\\d\\(\\)\\+\\-\\*/%<>=!&|]+\\s*$"
);
    return !regex_match(valueExpression, valueRegex);
}

string removeDataTypes(string cppCode){
    regex dataTypeRegex("\\b(int|short|long|float|double|char|bool|string|signed|unsigned)\\b");
    return regex_replace(cppCode, dataTypeRegex, "");
}

string removeWhiteSpace(string statment){
    for(int i =0;i<statment.size();i++)
        if(statment[i]==' ')
            statment.erase(i--,1);
    return statment;
}

string removeSemicolon(string variableName){
    variableName.erase(remove(variableName.begin(), variableName.end(), ';'), variableName.end());
    return variableName;
}

string formatter(string code){
    vector<string> cppOperators = {"&&","||","true","false"};
    vector<string> pythonOperators = {" and "," or "," True "," False "};
    for(int i =0;i<cppOperators.size();i++){
        size_t pos = 0;
        while ((pos = code.find(cppOperators[i], pos)) != string::npos) {
            code.replace(pos, cppOperators[i].size(), pythonOperators[i]);
            pos += pythonOperators[i].size();
        }
    }
    return code;
}

/* ************************************************************************** */

string convertStatment(string cppStatment, string statmentType)
{   
    vector<size_t> conditionExpressionIndecis = {};
    string conditionExpression = "";
    if(statmentType == "if" || statmentType == "else if" || statmentType == "while")
    {
    // extract the condition expression
    conditionExpressionIndecis = extractCondition(cppStatment);
    conditionExpression = cppStatment.substr(conditionExpressionIndecis[0], conditionExpressionIndecis[1] - conditionExpressionIndecis[0]);

    // check if the condition expression is valid
    if (checkCondition(conditionExpression)) {
        error("error in condition expression");
    }
    conditionExpression = removeWhiteSpace(conditionExpression);
    }

    // extract the body and the rest of the code of the if statement
    vector<string> code = {};
    if(statmentType != "else")
        code = extractCode(cppStatment, conditionExpressionIndecis[1]);
    else
        code = extractCode(cppStatment, 3);
    string body = code[0];
    string restOfCode = code[1];
    body = removeDataTypes(body);
    body = translate(body);
    

    cppStatment = removeWhiteSpace(cppStatment);

    // create the equivalent Python code
    string pythonCode = "";
    if(statmentType == "if")
    {
    pythonCode = "if " + conditionExpression + ":\n";
    }
    else if(statmentType == "else if")
    {
    pythonCode = "elif " + conditionExpression + ":\n";
    }
    else if(statmentType == "else")
    {
    pythonCode = "else:\n";
    }
    else
    {
    pythonCode = "while " + conditionExpression + ":\n";
    }

    size_t pos = 0;
    while ((pos = body.find("\n", pos)) != string::npos) {
        body.replace(pos, 1, "\n    ");
        pos += 2;
    }
    pythonCode += "    " + body ;
    for(int i =0;i<pythonCode.size();i++)
        if(pythonCode[i]==';')
            pythonCode.erase(i,1);

    string translatedCode = translate(restOfCode);
    if(translatedCode != ""){
        pythonCode += "\n" + translatedCode;
    }
    return pythonCode;
}

string convertAssignmentStatement(string cppAssignmentStatement) {
    // remove any leading/trailing white space
    cppAssignmentStatement = cppAssignmentStatement.erase(0, cppAssignmentStatement.find_first_not_of(" \t\r\n"));
    cppAssignmentStatement = cppAssignmentStatement.erase(cppAssignmentStatement.find_last_not_of(" \t\r\n") + 1);

    // check if the statement contains an equal sign
    if (cppAssignmentStatement.find("=") == string::npos) {
        return "";
    }
    // extract the variable name
    size_t variableEndIndex = cppAssignmentStatement.find("=");
    string variableName = cppAssignmentStatement.substr(0, variableEndIndex);


    // remove any data types
    variableName = removeDataTypes(variableName);
    variableName = removeWhiteSpace(variableName);

    // check if the variable name is valid
    if (checkVariableName(variableName)) {
        error("error in variable name");
    }

    // extract the value expression
    if(cppAssignmentStatement.find(";")==string::npos)
        error("error in assignment statement");
    string valueExpression = cppAssignmentStatement.substr(variableEndIndex + 1,cppAssignmentStatement.find(";")-variableEndIndex-1);

    // check if the value expression is valid
    if (checkValueExpression(valueExpression)) {
        error("error in value expression");
    }

    cppAssignmentStatement = cppAssignmentStatement.substr(cppAssignmentStatement.find(";")+1);

    // create the equivalent Python code
    string translated = translate(cppAssignmentStatement);
    string pythonCode = variableName + " = " + valueExpression + (translated==""?"":"\n" + translated);

    return pythonCode;
}

string translate(string code){
    string translatedCode;
    // remove any leading/trailing white space
    code = code.erase(0, code.find_first_not_of(" \t\r\n"));
    code = code.erase(code.find_last_not_of(" \t\r\n") + 1);

    // check if the statement starts with "if"
    if (code.substr(0, 2) == "if") {
        translatedCode += convertStatment(code, "if");
    }else if(code.substr(0, 7) == "else if"){
        translatedCode += convertStatment(code, "else if");
    }else if(code.substr(0, 4) == "else"){
        translatedCode += convertStatment(code, "else");
    }
    else if(code.substr(0, 5) == "while"){
        translatedCode += convertStatment(code, "while");
    }
    else{
        translatedCode += convertAssignmentStatement(code);
    }
    return translatedCode;
}

int main()
{
    freopen("input.cpp" ,"r" ,stdin);
    string code="",line;
    while(getline(cin,line)){
        code+=line;
    }
    string python_code = formatter(translate(code));
    cout << python_code << endl;
    freopen("output.py" ,"w" ,stdout);
    cout << python_code << endl;   
}