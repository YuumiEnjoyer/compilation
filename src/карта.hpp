#include <map>
#include <set>
#include <Windows.h>
#include <string>

using namespace std;


enum class State { START, ID, INT, FLOAT_DOT, FLOAT, OP, END, ERR, SEP };
enum class CharType { LETTER, DIGIT, DOT, OP, SPACE, UNKNOWN, SEP };
enum class TokenTypea {
    ID,                   // Имена (идентификаторы)
    KEYWORD,                  // Служебные слова
    INT,                      // Целые числа
    FLOAT,                    // Вещественные числа
    ADDITIVE_OPERATOR,        // + -
    MULTIPLICATIVE_OPERATOR,  // * /
    COMPARISON_OPERATOR,      // <, >, ==, <=, >=, !=
    ASSIGNMENT_OPERATOR,      // =
    DELIMITER,                // ( ) [ ] ,
    ERR                     // Ошибка
};


const string stateToString(State state) {
    switch (state) {
        case State::START: return "START";
        case State::ID:  return "ID";
        case State::INT:  return "INT";
        case State::FLOAT_DOT:  return "FLOAT_DOT";
        case State::FLOAT:  return "FLOAT";
        case State::OP:     return "OP";
        case State::END:     return "END";
        case State::ERR:     return "ERR";
        case State::SEP:     return "SEP";
        default:               return "ERR";
    }
}
const string charTypeToString(CharType ct) {
    switch (ct) {
        case CharType::LETTER: return "LETTER";
        case CharType::DIGIT:  return "DIGIT";
        case CharType::DOT:    return "DOT";
        case CharType::SPACE:  return "SPACE";
        case CharType::OP:     return "OP";
        case CharType::UNKNOWN:return "UNKNOWN";
        case CharType::SEP:     return "SEP";
        default:               return "ERROR";
    }
}
const string tokenTypeToString(TokenTypea type) {
    switch (type) {
        case TokenTypea::KEYWORD: return "KEYWORD";
        case TokenTypea::ID: return "ID";
        case TokenTypea::INT: return "INT";
        case TokenTypea::FLOAT: return "FLOAT";
        case TokenTypea::ADDITIVE_OPERATOR: return "ADDITIVE_OPERATOR";
        case TokenTypea::MULTIPLICATIVE_OPERATOR: return "MULTIPLICATIVE_OPERATOR";
        case TokenTypea::COMPARISON_OPERATOR: return "COMPARISON_OPERATOR";
        case TokenTypea::ASSIGNMENT_OPERATOR: return "ASSIGNMENT_OPERATOR";
        case TokenTypea::DELIMITER: return "DELIMITER";
        case TokenTypea::ERR: return "ERR";
        default: return "ERROR";
    }
}


const map<State, map<CharType, State>> MAP = {
    {State::START, {
        {CharType::LETTER, State::ID},
        {CharType::DIGIT, State::INT},
        {CharType::DOT, State::ERR},
        {CharType::OP, State::OP},
        {CharType::SPACE, State::START},
        {CharType::UNKNOWN, State::ERR},
        {CharType::SEP, State::SEP},
    }},

    {State::ID, {
        {CharType::LETTER, State::ID},
        {CharType::DIGIT, State::ID},
        {CharType::DOT, State::ERR},
        {CharType::OP, State::END},
        {CharType::SPACE, State::END},
        {CharType::UNKNOWN, State::ERR},
        {CharType::SEP, State::END},
    }},

    {State::INT, {
        {CharType::LETTER, State::ERR},
        {CharType::DIGIT, State::INT},
        {CharType::DOT, State::FLOAT_DOT},
        {CharType::OP, State::END},
        {CharType::SPACE, State::END},
        {CharType::UNKNOWN, State::ERR},
        {CharType::SEP, State::END},
    }},

    {State::FLOAT_DOT, {
        {CharType::LETTER, State::ERR},
        {CharType::DIGIT, State::FLOAT},
        {CharType::DOT, State::ERR},
        {CharType::OP, State::ERR},
        {CharType::SPACE, State::ERR},
        {CharType::UNKNOWN, State::ERR},
        {CharType::SEP, State::ERR},
    }},

    {State::FLOAT, {
        {CharType::LETTER, State::ERR},
        {CharType::DIGIT, State::FLOAT},
        {CharType::DOT, State::ERR},
        {CharType::OP, State::END},
        {CharType::SPACE, State::END},
        {CharType::UNKNOWN, State::ERR},
        {CharType::SEP, State::END},
    }},

    {State::OP, {
        {CharType::LETTER, State::END},
        {CharType::DIGIT, State::END},
        {CharType::DOT, State::ERR},
        {CharType::OP, State::END},
        {CharType::SPACE, State::END},
        {CharType::UNKNOWN, State::ERR},
        {CharType::SEP, State::END},
    }},

    {State::ERR, {
        {CharType::LETTER, State::ERR},
        {CharType::DIGIT, State::ERR},
        {CharType::DOT, State::ERR},
        {CharType::OP, State::ERR},
        {CharType::SPACE, State::END},
        {CharType::UNKNOWN, State::ERR},
        {CharType::SEP, State::ERR},
    }},

    {State::SEP, {
        {CharType::LETTER, State::END},
        {CharType::DIGIT, State::END},
        {CharType::DOT, State::ERR},
        {CharType::OP, State::END},
        {CharType::SPACE, State::END},
        {CharType::UNKNOWN, State::ERR},
        {CharType::SEP, State::END},
    }},
};

struct Token {
    TokenTypea type;
    string value;
    int line;
    int column;

    Token(TokenTypea type, string value, int line, int column) : type(type), value(value), line(line), column(column) {}

    string getTypeName() const;
    string repr() const {
        return tokenTypeToString(type) + ": " + value + " (line: " + to_string(line) + ", column: " + to_string(column) + ")";
    }
};

const set<string> keywords = {
    "коли", "отнюдь", "покуда"
};
