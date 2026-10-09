#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <filesystem>
#include <cctype>
#include <string_view>
#include <sstream>
#include <cstdlib>
#include <unordered_set>

namespace fs = std::filesystem;

// ==========================================
// 1. ИСКЛЮЧЕНИЯ И СИСТЕМА ОШИБОК
// ==========================================

enum class ErrorType {
    Syntax,
    MainKey,
    Math,
    Function,
    Var,
    Name
};

class ClsfException : public std::exception {
    std::string full_msg;
public:
    ClsfException(ErrorType type, size_t line, const std::string& reason, const std::string& tip) {
        std::string header;
        switch (type) {
            case ErrorType::Syntax:   header = "ErrorSyntax:"; break;
            case ErrorType::MainKey:  header = "ErrorMainKey:"; break;
            case ErrorType::Math:     header = "ErrorMath:"; break;
            case ErrorType::Function: header = "ErrorFunction:"; break;
            case ErrorType::Var:      header = "ErrorVar:"; break;
            case ErrorType::Name:     header = "ErrorName:"; break;
        }

        std::stringstream ss;
        ss << "\n[ " << header << " ] (Line " << line << ")\n"
           << "  Reason: " << reason << "\n"
           << "  Tip:    " << tip << "\n";
        full_msg = ss.str();
    }

    const char* what() const noexcept override {
        return full_msg.c_str();
    }
};

// ==========================================
// 2. СБОРКА
// ==========================================

bool run_compilation(const std::string& input_cpp, const std::string& output_bin) {
    std::string flags = " -std=c++17 -O2 -Wall ";
    std::string cmd = "clang++ " + flags + " \"" + input_cpp + "\" -o \"" + output_bin + "\" 2>/dev/null";
    int result = std::system(cmd.c_str());
    return (result == 0);
}

// ==========================================
// 3. ЛЕКСЕР
// ==========================================

enum TokenType {
    TOK_INT_TYPE, TOK_FLOAT_TYPE, TOK_STR_TYPE, TOK_BOOL_TYPE,
    TOK_IDENTIFIER, TOK_NUMBER, TOK_STRING_LITERAL,
    TOK_TRUE, TOK_FALSE,
    TOK_ASSIGN, TOK_PLUS, TOK_MINUS, TOK_MUL, TOK_DIV, TOK_MOD,
    TOK_GT, TOK_LT, TOK_GTE, TOK_LTE, TOK_EQ, TOK_NEQ,
    TOK_COMMA, TOK_SEMICOLON, TOK_LPAREN, TOK_RPAREN,
    TOK_LBRACE, TOK_RBRACE,
    TOK_PRINT, TOK_INPUT, TOK_IF, TOK_ELSE, TOK_EOF
};

struct Token {
    TokenType type;
    std::string_view text;
    size_t line;
};

class FastLexer {
    std::string_view src;
    size_t pos = 0;
    size_t current_line = 1;

public:
    FastLexer(std::string_view source) : src(source) {}

