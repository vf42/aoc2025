#include <iostream>
#include <print>
#include <vector>
#include <ranges>

int main() {
    // C++23 std::print
    std::println("Hello C++23!");
    
    // C++23 ranges
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    auto even_numbers = numbers | std::views::filter([](int n) { return n % 2 == 0; });
    
    std::print("Even numbers: ");
    for (int n : even_numbers) {
        std::print("{} ", n);
    }
    std::println("");
    
    return 0;
}