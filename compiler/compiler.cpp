#include <iostream>
#include <string>
#include <regex>
#include <vector>
#include <stdio.h>

using namespace std;
string translate(string stmt);
string extractRest(string cpp_for_statement, int index);
string formatter(string statement);

void error(string message) {
    if(message == "") message = "Error";
    cout << message << endl;
    exit(1);
}

string extractCondition(string cpp_if_statement){
int count = 0;
    if(cpp_if_statement.find("(") != string::npos){
        count++;

    }

    size_t body_start_index = cpp_if_statement.find("(") + 1;
    for(int i = body_start_index; i < cpp_if_statement.size(); i++){
        if(cpp_if_statement[i] == '('){
            count++;
        }
        else if(cpp_if_statement[i] == ')'){
            count--;
        }
        if(count == 0){
            return cpp_if_statement.substr(body_start_index, i - body_start_index);
        }
    }
    return "";
}

vector<string> extractBody(string cpp_body_statement){
    int count = 0;
    if(cpp_body_statement.find("{") != string::npos){
        count++;
    }

    size_t body_start_index = cpp_body_statement.find("{") + 1;
    for(int i = body_start_index; i < cpp_body_statement.size(); i++){
        if(cpp_body_statement[i] == '{'){
            count++;
        }
        else if(cpp_body_statement[i] == '}'){
            count--;
        }
        if(count == 0){
            return {cpp_body_statement.substr(body_start_index, i - body_start_index)+"\n",extractRest(cpp_body_statement, i+1)};
        }
    }
    return {};
}

string extractRest(string cpp_for_statement, int index){
    return cpp_for_statement.substr(index);
}

bool checkCondition(string condition){
    regex condition_regex("^\\s*(\\()?[\\w\\d\\+\\-\\*/%<>=!&|()\\s]+(\\))?(\\s*(&&|\\|\\|)\\s*(\\()?[\\w\\d\\+\\-\\*/%<>=!&|()\\s]+(\\))?)?\\s*$");
    return !regex_match(condition, condition_regex);
}

bool checkVariableName(string variable_name){
    regex variable_regex("^\\s*[a-zA-Z_]+[a-zA-Z0-9_]*\\s*$");
    return !regex_match(variable_name, variable_regex);
}

bool checkValueExpression(string value_expression){
    regex value_regex("^\\s*[\\w\\d\\(\\)\\+\\-\\*/%<>=!&|]+\\s*$"
);
    return !regex_match(value_expression, value_regex);
}

string removeDataTypes(string variable_name){
    regex data_type_regex("\\b(int|short|long|float|double|char|bool|string|signed|unsigned)\\b");
    return regex_replace(variable_name, data_type_regex, "");
}

string removeWhiteSpace(string variable_name){
    for(int i =0;i<variable_name.size();i++)
        if(variable_name[i]==' ')
            variable_name.erase(i--,1);
    return variable_name;
}

string removeSemicolon(string variable_name){
    variable_name.erase(remove(variable_name.begin(), variable_name.end(), ';'), variable_name.end());
    return variable_name;
}

/* ************************************************************************** */

string convert_if_statement(string cpp_if_statement) {

    // extract the condition expression
    string condition_expression = extractCondition(cpp_if_statement);

    // check if the condition expression is valid
    if (checkCondition(condition_expression)) {
        error("error in condition expression");
    }

    // extract the body of the if statement
    vector<string> body = extractBody(cpp_if_statement);
    body[0] = removeDataTypes(body[0]);
    body[0] = translate(body[0]);

    cpp_if_statement = removeWhiteSpace(cpp_if_statement);

    // create the equivalent Python code
    string python_code = "if " + condition_expression + ":\n";
    size_t pos = 0;
    while ((pos = body[0].find("\n", pos)) != string::npos) {
        body[0].replace(pos, 1, "\n    ");
        pos += 2;
    }
    python_code += "    " + body[0] ;

    for(int i =0;i<python_code.size();i++)
        if(python_code[i]==';')
            python_code.erase(i,1);

    string translated = translate(body[1]);
    if(translated != ""){
        python_code += "\n" + translated;
    }
    return python_code;
}

string convert_elseIf_statement(string cpp_elseIf_statement) {
    // extract the condition expression
    string condition_expression = extractCondition(cpp_elseIf_statement);

    // check if the condition expression is valid
    if (checkCondition(condition_expression)) {
        error("error in condition expression");
    }

    // extract the body of the if statement
    vector<string> body = extractBody(cpp_elseIf_statement);
    body[0] = removeDataTypes(body[0]);
    body[0] = translate(body[0]);

    cpp_elseIf_statement = removeWhiteSpace(cpp_elseIf_statement);

    // create the equivalent Python code
    string python_code = "elif " + condition_expression + ":\n";
    size_t pos = 0;
    while ((pos = body[0].find("\n", pos)) != string::npos) {
        body[0].replace(pos, 1, "\n    ");
        pos += 2;
    }
    python_code += "    " + body[0];

    for(int i =0;i<python_code.size();i++)
        if(python_code[i]==';')
            python_code.erase(i,1);

    string translated = translate(body[1]);
    if(translated != ""){
        python_code += "\n" + translated;
    }
    return python_code;
}

