#include "calculator.h"
#include "tokenizer.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <limits>

// ANSI color codes
namespace Color {
    const std::string RESET = "\033[0m";
    const std::string BOLD = "\033[1m";
    const std::string RED = "\033[31m";
    const std::string GREEN = "\033[32m";
    const std::string YELLOW = "\033[33m";
    const std::string BLUE = "\033[34m";
    const std::string MAGENTA = "\033[35m";
    const std::string CYAN = "\033[36m";
    const std::string WHITE = "\033[37m";
}

// Format number with thousands separator and proper precision
std::string format_number(double value) {
    if (std::isnan(value)) return Color::RED + "NaN" + Color::RESET;
    if (std::isinf(value)) return Color::RED + "Infinity" + Color::RESET;
    
    std::ostringstream ss;
    ss << std::fixed;
    
    // Determine appropriate precision
    double abs_val = std::fabs(value);
    if (abs_val > 0 && abs_val < 0.0001) {
        ss << std::scientific << std::setprecision(6);
    } else {
        double intpart;
        if (std::modf(value, &intpart) == 0.0) {
            ss << std::setprecision(0);
        } else {
            // Determine needed precision (max 10)
            int precision = 2;
            double diff = std::fabs(value - std::round(value));
            if (diff > 0) {
                precision = std::min(10, std::max(2, static_cast<int>(-std::log10(diff)) + 1));
            }
            ss << std::setprecision(precision);
        }
    }
    
    ss << value;
    std::string raw = ss.str();
    
    // If scientific notation, skip comma insertion to avoid breaking mantissa/exponent
    if (raw.find('e') != std::string::npos || raw.find('E') != std::string::npos) {
        return Color::GREEN + raw + Color::RESET;
    }

    // Split sign, integer, and fractional parts
    bool neg = !raw.empty() && raw[0] == '-';
    size_t dot = raw.find('.');
    std::string intpart = raw.substr(neg ? 1 : 0, dot == std::string::npos ? std::string::npos : dot - (neg ? 1 : 0));
    std::string fracpart = (dot == std::string::npos) ? std::string() : raw.substr(dot); // includes '.'

    // Insert commas safely into integer part
    std::string with_commas;
    int n = static_cast<int>(intpart.size());
    for (int i = 0; i < n; ++i) {
        with_commas.push_back(intpart[i]);
        int remaining = n - i - 1;
        if (remaining > 0 && remaining % 3 == 0) {
            with_commas.push_back(',');
        }
    }

    std::string result = (neg ? std::string("-") : std::string()) + with_commas + fracpart;
    return Color::GREEN + result + Color::RESET;
}

// Display ASCII art banner
void show_banner() {
    std::cout << Color::CYAN << Color::BOLD;
    std::cout << "  _____           _______      _      " << std::endl;
    std::cout << " / ____|         |__   __|    | |     " << std::endl;
    std::cout << "| |     _ __ ___    | | ___  | | ___ " << std::endl;
    std::cout << "| |    | '_ ` _ \\   | |/ __| | |/ __|"
              << std::endl;
    std::cout << "| |____| | | | | |  | | (__ _| | (__ " << std::endl;
    std::cout << " \\_____|_| |_| |_|  |_|\\___|_)_|\\___|"
              << std::endl;
    std::cout << Color::RESET << std::endl;
    std::cout << Color::YELLOW << "Command-line Calculator v1.0" << Color::RESET << std::endl;
    std::cout << "Type " << Color::CYAN << "help" << Color::RESET << " for available commands" << std::endl;
    std::cout << std::endl;
}

// Help command output
void show_help() {
    std::cout << Color::CYAN << Color::BOLD << "Available Commands:" << Color::RESET << std::endl;
    std::cout << "  " << Color::CYAN << "help" << Color::RESET << "      - Show this help message" << std::endl;
    std::cout << "  " << Color::CYAN << "clear" << Color::RESET << "     - Clear the screen" << std::endl;
    std::cout << "  " << Color::CYAN << "quit/exit" << Color::RESET << " - Exit the calculator" << std::endl;
    std::cout << std::endl;
    std::cout << Color::CYAN << Color::BOLD << "Supported Operations:" << Color::RESET << std::endl;
    std::cout << "  + - * /     - Basic arithmetic" << std::endl;
    std::cout << "  ^           - Exponentiation (right associative)" << std::endl;
    std::cout << "  ( )         - Parentheses for grouping" << std::endl;
    std::cout << "  -N          - Unary minus" << std::endl;
    std::cout << std::endl;
    std::cout << Color::CYAN << Color::BOLD << "Constants:" << Color::RESET << std::endl;
    std::cout << "  pi          - 3.14159..." << std::endl;
    std::cout << "  e           - 2.71828..." << std::endl;
    std::cout << std::endl;
    std::cout << Color::CYAN << Color::BOLD << "Variables:" << Color::RESET << std::endl;
    std::cout << "  ans         - Last computed result" << std::endl;
    std::cout << std::endl;
}

// Clear screen function
void clear_screen() {
    std::cout << "\033[2J\033[1;1H"; // ANSI escape sequence to clear screen and move cursor to top-left
}

int main(int argc, char **argv){
    if(argc>1){
        std::string expr;
        for(int i=1;i<argc;++i){ if(i>1) expr += " "; expr += argv[i]; }
        std::string err;
        double v = evaluate_expression(expr, &err);
        if(!err.empty()){ 
            std::cerr << Color::RED << "Error: " << err << Color::RESET << "\n"; 
            return 2; 
        }
        std::cout << format_number(v) << "\n";
        return 0;
    }
    // REPL with history
    show_banner();
    
    std::vector<std::string> history;
    std::string line;
    
    while(true){
        std::cout << Color::CYAN << Color::BOLD << "> " << Color::RESET;
        if(!std::getline(std::cin, line)) break;
        
        // Trim whitespace
        line.erase(0, line.find_first_not_of(" \t\n\r\f\v"));
        line.erase(line.find_last_not_of(" \t\n\r\f\v") + 1);
        
        if(line.empty()) continue;
        
        // Handle special commands
        if(line == "quit" || line == "exit") {
            std::cout << Color::YELLOW << "Goodbye!" << Color::RESET << std::endl;
            break;
        }
        if(line == "help") {
            show_help();
            continue;
        }
        if(line == "clear") {
            clear_screen();
            show_banner();
            continue;
        }
        
        // Add to history if not a duplicate of the last entry
        if(history.empty() || history.back() != line) {
            history.push_back(line);
            // Keep history to a reasonable size
            if(history.size() > 100) history.erase(history.begin());
        }
        
        // Process expression
        std::string err;
        double v = evaluate_expression(line, &err);
        
        if(!err.empty()) {
            std::cout << Color::RED << "Error: " << err << Color::RESET << "\n";
        } else {
            std::cout << "= " << format_number(v) << "\n";
            // Save last result for 'ans'
            set_last_result(v);
        }
    }
    return 0;
}