    std::vector<Token> tokenize() {
        std::vector<Token> tokens;
        tokens.reserve(src.size() / 4);

        while (pos < src.length()) {
            char current = src[pos];

            if (current == '\n') {
                current_line++;
                pos++;
                continue;
            }

            if (std::isspace(static_cast<unsigned char>(current))) {
                pos++;
                continue;
            }

            if (current == '/' && pos + 1 < src.length() && src[pos + 1] == '/') {
                while (pos < src.length() && src[pos] != '\n') pos++;
                continue;
            }

            if (std::isalpha(static_cast<unsigned char>(current)) || current == '_') {
                size_t start = pos;
                while (pos < src.length() && (std::isalnum(static_cast<unsigned char>(src[pos])) || src[pos] == '_' || src[pos] == '.')) pos++;
                std::string_view ident = src.substr(start, pos - start);

                if (ident == "int") tokens.push_back({TOK_INT_TYPE, ident, current_line});
                else if (ident == "float") tokens.push_back({TOK_FLOAT_TYPE, ident, current_line});
                else if (ident == "str") tokens.push_back({TOK_STR_TYPE, ident, current_line});
                else if (ident == "bool") tokens.push_back({TOK_BOOL_TYPE, ident, current_line});
                else if (ident == "true") tokens.push_back({TOK_TRUE, ident, current_line});
                else if (ident == "false") tokens.push_back({TOK_FALSE, ident, current_line});
                else if (ident == "if") tokens.push_back({TOK_IF, ident, current_line});
                else if (ident == "else") tokens.push_back({TOK_ELSE, ident, current_line});
                else if (ident == "console.print") tokens.push_back({TOK_PRINT, ident, current_line});
                else if (ident == "console.input") tokens.push_back({TOK_INPUT, ident, current_line});
                else tokens.push_back({TOK_IDENTIFIER, ident, current_line});
                continue;
            }

            if (std::isdigit(static_cast<unsigned char>(current))) {
                size_t start = pos;
                bool has_dot = false;
                while (pos < src.length() && (std::isdigit(static_cast<unsigned char>(src[pos])) || src[pos] == '.')) {
                    if (src[pos] == '.') {
                        if (has_dot) {
                            throw ClsfException(ErrorType::Math, current_line, "Multiple decimal points in number format", "Fix the number notation (e.g., 3.14).");
                        }
                        has_dot = true;
                    }
                    pos++;
                }
                tokens.push_back({TOK_NUMBER, src.substr(start, pos - start), current_line});
                continue;
            }

            if (current == '"') {
                pos++;
                size_t start = pos;
                while (pos < src.length() && src[pos] != '"') {
                    if (src[pos] == '\n') current_line++;
                    pos++;
                }
                if (pos >= src.length()) {
                    throw ClsfException(ErrorType::Syntax, current_line, "Unclosed string literal", "Add a closing double quote (\") at the end of string.");
                }
                std::string_view str_val = src.substr(start, pos - start);
                pos++;
                tokens.push_back({TOK_STRING_LITERAL, str_val, current_line});
                continue;
            }

            if (current == '>' || current == '<' || current == '=' || current == '!') {
                size_t start = pos;
                if (current == '>') {
                    pos++;
                    if (pos < src.length() && src[pos] == '=') {
                        pos++;
                        tokens.push_back({TOK_GTE, src.substr(start, pos - start), current_line});
                    } else {
                        tokens.push_back({TOK_GT, ">", current_line});
                    }
                    continue;
                }
                if (current == '<') {
                    pos++;
                    if (pos < src.length() && src[pos] == '=') {
                        pos++;
                        tokens.push_back({TOK_LTE, src.substr(start, pos - start), current_line});
                    } else {
                        tokens.push_back({TOK_LT, "<", current_line});
                    }
                    continue;
                }
                if (current == '=') {
                    pos++;
                    if (pos < src.length() && src[pos] == '=') {
                        pos++;
                        tokens.push_back({TOK_EQ, "==", current_line});
                    } else {
                        tokens.push_back({TOK_ASSIGN, "=", current_line});
                    }
                    continue;
                }
                if (current == '!') {
                    pos++;
                    if (pos < src.length() && src[pos] == '=') {
                        pos++;
                        tokens.push_back({TOK_NEQ, "!=", current_line});
                    } else {
                        throw ClsfException(ErrorType::Syntax, current_line, "Unexpected '!' symbol", "Use '!=' for comparison or remove the invalid character.");
                    }
                    continue;
                }
            }

            switch (current) {
                case '+': tokens.push_back({TOK_PLUS, "+", current_line}); break;
                case '-': tokens.push_back({TOK_MINUS, "-", current_line}); break;
                case '*': tokens.push_back({TOK_MUL, "*", current_line}); break;
                case '/': tokens.push_back({TOK_DIV, "/", current_line}); break;
                case '%': tokens.push_back({TOK_MOD, "%", current_line}); break;
                case ',': tokens.push_back({TOK_COMMA, ",", current_line}); break;
                case ';': tokens.push_back({TOK_SEMICOLON, ";", current_line}); break;
                case '(': tokens.push_back({TOK_LPAREN, "(", current_line}); break;
                case ')': tokens.push_back({TOK_RPAREN, ")", current_line}); break;
                case '{': tokens.push_back({TOK_LBRACE, "{", current_line}); break;
                case '}': tokens.push_back({TOK_RBRACE, "}", current_line}); break;
                default:
                    throw ClsfException(ErrorType::Syntax, current_line, std::string("Unknown symbol '") + current + "'", "Remove or fix the invalid character.");
            }
            pos++;
        }
        tokens.push_back({TOK_EOF, "", current_line});
        return tokens;
    }
};

// ==========================================
// 4. УЗЛЫ AST
// ==========================================

struct ASTNode {
    virtual ~ASTNode() = default;
    virtual std::string codegen() const = 0;
};

