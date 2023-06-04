#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <algorithm>
#include <list>
#include <fstream>


class Monomial {
private:
    std::vector<int> degrees;     // Набор степеней переменных
    std::vector<char> variables;  // Имена переменных
    double coefficient;           // Коэффициент

public:
    // Конструкторы
    Monomial() : coefficient(0.0) {};
    Monomial(const std::string& monomialString) {

        std::string str = monomialString;
        str.erase(std::remove_if(str.begin(), str.end(), ::isspace), str.end());
        size_t start = str.find('<') + 1;
        size_t end = str.find('>');
        std::string monomialStr = str.substr(start, end - start);
        if (isalpha(monomialStr[0])) {
            coefficient = 1;
        }
        else if (monomialStr[0] == '-' && isalpha(monomialStr[1])) {
            coefficient = -1;
            str.replace(1, 1, "");
        }
        else if (monomialStr[0] == '+' && isalpha(monomialStr[1])) {
            coefficient = 1;
            str.replace(1, 1, "");
        }
        else if (monomialStr[0] == '+' && isdigit(monomialStr[1])) {
            monomialStr.replace(0, 1, "");
            str.replace(1, 1, "");
            coefficient = std::stod(monomialStr);
        }
        else if (monomialStr[0] == '-' && isdigit(monomialStr[1])) {
            monomialStr.replace(0, 1, "");
            str.replace(1, 1, "");
            coefficient = -std::stod(monomialStr);
        }
        else {
            coefficient = std::stod(monomialStr);
        }

        size_t pos = 0;

        if (str.find('*') < end) {
            pos = str.find('*') + 1;
        }
        else {
            pos = 1;
            while (isdigit(str[pos])) {
                pos++;
            }
        }

        while (pos < (str.length() - 1)) {

            if (str[pos + 1] != '^') {
                variables.push_back(str[pos]);
                degrees.push_back(1);
                pos++;
                continue;
            }

            variables.push_back(str[pos]);
            std::string degree;
            pos += 2;

            if (str[pos] == '-') {
                pos += 1;
                while (std::isdigit(str[pos])) {
                    degree += str[pos];
                    pos++;
                }
                degrees.push_back(-stoi(degree));
            }
            else {
                while (std::isdigit(str[pos])) {
                    degree += str[pos];
                    pos++;
                }
                degrees.push_back(std::stoi(degree));
            }
        }
    };
    Monomial(double coeff, const std::vector<int>& degs, const std::vector<char>& vars) {
        coefficient = coeff;
        degrees = degs;
        variables = vars;
    };
    Monomial(const char* monomialString) {

        std::string str(monomialString);
        str.erase(std::remove_if(str.begin(), str.end(), ::isspace), str.end());
        size_t start = str.find('<') + 1;
        size_t end = str.find('>');
        std::string monomialStr = str.substr(start, end - start);
        if (isalpha(monomialStr[0])) {
            coefficient = 1;
        }
        else if (monomialStr[0] == '-' && isalpha(monomialStr[1])) {
            coefficient = -1;
            str.replace(1, 1, "");
        }
        else if (monomialStr[0] == '+' && isalpha(monomialStr[1])) {
            coefficient = 1;
            str.replace(1, 1, "");
        }
        else if (monomialStr[0] == '+' && isdigit(monomialStr[1])) {
            monomialStr.replace(0, 1, "");
            str.replace(1, 1, "");
            coefficient = std::stod(monomialStr);
        }
        else if (monomialStr[0] == '-' && isdigit(monomialStr[1])) {
            monomialStr.replace(0, 1, "");
            str.replace(1, 1, "");
            coefficient = -std::stod(monomialStr);
        }
        else {
            coefficient = std::stod(monomialStr);
        }

        size_t pos = 0;

        if (str.find('*') < end) {
            pos = str.find('*') + 1;
        }
        else {
            pos = 1;
            while (isdigit(str[pos])) {
                pos++;
            }
        }

        while (pos < (str.length() - 1)) {

            if (str[pos + 1] != '^') {
                variables.push_back(str[pos]);
                degrees.push_back(1);
                pos++;
                continue;
            }

            variables.push_back(str[pos]);
            std::string degree;
            pos += 2;

            if (str[pos] == '-') {
                pos += 1;
                while (std::isdigit(str[pos])) {
                    degree += str[pos];
                    pos++;
                }
                degrees.push_back(-stoi(degree));
            }
            else {
                while (std::isdigit(str[pos])) {
                    degree += str[pos];
                    pos++;
                }
                degrees.push_back(std::stoi(degree));
            }
        }



    };
    Monomial(const Monomial& other) {
        coefficient = other.coefficient;
        variables = other.variables;
        degrees = other.degrees;
    }

    // Деструктор
    ~Monomial() {}


    // Методы доступа
    double getCoefficient() const { return coefficient; }
    std::vector<int> getDegrees() const { return degrees; }
    std::vector<char> getVariables() const { return variables; }

