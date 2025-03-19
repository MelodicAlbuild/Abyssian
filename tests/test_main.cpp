#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>
#include "interpreter.h"
#include "lexer.h"
#include "parser.h"

namespace fs = std::filesystem;

struct TestCase {
    std::string name;
    std::string inputFile;
    std::string expectedFile;
};

std::vector<TestCase> getTestCases(const std::string& testDir) {
    std::vector<TestCase> testCases;
    for (const auto& entry : fs::directory_iterator(testDir)) {
        if (entry.path().extension() == ".aby") {
            std::string testName = entry.path().stem().string();
            testCases.push_back({testName, entry.path().string(), testDir + "/" + testName + ".expected"});
        }
    }
    return testCases;
}

std::string readFile(const std::string& filePath) {
    std::ifstream file(filePath);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

bool runTest(const TestCase& testCase) {
    std::stringstream nullout;
    std::streambuf* originalCout = std::cout.rdbuf(nullout.rdbuf());

    // Run the interpreter on the test input file
    std::string input = readFile(testCase.inputFile);

    // Create a lexer and parser for the input
    Lexer lexer(input);
    auto tokens = lexer.tokenize();
    Parser parser(tokens);
    auto ast = parser.parse();

    // Run the interpreter
    Interpreter interpreter;
    interpreter.interpret(std::move(ast));

    std::cout.rdbuf(originalCout);

    // Redirect std::cout to a string stream for capturing the output
    std::stringstream outputStream;
    std::streambuf* originalOut = std::cout.rdbuf(outputStream.rdbuf());

    interpreter.execute();

    // Restore std::cout
    std::cout.rdbuf(originalOut);

    // Read the expected output
    std::string expectedOutput = readFile(testCase.expectedFile);

    // Compare the actual output with the expected output
    return outputStream.str() == expectedOutput;
}

int main() {
    std::string testDir = "../../tests/test_cases";
    auto testCases = getTestCases(testDir);

    int passed = 0;
    for (const auto& testCase : testCases) {
        std::cout << "Running " << testCase.name << "..." << std::endl;
        if (runTest(testCase)) {
            std::cout << "Passed " << testCase.name << std::endl;
            ++passed;
        } else {
            std::cout << "Failed " << testCase.name << std::endl;
        }
    }

    std::cout << passed << " out of " << testCases.size() << " tests passed." << std::endl;
    return 0;
}