struct BlockNode : public ASTNode {
    std::vector<std::shared_ptr<ASTNode>> statements;
    BlockNode(std::vector<std::shared_ptr<ASTNode>> s) : statements(std::move(s)) {}

    std::string codegen() const override {
        std::string code = "{\n";
        for (const auto& stmt : statements) {
            code += "    " + stmt->codegen() + "\n";
        }
        code += "}\n";
        return code;
    }
};

struct VarDeclNode : public ASTNode {
    std::string type, name;
    std::shared_ptr<ASTNode> init_expr;
    VarDeclNode(std::string t, std::string n, std::shared_ptr<ASTNode> init)
        : type(std::move(t)), name(std::move(n)), init_expr(std::move(init)) {}

    std::string codegen() const override {
        std::string cpp_type = (type == "str") ? "std::string" : type;
        if (init_expr) {
            std::string init_value = init_expr->codegen();

            if (type == "int") {
                if (init_value.find("([&]()") != std::string::npos) {
                    init_value = "std::stoi(" + init_value + ")";
                }
            } else if (type == "float") {
                if (init_value.find("([&]()") != std::string::npos) {
                    init_value = "std::stof(" + init_value + ")";
                }
            }

            return cpp_type + " " + name + " = " + init_value + ";";
        } else {
            return cpp_type + " " + name + ";";
        }
    }
};

struct PrintNode : public ASTNode {
    std::vector<std::shared_ptr<ASTNode>> args;
    PrintNode(std::vector<std::shared_ptr<ASTNode>> a) : args(std::move(a)) {}

    std::string codegen() const override {
        std::string code;
        for (size_t i = 0; i < args.size(); ++i) {
            code += "std::cout << " + args[i]->codegen() + ";";
            if (i + 1 < args.size()) code += " ";
        }
        return code;
    }
};

struct InputNode : public ASTNode {
    std::shared_ptr<ASTNode> prompt;
    InputNode(std::shared_ptr<ASTNode> p) : prompt(std::move(p)) {}

    std::string codegen() const override {
        std::string res = "([&]() { ";
        if (prompt) {
            res += "std::cout << (" + prompt->codegen() + "); ";
            res += "std::cout.flush(); ";
        }
        res += "std::string _input; ";
        res += "std::getline(std::cin, _input); ";
        res += "return _input; ";
        res += "})()";
        return res;
    }
};

struct LiteralNode : public ASTNode {
    std::string value;
    bool is_string;
    LiteralNode(std::string v, bool str = false) : value(std::move(v)), is_string(str) {}

    std::string codegen() const override {
        if (is_string) {
            return "\"" + value + "\"";
        }
        return value;
    }
};

struct UnaryOpNode : public ASTNode {
    std::string op;
    std::shared_ptr<ASTNode> operand;
    UnaryOpNode(std::string o, std::shared_ptr<ASTNode> v) : op(std::move(o)), operand(std::move(v)) {}

    std::string codegen() const override {
        return "(" + op + operand->codegen() + ")";
    }
};

struct BinaryOpNode : public ASTNode {
    std::string op;
    std::shared_ptr<ASTNode> left, right;
    BinaryOpNode(std::string op, std::shared_ptr<ASTNode> l, std::shared_ptr<ASTNode> r)
        : op(std::move(op)), left(std::move(l)), right(std::move(r)) {}

    std::string codegen() const override {
        return "(" + left->codegen() + " " + op + " " + right->codegen() + ")";
    }
};

struct IfNode : public ASTNode {
    std::shared_ptr<ASTNode> condition;
    std::shared_ptr<BlockNode> then_block;
    std::shared_ptr<BlockNode> else_block;

    IfNode(std::shared_ptr<ASTNode> c, std::shared_ptr<BlockNode> tb, std::shared_ptr<BlockNode> eb)
        : condition(std::move(c)), then_block(std::move(tb)), else_block(std::move(eb)) {}

    std::string codegen() const override {
        std::string code = "if (" + condition->codegen() + ") " + then_block->codegen();
        if (else_block) code += " else " + else_block->codegen();
        return code;
    }
};

struct ProgramNode : public ASTNode {
    std::vector<std::shared_ptr<ASTNode>> statements;

    std::string codegen() const override {
        std::string code = "#include <iostream>\n";
        code += "#include <string>\n";
        code += "\n";
        code += "int main() {\n";

        for (const auto& stmt : statements) {
            code += "    " + stmt->codegen() + "\n";
        }

        code += "    return 0;\n";
        code += "}\n";
        return code;
    }
};