    void setCoefficient(double coef) { coefficient = coef; }
    void setDegrees(const std::vector<int>& degs) { degrees = degs; }
    void setVariables(const std::vector<char>& vars) { variables = vars; }


    //Перегрузки операторов
    Monomial& operator=(const Monomial& other) {

        if (this != &other) {
            coefficient = other.coefficient;
            variables = other.variables;
            degrees = other.degrees;
        }
        return *this;
    };
    Monomial operator*(const Monomial& other) const {

        Monomial result;
        result.coefficient = coefficient * other.coefficient;
        result.variables = variables;
        result.degrees = degrees;

        for (size_t i = 0; i < other.variables.size(); ++i) {
            bool found = false;
            for (size_t j = 0; j < result.variables.size(); ++j) {
                if (other.variables[i] == result.variables[j]) {
                    result.degrees[j] += other.degrees[i];
                    found = true;
                    break;
                }
            }
            if (!found) {
                result.variables.push_back(other.variables[i]);
                result.degrees.push_back(other.degrees[i]);
            }
        }
        return result;
    };
    Monomial& operator*=(const Monomial& other) {

        *this = *this * other;
        return *this;
    };
    Monomial& operator/=(const Monomial& other) {
        *this = *this / other;
        return *this;
    };
    Monomial operator/(const Monomial& other) const {

        Monomial result;
        result.coefficient = coefficient / other.coefficient;
        result.variables = variables;
        result.degrees = degrees;

        for (size_t i = 0; i < other.variables.size(); ++i) {
            bool found = false;
            for (size_t j = 0; j < result.variables.size(); ++j) {
                if (other.variables[i] == result.variables[j]) {
                    result.degrees[j] -= other.degrees[i];
                    found = true;
                    break;
                }
            }
            if (!found) {
                result.variables.push_back(other.variables[i]);
                result.degrees.push_back(-other.degrees[i]);
            }
        }
        return result;
    };
    bool operator==(const Monomial& other) const {
        return coefficient == other.coefficient && degrees == other.degrees && variables == other.variables;
    }

    friend std::ostream& operator<<(std::ostream& os, const Monomial& monomial);
    friend std::istream& operator>>(std::istream& os, const Monomial& monomial);
};

std::ostream& operator<<(std::ostream& os, const Monomial& monomial) {
    if (monomial.coefficient != 0) {
        if (monomial.coefficient != 1) {
            os << "<" << monomial.coefficient;
        }
        else if (monomial.coefficient == 1 && monomial.variables.size() != 0) {
            os << "<";
        }
        else {
            os << "<1";
        }

        for (size_t i = 0; i < monomial.variables.size(); ++i) {

            if (monomial.degrees[i] != 0) {
                os << monomial.variables[i];
                if (monomial.degrees[i] != 1) {
                    os << "^" << monomial.degrees[i];
                }
            }
        }
        os << ">";
    }
    return os;
}
std::istream& operator>>(std::istream& is, Monomial& monomial)
{
    std::string input;
    is >> input;
    Monomial temp(input);
    monomial = temp;
    return is;
}



class Polynomial {
private:
    std::list<Monomial> monomials;

public:
    // Конструкторы
    Polynomial() {};
    Polynomial(const std::string& polynomialStr) {
        std::string str = polynomialStr;
        str.erase(std::remove_if(str.begin(), str.end(), ::isspace), str.end());
        size_t start = str.find('<');
        size_t end = str.find('>');

        size_t pos1 = 1;
        size_t pos2 = 1;

        if (str.find_first_of("+-")) {

            while (pos1 != end) {

                if (str.find_first_of("+-", pos1 + 1) < str.length()) {

                    pos2 = str.find_first_of("+-", pos1 + 1);
                    while (str[pos2 - 1] == '<' || str[pos2 - 1] == '^') {
                        pos2 = str.find_first_of("+-", pos2 + 1);
                    }
                    std::string str1 = "<" + str.substr(pos1, pos2 - pos1) + ">";
                    Monomial mon(str1);
                    monomials.push_back(mon);
                    pos1 = pos2;
                }
                else {
                    pos2 = end;
                    std::string str1 = "<" + str.substr(pos1, pos2 - pos1) + ">";
                    Monomial mon(str1);
                    monomials.push_back(mon);
                    break;
                }
            }
        }
    };
    Polynomial(const char* polynomialStr) {
        std::string str(polynomialStr);
        str.erase(std::remove_if(str.begin(), str.end(), ::isspace), str.end());
        size_t start = str.find('<');
        size_t end = str.find('>');

        size_t pos1 = 1;
        size_t pos2 = 1;

        if (str.find_first_of("+-")) {

            while (pos1 != end) {

                if (str.find_first_of("+-", pos1 + 1) < str.length()) {

                    pos2 = str.find_first_of("+-", pos1 + 1);
                    while (str[pos2 - 1] == '<' || str[pos2 - 1] == '^') {
                        pos2 = str.find_first_of("+-", pos2 + 1);
                    }
                    std::string str1 = "<" + str.substr(pos1, pos2 - pos1) + ">";
                    Monomial mon(str1);
                    monomials.push_back(mon);
                    pos1 = pos2;
                }
                else {
                    pos2 = end;
                    std::string str1 = "<" + str.substr(pos1, pos2 - pos1) + ">";
                    Monomial mon(str1);
                    monomials.push_back(mon);
                    break;
                }
            }
        }
    };
    Polynomial(const Polynomial& other) {
        for (const Monomial& monomial : other.monomials) {
            monomials.push_back(monomial);
        }
    }