string convert_else_statement(string cpp_else_statement) {
    // extract the body of the if statement
    vector<string> body = extractBody(cpp_else_statement);
    body[0] = removeDataTypes(body[0]);
    body[0] = translate(body[0]);

    cpp_else_statement = removeWhiteSpace(cpp_else_statement);

    // create the equivalent Python code
    string python_code = "else :\n";
    size_t pos = 0;
    while ((pos = body[0].find("\n", pos)) != string::npos) {
        body[0].replace(pos, 1, "\n    ");
        pos += 2;
    }
    python_code += "    " +  body[0];

    for(int i =0;i<python_code.size();i++)
        if(python_code[i]==';')
            python_code.erase(i,1);

    string translated = translate(body[1]);
    if(translated != ""){
        python_code += "\n" + translated;
    }
    return python_code;
}

string convert_assignment_statement(string cpp_assignment_statement) {
    // remove any leading/trailing white space
    cpp_assignment_statement = cpp_assignment_statement.erase(0, cpp_assignment_statement.find_first_not_of(" \t\r\n"));
    cpp_assignment_statement = cpp_assignment_statement.erase(cpp_assignment_statement.find_last_not_of(" \t\r\n") + 1);

    // check if the statement contains an equal sign
    if (cpp_assignment_statement.find("=") == string::npos) {
        return "";
    }
    // extract the variable name
    size_t variable_end_index = cpp_assignment_statement.find("=");
    string variable_name = cpp_assignment_statement.substr(0, variable_end_index);


    // remove any data types
    variable_name = removeDataTypes(variable_name);
    variable_name = removeWhiteSpace(variable_name);

    // check if the variable name is valid
    if (checkVariableName(variable_name)) {
        error("error in variable name");
    }

    // extract the value expression
    if(cpp_assignment_statement.find(";")==string::npos)
        error("error in assignment statement");
    string value_expression = cpp_assignment_statement.substr(variable_end_index + 1,cpp_assignment_statement.find(";")-variable_end_index-1);

    // check if the value expression is valid
    if (checkValueExpression(value_expression)) {
        error("error in value expression");
    }

    cpp_assignment_statement = cpp_assignment_statement.substr(cpp_assignment_statement.find(";")+1);

    // create the equivalent Python code
    string translated = translate(cpp_assignment_statement);
    string python_code = variable_name + " = " + value_expression + (translated==""?"":"\n" + translated);

    return python_code;
}


/**********************************************************/
string convert_while_statement(string cpp_while_statement) {

    // extract the condition expression
    string condition_expression = extractCondition(cpp_while_statement);

    // check if the condition expression is valid
    if (checkCondition(condition_expression)) {
        error("error in condition expression");
    }

    // extract the body of the if statement
    vector<string> body = extractBody(cpp_while_statement);
    body[0] = removeDataTypes(body[0]);
    body[0] = translate(body[0]);

    cpp_while_statement = removeWhiteSpace(cpp_while_statement);

    // create the equivalent Python code
    string python_code = "while " + condition_expression + ":\n";
    size_t pos = 0;
    while ((pos = body[0].find("\n", pos)) != string::npos) {
        body[0].replace(pos, 1, "\n    ");
        pos += 2;
    }
    python_code += "    " + body[0] ;

    for(int i =0;i<python_code.size();i++)
        if(python_code[i]==';')
            python_code.erase(i,1);

    string translated = translate(body[1]);
    if(translated != ""){
        python_code += "\n" + translated;
    }
    return python_code;
}

string translate(string stmt){
    string converted;
    // remove any leading/trailing white space
    stmt = stmt.erase(0, stmt.find_first_not_of(" \t\r\n"));
    stmt = stmt.erase(stmt.find_last_not_of(" \t\r\n") + 1);

    // check if the statement starts with "if"
    if (stmt.substr(0, 2) == "if") {
        converted += convert_if_statement(stmt);
    }else if(stmt.substr(0, 7) == "else if"){
        converted += convert_elseIf_statement(stmt);
    }else if(stmt.substr(0, 4) == "else"){
        converted += convert_else_statement(stmt);
    }
    else if(stmt.substr(0, 5) == "while"){
        converted += convert_while_statement(stmt);
    }
    else{
        converted += convert_assignment_statement(stmt);
    }
    return converted;
}


string formatter(string statement){
    vector<string> cppOperators = {"&&","||","true","false"};
    vector<string> pythonOperators = {" and "," or "," True "," False "};
    for(int i =0;i<cppOperators.size();i++){
        size_t pos = 0;
        while ((pos = statement.find(cppOperators[i], pos)) != string::npos) {
            statement.replace(pos, cppOperators[i].size(), pythonOperators[i]);
            pos += pythonOperators[i].size();
        }
    }
    return statement;
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