// ==========================================
// 5. ПАРСЕР
// ==========================================

class Parser {
    std::vector<Token> tokens;
    size_t pos = 0;
    std::unordered_set<std::string> declared_vars;

    Token current() { return tokens[pos]; }
    Token consume() { return tokens[pos++]; }

    void expect_semicolon(size_t line) {
        if (current().type == TOK_SEMICOLON) {
            consume();
        } else {
            throw ClsfException(ErrorType::Syntax, line, "Missing semicolon ';'", "Place ';' at the end of statement.");
        }
    }

    bool is_type_keyword(TokenType type) {
        return type == TOK_INT_TYPE || type == TOK_FLOAT_TYPE || type == TOK_STR_TYPE || type == TOK_BOOL_TYPE;
    }

    std::shared_ptr<ASTNode> parse_statement();

    std::shared_ptr<BlockNode> parse_block() {
        if (current().type != TOK_LBRACE) {
            throw ClsfException(ErrorType::Syntax, current().line, "Expected '{' to start a block", "Add an opening '{' before the block body.");
        }
        consume();

        std::vector<std::shared_ptr<ASTNode>> statements;
        while (current().type != TOK_RBRACE && current().type != TOK_EOF) {
            statements.push_back(parse_statement());
        }

        if (current().type != TOK_RBRACE) {
            throw ClsfException(ErrorType::Syntax, current().line, "Missing closing '}'", "Close the block with '}'.");
        }
        consume();
        return std::make_shared<BlockNode>(statements);
    }

public:
    Parser(std::vector<Token> t) : tokens(std::move(t)) {}

    std::shared_ptr<ASTNode> parse_primary() {
        Token tok = current();

        if (tok.type == TOK_NUMBER) {
            consume();
            return std::make_shared<LiteralNode>(std::string(tok.text), false);
        }
        if (tok.type == TOK_STRING_LITERAL) {
            consume();
            return std::make_shared<LiteralNode>(std::string(tok.text), true);
        }
        if (tok.type == TOK_TRUE) {
            consume();
            return std::make_shared<LiteralNode>("true", false);
        }
        if (tok.type == TOK_FALSE) {
            consume();
            return std::make_shared<LiteralNode>("false", false);
        }

        if (tok.type == TOK_IDENTIFIER) {
            std::string var_name = std::string(tok.text);
            if (declared_vars.find(var_name) == declared_vars.end()) {
                throw ClsfException(ErrorType::Name, tok.line, "Undeclared name or variable '" + var_name + "'", "Declare the variable using 'int', 'float', 'str', or 'bool' before using it.");
            }
            consume();
            return std::make_shared<LiteralNode>(var_name, false);
        }

        if (tok.type == TOK_LPAREN) {
            size_t open_line = tok.line;
            consume();
            auto expr = parse_expression();
            if (current().type == TOK_RPAREN) {
                consume();
                return expr;
            } else {
                throw ClsfException(ErrorType::Syntax, open_line, "Unclosed '('", "Add closing ')' parenthesis.");
            }
        }

        if (tok.type == TOK_INPUT) {
            consume();
            if (current().type != TOK_LPAREN) {
                throw ClsfException(ErrorType::Function, tok.line, "Missing '(' in console.input call", "Use console.input(\"prompt\") or console.input().");
            }
            consume();

            std::shared_ptr<ASTNode> prompt = nullptr;
            if (current().type != TOK_RPAREN) {
                prompt = parse_expression();
            }

            if (current().type != TOK_RPAREN) {
                throw ClsfException(ErrorType::Function, current().line, "Missing closing ')' in console.input", "Close parameters with ')'.");
            }
            consume();

            return std::make_shared<InputNode>(prompt);
        }

        return nullptr;
    }

    std::shared_ptr<ASTNode> parse_multiplicative() {
        auto left = parse_primary();
        if (!left) {
            throw ClsfException(ErrorType::Math, current().line, "Expected value or variable in expression", "Provide a valid operand.");
        }

        while (current().type == TOK_MUL || current().type == TOK_DIV || current().type == TOK_MOD) {
            Token op = consume();
            if ((op.type == TOK_DIV || op.type == TOK_MOD) && current().type == TOK_NUMBER && current().text == "0") {
                throw ClsfException(ErrorType::Math, op.line, "Division or modulo by zero '0'", "Change divisor value.");
            }
            auto right = parse_primary();
            if (!right) {
                throw ClsfException(ErrorType::Math, op.line, "Operator '" + std::string(op.text) + "' lacks right operand", "Add value after operator.");
            }
            left = std::make_shared<BinaryOpNode>(std::string(op.text), left, right);
        }
        return left;
    }