    // Деструктор
    ~Polynomial() {};

    //Перегрузки операторов
    Polynomial& operator=(const Polynomial& other) {
        if (this != &other) {
            monomials = other.monomials;
        }
        return *this;
    };
    Polynomial& operator+=(const Polynomial& other) {
        for (const Monomial& term : other.monomials) {
            bool added = false;
            for (Monomial& t : monomials) {
                if (t.getVariables() == term.getVariables() && t.getDegrees() == term.getDegrees()) {
                    t.setCoefficient(t.getCoefficient() + term.getCoefficient());
                    added = true;
                    break;
                }
            }
            if (!added) {
                monomials.push_back(term);
            }
        }
        return *this;
    };
    Polynomial operator+(const Polynomial& other) const {
        Polynomial result(*this);
        result += other;
        return result;
    }
    Polynomial operator*(const Polynomial& other) const {
        Polynomial result;
        for (const Monomial& monomial1 : monomials) {
            for (const Monomial& monomial2 : other.monomials) {
                Monomial r = monomial1 * monomial2;
                bool added = false;
                for (Monomial& t : result.monomials) {
                    if (t.getVariables() == r.getVariables() && t.getDegrees() == r.getDegrees()) {
                        t.setCoefficient(t.getCoefficient() + r.getCoefficient());
                        added = true;
                        break;
                    }
                }
                if (!added) {
                    result.monomials.push_back(r);
                }
            }
        }
        return result;
    }
    Polynomial& operator-=(const Polynomial& other) {

        for (const Monomial& term : other.monomials) {
            bool added = false;
            for (Monomial& t : monomials) {
                if (t.getVariables() == term.getVariables() && t.getDegrees() == term.getDegrees()) {
                    t.setCoefficient(t.getCoefficient() - term.getCoefficient());
                    added = true;
                    break;
                }
            }
            if (!added) {
                Monomial r = term;
                r.setCoefficient(-term.getCoefficient());
                monomials.push_back(r);
            }
        }
        return *this;
    }
    Polynomial operator-(const Polynomial& other) const {
        Polynomial result(*this);
        result -= other;
        return result;
    }
    Polynomial& operator*=(const Polynomial& other) {
        *this = *this * other;
        return *this;

    }
    bool operator==(const Polynomial& other) const {
        return monomials == other.monomials;
    }
    bool operator!=(const Polynomial& other) const {
        return !(*this == other);
    }

    friend std::ostream& operator<<(std::ostream& os, const Polynomial& poly);
    friend std::istream& operator>>(std::istream& is, Polynomial& poly);
};

std::ostream& operator<<(std::ostream& os, const Polynomial& poly) {
    if (poly.monomials.empty()) {
        os << "0";
    }
    else {
        bool first = true;
        for (const auto& m : poly.monomials) {

            if (m.getCoefficient() != 0) {
                if (first) {
                    first = false;
                }
                else {
                    os << " + ";
                }
            }
            os << m;
        }
    }
    return os;
}
std::istream& operator>>(std::istream& is, Polynomial& poly) {
    std::string input;
    getline(is, input);
    Polynomial temp(input);
    poly = temp;
    return is;
}

void processFile(const std::string& filename) {
    std::ifstream fin;
    fin.open(filename);
    std::string str;
    size_t pos1, pos2;

    while (!fin.eof()) {
        str = "";
        std::getline(fin, str);
        if (str.length() == 0) {
            break;
        }
        size_t end = str.find_first_of(";");
        str.replace(end, 1, "");
        str.erase(std::remove_if(str.begin(), str.end(), ::isspace), str.end());

        Polynomial result;
        char operation;
        pos1 = 0;
        pos2 = str.find_first_of(">");
        std::string a = str.substr(pos1, pos2 - pos1 + 1);
        Polynomial p1(a);
        pos1 = str.find_first_of("<", pos1 + 1);
        operation = str[pos2 + 1];

        std::string b = str.substr(pos1, end - pos1);
        Polynomial p2(b);
        switch (operation) {
        case '+':
            result = p1 + p2;
            break;
        case '-':
            result = p1 - p2;
            break;
        case '*':
            result = p1 * p2;
            break;
        case '=':
            std::cout << (p1 == p2 ? "true" : "false") << std::endl;
            continue;
        case '!':
            std::cout << (p1 != p2 ? "true" : "false") << std::endl;
            continue;
        default:
            std::cout << "Invalid operation: " << operation << std::endl;
            continue;
        }
        std::cout << result << std::endl;
    }
}


int main() {

    processFile("e.txt");

}