    std::shared_ptr<ASTNode> parse_additive() {
        auto left = parse_multiplicative();
        while (current().type == TOK_PLUS || current().type == TOK_MINUS) {
            Token op = consume();
            auto right = parse_multiplicative();
            if (!right) {
                throw ClsfException(ErrorType::Math, op.line, "Operator '" + std::string(op.text) + "' lacks right operand", "Add value after operator.");
            }
            left = std::make_shared<BinaryOpNode>(std::string(op.text), left, right);
        }
        return left;
    }

    std::shared_ptr<ASTNode> parse_comparison() {
        auto left = parse_additive();
        while (current().type == TOK_GT || current().type == TOK_LT ||
               current().type == TOK_GTE || current().type == TOK_LTE ||
               current().type == TOK_EQ || current().type == TOK_NEQ) {
            Token op = consume();
            auto right = parse_additive();
            if (!right) {
                throw ClsfException(ErrorType::Math, op.line, "Comparison operator '" + std::string(op.text) + "' lacks right operand", "Add a value after the comparison operator.");
            }
            left = std::make_shared<BinaryOpNode>(std::string(op.text), left, right);
        }
        return left;
    }

    std::shared_ptr<ASTNode> parse_expression() {
        return parse_comparison();
    }

    std::shared_ptr<ASTNode> parse_statement() {
        if (is_type_keyword(current().type)) {
            Token type_tok = consume();
            size_t line = type_tok.line;

            if (current().type != TOK_IDENTIFIER) {
                throw ClsfException(ErrorType::MainKey, line, "Invalid syntax after key type '" + std::string(type_tok.text) + "'", "Provide a valid variable name after key type.");
            }

            Token name_tok = consume();
            std::string var_name = std::string(name_tok.text);

            if (declared_vars.find(var_name) != declared_vars.end()) {
                throw ClsfException(ErrorType::Var, name_tok.line, "Variable '" + var_name + "' is already declared", "Use a different variable name.");
            }
            declared_vars.insert(var_name);

            std::shared_ptr<ASTNode> init = nullptr;
            if (current().type == TOK_ASSIGN) {
                consume();
                init = parse_expression();
            }

            expect_semicolon(line);
            return std::make_shared<VarDeclNode>(std::string(type_tok.text), var_name, init);
        }

        if (current().type == TOK_PRINT) {
            Token print_tok = consume();
            size_t line = print_tok.line;

            if (current().type != TOK_LPAREN) {
                throw ClsfException(ErrorType::Function, line, "Expected '(' after console.print", "Pass parameters wrapped in parentheses.");
            }
            consume();

            std::vector<std::shared_ptr<ASTNode>> args;
            if (current().type == TOK_RPAREN) {
                throw ClsfException(ErrorType::Function, line, "console.print received 0 arguments", "Pass at least one argument.");
            }

            while (current().type != TOK_RPAREN && current().type != TOK_EOF) {
                args.push_back(parse_expression());

                if (current().type == TOK_COMMA) {
                    consume();
                    if (current().type == TOK_RPAREN) {
                        throw ClsfException(ErrorType::Function, current().line, "Trailing comma in console.print", "Remove comma before closing parenthesis.");
                    }
                } else if (current().type != TOK_RPAREN) {
                    throw ClsfException(ErrorType::Function, current().line, "Missing comma ',' between console.print arguments", "Separate arguments with commas.");
                }
            }

            if (current().type != TOK_RPAREN) {
                throw ClsfException(ErrorType::Function, line, "Unclosed console.print argument list", "Add closing ')'.");
            }
            consume();
            expect_semicolon(line);
            return std::make_shared<PrintNode>(args);
        }

        if (current().type == TOK_IF) {
            Token if_tok = consume();
            size_t line = if_tok.line;

            if (current().type != TOK_LPAREN) {
                throw ClsfException(ErrorType::Syntax, line, "Expected '(' after 'if'", "Use the form 'if (condition) { ... }'.");
            }
            consume();
            auto cond = parse_expression();
            if (current().type != TOK_RPAREN) {
                throw ClsfException(ErrorType::Syntax, current().line, "Missing closing ')' in if condition", "Close the condition with ')'.");
            }
            consume();

            auto then_block = parse_block();
            std::shared_ptr<BlockNode> else_block = nullptr;
            if (current().type == TOK_ELSE) {
                consume();
                else_block = parse_block();
            }

            return std::make_shared<IfNode>(cond, then_block, else_block);
        }

        Token bad_tok = current();
        if (bad_tok.type == TOK_IDENTIFIER) {
            throw ClsfException(ErrorType::Name, bad_tok.line, "Unknown name or invalid command statement '" + std::string(bad_tok.text) + "'", "Check name spelling or key type declaration.");
        }
        if (bad_tok.type == TOK_RPAREN) {
            throw ClsfException(ErrorType::Syntax, bad_tok.line, "Unexpected closing bracket ')'", "Remove stray bracket.");
        }
        if (bad_tok.type == TOK_SEMICOLON) {
            throw ClsfException(ErrorType::Syntax, bad_tok.line, "Unexpected standalone ';'", "Remove extra semicolon.");
        }
        if (bad_tok.type == TOK_LBRACE) {
            throw ClsfException(ErrorType::Syntax, bad_tok.line, "Unexpected block start '{' at statement level", "Use blocks only inside 'if' statements.");
        }

        throw ClsfException(ErrorType::MainKey, bad_tok.line, "Invalid token or structure at statement start '" + std::string(bad_tok.text) + "'", "Check keywords or statement syntax.");
    }

    std::shared_ptr<ProgramNode> parse_program() {
        auto prog = std::make_shared<ProgramNode>();
        while (current().type != TOK_EOF) {
            prog->statements.push_back(parse_statement());
        }
        return prog;
    }
};

// ==========================================
// 6. ЗАПУСК
// ==========================================

std::vector<fs::path> find_local_clsf_files() {
    std::vector<fs::path> files;
    std::error_code ec;

    for (const auto& entry : fs::directory_iterator(fs::current_path(), ec)) {
        if (!ec && entry.is_regular_file(ec) && entry.path().extension() == ".clsf") {
            files.push_back(entry.path());
        }
    }
    return files;
}

std::string read_file_content(const fs::path& filepath) {
    std::ifstream in(filepath, std::ios::in | std::ios::binary);
    if (!in.is_open()) return "";

    std::stringstream buffer;
    buffer << in.rdbuf();
    return buffer.str();
}

int main() {
    std::vector<fs::path> files = find_local_clsf_files();

    if (files.empty()) {
        std::cout << "No .clsf files found in current directory.\n";
        return 0;
    }

    std::cout << "Found .clsf files:\n========================\n";
    for (size_t i = 0; i < files.size(); ++i) {
        std::cout << (i + 1) << ": " << files[i].filename().string() << "\n";
    }
    std::cout << "========================\nChoose file: ";

    int choice;
    if (!(std::cin >> choice)) return 0;

    if (choice >= 1 && choice <= static_cast<int>(files.size())) {
        fs::path selected_file = files[choice - 1];
        std::string src = read_file_content(selected_file);

        if (src.empty()) {
            std::cout << "\n[ ErrorFile: File is empty or could not be read! ]\n";
            return 0;
        }

        try {
            FastLexer lexer(src);
            Parser parser(lexer.tokenize());
            auto ast = parser.parse_program();

            fs::path output_dir = fs::current_path() / "CompliteFilesClsf";
            std::error_code ec;
            fs::create_directories(output_dir, ec);

            std::string base_filename = selected_file.stem().string();
            fs::path cpp_out = output_dir / (base_filename + ".cpp");
            fs::path bin_out = output_dir / base_filename;

            std::ofstream out(cpp_out);
            out << ast->codegen();
            out.close();

            if (run_compilation(cpp_out.string(), bin_out.string())) {
                std::string chmod_cmd = "chmod +x \"" + bin_out.string() + "\"";
                std::system(chmod_cmd.c_str());

                std::string run_cmd = "\"" + bin_out.string() + "\"";
                std::cout << "\n";
                std::system(run_cmd.c_str());
                std::cout << "\n";
            } else {
                std::cout << "\n[ ErrorSyntax: Internal compilation error in generated C++ code ]\n";
            }
        } catch (const ClsfException& e) {
            std::cout << e.what();
        }
    } else {
        std::cout << "Invalid choice!\n";
    }
    return 0;
}
